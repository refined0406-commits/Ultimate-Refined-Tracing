#include "cr01_rt_legacy_core.h"
#include <stddef.h>
static uint32_t rd(uint32_t*p){return __atomic_load_n(p,__ATOMIC_ACQUIRE);}
static int cas(uint32_t*p,uint32_t expected,uint32_t target){
 return __atomic_compare_exchange_n(p,&expected,target,0,__ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE);
}
void cr01_rt_legacy_core_init(CR01RtLegacy*s){if(!s)return;
 unsigned i;s->mask=s->profile=s->native_preset=s->revision=s->generation=s->active=s->conflict=0;
 for(i=0;i<CR01_RT_LEGACY_FIELDS;i++){s->slot[i]=0;s->original[i]=s->applied[i]=0;}
}
static int allowed(uint32_t v,const uint32_t*a,unsigned n){
 for(unsigned i=0;i<n;i++)if(v==a[i])return 1;return 0;
}
int cr01_rt_legacy_profile_valid(const CR01RtLegacyProfile*p){
 static const uint32_t prim[]={0,32,48,64,96,128},second[]={0,4,6,8,12,16,24,32},
 cap[]={0,8192,32768,65536,131072,262144},ao[]={0,1,2,3,4},
 budget[]={0,2048,65536,131072,196608,262144,393216,524288},
 shade[]={0,8,16,24,32};
 return p&&allowed(p->values[0],prim,6)&&allowed(p->values[1],second,8)&&
 allowed(p->values[2],cap,6)&&allowed(p->values[3],ao,5)&&allowed(p->values[4],budget,8)&&allowed(p->values[5],shade,5);
}
void cr01_rt_legacy_release(CR01RtLegacy*s){
 if(!s)return;
 for(unsigned i=0;i<CR01_RT_LEGACY_FIELDS;i++)if(s->mask&(1u<<i)){
  /* Do not overwrite engine/foreign writes. A failed CAS is no longer ours. */
  if(!cas(s->slot[i],s->applied[i],s->original[i]))s->conflict=1;
 }
 const unsigned was_active=s->active||s->mask;
 s->mask=0;s->active=0;
 if(was_active){++s->generation;if(!s->generation)s->generation=1;}
}
static int native_signature(const CR01RtLegacy*s,unsigned preset){
 if(!s||!s->slot[0]||!s->slot[1])return 0;
 /* Exact Patch 5.01 native mapping, not historical 16/4 defaults. */
 return rd(s->slot[0])==128u&&rd(s->slot[1])==(preset?32u:16u);
}
int cr01_rt_legacy_update(CR01RtLegacy*s,const CR01RtLegacyPreferences*p,
 unsigned normal_rt,unsigned master,unsigned preset,unsigned native_shading32){
 if(!s||!p)return 0;
 /* The native preset is used ONLY to validate actual game state, never to
  * silently select the explicit Quality/Performance optimization profile. */
 if(!normal_rt||!master||p->enabled>3u||!p->enabled||preset>1||
 (!native_shading32&&!(s->active&&(s->mask&(1u<<CR01_RT_L_SHADING))))||
 !cr01_rt_legacy_profile_valid(&p->profile[p->enabled==2u?1u:p->enabled==3u?0u:preset])){
  cr01_rt_legacy_release(s);return 0;
 }
 const unsigned selected=(p->enabled==2u)?1u:((p->enabled==3u)?0u:preset);
 for(unsigned i=0;i<CR01_RT_LEGACY_FIELDS;i++)if(!s->slot[i]){cr01_rt_legacy_release(s);return 0;}
 if(s->active){
  if(s->profile!=selected||s->native_preset!=preset||s->revision!=p->revision){cr01_rt_legacy_release(s);}
  else {
   unsigned matches=1;
   for(unsigned i=0;i<CR01_RT_LEGACY_FIELDS;i++)if(s->mask&(1u<<i)){
    if(rd(s->slot[i])!=s->applied[i]){matches=0;break;}
   }
   if(matches)return 1;
   /* Native renderer changed an owned field; release only surviving owns. */
   cr01_rt_legacy_release(s);return 0;
  }
 }
 if(!native_signature(s,preset))return 0;
 const CR01RtLegacyProfile*profile=&p->profile[selected];
 unsigned i;
 for(i=0;i<CR01_RT_LEGACY_FIELDS;i++){
  s->original[i]=rd(s->slot[i]);s->applied[i]=s->original[i];
  /* Range checks on native state: never learn corrupted or stale values. */
  if(i==2&&(s->original[i]<1024u||s->original[i]>134217728u))return 0;
  if(i==3&&s->original[i]>16u)return 0;
  if(i==4&&(s->original[i]<1024u||s->original[i]>134217728u))return 0;
  if(i==CR01_RT_L_SHADING&&s->original[i]!=32u)return 0;
 }
 for(i=0;i<CR01_RT_LEGACY_FIELDS;i++){
  uint32_t target=profile->values[i];
  if(!target||target==s->original[i])continue;
  if(!cas(s->slot[i],s->original[i],target)){
   cr01_rt_legacy_release(s);return 0;
  }
  s->mask|=1u<<i;s->applied[i]=target;
 }
 s->active=1;s->profile=selected;s->native_preset=preset;s->revision=p->revision;
 ++s->generation;if(!s->generation)s->generation=1;
 return 1;
}
