#include "cr01_reg_hotpatch_core.h"
#include <limits.h>
static const uint8_t kExactEntry[9]={0x48,0x85,0xd2,0x0f,0x84,0xe1,0x08,0,0};
static int exact_entry(const uint8_t *helper){
 if(!helper)return 0;
 for(unsigned i=0;i<9;i++)if(helper[i]!=kExactEntry[i])return 0;
 return 1;
}
static int exact_pad(const uint8_t *helper){
 for(unsigned i=1;i<=8;i++)if(helper[-(int)i]!=0xcc)return 0;
 return 1;
}
static void put32(uint8_t*p,uint32_t x){for(unsigned i=0;i<4;i++)p[i]=(uint8_t)(x>>(8*i));}
static void put64(uint8_t*p,uint64_t x){for(unsigned i=0;i<8;i++)p[i]=(uint8_t)(x>>(8*i));}
static void absolute_jmp(uint8_t*p,uintptr_t destination){
 p[0]=0xff;p[1]=0x25;for(unsigned i=2;i<6;i++)p[i]=0;
 put64(p+6,destination);
}
int cr01_hotpatch_build(const uint8_t*helper,uintptr_t hva,
                        uintptr_t rva,uintptr_t hook,
                        uint8_t pad[8],uint8_t relay[CR01_REG_RELAY_LENGTH]){
 int64_t disp;
 if(!helper||!hva||!rva||!hook||!pad||!relay||hva<8 ||
    (hva&1) || rva>INT64_MAX || hva>INT64_MAX || !hook)return 0;
 if(!exact_entry(helper)||!exact_pad(helper))return 0;
 disp=(int64_t)rva-((int64_t)hva-8+5);
 if(disp<INT32_MIN || disp>INT32_MAX)return 0;
 for(unsigned i=0;i<8;i++)pad[i]=0xcc;pad[0]=0xe9;put32(pad+1,(uint32_t)(int32_t)disp);
 for(unsigned i=0;i<CR01_REG_RELAY_LENGTH;i++)relay[i]=0xcc;
 absolute_jmp(relay,hook);
 /* Transferred 9 bytes: test rdx,rdx, je original+0x8e1.
  * Branch rel32 reconstructed as a local 6-byte JE, with independent
  * 14-byte RIP-indirect far JMPs so ASLR cannot invalidate relocation. */
 uint8_t*t=relay+CR01_REG_TRAMPOLINE_OFFSET;
 t[0]=0x48;t[1]=0x85;t[2]=0xd2;
 t[3]=0x0f;t[4]=0x84;put32(t+5,14);
 absolute_jmp(t+9,hva+9);
 absolute_jmp(t+23,hva+0x8eau); /* JE target: entry+9+0x8e1 = +0x8ea */
 return 1;
}
int cr01_hotpatch_commit(uint8_t*helper,const uint8_t pad[8],
                         const CR01HotpatchOps*ops){
 uint16_t old;
 if(!helper||!pad||!ops||!ops->make_writable||!ops->restore_protection||
    !ops->flush||((uintptr_t)helper&1))return 0;
 if(!exact_entry(helper)||!exact_pad(helper))return 0;
 if(pad[0]!=0xe9 || pad[5]!=0xcc || pad[6]!=0xcc || pad[7]!=0xcc)return 0;
 if(!ops->make_writable(helper-8,11,ops->ctx))return 0;
 /* A competing owner may change a byte during the protection transition.
    Detect changes BEFORE writing the pad. External startup quiescence is
    still mandatory: this check alone cannot prevent concurrent execution. */
 if(!exact_entry(helper)||!exact_pad(helper)){
  (void)ops->restore_protection(helper-8,11,ops->ctx);return 0;
 }
 /* Original program cannot reach CC pad until the entry CAS publishes it. */
 for(unsigned i=0;i<8;i++)helper[(int)i-8]=pad[i];
 if(!ops->flush(helper-8,8,ops->ctx))goto undo_pad;
 /* Refuse a modified 9-byte instruction boundary before atomic publication. */
 if(!exact_entry(helper))goto undo_pad;
 old=__sync_val_compare_and_swap((volatile uint16_t*)(void*)helper,
                                 CR01_ATOMIC_OLD,CR01_ATOMIC_NEW);
 if(old!=CR01_ATOMIC_OLD)goto undo_pad;
 if(!ops->flush(helper,2,ops->ctx)){
  uint16_t now=__sync_val_compare_and_swap((volatile uint16_t*)(void*)helper,
                                 CR01_ATOMIC_NEW,CR01_ATOMIC_OLD);
  (void)ops->flush(helper,2,ops->ctx);
  if(now!=CR01_ATOMIC_NEW){
   /* Hook may still be reachable. Do not erase the pad. Keep code pinned. */
   (void)ops->restore_protection(helper-8,11,ops->ctx);return 0;
  }
  goto undo_pad;
 }
 if(!ops->restore_protection(helper-8,11,ops->ctx)){
  /* Publisher still owns the quiescent window. Attempt to restore the exact
     original entry and CC pad rather than leave a reachable partial hook.
     If another owner races us, never erase reachable relay/pad bytes. */
  uint16_t prior=__sync_val_compare_and_swap((volatile uint16_t*)(void*)helper,
                                             CR01_ATOMIC_NEW,CR01_ATOMIC_OLD);
  if(prior==CR01_ATOMIC_NEW){
   (void)ops->flush(helper,2,ops->ctx);
   /* The failed restore does not attest that the pad remains writable.
    * Reacquire write permission before erasing even unreachable pad bytes.
    * If reacquisition fails, leave the CC-independent pad in place and
    * return failure. Entry is already native, so the relay is unreachable. */
   if(!ops->make_writable(helper-8,11,ops->ctx))return 0;
   goto undo_pad;
  }
  /* Entry ownership could not be proven. The owner must remain pinned and
     force native pass-through for the rest of this process. */
  return 0;
 }
 return 1;
undo_pad:
 for(unsigned i=0;i<8;i++)helper[(int)i-8]=0xcc;
 (void)ops->flush(helper-8,8,ops->ctx);
 (void)ops->restore_protection(helper-8,11,ops->ctx);
 return 0;
}

int cr01_hotpatch_uncommit(uint8_t*helper,const uint8_t expected_pad[8],
                           const CR01HotpatchOps*ops){
 if(!helper||!expected_pad||!ops||!ops->make_writable||
    !ops->restore_protection||!ops->flush||((uintptr_t)helper&1))return 0;
 if(helper[0]!=0xeb||helper[1]!=0xf6)return 0;
 for(unsigned i=0;i<8;i++)if(helper[(int)i-8]!=expected_pad[i])return 0;
 for(unsigned i=2;i<9;i++)if(helper[i]!=kExactEntry[i])return 0;
 if(!ops->make_writable(helper-8,11,ops->ctx))return 0;
 /* Validate AFTER protection change in case another owner raced it. */
 if(helper[0]!=0xeb||helper[1]!=0xf6){
  (void)ops->restore_protection(helper-8,11,ops->ctx);return 0;
 }
 for(unsigned i=0;i<8;i++)if(helper[(int)i-8]!=expected_pad[i]){
  (void)ops->restore_protection(helper-8,11,ops->ctx);return 0;
 }
 if(__sync_val_compare_and_swap((volatile uint16_t*)(void*)helper,
         CR01_ATOMIC_NEW,CR01_ATOMIC_OLD)!=CR01_ATOMIC_NEW){
  (void)ops->restore_protection(helper-8,11,ops->ctx);return 0;
 }
 if(!ops->flush(helper,2,ops->ctx)){
  /* Entry was restored but its visibility is not confirmed: preserve relay,
     pad and module pin. No destructive cleanup after a failed flush. */
  (void)ops->restore_protection(helper-8,11,ops->ctx);return 0;
 }
 /* External quiescence ensures nobody is executing the pre-entry relay. */
 for(unsigned i=0;i<8;i++)helper[(int)i-8]=0xcc;
 int clean=ops->flush(helper-8,8,ops->ctx);
 int protected=ops->restore_protection(helper-8,11,ops->ctx);
 return clean&&protected;
}
