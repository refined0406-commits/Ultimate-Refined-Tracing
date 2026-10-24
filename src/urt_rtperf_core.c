/* V5.1 Normal RT performance controls: pure logic. See urt_rtperf_core.h. */
#include "urt_rtperf_core.h"

const char *const urt_rp_keys[URT_RP_COUNT] = {
  "RTReflLocalLights", "RTShadowLocalLights", "RTShadowFoliageDetail"
};
const uint32_t urt_rp_option_count[URT_RP_COUNT] = { 2u, 5u, 4u };

int urt_rp_prefs_valid(const UrtRpPrefs *p){
  if(!p)return 0;
  for(unsigned i=0;i<URT_RP_COUNT;i++)if(p->v[i]>=urt_rp_option_count[i])return 0;
  return 1;
}

typedef union { uint32_t u; float f; } Bits;
static float f_of(uint32_t u){Bits b;b.u=u;return b.f;}
static uint32_t u_of(float f){Bits b;b.f=f;return b.u;}

int urt_rf_native_plausible(unsigned field, uint32_t native){
  if(field==URT_RF_SHADOW_ALPHA_MIP){
    float v=f_of(native);
    if(!(v==v))return 0;                                    /* NaN */
    return v>=0.5f&&v<=16.0f;   /* mip level; 0 = not yet initialised by the engine */
  }
  if(field==URT_RF_REFL_LOCAL)return (native&0xffu)<=1u&&(native>>8)==0u;
  if(field==URT_RF_SHADOW_SLOTS)return native<=64u;
  return 0;
}

int urt_rf_target(const UrtRpPrefs *p, unsigned field, uint32_t native, uint32_t *out){
  static const float shmip[4] ={0.0f,3.0f,4.0f,6.0f};
  static const uint32_t slots[5]={0u,6u,4u,2u,0u};
  if(!p||!out||field>=URT_RF_COUNT||!urt_rp_prefs_valid(p))return 0;
  if(!urt_rf_native_plausible(field,native))return 0;
  uint32_t idx;
  switch(field){
  case URT_RF_REFL_LOCAL:
    if(!p->v[URT_RP_REFL_LIGHTS])return 0;
    if((native&0xffu)==0u)return 0;              /* already off natively */
    *out=native&~0xffu; return 1;                /* clear the bool byte only */
  case URT_RF_SHADOW_SLOTS:
    idx=p->v[URT_RP_SHADOW_SLOTS]; if(!idx)return 0;
    if(slots[idx]>=native)return 0;              /* only fewer shadowed lights */
    *out=slots[idx]; return 1;
  case URT_RF_SHADOW_ALPHA_MIP:
    idx=p->v[URT_RP_SHADOW_FOLIAGE]; if(!idx)return 0;
    if(shmip[idx]<=f_of(native))return 0;        /* mip: only coarser (cheaper) */
    *out=u_of(shmip[idx]); return 1;
  default: return 0;
  }
}

static uint32_t ld(uint32_t *p){return __atomic_load_n(p,__ATOMIC_ACQUIRE);}
static int cas(uint32_t *p,uint32_t expect,uint32_t want){
  return __atomic_compare_exchange_n(p,&expect,want,0,__ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE);
}
void urt_rf_init(UrtRfOwner *o){
  if(!o)return;
  for(unsigned i=0;i<URT_RF_COUNT;i++){o->slot[i]=0;o->original[i]=o->applied[i]=0;}
  o->mask=0;o->conflicts=0;
}
static void release_one(UrtRfOwner *o,unsigned i){
  if(!(o->mask&(1u<<i)))return;
  if(!o->slot[i]||!cas(o->slot[i],o->applied[i],o->original[i]))o->conflicts++;
  o->mask&=~(1u<<i);
}
void urt_rf_release(UrtRfOwner *o){
  if(!o)return;
  for(unsigned i=0;i<URT_RF_COUNT;i++)release_one(o,i);
}
void urt_rf_update(UrtRfOwner *o, const UrtRpPrefs *p, unsigned eligible){
  if(!o)return;
  if(!eligible||!p||!urt_rp_prefs_valid(p)){urt_rf_release(o);return;}
  for(unsigned i=0;i<URT_RF_COUNT;i++){
    if(!o->slot[i]){continue;}
    if(o->mask&(1u<<i)){
      uint32_t now=ld(o->slot[i]);
      if(now!=o->applied[i]){            /* foreign write: give it up, never clobber */
        o->mask&=~(1u<<i);o->conflicts++;continue;
      }
      uint32_t t;
      if(!urt_rf_target(p,i,o->original[i],&t)){release_one(o,i);continue;}
      if(t==o->applied[i])continue;
      if(cas(o->slot[i],o->applied[i],t))o->applied[i]=t;
      else {o->mask&=~(1u<<i);o->conflicts++;}
      continue;
    }
    uint32_t native=ld(o->slot[i]),t;
    if(!urt_rf_target(p,i,native,&t))continue;
    if(cas(o->slot[i],native,t)){o->original[i]=native;o->applied[i]=t;o->mask|=1u<<i;}
  }
}
