#include "cr01_controls.h"
#include "cr01_validation.h"
static uint32_t floatbits(float v){union{uint32_t bits;float value;}p;p.value=v;return p.bits;}
static float discrete(unsigned f,uint32_t k){
 static const float strength[]={0,0,.5f,1,1.5f,2};
 static const float radius[]={0,.1f,.25f,.5f,1,2,4};
 static const float contrast[]={0,.5f,.75f,1,1.5f,2,3};
 static const float grass[]={0,.5f,.75f,1,1.5f,2,3};
 if(f==F_AO_STRENGTH && k<=5)return strength[k];
 if(f==F_AO_RADIUS && k<=6)return radius[k];
 if(f==F_AO_CONTRAST && k<=6)return contrast[k];
 if((f==F_GRASS_DAY||f==F_GRASS_NIGHT) && k<=6)return grass[k];
 return -1.f;
}
int cr01_control_value(unsigned f,uint32_t k,uint32_t*out){
 float v;
 if(!out||f>=CR01_FEATURE_COUNT||f==F_GEOM||f==F_HAIR||f==F_INDIRECT||f==F_SHADOWS)return 0;
 if(f==F_SUN||f==F_SKY){if(!cr01_valid_float_100(f,k)||k==100)return 0;*out=floatbits((float)k/100.f);return 1;}
 if(!cr01_valid_selection(f,k)||k==0)return 0;
 if(f==F_AO||f==F_SPEC_SS||f==F_SPEC_CHECKER||f==F_BILLBOARDS){*out=(k==CR01_ON)?1:0;return 1;}
 v=discrete(f,k);if(v<0.f)return 0;
 *out=floatbits(v);return 1;
}
int cr01_control_native_sane(unsigned f,uint32_t raw){
 union{uint32_t u;float v;}b;b.u=raw;
 if(f==F_AO||f==F_SPEC_SS||f==F_SPEC_CHECKER||f==F_BILLBOARDS)return raw<=1;
 /* Patch 5.01 native -1.0f is an observed sentinel for exactly these five
  * carriers; preserve the raw bits as NativeState for restoration. Reject
  * every other negative, NaN, Inf or unexpected value, fail closed. */
 if(f==F_AO_STRENGTH||f==F_AO_RADIUS||f==F_AO_CONTRAST||f==F_GRASS_DAY||f==F_GRASS_NIGHT){
   if(raw==0xBF800000u)return 1;
   return b.v>=0.f&&b.v<=64.f;
 }
 if(f==F_SUN||f==F_SKY)return b.v>=0.f&&b.v<=64.f; /* rejects inf and NaN */
 return 0;
}
void cr01_sync_simple_pt(const CR01Settings*s,uint32_t pt,CR01Owner owners[CR01_FEATURE_COUNT],uint32_t *slots[CR01_FEATURE_COUNT]){
 unsigned f;uint32_t target;
 if(!owners||!slots)return;
 for(f=0;f<CR01_FEATURE_COUNT;f++){
  if(f==F_SHADOWS||f==F_GEOM||f==F_HAIR||f==F_INDIRECT)continue;
  if(!cr01_should_bind_renderer(s,pt)||!cr01_wants_write(s,f)||!cr01_control_value(f,s->v[f],&target)){
   (void)cr01_release_owner(&owners[f]);continue;
  }
  if(!slots[f])continue;
  if(!owners[f].owned&&!cr01_control_native_sane(f,*(volatile uint32_t *)slots[f]))continue;
  (void)cr01_write_owner(&owners[f],slots[f],target);
 }
}
