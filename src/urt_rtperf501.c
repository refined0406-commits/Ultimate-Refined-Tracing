/* V5.1 Normal RT performance controls: Patch 5.01 adapter (Windows side).
 *
 * - Every engine variable is resolved through the one-pass registry index by
 *   name + REAL category (+48) + flags (+8) + registered default (+24); on the
 *   known Patch 5.01 executable the value slot must also equal the pinned RVA.
 * - Live controls are owned with compare-and-swap only in Normal RT with
 *   Master On, and released (native restored) otherwise. Nothing is persisted:
 *   these are internal engine variables, not game menu settings.
 * - rc3: the RTGI console probe grid was removed after gameplay testing; this
 *   module no longer touches BalanceModeRT or RTXGI re-initialisation at all.
 * See docs/V5.1_RT_RESEARCH.md for the evidence behind every address. */
#include "urt_build.h"
#include "urt_rtperf501.h"
#include "urt_rtperf_core.h"
#include "urt_cvar_index.h"
#include "cr01_release_log.h"
#include "cr01_rt_legacy_mode.h"
#include <stddef.h>
typedef unsigned short WCHAR;
typedef unsigned long DWORD;
extern DWORD urt_cfg_get(const WCHAR*,WCHAR*,DWORD);
extern const UrtCvarIndex *urt_runtime_cvar_index(void);
extern int cr01_gate_verify_exe_file(void);

struct Bind { const char *name,*category; uint64_t flags; uint32_t def, pinned_value_rva; };
/* Owned live fields, in UrtRfOwner field order. */
static const struct Bind kField[URT_RF_COUNT]={
  {"RtRadianceShadingLocalLights","Rendering/RT",0x200u,1u,0x514e588u},
  {"LocalShadowsSlots","Rendering/RT",0x201u,8u,0x514e898u},
  {"ShadowsAlphaTestMipLevel","Rendering/RT",0x202u,0x40000000u,0x514def8u},
};
static UrtRfOwner g_rf;
static UrtRpPrefs g_prefs;
static unsigned g_ready, g_prefs_ok, g_logged;

static void log1(const char*s){if(g_logged++<64u)cr01_release_log_event(s);}

static uint32_t *bind(uint8_t*base,uint32_t image,const struct Bind*b,int known){
  const UrtCvarIndex*ix=urt_runtime_cvar_index();uint32_t desc=0;
  if(!ix||ix->base!=base||ix->image_size!=image)return 0;
  uint32_t*slot=urt_cvar_lookup_cat(ix,b->name,b->category,b->flags,b->def,&desc);
  if(!slot)return 0;
  if(known&&(uint8_t*)slot!=base+urt_rva(b->pinned_value_rva))return 0;
  return slot;
}
int urt_rtperf501_init(uint8_t*base,uint32_t image){
  const int known=cr01_gate_verify_exe_file();
  urt_rf_init(&g_rf);
  g_ready=g_prefs_ok=0;
  for(unsigned i=0;i<URT_RP_COUNT;i++)g_prefs.v[i]=0;
  unsigned bound=0;
  for(unsigned i=0;i<URT_RF_COUNT;i++){g_rf.slot[i]=bind(base,image,&kField[i],known);if(g_rf.slot[i])bound++;}
  g_ready=1;
  cr01_release_log_event(bound==URT_RF_COUNT?"URT: Normal RT performance controls ready (RT reflection local lights, RT local-light shadows, RT foliage shadows)\r\n":
    "URT: Normal RT performance controls: some engine settings not verified; those controls stay native\r\n");
  return 1;
}

static int read_index(const char*name,uint32_t*out){
  WCHAR k[64],s[16];unsigned n=0;
  while(name[n]&&n<63){k[n]=(WCHAR)(unsigned char)name[n];n++;}
  if(name[n])return 0;k[n]=0;s[0]=0;
  DWORD c=urt_cfg_get(k,s,16);
  if(!c){*out=0;return 1;}                 /* game prunes defaults: missing = Native */
  if(c>2)return 0;
  uint32_t v=0;for(DWORD i=0;i<c;i++){if(s[i]<'0'||s[i]>'9')return 0;v=v*10u+(uint32_t)(s[i]-'0');}
  *out=v;return 1;
}
int urt_rtperf501_reload(void){
  if(!g_ready)return 0;
  UrtRpPrefs p;
  for(unsigned i=0;i<URT_RP_COUNT;i++)if(!read_index(urt_rp_keys[i],&p.v[i]))goto invalid;
  if(!urt_rp_prefs_valid(&p))goto invalid;
  g_prefs=p;g_prefs_ok=1;return 1;
invalid:
  g_prefs_ok=0;urt_rf_release(&g_rf);
  log1("URT: Normal RT performance controls: invalid menu value; released to native\r\n");
  return 0;
}

void urt_rtperf501_tick(unsigned mode,unsigned master){
  if(!g_ready)return;
  const unsigned normal_rt=cr01_rt_legacy_mode_eligible((CR01RenderMode)mode);
  urt_rf_update(&g_rf,&g_prefs,g_prefs_ok&&master&&normal_rt);
}
void urt_rtperf501_shutdown(void){
  if(!g_ready)return;
  urt_rf_release(&g_rf);
}
