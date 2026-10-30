/* V5.2 Path Tracing performance control (PT: Scene Range): Patch 5.01 adapter.
 *
 * Engine side (live, Path Tracing + Master On only, compare-and-swap ownership):
 *  - PT: Scene Range owns ProximityTLASSize + ProximityExtendedTLASSize (the PT box)
 *    and only ever lowers them.
 * No shader and no game code is changed: the PT library is the V5.1.0 one (Orange
 * Hair Fix identity). Normal RT and raster never see an owned value.
 * V5.2 r2: PT: Small Object Detail (two more values + four PT-path code bytes) was
 * removed after RTX 4080 testing (no FPS change); nothing of it remains here.
 * Evidence: private research builds R2-R5 (docs/V5.2_PT_RESEARCH.md there). */
#include "urt_build.h"
#include "urt_ptperf501.h"
#include "urt_ptperf_core.h"
#include "urt_cvar_index.h"
#include "cr01_release_log.h"
#include "cr01_native_execution.h"
#include <stddef.h>
typedef unsigned short WCHAR;
typedef unsigned long DWORD;
extern DWORD urt_cfg_get(const WCHAR*,WCHAR*,DWORD);
extern const UrtCvarIndex *urt_runtime_cvar_index(void);
extern int cr01_gate_verify_exe_file(void);

struct Bind { const char *name,*category; uint64_t flags; uint32_t def, pinned_value_rva; };
static const struct Bind kField[URT_PF_COUNT]={
  {"ProximityTLASSize","Rendering/RT/SpeedTree",0x202u,0x43c80000u,0x514c800u},
  {"ProximityExtendedTLASSize","Rendering/RT",0x202u,0x44480000u,0x514c7c8u},
};
/* The game's own "PT is rendering" condition in the criteria builder:
 * PTEnable && EnableRT && !DisableEnableRayTracing. URT acts only when it holds. */
static const struct Bind kGateRT={"EnableRT","Rendering/RT",0x100u,1u,0x514f4d8u};
static const struct Bind kGateOff={"DisableEnableRayTracing","PostProcess",0x200u,0u,0x51510a0u};
static uint8_t *g_rt_on,*g_rt_disabled;
static UrtPfOwner g_pf;
static UrtPxPrefs g_prefs;
static unsigned g_ready, g_prefs_ok, g_logged;
static uint32_t g_last_conflicts;

static void log1(const char*s){if(g_logged++<64u)cr01_release_log_event(s);}

static uint32_t *bind(uint8_t*base,uint32_t image,const struct Bind*b,int known){
  const UrtCvarIndex*ix=urt_runtime_cvar_index();uint32_t desc=0;
  if(!ix||ix->base!=base||ix->image_size!=image)return 0;
  uint32_t*slot=urt_cvar_lookup_cat(ix,b->name,b->category,b->flags,b->def,&desc);
  if(!slot)return 0;
  if(known&&(uint8_t*)slot!=base+urt_rva(b->pinned_value_rva))return 0;
  return slot;
}
int urt_ptperf501_init(uint8_t*base,uint32_t image){
  const int known=cr01_gate_verify_exe_file();
  urt_pf_init(&g_pf);
  g_ready=g_prefs_ok=0;g_last_conflicts=0;
  for(unsigned i=0;i<URT_PX_COUNT;i++)g_prefs.v[i]=0;
  unsigned bound=0;
  for(unsigned i=0;i<URT_PF_COUNT;i++){g_pf.slot[i]=bind(base,image,&kField[i],known);if(g_pf.slot[i])bound++;}
  g_rt_on=(uint8_t*)bind(base,image,&kGateRT,known);
  g_rt_disabled=(uint8_t*)bind(base,image,&kGateOff,known);
  if(!g_rt_on||!g_rt_disabled){for(unsigned i=0;i<URT_PF_COUNT;i++)g_pf.slot[i]=0;bound=0;}
  g_ready=1;
  cr01_release_log_event(bound==URT_PF_COUNT?
    "URT: PT performance control ready (PT scene range)\r\n":
    "URT: PT performance control: engine sites not verified; PT Scene Range stays native\r\n");
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
int urt_ptperf501_reload(void){
  if(!g_ready)return 0;
  UrtPxPrefs p;
  for(unsigned i=0;i<URT_PX_COUNT;i++)if(!read_index(urt_px_keys[i],&p.v[i]))goto invalid;
  if(!urt_px_prefs_valid(&p))goto invalid;
  g_prefs=p;g_prefs_ok=1;
  return 1;
invalid:
  g_prefs_ok=0;urt_pf_release(&g_pf);
  log1("URT: PT Scene Range: invalid menu value; released to native\r\n");
  return 0;
}
void urt_ptperf501_tick(unsigned mode,unsigned master){
  if(!g_ready)return;
  const unsigned pt_path=g_rt_on&&g_rt_disabled&&
    __atomic_load_n(g_rt_on,__ATOMIC_ACQUIRE)==1u&&__atomic_load_n(g_rt_disabled,__ATOMIC_ACQUIRE)==0u;
  const unsigned eligible=g_prefs_ok&&master&&mode==(unsigned)CR01_RENDER_PT&&pt_path;
  urt_pf_update(&g_pf,&g_prefs,eligible);
  if(g_pf.conflicts!=g_last_conflicts){
    g_last_conflicts=g_pf.conflicts;
    log1("URT: PT Scene Range: a value was changed by the game or another mod; released (never overwritten)\r\n");
  }
}
void urt_ptperf501_shutdown(void){
  if(!g_ready)return;
  urt_pf_release(&g_pf);
}
