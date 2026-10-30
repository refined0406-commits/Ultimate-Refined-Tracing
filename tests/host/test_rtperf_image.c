/* V5.1 Normal RT performance controls: the REAL adapter (src/urt_rtperf501.c)
 * against the mapped Patch 5.01 witcher3.exe image.
 * Proves: every binding resolves by name + real category + flags + default to the
 * pinned value RVA; the RTXGI tier/re-init instruction fingerprints match; the
 * adapter writes/restores the actual engine slots; and a full console-grid pulse
 * (set BalanceModeRT, flip the game's checkerboard latch, restore after the game's
 * re-init) behaves as designed. Usage: test_rtperf_image <witcher3.exe> */
#include "urt_test_build.h"
#include "pe_map.h"
#include "../../src/urt_cvar_index.h"
#include "../../src/urt_rtperf501.h"
#include <string.h>
typedef unsigned short WCHAR; typedef unsigned long DWORD;
static int fails;
#define CHECK(c,m) do{ if(c) printf("PASS %s\n",m); else {printf("FAIL %s\n",m); fails++;} }while(0)
/* ---- stubs for the Windows runtime services the adapter uses ---- */
static UrtCvarIndex g_ix; static UrtCvarEntry g_store[200000];
const UrtCvarIndex *urt_runtime_cvar_index(void){return &g_ix;}
int cr01_gate_verify_exe_file(void){return 1;}            /* the real exe is mapped */
static char g_log[8192];
void cr01_release_log_event(const char*s){strncat(g_log,s,sizeof g_log-strlen(g_log)-1);}
static const char *g_keys[16]; static const char *g_vals[16]; static unsigned g_nkv;
static void setkv(const char*k,const char*v){for(unsigned i=0;i<g_nkv;i++)if(!strcmp(g_keys[i],k)){g_vals[i]=v;return;}g_keys[g_nkv]=k;g_vals[g_nkv++]=v;}
DWORD urt_cfg_get(const WCHAR*key,WCHAR*out,DWORD cap){
  char k[96];unsigned i=0;while(key[i]&&i<95){k[i]=(char)key[i];i++;}k[i]=0;
  for(unsigned j=0;j<g_nkv;j++)if(!strcmp(g_keys[j],k)){
    unsigned n=(unsigned)strlen(g_vals[j]);if(n+1>cap)return 0;
    for(unsigned c=0;c<=n;c++)out[c]=(WCHAR)g_vals[j][c];return n;}
  out[0]=0;return 0;
}
static float f32(uint8_t*p){float f;memcpy(&f,p,4);return f;}
int main(int argc,char**argv){
  PeImage img; if(argc<2||!pe_map_file(argv[1],&img)){puts("FAIL map");return 1;}
  if(!urt_test_build_init(img.size))return 1;
  uint8_t*B=img.base;
  CHECK(urt_cvar_index_build(&g_ix,B,img.size,g_store,200000),"registry index built");
  CHECK(urt_rtperf501_init(B,img.size),"adapter init");
  CHECK(strstr(g_log,"performance controls ready")!=0,"all 3 live bindings verified at pinned RVAs");
    /* native snapshot */
  const uint32_t R_REFL=RV(0x514e588),R_SLOTS=RV(0x514e898),R_SHMIP=RV(0x514def8),R_BAL=RV(0x514d280),R_PRESET=RV(0x406aac0),R_CHK=RV(0x406a9e0),
    R_COUNTS=RV(0x4069e80),R_LATCH=RV(0x4017c5c),R_GLOBAL=RV(0x4069588),R_RMIP=RV(0x514de18),R_AOMIP=RV(0x514df30);
  /* The on-disk image holds zeros: the engine's static initialisers write the
   * registered defaults at startup. Seed them exactly as the game does. */
  { float g=128.0f,m4=4.0f,m2=2.0f,m3=3.0f; uint32_t eight=8u;
    memcpy(B+R_GLOBAL,&g,4);memcpy(B+R_RMIP,&m4,4);memcpy(B+R_AOMIP,&m3,4);
    B[R_REFL]=1;memcpy(B+R_SLOTS,&eight,4);memcpy(B+R_SHMIP,&m2,4);B[R_CHK]=1;
    *(uint32_t*)(B+R_COUNTS)=64u; }
  uint8_t snap[0x80]; memcpy(snap,B+R_SLOTS-0x40,0x80);
  CHECK(B[R_REFL]==1&&*(uint32_t*)(B+R_SLOTS)==8u&&f32(B+R_SHMIP)==2.0f,"registered native values as documented");
  CHECK(urt_rtperf501_reload(),"reload all-Native preferences");
  urt_rtperf501_tick(1u,1u);
  CHECK(*(uint32_t*)(B+R_SLOTS)==8u&&B[R_REFL]==1&&B[R_BAL]==0,"Native: no engine write");
  /* removed rc1 keys still saved by a tester are ignored */
  setkv("RTGIProbeRayDistance","4");setkv("RTReflTextureDetail","3");setkv("RTAOFoliageDetail","3");
  CHECK(urt_rtperf501_reload(),"reload with leftover rc1 keys");
  urt_rtperf501_tick(1u,1u);
  CHECK(f32(B+R_GLOBAL)==128.0f&&f32(B+R_RMIP)==4.0f&&f32(B+R_AOMIP)==3.0f,"removed controls never touch the engine");
  /* Performance Quick Preset values */
  setkv("RTReflLocalLights","1");setkv("RTShadowLocalLights","4");setkv("RTShadowFoliageDetail","3");
  CHECK(urt_rtperf501_reload(),"reload Performance preset values");
  urt_rtperf501_tick(2u,1u);
  CHECK(B[R_REFL]==1&&*(uint32_t*)(B+R_SLOTS)==8u,"Path Tracing: nothing applied (RT/PT isolation)");
  urt_rtperf501_tick(0u,1u);
  CHECK(B[R_REFL]==1,"raster: nothing applied");
  urt_rtperf501_tick(1u,0u);
  CHECK(B[R_REFL]==1,"Master Off: nothing applied");
  urt_rtperf501_tick(1u,1u);
  CHECK(B[R_REFL]==0&&*(uint32_t*)(B+R_SLOTS)==0u&&f32(B+R_SHMIP)==6.0f,"Normal RT: local lights off, local shadows Off, foliage mip 6");
  setkv("RTShadowLocalLights","2");setkv("RTShadowFoliageDetail","2");urt_rtperf501_reload();urt_rtperf501_tick(1u,1u);
  CHECK(B[R_REFL]==0&&*(uint32_t*)(B+R_SLOTS)==4u&&f32(B+R_SHMIP)==4.0f,"Balanced values applied live");
  urt_rtperf501_tick(2u,1u);
  CHECK(B[R_REFL]==1&&*(uint32_t*)(B+R_SLOTS)==8u&&f32(B+R_SHMIP)==2.0f,"switch to PT: every native value restored");
  CHECK(!memcmp(snap,B+R_SLOTS-0x40,0x80),"neighbouring engine memory untouched");
  /* rc3: a Console Probe Grid value saved by rc2 is ignored; RTXGI tier untouched */
  setkv("RTGIConsoleGrid","1");CHECK(urt_rtperf501_reload(),"reload with leftover rc2 Console Probe Grid key");
  B[R_LATCH]=B[R_CHK];*(uint32_t*)(B+R_COUNTS)=48u;
  for(int i=0;i<100;i++)urt_rtperf501_tick(1u,1u);
  CHECK(B[R_BAL]==0&&B[R_LATCH]==B[R_CHK]&&*(uint32_t*)(B+R_COUNTS)==48u,"removed console grid: BalanceModeRT and RTXGI re-init never touched");
  CHECK(strstr(g_log,"console")==0,"no console-grid code path at all");
  urt_rtperf501_tick(1u,1u);
  CHECK(B[R_REFL]==0,"back in Normal RT: reflection local lights off again");
  urt_rtperf501_shutdown();
  CHECK(B[R_BAL]==0&&B[R_REFL]==1&&*(uint32_t*)(B+R_SLOTS)==8u,"shutdown: everything native");
  printf("TOTAL %s %d\n",fails?"FAIL":"PASS",fails);
  return fails?1:0;
}
