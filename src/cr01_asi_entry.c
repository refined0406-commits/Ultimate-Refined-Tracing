/* CR01 Patch 5.01 native DXR runtime.
 * Heavy preparation always runs outside DllMain, via the proven DI gateway
 * or the optional standards-based InitializeASI callback. Every caller
 * shares a one-shot safety gate and the existing early-eligibility checks. */
#include "urt_build.h"
#include "cr01_bootstrap_owner.h"
#include "cr01_release_log.h"
#include "cr01_register_sites.h"
#include "cr01_gate_core.h"
#include "cr01_startup_barrier.h"
#include "urt_exe_identity.h"
#include "urt_environment.h"
#include "urt_runtime.h"
#include <stdint.h>
#include <stddef.h>
#define IMP __declspec(dllimport)
#define EXP __declspec(dllexport)
#define WIN __stdcall
typedef void* HMODULE;typedef unsigned short WCHAR;typedef unsigned long DWORD;
IMP HMODULE WIN GetModuleHandleW(const WCHAR*);
IMP HMODULE WIN LoadLibraryW(const WCHAR*);
IMP void* WIN GetProcAddress(HMODULE,const char*);
IMP int WIN DisableThreadLibraryCalls(HMODULE);
extern int cr01_di_bind_preparation(CR01BootstrapOwner*,CR01PrepareOneContainer,void*);
extern int cr01_di_prepare_synchronously(void);
extern int cr01_gate_verify_exe_file(void);
extern int cr01_gate_verify_native_blob(const uint8_t*,size_t);
extern int cr01_prod_early501_enter(void);
extern void cr01_prod_early501_exit(void);
extern int cr01_prod_runtime501_start(void);
extern int cr01_shadow501_prepare(uint8_t*,uint32_t);
extern int cr01_register_sites_preflight(const uint8_t*,size_t);
extern int cr01_pair_init(CR01RegistrationPair*,const uint8_t*,size_t,uintptr_t,uintptr_t);
/* Signed native shader plus early virgin-site checks. Publication uses the
 * protected atomic hooks; actual GPU visual parity awaits private game QA. */
extern const uint8_t*cr01_approved_shared_pt(size_t*);
extern int cr01_pre_registration_quiescence_approved(void);
extern int cr01_custom_shader_identity(const uint8_t*,size_t);
extern int cr01_custom_shader_identity_register(const uint8_t*,size_t);
#include "cr01_pt_shared_dxil.h"
#include "urt_bounce501.h"
#include "urt_bounce_boot501.h"
extern int cr01_custom_shader_identity_register_variant(const uint8_t*,size_t);
extern void cr01_gate_set_bounce_level(unsigned);
extern int cr01_gate_win_install(int,int);
extern int cr01_shader_slots_install(uint8_t*,const uint8_t*,size_t);
extern int cr01_shader_slots_commit(void);
extern int cr01_shader_slots_rollback(void);
static CR01BootstrapOwner g_owner;
static volatile uint32_t g_preflight_once;
static int CR01_prepare_shader(void*ignored,const uint8_t**out,size_t*n){
 const uint8_t*verified=0;size_t count=0;uint8_t*exe=(uint8_t*)GetModuleHandleW(0);
 HMODULE dxc=0;CR01DxcCreate create=0;const uint8_t*native=0;
 (void)ignored;
 if(!out||!n)return 0;*out=0;*n=0;
 if(!exe||!cr01_pre_registration_quiescence_approved())return 0;
 native=exe+CR01_NATIVE_BLOB_RVA;
 /* R14 diagnostic only: build a freshly DXC-validated container from the
    exact signed native 5.01 shader, adding the final-visible-RGB probe while
    preserving Geometry/Hair and Witcher's native DXR ownership. */
 dxc=GetModuleHandleW(L"dxcompiler.dll");
 if(!dxc)dxc=LoadLibraryW(L"dxcompiler.dll");
 if(!dxc)return 0;
 create=(CR01DxcCreate)(void*)GetProcAddress(dxc,"DxcCreateInstance");
 if(!create||!cr01_build_shared_dxil(native,CR01_NATIVE_BLOB_LENGTH,create,&verified,&count))return 0;
 if(!cr01_custom_shader_identity_register(verified,count)||!cr01_custom_shader_identity(verified,count))return 0;
 /* V5.3.0 R11/R12: PT: Second Bounce Coverage / PT: First Bounce Diffuse Coverage (urt_bounce501.c). Either not Native
    at this start: a variant of the library just verified (both coverage edits + Orange Hair Fix's PT hair patch) replaces
    it, or nothing changes. Both Native: untouched. The packed level (bits 0-1 / 2-3) reaches the shader via the gate. */
 {const unsigned level=urt_bc_startup_level();
  if(level){
   uint8_t*v=0;uint32_t vn=0;const char*why=0;
   /* R12 fix: each stage reports its own reason (urt_bc_diag: compiler, disassembly, text passes, assembly, container,
      validation, re-check; here: slot capacity and identity) */
   int ok=urt_bc_build_variant_lv(verified,(uint32_t)count,(UrtBcDxcCreate)create,&v,&vn,level);   /* R12 FIX3: module 4 only when set */
   if(!ok)why=urt_bc_diag();
   else if(vn<200000u||vn>253288u){ok=0;why="11 slot capacity: the variant does not fit the PT shader slot (native 231144 + 22144 free bytes = 253288 max)";}
   else if(!cr01_custom_shader_identity_register_variant(v,vn)||!cr01_custom_shader_identity(v,vn)){ok=0;why="12 identity: the variant could not be registered as URT's PT library";}
   if(ok){verified=v;count=vn;cr01_gate_set_bounce_level(level);}
   else if(!cr01_custom_shader_identity_register(verified,count))return 0;   /* back to URT's verified library */
   urt_bc_note_result(level,ok,vn,why);
  }}
 if(!cr01_shader_slots_install(exe,verified,count))return 0;
 if(!cr01_gate_win_install(1,1)){
  (void)cr01_shader_slots_rollback();return 0;
 }
 if(!cr01_shader_slots_commit())return 0;
 *out=verified;*n=count;
 return 1;
}

/* V5 startup. Executable identity is computed once (urt_exe_identity).
 * The runtime worker no longer depends on PT shader preparation succeeding:
 *   tier FULL     known code, early (pre-renderer): shader + code patches +
 *                 runtime features, exactly the V4.3.1 path;
 *   tier LATE     known code but attached after renderer start (late loader):
 *                 no shader/code-site patches, runtime CVar/CPU features run;
 *   tier ADAPTIVE unknown executable: only name-resolved, value-validated
 *                 CVar features; every code patch and the PT shader stay off.
 * (RC9: no user shader selector; URT always installs its PT shader on tier FULL.) */
extern int cr01_prod_runtime501_start_tier(int tier);
extern void urt_runtime_note_shader(int state);
EXP int WIN CR01EarlyPrepare(void){
 uint8_t*exe=(uint8_t*)GetModuleHandleW(0);
 const char*why=0;int prepared=0,early=0,tier;
 if(__sync_val_compare_and_swap(&g_preflight_once,0u,1u)!=0u)return 0;
 if(!urt_instance_claim(&why)){
  /* A legacy V4 runtime writes RefinedTracing.log, so our own log can record
   * why V5 stays dormant. A second V5 copy must not truncate the live log. */
  if(why&&why[0]=='l'){
   (void)cr01_release_log_begin();
   cr01_release_log_event("URT: dormant - legacy V4 RefinedTracing.asi owns this process. Remove bin\\x64_dx12\\RefinedTracing.asi (and the old URT dinput8.dll) to run V5");
  }
  return 0;
 }
 (void)cr01_release_log_begin();
 cr01_release_log_event("URT: loaded by the ASI loader; starting");
 (void)urt_bc_prepare_paths();   /* R12 FIX2: startup-guard file path, so a clean exit can be recorded from DllMain */
 int identity=urt_exe_identity();
 cr01_release_log_event(urt_exe_identity_name(identity));
 if(identity!=URT_EXE_UNKNOWN&&urt_build()==URT_BUILD_1090142)
  cr01_release_log_event("URT: game build 1090142: every URT code site, engine setting and the PT shader slots verified at their build-1090142 addresses (shaders and serializer code byte-identical to Patch 5.01)");
 if(!exe){cr01_release_log_event("URT: no executable module; fail-closed");return 0;}
 if(identity==URT_EXE_UNKNOWN){
  tier=URT_TIER_ADAPTIVE;
  urt_runtime_note_shader(URT_SHADER_UNSUPPORTED_EXE);
 }else if(!cr01_prod_early501_enter()){
  cr01_prod_early501_exit();
  tier=URT_TIER_LATE;
  urt_runtime_note_shader(URT_SHADER_LATE);
  cr01_release_log_event("URT: attached after renderer start or a code site is already owned by another mod: PT shader and code-site patches refused; runtime CVar/CPU features continue");
 }else{
  early=1;tier=URT_TIER_FULL;
  {
   if(cr01_register_sites_preflight(exe,CR01_PT_SER_CALL_RVA+6u)){
    const uint8_t*native=exe+CR01_NATIVE_BLOB_RVA;
    if(cr01_gate_verify_native_blob(native,CR01_NATIVE_BLOB_LENGTH)&&
       cr01_pair_init(&g_owner.pair,native,CR01_NATIVE_BLOB_LENGTH,
        (uintptr_t)(exe+CR01_PT_FINAL_CALL_RVA+5u),(uintptr_t)(exe+CR01_PT_SER_CALL_RVA+5u))&&
       cr01_di_bind_preparation(&g_owner,CR01_prepare_shader,0))
     prepared=cr01_di_prepare_synchronously();
   }
   urt_runtime_note_shader(prepared?URT_SHADER_ACTIVE:URT_SHADER_FAILED);
   cr01_release_log_event(prepared?"URT: PT shader registration/hook preparation complete":
     "URT: PT shader preparation refused (signature/DXC/validation); shader-driven controls stay native, other features continue");
  }
  /* PT shadow read redirection is an independent early code patch. V4.3.1
   * only installed it after shader success; V5 keeps it tied to a genuinely
   * early, known-code start and installs it when the shader was prepared,
   * never after an unexplained shader failure. */
  if(prepared)(void)cr01_shadow501_prepare(exe,urt_image_bytes());
  cr01_prod_early501_exit();
 }
 (void)early;
 if(cr01_prod_runtime501_start_tier(tier))cr01_release_log_event("URT: runtime worker started");
 else cr01_release_log_event("URT: runtime worker start refused; fail-closed");
 return prepared;
}
/* ThirteenAG Ultimate ASI Loader (including WINMM variants) invokes this
 * callback after LoadLibrary returns, outside the loader lock in its default
 * configuration. Reuse the SAME one-shot early bootstrap as our own DI proxy.
 * Do not bypass the original EXE, renderer, pristine-site or atomic guards.
 * Loader timing is not guaranteed: a late callback fails closed to native.
 * LoadLibrary-only loaders without InitializeASI support remain unsupported.
 * NEVER call this callback from DllMain. */
EXP void WIN InitializeASI(void){
 (void)CR01EarlyPrepare();
}
int WIN DllMain(HMODULE instance,DWORD reason,void*unused){
 if(reason==1u)DisableThreadLibraryCalls(instance);
 /* R12 FIX2: DLL_PROCESS_DETACH with a non-NULL reserved pointer = the process is ending normally (ExitProcess).
    A crash or a killed process never gets here. File I/O only (paths prepared at load); nothing else runs. */
 else if(reason==0u&&unused)urt_bc_clean_exit();
 return 1;
}
