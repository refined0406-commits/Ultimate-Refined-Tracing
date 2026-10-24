/* V5.1.0-rc3: RTGI Shading Rays Per Probe is removed and always Native/Auto.
 * Drives the real cr01_rt_legacy501.c adapter with mocked engine slots and a
 * mocked settings store that still holds rc2/V5.0 Shading Rays values. Proves
 * the saved values are ignored (NumShadingRaysPerProbe stays native 32) while
 * the other RTGI fields still apply and release exactly as before. */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "../../src/cr01_rt_legacy501.h"
#include "../../src/urt_cvar_index.h"
typedef unsigned short WCHAR; typedef unsigned long DWORD;
static int fails;
#define CHECK(c,m) do{int _c=!!(c);printf("%s %s\n",_c?"PASS":"FAIL",m);if(!_c)fails++;}while(0)
static uint8_t img[0x1000]; static UrtCvarIndex ix;
static uint32_t slot[8]; /* 0..4 bindings, 5 c17 global, 6 c17 grid1 */
static uint32_t shading=32u, preset=1u;
static const char*kv[64][2]; static int nkv;
static void setkv(const char*k,const char*v){for(int i=0;i<nkv;i++)if(!strcmp(kv[i][0],k)){kv[i][1]=v;return;}kv[nkv][0]=k;kv[nkv][1]=v;nkv++;}
DWORD urt_cfg_get(const WCHAR*k,WCHAR*out,DWORD cap){char n[64];int i=0;while(k[i]&&i<63){n[i]=(char)k[i];i++;}n[i]=0;
 for(int j=0;j<nkv;j++)if(!strcmp(kv[j][0],n)){DWORD l=(DWORD)strlen(kv[j][1]);if(l>=cap)l=cap-1;for(DWORD x=0;x<l;x++)out[x]=kv[j][1][x];out[l]=0;return l;}
 out[0]=0;return 0;}
int cr01_gate_verify_exe_file(void){return 0;}
void cr01_release_log_event(const char*t){(void)t;}
const UrtCvarIndex*urt_runtime_cvar_index(void){return &ix;}
static const char*names[7]={"NumClassificationRaysPerProbe","NumClassificationRaysPerProbe","MaxProbesClassificatedPerFrame","AOPretraceQualityLevel","ShadingRaysPerFrameBudget","MaxProbesShadedPerFrame","MaxProbesShadedPerFrame"};
static const char*cats[7]={"RTXGI/GlobalGrid/Dbg","RTXGI/Grid1","RTXGI/Grid1","Rendering/NRD/Shadows","RTXGI/Grid1","RTXGI/GlobalGrid","RTXGI/Grid1"};
static const uint32_t defs[7]={128u,32u,262144u,0u,524288u,16384u,262144u};
uint32_t*urt_cvar_lookup_scoped(const UrtCvarIndex*x,const char*n,const char*c,uint64_t k,uint32_t*d){(void)x;(void)k;
 for(int i=0;i<7;i++)if(!strcmp(n,names[i])&&!strcmp(c,cats[i])){uint32_t desc=0x100u+0x40u*i;memcpy(img+desc+24,&defs[i],4);*d=desc;return &slot[i];}
 return 0;}
uint32_t*cr01_legacy_approved_find_cvar(uint8_t*b,uint32_t s,const char*n){(void)b;(void)s;
 if(!strcmp(n,"NumShadingRaysPerProbe"))return &shading; if(!strcmp(n,"RTGIPreset"))return &preset; return 0;}
static void native(void){slot[0]=128u;slot[1]=32u;slot[2]=262144u;slot[3]=4u;slot[4]=524288u;slot[5]=16384u;slot[6]=262144u;shading=32u;preset=1u;}
static const WCHAR path[]={'x',0};
int main(void){
 ix.base=img;ix.image_size=sizeof img;native();
 CHECK(cr01_rt_legacy501_init(img,sizeof img),"adapter init with mocked Patch 5.01 slots");
 setkv("RayTracedOptimizations","2"); /* explicit Quality profile */
 const char*g[2]={"RTQuality","RTPerf"};
 for(int p=0;p<2;p++){char*b;static char buf[2][6][64];
  const char*f[6]={"ClassificationPrimary","ClassificationSecondary","MaxProbes","AOQuality","ShadingBudget","ShadingRaysPerProbe"};
  const char*v[6]={"64","16","65536","2","262144","16"};  /* rc2 Balanced incl. Shading Rays 16 */
  for(int i=0;i<6;i++){b=buf[p][i];snprintf(b,64,"%s%s",g[p],f[i]);setkv(b,v[i]);}}
 CHECK(cr01_rt_legacy501_reload(path),"reload with leftover saved Shading Rays Per Probe = 16 (both groups)");
 for(int i=0;i<50;i++)cr01_rt_legacy501_tick(1u,1u);
 CHECK(shading==32u,"Normal RT + Master On: NumShadingRaysPerProbe stays native 32 (saved 16 ignored)");
 CHECK(slot[0]==64u&&slot[1]==16u&&slot[2]==65536u&&slot[3]==2u&&slot[4]==262144u,"other five RTGI fields still applied (64/16/65536/2/262144)");
 setkv("RTQualityShadingRaysPerProbe","8");setkv("RTPerfShadingRaysPerProbe","24");
 CHECK(cr01_rt_legacy501_reload(path),"reload with other leftover values (8/24) still valid");
 for(int i=0;i<10;i++)cr01_rt_legacy501_tick(1u,1u);
 CHECK(shading==32u,"still native 32 after any saved Shading Rays value");
 setkv("RTQualityShadingRaysPerProbe","999");
 CHECK(cr01_rt_legacy501_reload(path),"garbage leftover Shading Rays value cannot invalidate the other RTGI preferences");
 for(int i=0;i<10;i++)cr01_rt_legacy501_tick(1u,1u);
 CHECK(shading==32u&&slot[0]==64u,"garbage leftover: shading native, others still applied");
 setkv("RayTracedOptimizations","3");CHECK(cr01_rt_legacy501_reload(path),"switch to Performance profile");
 for(int i=0;i<10;i++)cr01_rt_legacy501_tick(1u,1u);
 CHECK(shading==32u&&slot[0]==64u&&slot[1]==16u,"Performance group: shading native, others applied");
 cr01_rt_legacy501_tick(2u,1u);
 CHECK(shading==32u&&slot[0]==128u&&slot[1]==32u&&slot[2]==262144u&&slot[3]==4u&&slot[4]==524288u,"PT: every RTGI field restored to native");
 cr01_rt_legacy501_tick(1u,1u);cr01_rt_legacy501_tick(1u,0u);
 CHECK(shading==32u&&slot[0]==128u,"Master Off: native");
 cr01_rt_legacy501_tick(1u,1u);cr01_rt_legacy501_shutdown();
 CHECK(shading==32u&&slot[0]==128u&&slot[4]==524288u,"shutdown: native");
 printf("TOTAL %s %d\n",fails?"FAIL":"PASS",fails);return fails?1:0;}
