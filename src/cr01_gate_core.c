#include "cr01_gate_core.h"
#include <limits.h>
/* Historical LC44 packed shader contract: native-equivalent geometry 2,
   hair 4, indirect 100. Shader transformation MUST decode word==2 as neutral;
   the CPU bridge does not establish GPU equivalence. */
#define GEO_MASK UINT64_C(0xff)
#define HAIR_SHIFT 8
#define COMP_SHIFT 16
#define MASTER_SHIFT 24
#define PT_SHIFT 25
#define RENDER_SHIFT 26
#define SHADER_SHIFT 27
static uint64_t pack(CR01GateFields f,uint32_t epoch){
 return ((uint64_t)epoch<<32)|f.geometry|((uint64_t)f.hair<<HAIR_SHIFT)|
    ((uint64_t)f.comp<<COMP_SHIFT)|((uint64_t)f.master<<MASTER_SHIFT)|
    ((uint64_t)f.pt<<PT_SHIFT)|((uint64_t)f.renderer_ready<<RENDER_SHIFT)|
    ((uint64_t)f.shader_ready<<SHADER_SHIFT);
}
int cr01_gate_publish(CR01GateState*s,CR01GateFields f,uint32_t epoch){
 if(!s||f.geometry<1||f.geometry>6||f.hair<1||f.hair>10||
    (f.comp<100||f.comp>150)||f.master>1||f.pt>1||f.renderer_ready>1||f.shader_ready>1)return 0;
 /* Epoch must be ordered by the serialized event owner. */
 __atomic_store_n(&s->word,pack(f,epoch),__ATOMIC_RELEASE);
 return 1;
}
uint64_t cr01_gate_snapshot(const CR01GateState*s){
 return s?__atomic_load_n(&s->word,__ATOMIC_ACQUIRE):0;
}
int cr01_gate_post_serialize(const CR01GateState *s,void *native_dest,int custom_shader_live){
 uint64_t p=cr01_gate_snapshot(s);
 uint32_t bits=(uint32_t)p;
 uint32_t geometry=bits&255u, hair=(bits>>HAIR_SHIFT)&255u;
 uint32_t indirect=(bits>>COMP_SHIFT)&255u;
 uint8_t *dst=(uint8_t*)native_dest;
 /* The game-owned native serializer always runs first. CR01 only edits its
    CPU output when Master+PT are staged AND the protected registration pair
    is live right now. This removes any need to poll registration readiness or
    signal Windows events from the render thread. */
 if(!custom_shader_live||!dst||((bits>>MASTER_SHIFT)&3u)!=3u ||
    geometry<1||geometry>6||hair<1||hair>10||indirect<100)return 0;
 if(geometry==CR01_NATIVE_GEOM&&hair==CR01_NATIVE_HAIR&&indirect==CR01_NATIVE_COMP)return 0;
 /* Owner-controlled CPU scratch buffer, called from original serializer's
    immediate caller AFTER native return. Never write GPU memory/resources. */
 /* R15: true fixed-point slider, no diagnostic gain stretch. */
 uint32_t shader_indirect=indirect;
 if(shader_indirect>150u)return 0;
 uint32_t packed=geometry | (shader_indirect<<8);
 __builtin_memcpy(dst+CR01_GEOM_OFFSET,&packed,sizeof(packed));
 __builtin_memcpy(dst+CR01_HAIR_OFFSET,&hair,sizeof(hair));
 return 1;
}
static uint32_t rd32(const uint8_t*p){
 return (uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;
}
int cr01_gate_exact_call_preflight(const uint8_t *call_instruction,
 const uint8_t *geo,const uint8_t *hair,uintptr_t call_va,
 uintptr_t native_va,uintptr_t replacement_va,int32_t*old_disp,int32_t*new_disp){
 static const uint8_t gp[10]={0xc7,0x83,0x10,0x02,0,0,2,0,0,0};
 static const uint8_t hp[10]={0xc7,0x83,0x80,0x03,0,0,4,0,0,0};
 const uint8_t expected[5]={0xe8,0x6c,0x13,0,0};
 uint64_t return_va;int64_t delta;
 unsigned i;
 if(!call_instruction||!geo||!hair||!old_disp||!new_disp)return 0;
 /* Operand at 0x...FD0 is naturally aligned for atomic 32-bit CAS. */
 if(((call_va+1)&3u)!=0)return 0;
 for(i=0;i<5;i++)if(call_instruction[i]!=expected[i])return 0;
 for(i=0;i<10;i++)if(geo[i]!=gp[i]||hair[i]!=hp[i])return 0;
 if(call_va>UINTPTR_MAX-5)return 0;
 return_va=(uint64_t)call_va+5u;
 if(native_va!=return_va+0x136cULL)return 0; /* 0x136c == original rel32 */
 if(replacement_va>INT64_MAX||return_va>INT64_MAX)return 0;
 delta=(int64_t)replacement_va-(int64_t)return_va;
 if(delta<INT32_MIN||delta>INT32_MAX)return 0;
 *old_disp=(int32_t)rd32(call_instruction+1);
 *new_disp=(int32_t)delta;
 return 1;
}
int cr01_gate_commit_operand(uint8_t*call_site,int32_t expected,int32_t next,
                            const CR01CallPlatform*p){
 volatile int32_t*operand;
 if(!call_site||call_site[0]!=0xe8||!p||!p->writable||
    !p->restore_protection||!p->flush_icache||
    (((uintptr_t)(call_site+1))&3u))return 0;
 operand=(volatile int32_t*)(void*)(call_site+1);
 if(!p->writable(call_site,5,p->ctx))return 0;
 /* Never steal someone else's hook; only swap the original known displacement.
    The 5-byte call opcode does not change, unlike LC44 17/18-byte replacement. */
 int32_t actual=__sync_val_compare_and_swap(operand,expected,next);
 if(actual!=expected){(void)p->restore_protection(call_site,5,p->ctx);return 0;}
 int flushed=p->flush_icache(call_site,5,p->ctx);
 int restored=p->restore_protection(call_site,5,p->ctx);
 if(flushed&&restored)return 1;
 /* A failed protection restore is NOT evidence the bytes stayed writable.
    * Reacquire before attempting any rollback CAS. The platform must retain
    * the ORIGINAL page protection across repeated writable requests. If
    * reacquisition fails, the owner keeps the relay pinned and remains neutral.
    * No unverified write to a possibly RX page is permitted. */
 if(p->writable(call_site,5,p->ctx)){
   (void)__sync_val_compare_and_swap(operand,next,expected);
   (void)p->flush_icache(call_site,5,p->ctx);
   (void)p->restore_protection(call_site,5,p->ctx);
 }
 return 0;
}
