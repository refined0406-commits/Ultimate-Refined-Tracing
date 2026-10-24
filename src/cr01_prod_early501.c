#include "cr01_prod_early501.h"
#include <stdint.h>
#include <stddef.h>
/* Exact Patch 5.01 evidence: both D3D12 device owner COM globals remain NULL
 * during the protected first DirectInput call, before native Create/Add starts.
 * Later attachment is refused instead of repairing an already-created renderer.
 * This does not independently prove exclusive thread quiescence. Atomic code
 * publication and fail-closed registration hooks remain separately required. */
#define IMPORT __declspec(dllimport)
#define WIN __stdcall
typedef unsigned short WCHAR;
IMPORT void* WIN GetModuleHandleW(const WCHAR*);
extern int cr01_gate_verify_exe_file(void);
extern int cr01_gate_verify_native_blob(const uint8_t*,size_t);
extern int cr01_register_sites_preflight(const uint8_t*,size_t);
extern int cr01_gate_exact_call_preflight(const uint8_t*,const uint8_t*,const uint8_t*,
 uintptr_t,uintptr_t,uintptr_t,int32_t*,int32_t*);
#define CR01_CREATE_GLOBAL_RVA 0x5e514d8u
#define CR01_ADD_GLOBAL_RVA 0x5e514d0u
#define CR01_REG_SER_RVA 0x1ee6ff3u
#define CR01_NATIVE_RVA 0x32e7c28u
#define CR01_CALL_RVA 0x1ef0fcfu
#define CR01_GEOM_RVA 0x1ef29e1u
#define CR01_HAIR_RVA 0x1ef2f6cu
#define CR01_WRITER_RVA 0x1ef2340u
static volatile uint32_t g_entered;
static volatile uint32_t g_valid;
int cr01_prod_early501_eligibility(int first,uint64_t create,uint64_t add,
 int registration,int shader,int camera){
 return first&&create==0&&add==0&&registration&&shader&&camera;
}
static int snapshot(void){
 uint8_t*base=(uint8_t*)GetModuleHandleW(0);int32_t olddisp,newdisp;
 if(!base||!cr01_gate_verify_exe_file())return 0;
 /* Exact original image layout. No pointer dereference until signed EXE
  * matches, so offsets cannot drift silently across patches. */
 uint64_t create=__atomic_load_n((const uint64_t*)(base+CR01_CREATE_GLOBAL_RVA),__ATOMIC_ACQUIRE);
 uint64_t add=__atomic_load_n((const uint64_t*)(base+CR01_ADD_GLOBAL_RVA),__ATOMIC_ACQUIRE);
 int regs=cr01_register_sites_preflight(base,CR01_REG_SER_RVA+6u);
 int native=cr01_gate_verify_native_blob(base+CR01_NATIVE_RVA,231144u);
 /* A non-mutating check of the CameraCB original branch, verifying that no
  * competing mod already controls it. Reuse existing CR01 signature code. */
 int camera=cr01_gate_exact_call_preflight(base+CR01_CALL_RVA,base+CR01_GEOM_RVA,
   base+CR01_HAIR_RVA,(uintptr_t)(base+CR01_CALL_RVA),
   (uintptr_t)(base+CR01_WRITER_RVA),
   (uintptr_t)(base+CR01_WRITER_RVA),&olddisp,&newdisp);
 return cr01_prod_early501_eligibility(1,create,add,regs,native,camera);
}
int cr01_prod_early501_enter(void){
 uint32_t expected=0;
 if(!__atomic_compare_exchange_n(&g_entered,&expected,1,0,__ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE))return 0;
 int ok=snapshot();__atomic_store_n(&g_valid,ok?1u:0u,__ATOMIC_RELEASE);return ok;
}
/* Live recheck of both native owner globals immediately before publication.
 * Atomic helper hooks are still required. Any observed registration activity
 * forces pass-through; it is never retroactively made 'early'. */
int cr01_pre_registration_quiescence_approved(void){
 return __atomic_load_n(&g_entered,__ATOMIC_ACQUIRE)==1u &&
        __atomic_load_n(&g_valid,__ATOMIC_ACQUIRE)==1u && snapshot();
}
void cr01_prod_early501_exit(void){
 __atomic_store_n(&g_valid,0u,__ATOMIC_RELEASE);
 __atomic_store_n(&g_entered,2u,__ATOMIC_RELEASE);
}
