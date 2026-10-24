/* CR01 CallGate01 guarded Windows x64 adapter.
   SOURCE-ONLY. Not production-enabled; no hook is invoked automatically.
   A production bootstrap owner must establish early pre-registration completion
   AND shared PT shader readiness/acceptance before any activation. */
#include "cr01_gate_live_adapter.h"
#include "cr01_dxr_submission_gate.h"
#include <stdint.h>
#include <stddef.h>
#if !defined(_WIN64) && !defined(_M_X64)
#error Windows x64 only
#endif
#define WIN __stdcall
#define IMP __declspec(dllimport)
#define NOINLINE __declspec(noinline)
typedef void* HANDLE;
typedef void* HMODULE;
typedef unsigned long DWORD;
typedef int BOOL;
typedef unsigned short WCHAR;
typedef uintptr_t HCRYPTPROV;
typedef uintptr_t HCRYPTHASH;
#define PAGE_EXECUTE_READWRITE 0x40ul
#define PAGE_READWRITE 0x04ul
#define PAGE_EXECUTE_READ 0x20ul
#define MEM_COMMIT 0x1000ul
#define MEM_RESERVE 0x2000ul
#define MEM_RELEASE 0x8000ul
#define GENERIC_READ 0x80000000ul
#define FILE_SHARE_READ 1ul
#define FILE_SHARE_WRITE 2ul
#define FILE_SHARE_DELETE 4ul
#define OPEN_EXISTING 3ul
#define FILE_ATTRIBUTE_NORMAL 0x80ul
#define CRYPT_VERIFYCONTEXT 0xf0000000ul
#define PROV_RSA_AES 24ul
#define CALG_SHA_256 0x800cul
#define HP_HASHVAL 2ul
#define MFLAG_PIN_FROM_ADDRESS 5ul
IMP HMODULE WIN GetModuleHandleW(const WCHAR*);
IMP DWORD WIN GetModuleFileNameW(HMODULE,WCHAR*,DWORD);
IMP HANDLE WIN CreateFileW(const WCHAR*,DWORD,DWORD,void*,DWORD,DWORD,HANDLE);
IMP BOOL WIN ReadFile(HANDLE,void*,DWORD,DWORD*,void*);
IMP BOOL WIN CloseHandle(HANDLE);
IMP BOOL WIN CryptAcquireContextW(HCRYPTPROV*,const WCHAR*,const WCHAR*,DWORD,DWORD);
IMP BOOL WIN CryptCreateHash(HCRYPTPROV,DWORD,HCRYPTPROV,DWORD,HCRYPTHASH*);
IMP BOOL WIN CryptHashData(HCRYPTHASH,const unsigned char*,DWORD,DWORD);
IMP BOOL WIN CryptGetHashParam(HCRYPTHASH,DWORD,unsigned char*,DWORD*,DWORD);
IMP BOOL WIN CryptDestroyHash(HCRYPTHASH);
IMP BOOL WIN CryptReleaseContext(HCRYPTPROV,DWORD);
IMP void* WIN VirtualAlloc(void*,size_t,DWORD,DWORD);
IMP BOOL WIN VirtualFree(void*,size_t,DWORD);
IMP BOOL WIN VirtualProtect(void*,size_t,DWORD,DWORD*);
IMP BOOL WIN FlushInstructionCache(HANDLE,const void*,size_t);
IMP HANDLE WIN GetCurrentProcess(void);
IMP BOOL WIN GetProcessMitigationPolicy(HANDLE,int,void*,size_t);
IMP BOOL WIN GetModuleHandleExW(DWORD,const WCHAR*,HMODULE*);
static CR01GateState g_cr01_gate={0};
/* Separate atomic generation-paired PT-only feature payload.  No changes to
 * C17 FIX1 gate packing or full-width 32-bit epoch publication. */
static uint64_t g_cr01_rtao_generation=0;
static void (*g_native_writer)(void*,void*,void*,void*)=0;
static uint8_t*g_relay=0;
static uint8_t*g_owned_call=0;
static int32_t g_owned_disp=0;
static int32_t g_original_disp=0;
static DWORD g_saved_page_protection=0;
static int g_original_protection_captured=0;
static HMODULE g_pinned_module=0;
static volatile DWORD g_install_attempted=0;
/* R11: only the proven Patch 5.01 Geometry/Indirect store is bridged in-function.
   The trampoline itself contains no conditional instructions and therefore
   preserves native RFLAGS semantics. Hair remains on the stable R9 path. */
static volatile uint32_t g_infunc_packed=2u;
static uint8_t g_bounce_trampoline[96];
static uint8_t *g_bounce_site=0;
static uint8_t g_bounce_original[17];
static DWORD g_bounce_site_protection=0;
static DWORD g_bounce_trampoline_protection=0;
static int g_bounce_installed=0;
/* V5: executable identity (exact or code-identical) is computed once and
 * cached in urt_exe_identity_win.c; cr01_gate_verify_exe_file lives there. */
/* Do not trust only the original on-disk EXE: a second mod could already
 * have modified the PT container in mapped process memory. This is a one-shot
 * 231144-byte SHA-256 prior to installing the registration gateway. */
int cr01_gate_verify_native_blob(const uint8_t*data,size_t len){
 static const uint8_t required[32]={
  0x63,0x82,0x8a,0xe5,0x5a,0x44,0xce,0x1a,
  0xb0,0x41,0x06,0x5b,0xa4,0xe3,0x16,0x9e,
  0x0b,0x33,0xc2,0x77,0x94,0x3b,0x19,0x54,
  0x5b,0xe7,0xa8,0x10,0xae,0x04,0x81,0x21};
 HCRYPTPROV provider=0;HCRYPTHASH digest=0;
 unsigned char actual[32];DWORD count=32;int valid=0;
 if(!data||len!=231144u)return 0;
 if(!CryptAcquireContextW(&provider,0,0,PROV_RSA_AES,CRYPT_VERIFYCONTEXT))return 0;
 if(!CryptCreateHash(provider,CALG_SHA_256,0,0,&digest))goto done;
 if(!CryptHashData(digest,data,(DWORD)len,0))goto done;
 if(!CryptGetHashParam(digest,HP_HASHVAL,actual,&count,0)||count!=32)goto done;
 valid=1;
 for(unsigned i=0;i<32;i++)if(actual[i]!=required[i]){valid=0;break;}
 done:
 if(digest)CryptDestroyHash(digest);
 if(provider)CryptReleaseContext(provider,0);
 return valid;
}
extern int cr01_shader_slots_active(void);
static uint32_t cr01_infunc_next_packed(int custom_live){
 uint64_t word=cr01_gate_snapshot(&g_cr01_gate);
 uint32_t bits=(uint32_t)word;
 uint32_t geom=bits&255u,indirect=(bits>>16)&255u;
 /* bits 24/25 are Master/PT. Native instruction value 2 is used whenever
    the custom PT pair is not actually live or CR01 is not renderer-eligible. */
 if(!custom_live||((bits>>24)&3u)!=3u||geom<1u||geom>6u||indirect<100u)return 2u;
 /* R15 uses the exact 100..150 fixed-point slider. */
 uint32_t shader_indirect=indirect;
 if(shader_indirect>150u)return 2u;
 uint32_t base=geom|(shader_indirect<<8);
 uint64_t extra=__atomic_load_n(&g_cr01_rtao_generation,__ATOMIC_ACQUIRE);
 uint32_t generation=(uint32_t)(word>>32);
 uint32_t extra_generation=(uint32_t)(extra>>32);
 CR01RtaoSettings decoded={0};
 uint32_t code=(uint32_t)extra;
 /* The full-generation guard rejects torn publication, stale PT/RT
  * transitions or an uninitialized feature carrier. */
 decoded.mode=(code>>24)&1u;
 decoded.rays=(code>>16)&3u;
 decoded.radius=(code>>18)&7u;
 decoded.strength=(code>>21)&7u;
 return cr01_rtao_apply(base,&decoded,(bits>>24)&1u,(bits>>25)&1u,
                        (unsigned)custom_live,generation,extra_generation);
}
/* Hook entry is the stable R9 wrapper. It still performs post-return staging for
   compatibility, and additionally arms the next native in-function store using
   only atomics. No render-thread Win32 signalling or logging is introduced. */
NOINLINE void cr01_gate_win_entry(void*dst,void*src,void*r8,void*r9){
 int custom;
 if(!g_native_writer)return; /* Never possible after successful commit. */
 g_native_writer(dst,src,r8,r9);
 custom=cr01_shader_slots_active();
 (void)cr01_gate_post_serialize(&g_cr01_gate,dst,custom);
 uint32_t next=cr01_infunc_next_packed(custom);
 /* Match the existing synchronous CPU-owned serializer destination; when
  * the PT-only overlay is active at neutral bounce defaults, explicitly
  * publish the encoded value even though legacy C17 would write nothing. */
 if(custom && dst && (next & (1u<<24))){
   __builtin_memcpy((uint8_t*)dst+CR01_GEOM_OFFSET,&next,sizeof(next));
 }
 __atomic_store_n(&g_infunc_packed,next,__ATOMIC_RELEASE);
}
static void wr32le(uint8_t*p,uint32_t v){for(unsigned i=0;i<4;i++)p[i]=(uint8_t)(v>>(8u*i));}
static void wr64le(uint8_t*p,uint64_t v){for(unsigned i=0;i<8;i++)p[i]=(uint8_t)(v>>(8u*i));}
static unsigned emit_abs_jump(uint8_t*p,uintptr_t target){
 p[0]=0xff;p[1]=0x25;for(unsigned i=2;i<6;i++)p[i]=0;wr64le(p+6,(uint64_t)target);return 14u;
}
static int bytes_equal(const uint8_t*a,const uint8_t*b,unsigned n){for(unsigned i=0;i<n;i++)if(a[i]!=b[i])return 0;return 1;}
static int bounce_jump_owned(const uint8_t*site,const uint8_t*target){
 uint8_t e[17];emit_abs_jump(e,(uintptr_t)target);for(unsigned i=14;i<17;i++)e[i]=0x90;
 return bytes_equal(site,e,17);
}
static int build_bounce_trampoline(uint8_t*site){
 unsigned n=0;intptr_t rel;uint8_t*next;
 /* Exact old working bridge shape: push rax; mov eax,[rip+packed];
    mov [rbx+210],eax; pop rax; mov [rbx+214],r8d; jump back.
    MOV/PUSH/POP/JMP do not modify arithmetic flags. */
 g_bounce_trampoline[n++]=0x50;
 g_bounce_trampoline[n++]=0x8b;g_bounce_trampoline[n++]=0x05;
 next=g_bounce_trampoline+n+4;rel=(intptr_t)(const void*)&g_infunc_packed-(intptr_t)next;
 if(rel<INT32_MIN||rel>INT32_MAX)return 0;wr32le(g_bounce_trampoline+n,(uint32_t)(int32_t)rel);n+=4;
 {const uint8_t q[]={0x89,0x83,0x10,0x02,0,0};for(unsigned i=0;i<sizeof(q);i++)g_bounce_trampoline[n++]=q[i];}
 g_bounce_trampoline[n++]=0x58;
 {const uint8_t q[]={0x44,0x89,0x83,0x14,0x02,0,0};for(unsigned i=0;i<sizeof(q);i++)g_bounce_trampoline[n++]=q[i];}
 n+=emit_abs_jump(g_bounce_trampoline+n,(uintptr_t)(site+17));
 return n<=sizeof(g_bounce_trampoline);
}
static int install_bounce_bridge(uint8_t*site){
 static const uint8_t expected[17]={0xc7,0x83,0x10,0x02,0,0,2,0,0,0,0x44,0x89,0x83,0x14,0x02,0,0};
 DWORD old=0,tmp=0;
 if(!site||!bytes_equal(site,expected,17)||!build_bounce_trampoline(site))return 0;
 for(unsigned i=0;i<17;i++)g_bounce_original[i]=site[i];
 if(!VirtualProtect(g_bounce_trampoline,sizeof(g_bounce_trampoline),PAGE_EXECUTE_READWRITE,&old))return 0;
 g_bounce_trampoline_protection=old;
 if(!FlushInstructionCache(GetCurrentProcess(),g_bounce_trampoline,sizeof(g_bounce_trampoline)))goto fail_tramp;
 if(!VirtualProtect(site,17,PAGE_EXECUTE_READWRITE,&old))goto fail_tramp;
 g_bounce_site_protection=old;
 emit_abs_jump(site,(uintptr_t)g_bounce_trampoline);for(unsigned i=14;i<17;i++)site[i]=0x90;
 if(!FlushInstructionCache(GetCurrentProcess(),site,17))goto fail_site;
 if(!VirtualProtect(site,17,old,&tmp))goto fail_site;
 g_bounce_site=site;g_bounce_installed=1;return 1;
fail_site:
 {volatile uint8_t*v=site;for(unsigned i=0;i<17;i++)v[i]=g_bounce_original[i];}
 (void)FlushInstructionCache(GetCurrentProcess(),site,17);(void)VirtualProtect(site,17,old,&tmp);
fail_tramp:
 (void)VirtualProtect(g_bounce_trampoline,sizeof(g_bounce_trampoline),g_bounce_trampoline_protection,&tmp);
 return 0;
}
static int revert_bounce_bridge(void){
 DWORD old=0,tmp=0;
 __atomic_store_n(&g_infunc_packed,2u,__ATOMIC_RELEASE);
 if(!g_bounce_installed||!g_bounce_site)return 0;
 if(!bounce_jump_owned(g_bounce_site,g_bounce_trampoline))return 0;
 if(!VirtualProtect(g_bounce_site,17,PAGE_EXECUTE_READWRITE,&old))return 0;
 {volatile uint8_t*v=g_bounce_site;for(unsigned i=0;i<17;i++)v[i]=g_bounce_original[i];}
 if(!FlushInstructionCache(GetCurrentProcess(),g_bounce_site,17)){(void)VirtualProtect(g_bounce_site,17,old,&tmp);return 0;}
 if(!VirtualProtect(g_bounce_site,17,g_bounce_site_protection?g_bounce_site_protection:old,&tmp))return 0;
 g_bounce_installed=0;return 1;
}
static int writable(void*site,size_t length,void*ignored){
 DWORD old=0;(void)ignored;
 if(!VirtualProtect(site,length,PAGE_EXECUTE_READWRITE,&old))return 0;
 if(!g_original_protection_captured){
  g_saved_page_protection=old;g_original_protection_captured=1;
 }return 1;
}
static int restore_page(void*site,size_t length,void*ignored){
 DWORD ignored_old=0;(void)ignored;
 return VirtualProtect(site,length,g_saved_page_protection,&ignored_old)!=0;
}
static int flush(void*site,size_t length,void*ignored){
 (void)ignored;return FlushInstructionCache(GetCurrentProcess(),site,length)!=0;
}
/* Every candidate relay is within signed rel32 reach and page-aligned to the
   usual Windows allocation granularity (64KiB). If there is no suitable free
   nearby page, abort without touching the original callsite. */
static uint8_t*alloc_relay_near(uintptr_t call){
 uintptr_t center=call&~(uintptr_t)0xffffu;
 for(uintptr_t delta=0x10000u;delta<=0x10000000u;delta+=0x10000u){
  for(unsigned sign=0;sign<2;sign++){
   uintptr_t hint=sign?(center>=delta?center-delta:0):center+delta;
   if(!hint||hint<0x10000u||hint>INTPTR_MAX||
      hint>(uintptr_t)INT32_MAX+call||call>hint+(uintptr_t)INT32_MAX)continue;
   void *p=VirtualAlloc((void*)hint,0x1000u,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
   if(p)return (uint8_t*)p;
  }
 }
 return 0;
}
/* Control owner calls on real menu/mode events. Caller MUST have validated
   custom shader GPU acceptance; Boolean assertions from mere DXIL validation
   are not sufficient. This function alone is NOT a renderer proof. */
/* Only the linked DXR submission witness AND the real registration hook may
 * approve staged GPU parameter changes. No caller-supplied acceptance flag.
 * State-object creation is still NOT proof of rendering use or visual parity. */

int cr01_gate_win_publish_settings(const CR01Settings *saved,
   uint32_t known_pt_active,uint32_t epoch){
 int ok,custom;
 if(!g_owned_call)return 0;
 /* Stage validated preferences immediately. The stable wrapper still checks
    verified native-slot custom shader installation on render calls. When already live,
    arm the in-function packed store before the next serializer invocation. */
 /* Pair every RTAO preference generation with the existing atomic C17
  * epoch so render-side cross-generation snapshots fail closed. */
 if(!cr01_rtao_valid(&saved->rtao))return 0;
 uint32_t overlay=cr01_rtao_overlay(&saved->rtao);
 __atomic_store_n(&g_cr01_rtao_generation,
                   ((uint64_t)epoch<<32)|overlay,__ATOMIC_RELEASE);
 ok=cr01_gate_publish_e06(&g_cr01_gate,saved,known_pt_active,1u,1u,epoch);
 if(!ok)return 0;
 custom=cr01_shader_slots_active();
 __atomic_store_n(&g_infunc_packed,cr01_infunc_next_packed(custom),__ATOMIC_RELEASE);
 return 1;
}

/* Bootstrap owner calls this only after establishing ordering and thread
   quiescence for the hook callsite. No automatic DllMain/worker installation.
   Returns false without changes on unknown build/layout, denied dynamic code,
   missing entry guarantees, or competing callsite owner. */
int cr01_gate_win_install(int proven_early_before_renderer,int proven_site_quiescent){
 uint8_t *base,*call,*geo,*hair,*relay;
 DWORD dynFlags=0,old=0;
 int32_t orig=0,repl=0;
 CR01CallPlatform hooks={writable,restore_page,flush,0};
 uint64_t wrapper_addr=(uint64_t)(uintptr_t)&cr01_gate_win_entry;
 if(__sync_val_compare_and_swap(&g_install_attempted,0,1)!=0)return 0;
 if(!proven_early_before_renderer||!proven_site_quiescent)return 0;
 if(!cr01_gate_verify_exe_file())return 0;
 if(!GetProcessMitigationPolicy(GetCurrentProcess(),2,&dynFlags,sizeof(dynFlags)))return 0;
 if(dynFlags&1u)return 0;/* ProhibitDynamicCode */
 base=(uint8_t*)GetModuleHandleW(0);
 if(!base)return 0;
 call=base+CR01_CALL_RVA;geo=base+CR01_GEOM_RVA;hair=base+CR01_HAIR_RVA;
 relay=alloc_relay_near((uintptr_t)call);
 if(!relay)return 0;
 if(!cr01_gate_exact_call_preflight(call,geo,hair,(uintptr_t)call,
     (uintptr_t)(base+CR01_WRITER_RVA),(uintptr_t)relay,&orig,&repl))goto fail;
 /* Pin before installation. No unload can free a referenced code target.
    The permanent one-shot lifetime is intentional; rollback is next launch. */
 if(!GetModuleHandleExW(MFLAG_PIN_FROM_ADDRESS,(const WCHAR*)(const void*)&cr01_gate_win_entry,
                        &g_pinned_module)||!g_pinned_module)goto fail;
 g_native_writer=(void(*)(void*,void*,void*,void*))(void*)(base+CR01_WRITER_RVA);
 __atomic_store_n(&g_infunc_packed,2u,__ATOMIC_RELEASE);
 if(!install_bounce_bridge(geo))goto fail;
 /* JMP [RIP+0] + pointer: does not clobber volatile argument registers. */
 relay[0]=0xff;relay[1]=0x25;
 for(unsigned i=2;i<6;i++)relay[i]=0;
 for(unsigned i=0;i<8;i++)relay[6+i]=(uint8_t)(wrapper_addr>>(i*8));
 if(!VirtualProtect(relay,0x1000,PAGE_EXECUTE_READ,&old)||
    !FlushInstructionCache(GetCurrentProcess(),relay,14))goto fail_bridge;
 if(!cr01_gate_commit_operand(call,orig,repl,&hooks)){
    /* If the stable call wrapper cannot be committed, restore our own bounce
       bridge while startup is still externally quiescent. */
    (void)revert_bounce_bridge();
    return 0;
 }
 g_owned_call=call;g_owned_disp=repl;g_original_disp=orig;g_relay=relay;
 return 1;
 fail_bridge:
 (void)revert_bounce_bridge();
 fail:
 if(relay)VirtualFree(relay,0,MEM_RELEASE); /* only before callsite commit */
 return 0;
}
/* A native source rollback requires process restart. Never unload an ASI that
   could remain referenced by an in-flight engine call. No live unhook API. */

/* Startup-quiescent inverse CAS; never unload/free referenced code while a
 * thread could still be using the relay. Native CPU parameters are selected
 * BEFORE touching a live operand. */
int cr01_gate_win_revert_quiescent(void){
 int call_ok,bounce_ok;
 __atomic_store_n(&g_infunc_packed,2u,__ATOMIC_RELEASE);
 __atomic_store_n(&g_cr01_gate.word,0u,__ATOMIC_RELEASE);
 __atomic_store_n(&g_cr01_rtao_generation,0u,__ATOMIC_RELEASE);
 if(!g_owned_call||!g_relay)return 0;
 CR01CallPlatform hooks={writable,restore_page,flush,0};
 call_ok=cr01_gate_commit_operand(g_owned_call,g_owned_disp,g_original_disp,&hooks);
 bounce_ok=revert_bounce_bridge();
 if(!call_ok||!bounce_ok)return 0;
 g_owned_call=0;
 return 1;
}
