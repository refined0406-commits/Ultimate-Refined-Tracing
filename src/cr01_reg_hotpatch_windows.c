/* Patch 5.01 startup-only atomic entry hotpatch. This has no DllMain,
 * no self-activation, and is NOT production authorized before startup-order
 * and GPU acceptance proofs. No DXR construction is modified here. */
#include "cr01_reg_hotpatch_core.h"
#include "cr01_register_sites.h"
#include "cr01_gate_core.h"
#include "cr01_registration_pair.h"
#include <stdint.h>
#include <stddef.h>
#if !defined(_WIN64) && !defined(_M_X64)
#error Windows x64 only
#endif
#define WIN __stdcall
#define IMP __declspec(dllimport)
#define PAGE_RW 0x04ul
#define PAGE_RX 0x20ul
#define PAGE_RWX 0x40ul
#define MEM_COMMIT 0x1000ul
#define MEM_RESERVE 0x2000ul
#define MEM_RELEASE 0x8000ul
#define PIN_FROM_ADDRESS 5ul
#define IMPL_NOINLINE __declspec(noinline)
typedef unsigned long DWORD;typedef int BOOL;typedef unsigned short WCHAR;
typedef void* HANDLE;typedef void* HMODULE;
IMP HMODULE WIN GetModuleHandleW(const WCHAR*);
IMP BOOL WIN GetModuleHandleExW(DWORD,const WCHAR*,HMODULE*);
IMP void* WIN VirtualAlloc(void*,size_t,DWORD,DWORD);
IMP BOOL WIN VirtualFree(void*,size_t,DWORD);
IMP BOOL WIN VirtualProtect(void*,size_t,DWORD,DWORD*);
IMP BOOL WIN FlushInstructionCache(HANDLE,const void*,size_t);
IMP HANDLE WIN GetCurrentProcess(void);
IMP BOOL WIN GetProcessMitigationPolicy(HANDLE,int,void*,size_t);
/* The helper has five ABI parameters at both protected PT registration sites:
   RCX owner, RDX DXIL pointer, R8 size, R9 context, fifth stack argument.
   Caller setup at 0x1EE6DA2..6DC6 and 0x1EE6FCB..6FF3 is fingerprinted.
   RAX return value is forwarded unchanged. */
typedef uintptr_t (WIN *CR01RegOriginal)(void*,const uint8_t*,size_t,void*,void*);
static CR01RegOriginal g_original;
static CR01RegistrationPair*g_pair;
static uint8_t*g_relay;
static HMODULE g_pinned;
static volatile uint32_t g_attempted;
static volatile uint32_t g_healthy;
static volatile uint32_t g_installed;
static uint8_t* g_helper;
static uint8_t g_expected_pad[8];

static DWORD g_old_page;
static int g_original_page_captured;
IMPL_NOINLINE uintptr_t WIN cr01_native_reg_entry(void*owner,const uint8_t*bytes,
                             size_t size,void*context,void*arg5){
 CR01RegOriginal original=g_original;
 CR01RegistrationPair*p=g_pair;
 if(!original)return 0; /* Installation cannot succeed without trampoline. */
 if(p&&__atomic_load_n(&g_healthy,__ATOMIC_ACQUIRE)){
  uintptr_t origin=(uintptr_t)__builtin_return_address(0);
  CR01PairSelection s=cr01_pair_select(p,origin,bytes,size);
  if(s.classification==CR01_PAIR_CUSTOM && s.data && s.size){
   bytes=s.data;size=s.size;
  }
 }
 return original(owner,bytes,size,context,arg5);
}
static int w_write(void*a,size_t n,void*ctx){
 DWORD old=0;(void)ctx;
 if(!VirtualProtect(a,n,PAGE_RWX,&old))return 0;
 /* Rollback reacquisition MUST NOT replace the true original protection
    with the transient RWX value from an earlier failed restore. */
 if(!g_original_page_captured){g_old_page=old;g_original_page_captured=1;}
 return 1;
}
static int w_restore(void*a,size_t n,void*ctx){DWORD ignored;(void)ctx;return VirtualProtect(a,n,g_old_page,&ignored)!=0;}
static int w_flush(void*a,size_t n,void*ctx){(void)ctx;return FlushInstructionCache(GetCurrentProcess(),a,n)!=0;}
static uint8_t*alloc_near(uintptr_t entry){
 uintptr_t center=entry&~(uintptr_t)0xffffu;
 for(uintptr_t delta=0x10000u;delta<=0x10000000u;delta+=0x10000u){
  for(unsigned sign=0;sign<2;sign++){
   uintptr_t addr=sign?(center>=delta?center-delta:0):center+delta;
   if(!addr||addr>INTPTR_MAX||addr<0x10000u)continue;
   if((addr>entry && addr-entry>INT32_MAX) ||
      (entry>addr && entry-addr>INT32_MAX))continue;
   void*p=VirtualAlloc((void*)addr,0x1000,MEM_RESERVE|MEM_COMMIT,PAGE_RW);
   if(p)return (uint8_t*)p;
  }
 }
 return 0;
}
/* Required caller attests that this is early startup before ANY helper use or
   has independently established full thread quiescence. A Boolean assertion
   here is NOT itself proof. Do not call from DllMain. */
int cr01_native_reg_install(CR01RegistrationPair*pair,int early,int quiescent){
 uint8_t*base,*helper,*relay;uint8_t pad[8],code[CR01_REG_RELAY_LENGTH];
 DWORD old=0,dyn=0,cfg=0,cet=0;
 CR01HotpatchOps ops={w_write,w_restore,w_flush,0};
 if(__sync_val_compare_and_swap(&g_attempted,0u,1u)!=0u)return 0;
 if(!pair||!early||!quiescent||!pair->original||
    !cr01_gate_verify_exe_file())return 0;
 if(!GetProcessMitigationPolicy(GetCurrentProcess(),2,&dyn,sizeof(dyn))||
    !GetProcessMitigationPolicy(GetCurrentProcess(),7,&cfg,sizeof(cfg))||
    !GetProcessMitigationPolicy(GetCurrentProcess(),15,&cet,sizeof(cet))||
    (dyn&1u)||(cfg&1u)||(cet&1u))return 0;
 base=(uint8_t*)GetModuleHandleW(0);
 if(!base || pair->original!=base+CR01_NATIVE_BLOB_RVA ||
    pair->original_size!=CR01_NATIVE_BLOB_LENGTH ||
    !cr01_gate_verify_native_blob(pair->original,pair->original_size) ||
    pair->expected_final_return!=(uintptr_t)(base+CR01_PT_FINAL_CALL_RVA+5u)||
    pair->expected_ser_return!=(uintptr_t)(base+CR01_PT_SER_CALL_RVA+5u)||
    !cr01_register_sites_preflight(base,CR01_PT_SER_CALL_RVA+6u))return 0;
 helper=base+CR01_HELPER_RVA;
 relay=alloc_near((uintptr_t)helper);if(!relay)return 0;
 if(!cr01_hotpatch_build(helper,(uintptr_t)helper,(uintptr_t)relay,
                         (uintptr_t)&cr01_native_reg_entry,pad,code))goto fail;
 /* Pin module before creating even one reachable executable pointer. */
 if(!GetModuleHandleExW(PIN_FROM_ADDRESS,
            (const WCHAR*)(const void*)&cr01_native_reg_entry,&g_pinned) || !g_pinned)goto fail;
 for(unsigned i=0;i<CR01_REG_RELAY_LENGTH;i++)relay[i]=code[i];
 if(!VirtualProtect(relay,0x1000,PAGE_RX,&old) ||
    !FlushInstructionCache(GetCurrentProcess(),relay,CR01_REG_RELAY_LENGTH))goto fail_pinned;
 g_relay=relay;g_original=(CR01RegOriginal)(void*)(relay+CR01_REG_TRAMPOLINE_OFFSET);
 g_pair=pair;
 /* Published hook MUST forward native until the separate serializer
    hook also commits. A success return is not permission to select custom. */
 __atomic_store_n(&g_healthy,0u,__ATOMIC_RELEASE);
 if(!cr01_hotpatch_commit(helper,pad,&ops)){
  /* This can leave a reachable published hook after a rollback failure.
   * We pin the library for life and prevent custom selection even if so. */
  __atomic_store_n(&g_healthy,0u,__ATOMIC_RELEASE);
  return 0;
 }
 g_helper=helper;
 for(unsigned i=0;i<8;i++)g_expected_pad[i]=pad[i];
 __atomic_store_n(&g_installed,1u,__ATOMIC_RELEASE);
 return 1;
fail_pinned:
 /* The module remains pinned, but relay never became reachable from helper. */
fail:
 if(!g_relay && relay)VirtualFree(relay,0,MEM_RELEASE);
 return 0;
}

/* This publication occurs only after both independent hook commits.
 * The caller must still establish actual startup ownership and quiescence. */
int cr01_native_reg_arm(void){
 uint8_t*helper=g_helper;
 CR01RegistrationPair*p=g_pair;
 if(!helper||!p||!__atomic_load_n(&g_installed,__ATOMIC_ACQUIRE)||
    !g_original||!g_relay||!g_pinned||
    __atomic_load_n(&p->fault,__ATOMIC_ACQUIRE)||
    __atomic_load_n(&p->seen_mask,__ATOMIC_ACQUIRE)||
    __atomic_load_n(&p->startup.registration_policy,__ATOMIC_ACQUIRE)!=CR01_REG_UNDECIDED||
    __atomic_load_n(&p->startup.stage,__ATOMIC_ACQUIRE)!=CR01_BS_PREPARING)
  return 0;
 /* Competing code owners or a mid-preparation native registration forbid
    promotion, even if both physical hooks were installed earlier. */
 if(helper[0]!=0xeb||helper[1]!=0xf6||helper[-8]!=0xe9)return 0;
 __atomic_store_n(&g_healthy,1u,__ATOMIC_RELEASE);
 return 1;
}
void cr01_native_reg_force_native(void){
 __atomic_store_n(&g_healthy,0u,__ATOMIC_RELEASE);
 CR01RegistrationPair*p=g_pair;
 if(p){
  uint32_t expected=CR01_REG_UNDECIDED;
  (void)__atomic_compare_exchange_n(&p->startup.registration_policy,&expected,
    CR01_REG_NATIVE,0,__ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE);
 }
}

/* Only invoked in the SAME externally proven exclusive startup window.
 * A failed rollback leaves neutral, permanently pinned code in place. */
int cr01_native_reg_revert_quiescent(void){
 cr01_native_reg_force_native();
 if(!__atomic_load_n(&g_installed,__ATOMIC_ACQUIRE)||!g_helper)return 0;
 CR01HotpatchOps ops={w_write,w_restore,w_flush,0};
 if(!cr01_hotpatch_uncommit(g_helper,g_expected_pad,&ops))return 0;
 __atomic_store_n(&g_installed,0u,__ATOMIC_RELEASE);
 return 1;
}

/* Production-only: verified protected pair committed under this hook.
 * Unrelated helper registrations and caller-provided GPU readiness flags
 * cannot satisfy this predicate. */
int cr01_native_reg_both_custom(void){
 CR01RegistrationPair*p=g_pair;
 return p&&__atomic_load_n(&g_installed,__ATOMIC_ACQUIRE)&&
 __atomic_load_n(&g_healthy,__ATOMIC_ACQUIRE)&&
 __atomic_load_n(&p->seen_mask,__ATOMIC_ACQUIRE)==3u&&
 !__atomic_load_n(&p->fault,__ATOMIC_ACQUIRE)&&
 __atomic_load_n(&p->startup.registration_policy,__ATOMIC_ACQUIRE)==CR01_REG_CUSTOM;
}
