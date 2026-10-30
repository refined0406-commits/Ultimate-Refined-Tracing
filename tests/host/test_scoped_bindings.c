/* RTGI/C17 descriptors resolved by (name,category,kind,default) land exactly on
 * the V4.3.1 pinned descriptor and storage RVAs of the real Patch 5.01 image. */
#include "urt_test_build.h"
#include "pe_map.h"
#include "../../src/urt_cvar_index.h"
static const struct {const char*n,*c;uint64_t k;uint32_t def,reg,stor;} B[]={  /* Patch 5.01 RVAs; RV() gives the build under test */
 {"NumClassificationRaysPerProbe","RTXGI/GlobalGrid/Dbg",0x201,128,0x4069b88,0x4069ba8},
 {"NumClassificationRaysPerProbe","RTXGI/Grid1",0x201,32,0x4069fb0,0x4069fd0},
 {"MaxProbesClassificatedPerFrame","RTXGI/Grid1",0x201,262144,0x406a0c8,0x406a0e8},
 {"AOPretraceQualityLevel","Rendering/NRD/Shadows",0x101,0,0x4068508,0x4068528},
 {"ShadingRaysPerFrameBudget","RTXGI/Grid1",0x201,524288,0x4069f78,0x4069f98},
 {"MaxProbesShadedPerFrame","RTXGI/GlobalGrid",0x201,16384,0x4069c68,0x4069c88},
 {"MaxProbesShadedPerFrame","RTXGI/Grid1",0x201,262144,0x406a090,0x406a0b0}};
int main(int c,char**v){(void)c;PeImage im;static UrtCvarEntry st[16384];UrtCvarIndex ix;int f=0;
 if(!pe_map_file(v[1],&im)||!urt_test_build_init(im.size)||!urt_cvar_index_build(&ix,im.base,im.size,st,16384)){puts("FAIL setup");return 1;}
 for(unsigned i=0;i<sizeof B/sizeof B[0];i++){uint32_t d=0;uint32_t*s=urt_cvar_lookup_scoped(&ix,B[i].n,B[i].c,B[i].k,&d);
  uint32_t dv; if(s)memcpy(&dv,im.base+d+24,4);
  int ok=s&&d==RV(B[i].reg)&&(uint32_t)((uint8_t*)s-im.base)==RV(B[i].stor)&&dv==B[i].def;
  printf("  %-32s %-22s desc=0x%x %s\n",B[i].n,B[i].c,d,ok?"OK":"MISMATCH"); f+=!ok;}
 /* scoping matters: the bare name must be ambiguous */
 if(urt_cvar_lookup(&ix,"MaxProbesShadedPerFrame")||urt_cvar_lookup(&ix,"NumClassificationRaysPerProbe")){puts("FAIL: duplicate names not ambiguous");f++;}
 /* wrong category / kind are rejected */
 if(urt_cvar_lookup_scoped(&ix,"MaxProbesShadedPerFrame","RTXGI/Grid2",0x201,0)||urt_cvar_lookup_scoped(&ix,"MaxProbesShadedPerFrame","RTXGI/Grid1",0x101,0)){puts("FAIL scope");f++;}
 if(f){puts("FAIL");return 1;} puts("PASS scoped RTGI descriptors == V4.3.1 pins");return 0;}
