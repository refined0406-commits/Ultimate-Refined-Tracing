/* Proves the V5 one-pass CVar index returns exactly what the V4.3.1
 * find_cvar resolver returns, on the real Patch 5.01 witcher3.exe image,
 * and measures both. Usage: test_cvar_index <witcher3.exe> */
#define _POSIX_C_SOURCE 199309L
#include "pe_map.h"
#include "../../src/urt_cvar_index.h"
#include <time.h>
#define IMAGE_SCN_MEM_READ 0x40000000u
#define IMAGE_SCN_MEM_WRITE 0x80000000u
static uint32_t u32(const uint8_t*p){return (uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;}
static unsigned u16(const uint8_t*p){return (unsigned)p[0]|(unsigned)p[1]<<8;}
#include "ref_find_cvar.inc"   /* verbatim V4.3.1 writable_range + find_cvar */

static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec/1e9;}
static const char *kUsed[]={"PTEnable","EnableRT","EnableRtRadiance","PTIndirectLightMultiplier",
 "PTSunImpact","PTSkyImpact","PTAOEnable","PTAOStrength","PTAORadius","PTAOFinalValuePower",
 "PTGrassScale","PTGrassNightScale","PTLateSpecularScreenSpaceTrace","PTLateSpecularCheckerboard",
 "RTSTBillboards","NumShadingRaysPerProbe","RTGIPreset","LODBudget","LODDistanceLimit",
 "RTSTBillboardsProximity","MaxProbesShadedPerFrame","PTNumBounces","PTHairNumBounces",
 "TextureUnloadSpeed","MeshOffset","GenericOffset","AutoDiscardCount","IdleFrameCount",
 "DoesNotExist_URT"};

int main(int argc,char**argv){
  PeImage img; if(argc<2||!pe_map_file(argv[1],&img)){puts("FAIL map");return 1;}
  static UrtCvarEntry store[200000]; UrtCvarIndex ix;
  double t0=now();
  if(!urt_cvar_index_build(&ix,img.base,img.size,store,200000)){puts("FAIL build");return 1;}
  double t1=now();
  unsigned nused=sizeof(kUsed)/sizeof(kUsed[0]), fail=0, resolved=0;
  double ref_total=0,new_total=0;
  for(unsigned i=0;i<nused;i++){
    double a=now(); uint32_t*r=find_cvar(img.base,img.size,kUsed[i]); double b=now();
    uint32_t*n=urt_cvar_lookup(&ix,kUsed[i]); double c=now();
    ref_total+=b-a; new_total+=c-b;
    if(r!=n){printf("MISMATCH %s ref=%p new=%p\n",kUsed[i],(void*)r,(void*)n);fail++;}
    if(r){resolved++;printf("  %-32s rva=0x%07x value=0x%08x\n",kUsed[i],(unsigned)((uint8_t*)r-img.base),*r);}
    else printf("  %-32s (unresolved/ambiguous in both)\n",kUsed[i]);
  }
  /* Sample of all distinct registry names in the index: every 37th entry. */
  unsigned sampled=0;
  for(uint32_t i=0;i<ix.count;i+=37){
    const char*nm=(const char*)img.base+store[i].name_rva;
    if(find_cvar(img.base,img.size,nm)!=urt_cvar_lookup(&ix,nm)){printf("MISMATCH sample %s\n",nm);fail++;}
    sampled++;
  }
  printf("index entries=%u build=%.1f ms; %u used names: ref %.1f ms total, new %.3f ms total; %u resolved; %u sampled names\n",
    ix.count,(t1-t0)*1e3,nused,ref_total*1e3,new_total*1e3,resolved,sampled);
  if(fail){printf("FAIL %u mismatches\n",fail);return 1;}
  puts("PASS cvar index == V4.3.1 find_cvar on real image");
  return 0;
}
