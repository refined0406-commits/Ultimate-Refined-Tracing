/* Windows x64 synchronous bootstrap OWNER helper, not yet a DirectInput proxy.
   The loader must invoke from a verified DirectInput8Create entry outside
   the Windows loader lock. NEVER from DllMain. */
#include "cr01_bootstrap_owner.h"
#if !defined(_WIN64) && !defined(_M_X64)
#error Win64 only
#endif
#define WIN __stdcall
#define IMP __declspec(dllimport)
typedef int BOOL;
typedef struct CR01_WIN_INIT_ONCE { void *ptr; } CR01_WIN_INIT_ONCE;
typedef BOOL (WIN *CR01_WIN_INIT_FN)(CR01_WIN_INIT_ONCE*,void*,void**);
IMP BOOL WIN InitOnceExecuteOnce(CR01_WIN_INIT_ONCE*,CR01_WIN_INIT_FN,void*,void**);
/* All storage is process-lifetime. No TLS pointers escape this boundary. */
static CR01_WIN_INIT_ONCE g_once={0};
static CR01BootstrapOwner*g_owner;
static CR01PrepareOneContainer g_prepare;
static void*g_opaque;
static int g_result;
static volatile unsigned g_bound;
static BOOL WIN prepare_under_initonce(CR01_WIN_INIT_ONCE *unused,void*arg,void**context){
 (void)unused;(void)arg;(void)context;
 g_result=cr01_owner_prepare(g_owner,g_prepare,g_opaque);
 /* Mark once even if preparation fails, so subsequent DirectInput calls
    remain native and do not retry risky shader operations. */
 return 1;
}
/* Binding is a one-shot STARTUP operation; no changes after first call.
   The owning DI gateway must have already verified exact game/ASI identity. */
int cr01_di_bind_preparation(CR01BootstrapOwner*owner,
    CR01PrepareOneContainer prepare,void*ctx){
 if(!owner||!prepare||!owner->pair.original||
   __atomic_load_n(&owner->event_state,__ATOMIC_ACQUIRE)!=CR01_OWNER_COLD)return 0;
 if(__sync_val_compare_and_swap(&g_bound,0u,1u)!=0u)return 0;
 g_owner=owner;g_prepare=prepare;g_opaque=ctx;
 __atomic_store_n(&g_bound,2u,__ATOMIC_RELEASE);return 1;
}
int cr01_di_prepare_synchronously(void){
 if(__atomic_load_n(&g_bound,__ATOMIC_ACQUIRE)!=2u||!g_owner||!g_prepare)return 0;
 if(!InitOnceExecuteOnce(&g_once,prepare_under_initonce,0,0))return 0;
 return g_result&&cr01_owner_is_early_custom(g_owner);
}
