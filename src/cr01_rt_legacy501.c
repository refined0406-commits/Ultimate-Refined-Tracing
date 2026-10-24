/* CR01 private legacy-compatible Normal RT worker extension.
 * Uses exact Patch 5.01 registry descriptors, live dx12user.settings edges,
 * two independent profiles and compare/exchange ownership restoration.
 * NOTE: mode checks are asynchronous. NOT a synchronized pre-consumption gate.
 */
#include "cr01_rt_legacy501.h"
#include "cr01_rt_legacy_mode.h"
#include "cr01_rt_legacy_core.h"
#include "cr01_rt_c17_core.h"
#include "cr01_release_log.h"
#include <stddef.h>
#include "urt_cvar_index.h"
extern int cr01_gate_verify_exe_file(void);
#define IMP __declspec(dllimport)
#define WIN __stdcall
typedef unsigned short WCHAR;
typedef unsigned long DWORD;
extern DWORD urt_cfg_get(const WCHAR*,WCHAR*,DWORD);
static CR01RtLegacy g_owner;
static CR01RtLegacyPreferences g_pref;
static uint32_t*g_shading;
static CR01C17CapsOwner g_c17;
static CR01C17CapsProfile g_c17_profiles[2];
static uint32_t g_c17_revision;
static int g_c17_ready;
static int g_c17_preferences_valid;
static uint32_t*g_preset;
static int g_ready;
static unsigned g_logged, g_prior_mask;
/* V5: each descriptor is resolved by (name, category, kind) through the
 * one-pass registry index and must carry the registered default at +24 that
 * V4.x pinned. On the known Patch 5.01 code the resolved descriptor must also
 * sit at the V4.x pinned RVA (cross-check); on other builds the name/category/
 * kind/default identity alone is required. Category scoping is mandatory
 * because the RTXGI grids register identical short names. */
struct Binding {const char*name;const char*category;uint64_t kind;uint32_t registration_default,pinned_reg;};
static const struct Binding bindings[5]={
 {"NumClassificationRaysPerProbe","RTXGI/GlobalGrid/Dbg",0x201u,128u,0x4069b88u},
 {"NumClassificationRaysPerProbe","RTXGI/Grid1",0x201u,32u,0x4069fb0u},
 {"MaxProbesClassificatedPerFrame","RTXGI/Grid1",0x201u,262144u,0x406a0c8u},
 {"AOPretraceQualityLevel","Rendering/NRD/Shadows",0x101u,0u,0x4068508u},
 {"ShadingRaysPerFrameBudget","RTXGI/Grid1",0x201u,524288u,0x4069f78u}
};
static const struct Binding c17_bindings[2]={
 {"MaxProbesShadedPerFrame","RTXGI/GlobalGrid",0x201u,16384u,0x4069c68u},
 {"MaxProbesShadedPerFrame","RTXGI/Grid1",0x201u,262144u,0x406a090u}
};
static uint32_t u32(const uint8_t*p){return (uint32_t)p[0]|(uint32_t)p[1]<<8|
 (uint32_t)p[2]<<16|(uint32_t)p[3]<<24;}
extern const UrtCvarIndex *urt_runtime_cvar_index(void);
static uint32_t*slot_from_descriptor(uint8_t*base,uint32_t image,const struct Binding*b){
 const UrtCvarIndex*ix=urt_runtime_cvar_index();uint32_t desc=0;
 if(!ix||!b||ix->base!=base||ix->image_size!=image)return 0;
 uint32_t*slot=urt_cvar_lookup_scoped(ix,b->name,b->category,b->kind,&desc);
 if(!slot||desc+28u>image)return 0;
 if(u32(base+desc+24u)!=b->registration_default)return 0;
 if(cr01_gate_verify_exe_file()&&desc!=b->pinned_reg)return 0; /* known code: must equal V4.x pin */
 return slot;
}
#define c17_slot slot_from_descriptor
/* Native RTGI preset and shading descriptors are resolved using the accepted
 * RC1 exact-name registry resolver; never use guessed absolute addresses. */
int cr01_rt_legacy501_init(uint8_t*base,uint32_t image){
 extern uint32_t*cr01_legacy_approved_find_cvar(uint8_t*,uint32_t,const char*);
 cr01_rt_legacy_core_init(&g_owner);g_ready=0;g_pref.revision=1;
 cr01_c17_caps_init(&g_c17);g_c17_ready=0;g_c17_preferences_valid=1;g_c17_revision=1;
 for(unsigned i=0;i<2;i++)for(unsigned j=0;j<CR01_C17_CAP_FIELDS;j++)g_c17_profiles[i].values[j]=0;
 for(unsigned i=0;i<2;i++)for(unsigned j=0;j<CR01_RT_LEGACY_FIELDS;j++)g_pref.profile[i].values[j]=0;
 g_pref.enabled=0;
 for(unsigned i=0;i<5;i++)if(!(g_owner.slot[i]=slot_from_descriptor(base,image,&bindings[i])))return 0;
 g_shading=cr01_legacy_approved_find_cvar(base,image,"NumShadingRaysPerProbe");
 g_owner.slot[CR01_RT_L_SHADING]=g_shading;
 g_preset=cr01_legacy_approved_find_cvar(base,image,"RTGIPreset");
 if(!g_shading||!g_preset)return 0;
 /* C17 binding failure only disables the optional C17 extension, not C16. */
 g_c17.slot[0]=c17_slot(base,image,&c17_bindings[0]);
 g_c17.slot[1]=c17_slot(base,image,&c17_bindings[1]);
 g_c17_ready=(g_c17.slot[0]!=0&&g_c17.slot[1]!=0);
 if(!g_c17_ready)cr01_release_log_event("URT: Normal RT: RTGI grid-limit settings rejected (not the expected engine settings); other RTGI controls unaffected\r\n");
 g_ready=1;return 1;
}
static int value(const WCHAR*file,const char*name,uint32_t*out){
 WCHAR k[64],s[64];unsigned n=0;
 while(name[n]&&n<63){k[n]=(WCHAR)(unsigned char)name[n];n++;}if(name[n])return 0;k[n]=0;
 s[0]=0;
 DWORD count=urt_cfg_get(k,s,64);(void)file;
 if(!count){*out=0;return 1;}
 if(count>=63)return 0;
 uint32_t num=0;
 for(DWORD i=0;i<count;i++){
  if(s[i]<'0'||s[i]>'9'||num>0xffffffu)return 0;
  num=num*10u+(s[i]-'0');
 }
 *out=num;return 1;
}
static int c17_reload(const WCHAR*file){
 if(!g_c17_ready||!file)return 0;
 static const char*keys[2][CR01_C17_CAP_FIELDS]={
  {"RTPerfGlobalShadingLimit","RTPerfGrid1ShadingLimit"},
  {"RTQualityGlobalShadingLimit","RTQualityGrid1ShadingLimit"}
 };
 CR01C17CapsProfile new_values[2];
 for(unsigned p=0;p<2;p++){
  for(unsigned i=0;i<CR01_C17_CAP_FIELDS;i++){
   if(!value(file,keys[p][i],&new_values[p].values[i]))goto invalid;
  }
  if(!cr01_c17_caps_profile_valid(&new_values[p]))goto invalid;
 }
 unsigned changed=0;
 for(unsigned p=0;p<2;p++)for(unsigned i=0;i<CR01_C17_CAP_FIELDS;i++){
  if(g_c17_profiles[p].values[i]!=new_values[p].values[i])changed=1;
  g_c17_profiles[p].values[i]=new_values[p].values[i];
 }
 if(changed){++g_c17_revision;if(!g_c17_revision)g_c17_revision=1;}
 g_c17_preferences_valid=1;
 return 1;
invalid:
 cr01_c17_caps_release(&g_c17);g_c17_preferences_valid=0; /* repairable on next valid menu save */
 cr01_release_log_event("URT: Normal RT: invalid RTGI grid-limit preference; previous values kept\r\n");
 return 0;
}
int cr01_rt_legacy501_reload(const WCHAR*file){
 if(!g_ready||!file)return 0;
 static const char*keys[2][CR01_RT_LEGACY_FIELDS]={
 {"RTPerfClassificationPrimary","RTPerfClassificationSecondary","RTPerfMaxProbes","RTPerfAOQuality","RTPerfShadingBudget",0 /* rc3: Shading Rays removed, never read */},
 {"RTQualityClassificationPrimary","RTQualityClassificationSecondary","RTQualityMaxProbes","RTQualityAOQuality","RTQualityShadingBudget",0 /* rc3: Shading Rays removed, never read */}};
 CR01RtLegacyPreferences fresh=g_pref;
 if(!value(file,"RayTracedOptimizations",&fresh.enabled)||fresh.enabled>3u)goto invalid;
 /* V4 upgrade compatibility: old saved Quality=2 / Performance=3 now both
  * mean Auto=1. Do not overwrite their independently persisted profiles.
  * The menu persists the canonical 1 when it is first opened; native apply
  * follows RTGIPreset immediately, even before that menu migration happens. */
 if(fresh.enabled==2u||fresh.enabled==3u)fresh.enabled=1u;
 for(unsigned p=0;p<2;p++){
  for(unsigned i=0;i<CR01_RT_LEGACY_FIELDS;i++){
   /* V5.1.0-rc3: RTGI Shading Rays Per Probe was removed from the menu and is
    * ALWAYS Native/Auto: any value still saved in dx12user.settings is ignored. */
   if(i==CR01_RT_L_SHADING){fresh.profile[p].values[i]=0u;continue;}
   if(!value(file,keys[p][i],&fresh.profile[p].values[i]))goto invalid;
  }
  if(!cr01_rt_legacy_profile_valid(&fresh.profile[p]))goto invalid;
 }
 unsigned changed=(fresh.enabled!=g_pref.enabled);
 for(unsigned p=0;p<2;p++)for(unsigned i=0;i<CR01_RT_LEGACY_FIELDS;i++)
  if(fresh.profile[p].values[i]!=g_pref.profile[p].values[i])changed=1;
 if(changed){fresh.revision=g_pref.revision+1u;if(!fresh.revision)fresh.revision=1u;g_pref=fresh;}
 (void)c17_reload(file);
 return 1;
invalid:
 cr01_rt_legacy_release(&g_owner);
 cr01_c17_caps_release(&g_c17);
 if(g_logged++<48u)cr01_release_log_event("URT: Normal RT: invalid RTGI preference; URT values released to native\r\n");
 g_pref.enabled=0;
 ++g_pref.revision;
 return 0;
}
void cr01_rt_legacy501_tick(unsigned mode,unsigned master){
 if(!g_ready)return;
 unsigned preset=__atomic_load_n(g_preset,__ATOMIC_ACQUIRE);
 unsigned shading=__atomic_load_n(g_shading,__ATOMIC_ACQUIRE);
 (void)cr01_rt_legacy_update(&g_owner,&g_pref,cr01_rt_legacy_mode_eligible((CR01RenderMode)mode),master,preset,shading==32u);
 /* C17 only eligible after accepted C16 owner has validated live RT mode,
  * Master, RT parent, six C16 fields, preset signature and ray ownership. */
 const unsigned selection=g_pref.enabled==2u?1u:g_pref.enabled==3u?0u:preset;
 if(g_c17_ready)(void)cr01_c17_caps_update(&g_c17,&g_c17_profiles[selection<=1u?selection:0u],
   g_c17_preferences_valid&&g_owner.active&&g_pref.enabled&&master&&
   cr01_rt_legacy_mode_eligible((CR01RenderMode)mode),selection,g_owner.generation,g_c17_revision);
 if(g_owner.mask!=g_prior_mask){
  if(g_logged++<48u)cr01_release_log_event(g_owner.mask?
   "URT: Normal RT: RTGI settings applied\r\n":
   "URT: Normal RT: RTGI settings released (native values restored)\r\n");
  g_prior_mask=g_owner.mask;
 }
}
void cr01_rt_legacy501_shutdown(void){cr01_c17_caps_release(&g_c17);cr01_rt_legacy_release(&g_owner);}
