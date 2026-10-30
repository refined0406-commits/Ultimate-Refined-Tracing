/* PRIVATE FBR38 Patch 5.01 class-vtable CPU optimization.
 * No GPU ownership. No code patch. Native fallback and failed-owner checks.
 * IMPORTANT: ASI pinned for process lifetime before publishing any code pointer.
 * DllMain must never call this module. Only settings worker mutates the slot.
 */
#include "urt_build.h"
#include "cr01_bone_hook501.h"
#include <stdint.h>
#include <stddef.h>
#define IMP __declspec(dllimport)
#define WIN __stdcall
#define PAGE_READWRITE 0x04u
#define GET_MODULE_HANDLE_EX_FLAG_PIN 0x00000001u
#define GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS 0x00000004u
#define IMAGE_BYTES urt_image_bytes()
#define NATIVE_RVA urt_rva(0x1cac050u)
#define VTABLE_RVA urt_rva(0x37ae730u)
#define VTABLE_HEAD_RVA urt_rva(0x37ae6e0u)
#define RTTI_COL_RVA urt_rva(0x3ac8e10u)
typedef unsigned long DWORD;
typedef void* HMODULE;
typedef uint16_t WCHAR;
IMP HMODULE WIN GetModuleHandleW(const WCHAR*);
IMP int WIN GetModuleHandleExW(DWORD,const WCHAR*,HMODULE*);
IMP int WIN VirtualProtect(void*,size_t,DWORD,DWORD*);
extern int cr01_gate_verify_exe_file(void);
typedef uint8_t (*CR01BoneNative)(const uint8_t*,int32_t,uint8_t*);
extern uint8_t cr01_bone_fast_safe501(const uint8_t*,int32_t,uint8_t*,CR01BoneNative);
static const uint8_t kNative[192] = {0x48, 0x63, 0xc2, 0x4c, 0x0f, 0xbf, 0x4c, 0x41, 0x26, 0xb8, 0xff, 0xff, 0xff, 0xff, 0x66, 0x41, 0x3b, 0xc1, 0x0f, 0x84, 0xa5, 0x00, 0x00, 0x00, 0x0f, 0xb7, 0x51, 0x20, 0x44, 0x3b, 0xca, 0x0f, 0x8d, 0x98, 0x00, 0x00, 0x00, 0x0f, 0xb6, 0x41, 0x23, 0x0f, 0xb7, 0x51, 0x1c, 0x48, 0x8b, 0x49, 0x10, 0x48, 0x0f, 0xaf, 0xd0, 0x49, 0x03, 0xd1, 0x48, 0xc1, 0xe2, 0x06, 0x8b, 0x04, 0x0a, 0x41, 0x89, 0x00, 0x8b, 0x44, 0x0a, 0x04, 0x41, 0x89, 0x40, 0x04, 0x8b, 0x44, 0x0a, 0x08, 0x41, 0x89, 0x40, 0x08, 0x8b, 0x44, 0x0a, 0x0c, 0x41, 0x89, 0x40, 0x0c, 0x8b, 0x44, 0x0a, 0x10, 0x41, 0x89, 0x40, 0x10, 0x8b, 0x44, 0x0a, 0x14, 0x41, 0x89, 0x40, 0x14, 0x8b, 0x44, 0x0a, 0x18, 0x41, 0x89, 0x40, 0x18, 0x8b, 0x44, 0x0a, 0x1c, 0x41, 0x89, 0x40, 0x1c, 0x8b, 0x44, 0x0a, 0x20, 0x41, 0x89, 0x40, 0x20, 0x8b, 0x44, 0x0a, 0x24, 0x41, 0x89, 0x40, 0x24, 0x8b, 0x44, 0x0a, 0x28, 0x41, 0x89, 0x40, 0x28, 0x8b, 0x44, 0x0a, 0x2c, 0x41, 0x89, 0x40, 0x2c, 0x8b, 0x44, 0x0a, 0x30, 0x41, 0x89, 0x40, 0x30, 0x8b, 0x44, 0x0a, 0x34, 0x41, 0x89, 0x40, 0x34, 0x8b, 0x44, 0x0a, 0x38, 0x41, 0x89, 0x40, 0x38, 0xb0, 0x01, 0x41, 0xc7, 0x40, 0x3c, 0x00, 0x00, 0x80, 0x3f, 0xc3, 0x32, 0xc0, 0xc3};
static volatile uintptr_t g_original = 0;
static volatile unsigned g_enabled = 0;
static volatile unsigned g_pinned = 0;
static int equal(const uint8_t*a,const uint8_t*b,size_t n){
 size_t i;for(i=0;i<n;i++)if(a[i]!=b[i])return 0;return 1;
}
static uint32_t rd32(const uint8_t*p){return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static uint64_t rd64(const uint8_t*p){return (uint64_t)rd32(p)|((uint64_t)rd32(p+4)<<32);}
static uint8_t cr01_bone501_thunk(const uint8_t*pose,int32_t index,uint8_t*dst){
 uintptr_t p=__atomic_load_n(&g_original,__ATOMIC_ACQUIRE);
 CR01BoneNative native=(CR01BoneNative)p;
 if(!native)return 0; /* impossible after publication; fail closed */
 if(!__atomic_load_n(&g_enabled,__ATOMIC_ACQUIRE))return native(pose,index,dst);
 return cr01_bone_fast_safe501(pose,index,dst,native);
}
static int exact_owner(uint8_t*exe){
 uintptr_t n;
 if(!exe||!cr01_gate_verify_exe_file())return 0;
 if(exe[0]!='M'||exe[1]!='Z')return 0;
 {uint32_t pe=rd32(exe+0x3c);
  if(pe>0x1000||rd32(exe+pe+24+56)!=IMAGE_BYTES)return 0;}
 n=(uintptr_t)(exe+NATIVE_RVA);
 if(!equal(exe+NATIVE_RVA,kNative,sizeof(kNative)))return 0;
 /* Recognize the entire expected class-table owner, not an arbitrary data pointer. */
 if(rd64(exe+VTABLE_HEAD_RVA-8)!=(uintptr_t)(exe+RTTI_COL_RVA))return 0;
 if(rd32(exe+RTTI_COL_RVA)!=1u||rd32(exe+RTTI_COL_RVA+20)!=RTTI_COL_RVA)return 0;
 if((uintptr_t)(exe+VTABLE_RVA)&7u)return 0;
 return __atomic_load_n((volatile uintptr_t*)(exe+VTABLE_RVA),__ATOMIC_ACQUIRE)==n;
}
/* Already pinned ASI memory can safely service a pre-fetched thunk pointer even
 * after a concurrent Off/slot restoration. This intentionally sacrifices hot-
 * uninstallation of this ASI during a running Witcher process. */
static int pin_code(void){
 HMODULE h=0;
 if(__atomic_load_n(&g_pinned,__ATOMIC_ACQUIRE))return 1;
 if(!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_PIN|GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                         (const WCHAR*)(uintptr_t)(const void*)&cr01_bone501_thunk,&h)||!h)return 0;
 __atomic_store_n(&g_pinned,1u,__ATOMIC_RELEASE);
 return 1;
}
/* Compare/exchange is atomic for the aligned 64-bit slot. VirtualProtect only
 * changes permissions; it is NOT an ownership transaction. The CAS is.
 * Failure to restore protection prevents further activation. */
static int swap_slot(uintptr_t slot,uintptr_t expected,uintptr_t replacement){
 DWORD old=0,unused=0;uintptr_t observed=expected;
 int swapped;
 if(!VirtualProtect((void*)slot,sizeof(uintptr_t),PAGE_READWRITE,&old))return 0;
 swapped=__atomic_compare_exchange_n((volatile uintptr_t*)slot,&observed,replacement,0,
                                     __ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE);
 if(!VirtualProtect((void*)slot,sizeof(uintptr_t),old,&unused)){
  /* A permission-restore failure is never success. If our CAS was admitted,
   * attempt its exact reverse while this page is still writable. Never touch
   * a different owner's pointer. Keep thunk pinned even if reverse fails. */
  __atomic_store_n(&g_enabled,0u,__ATOMIC_RELEASE);
  if(swapped){
   uintptr_t rollback=replacement;
   (void)__atomic_compare_exchange_n((volatile uintptr_t*)slot,&rollback,expected,0,
                                     __ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE);
  }
  (void)VirtualProtect((void*)slot,sizeof(uintptr_t),old,&unused);
  return 0;
 }
 return swapped;
}
int cr01_bone501_set(unsigned on){
 uint8_t*exe=(uint8_t*)GetModuleHandleW(0);
 uintptr_t native,slot,hook,cur;
 if(!exe)return 0;
 native=(uintptr_t)(exe+NATIVE_RVA);
 slot=(uintptr_t)(exe+VTABLE_RVA);
 hook=(uintptr_t)(const void*)&cr01_bone501_thunk;
 if(!on){
  /* Turn off dispatch first. Any core already running may finish before return;
   * its source remains game-owned and our pinned code remains executable. */
  __atomic_store_n(&g_enabled,0u,__ATOMIC_RELEASE);
  if(!__atomic_load_n(&g_original,__ATOMIC_ACQUIRE))return 1;
  cur=__atomic_load_n((volatile uintptr_t*)slot,__ATOMIC_ACQUIRE);
  if(cur==native)return 1;
  if(cur!=hook)return 0; /* third-party owner; do not overwrite */
  return swap_slot(slot,hook,native);
 }
 if(!exact_owner(exe))return 0;
 if(!pin_code())return 0;
 if(!__atomic_load_n(&g_original,__ATOMIC_ACQUIRE))
  __atomic_store_n(&g_original,native,__ATOMIC_RELEASE);
 else if(__atomic_load_n(&g_original,__ATOMIC_ACQUIRE)!=native)return 0;
 /* No permanent Off-path intercept: all Off calls go directly to CDPR code. */
 if(!swap_slot(slot,native,hook))return 0;
 __atomic_store_n(&g_enabled,1u,__ATOMIC_RELEASE);
 return 1;
}
void cr01_bone501_shutdown(void){
 (void)cr01_bone501_set(0u);
 /* Never unpin: other threads may retain the old vtable target. */
}
