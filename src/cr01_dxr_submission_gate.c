#include "cr01_dxr_submission_gate.h"
#include <stddef.h>
#if defined(_WIN64) || defined(_M_X64) || defined(__x86_64__)
_Static_assert(sizeof(CR01DxrBytecode)==16,"D3D12_SHADER_BYTECODE x64");
_Static_assert(sizeof(CR01DxrLibraryDesc)==32,"D3D12_DXIL_LIBRARY_DESC x64");
_Static_assert(sizeof(CR01DxrSubobject)==16,"D3D12_STATE_SUBOBJECT x64");
_Static_assert(sizeof(CR01DxrExportDesc)==24,"D3D12_EXPORT_DESC x64");
_Static_assert(sizeof(CR01DxrStateDesc)==16,"D3D12_STATE_OBJECT_DESC x64");
_Static_assert(offsetof(CR01DxrStateDesc,subobjects)==8,"D3D12 descriptor pointer");
#endif
static int match_export(const uint16_t *s,const char*name){
 if(!s)return 0;
 for(unsigned i=0;i<48;i++){
  if(!name[i])return s[i]==0;
  if(s[i]!=(uint8_t)name[i])return 0;
 }
 return 0;
}
static int both_exports(const CR01DxrLibraryDesc*l){
 uint32_t bits=0;
 if(l->number_of_exports==0)return 1; /* SDK: 0 selects all DXIL exports */
 if(l->number_of_exports>128u||!l->exports)return 0;
 const CR01DxrExportDesc*e=(const CR01DxrExportDesc*)l->exports;
 for(uint32_t i=0;i<l->number_of_exports;i++){
  const uint16_t *s=e[i].export_to_rename?e[i].export_to_rename:e[i].name;
  if(match_export(s,"PTRGSFinal"))bits|=1;
  if(match_export(s,"PTRGSWithSERFinal"))bits|=2;
 }
 return bits==3;
}

int cr01_dxr_gate_init(CR01DxrSubmissionGate*g,const uint8_t*b,size_t size){
 if(!g||!b||size!=231524u)return 0;
 g->approved=b;g->approved_size=size;
 g->accepted_method_mask=0;g->rejected=0;g->accepted_state_object=0;
 return 1;
}
int cr01_dxr_gate_note_submit(CR01DxrSubmissionGate*g,unsigned method,
 const CR01DxrStateDesc*d,int32_t hr,const void *state){
 unsigned found=0;
 if(!g||!g->approved||__atomic_load_n(&g->rejected,__ATOMIC_ACQUIRE))return 0;
 if((method!=CR01_DXR_CREATE_STATE_OBJECT&&method!=CR01_DXR_ADD_TO_STATE_OBJECT)||
    !d||d->count==0||d->count>256u||!d->subobjects||
    d->type!=3u){
  __atomic_store_n(&g->rejected,1u,__ATOMIC_RELEASE);return 0;
 }
 for(uint32_t i=0;i<d->count;i++){
  const CR01DxrSubobject*s=&d->subobjects[i];
  if(s->type!=CR01_D3D12_SUBOBJECT_DXIL_LIBRARY)continue;
  if(!s->desc){__atomic_store_n(&g->rejected,1u,__ATOMIC_RELEASE);return 0;}
  const CR01DxrLibraryDesc*l=(const CR01DxrLibraryDesc*)s->desc;
  /* One immutable container, exact startup-attested pointer identity.
   * Never accept a copied or unrelated candidate without separate evidence. */
  if(l->dxil.code==g->approved && l->dxil.bytes==g->approved_size &&
     both_exports(l))found=1;
 }
 if(!found)return 0; /* Unrelated DXR submissions (even failures) are ignored. */
 if(!state||hr<0){
  __atomic_store_n(&g->rejected,1u,__ATOMIC_RELEASE);return 0;
 }
 const void*seen=__atomic_load_n(&g->accepted_state_object,__ATOMIC_ACQUIRE);
 if(!seen){
  const void*empty=0;
  (void)__atomic_compare_exchange_n(&g->accepted_state_object,&empty,state,0,
                                   __ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE);
  seen=__atomic_load_n(&g->accepted_state_object,__ATOMIC_ACQUIRE);
 }
 if(seen!=state){
  /* Different objects need separate lifetime/binding evidence. */
  __atomic_store_n(&g->rejected,1u,__ATOMIC_RELEASE);return 0;
 }
 __atomic_fetch_or(&g->accepted_method_mask,(uint32_t)method,__ATOMIC_ACQ_REL);
 return !__atomic_load_n(&g->rejected,__ATOMIC_ACQUIRE);
}
int cr01_dxr_gate_state_created(const CR01DxrSubmissionGate*g){
 return g&&g->approved&&!__atomic_load_n(&g->rejected,__ATOMIC_ACQUIRE)&&
  __atomic_load_n(&g->accepted_state_object,__ATOMIC_ACQUIRE)!=0&&
  __atomic_load_n(&g->accepted_method_mask,__ATOMIC_ACQUIRE)!=0;
}

static CR01DxrSubmissionGate g_runtime;
static volatile uint32_t g_runtime_init_once;
int cr01_dxr_runtime_init(const uint8_t*approved,size_t size){
 uint32_t expected=0;
 if(!approved||size!=231524u)return 0;
 if(!__atomic_compare_exchange_n(&g_runtime_init_once,&expected,1u,0,
         __ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE))return 0;
 if(!cr01_dxr_gate_init(&g_runtime,approved,size))return 0;
 __atomic_store_n(&g_runtime_init_once,2u,__ATOMIC_RELEASE);
 return 1;
}
int cr01_dxr_runtime_note_submit(unsigned method,const CR01DxrStateDesc*desc,
 int32_t hr,const void*result){
 if(__atomic_load_n(&g_runtime_init_once,__ATOMIC_ACQUIRE)!=2u)return 0;
 /* This function is NOT exported as an ASI API. A native D3D12 submission
  * bridge must still be implemented; until then this function is never called
  * by the production game path. */
 return cr01_dxr_gate_note_submit(&g_runtime,method,desc,hr,result);
}
int cr01_dxr_runtime_state_created(void){
 return __atomic_load_n(&g_runtime_init_once,__ATOMIC_ACQUIRE)==2u&&
         cr01_dxr_gate_state_created(&g_runtime);
}
