/* V5.2 Path Tracing performance control (PT: Scene Range): pure logic. See urt_ptperf_core.h. */
#include "urt_ptperf_core.h"

const char *const urt_px_keys[URT_PX_COUNT] = { "PTSceneRange" };
const uint32_t urt_px_option_count[URT_PX_COUNT] = { 5u };

int urt_px_prefs_valid(const UrtPxPrefs *p){
  if(!p)return 0;
  for(unsigned i=0;i<URT_PX_COUNT;i++)if(p->v[i]>=urt_px_option_count[i])return 0;
  return 1;
}

typedef union { uint32_t u; float f; } Bits;
static float f_of(uint32_t u){Bits b;b.u=u;return b.f;}
static uint32_t u_of(float f){Bits b;b.f=f;return b.u;}

static const float k_range[5]={0.0f,300.0f,250.0f,200.0f,150.0f};

int urt_pf_native_plausible(unsigned field, uint32_t native){
  float v=f_of(native);
  if(!(v==v))return 0;
  switch(field){
  case URT_PF_RANGE: case URT_PF_RANGE_EXT: return v>=10.0f&&v<=100000.0f;
  default: return 0;
  }
}
int urt_pf_target(const UrtPxPrefs *p, unsigned field, uint32_t native, uint32_t *out){
  if(!p||!out||field>=URT_PF_COUNT||!urt_px_prefs_valid(p))return 0;
  if(!urt_pf_native_plausible(field,native))return 0;
  uint32_t i;
  switch(field){
  case URT_PF_RANGE: case URT_PF_RANGE_EXT:
    i=p->v[URT_PX_SCENE_RANGE]; if(!i)return 0;
    if(k_range[i]>=f_of(native))return 0;     /* only ever a smaller scene */
    *out=u_of(k_range[i]); return 1;
  default: return 0;
  }
}
static uint32_t ld(uint32_t *p){return __atomic_load_n(p,__ATOMIC_ACQUIRE);}
static int cas(uint32_t *p,uint32_t expect,uint32_t want){
  return __atomic_compare_exchange_n(p,&expect,want,0,__ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE);
}
void urt_pf_init(UrtPfOwner *o){
  if(!o)return;
  for(unsigned i=0;i<URT_PF_COUNT;i++){o->slot[i]=0;o->original[i]=o->applied[i]=0;}
  o->mask=0;o->conflicts=0;
}
static void release_one(UrtPfOwner *o,unsigned i){
  if(!(o->mask&(1u<<i)))return;
  if(!o->slot[i]||!cas(o->slot[i],o->applied[i],o->original[i]))o->conflicts++;
  o->mask&=~(1u<<i);
}
void urt_pf_release(UrtPfOwner *o){
  if(!o)return;
  for(unsigned i=0;i<URT_PF_COUNT;i++)release_one(o,i);
}
void urt_pf_update(UrtPfOwner *o, const UrtPxPrefs *p, unsigned eligible){
  if(!o)return;
  if(!eligible||!p||!urt_px_prefs_valid(p)){urt_pf_release(o);return;}
  for(unsigned i=0;i<URT_PF_COUNT;i++){
    if(!o->slot[i])continue;
    if(o->mask&(1u<<i)){
      if(ld(o->slot[i])!=o->applied[i]){o->mask&=~(1u<<i);o->conflicts++;continue;}
      uint32_t t;
      if(!urt_pf_target(p,i,o->original[i],&t)){release_one(o,i);continue;}
      if(t==o->applied[i])continue;
      if(cas(o->slot[i],o->applied[i],t))o->applied[i]=t;
      else {o->mask&=~(1u<<i);o->conflicts++;}
      continue;
    }
    uint32_t native=ld(o->slot[i]),t;
    if(!urt_pf_target(p,i,native,&t))continue;
    if(cas(o->slot[i],native,t)){o->original[i]=native;o->applied[i]=t;o->mask|=1u<<i;}
  }
}
