/* V5.2 PT: Scene Range: the REAL adapter (src/urt_ptperf501.c + src/urt_ptperf_core.c)
 * against the mapped Patch 5.01 witcher3.exe image.
 * Proves: both engine values resolve by name + real category + flags + default to the
 * pinned value RVAs; they are owned only in Path Tracing with Master On; Normal RT,
 * raster, Master Off, invalid input, foreign writes and shutdown restore the exact native
 * values; NO game code byte anywhere is ever written (r2: Small Object Detail and its
 * code bytes were removed) and a saved PTObjectDetail value is ignored.
 * Usage: test_ptperf_image <witcher3.exe> */
#include "pe_map.h"
#include "../../src/urt_cvar_index.h"
#include "../../src/urt_ptperf501.h"
#include "../../src/urt_ptperf_core.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
typedef unsigned short WCHAR; typedef unsigned long DWORD;
static int fails;
#define CHECK(c,m) do{ if(c) printf("PASS %s\n",m); else {printf("FAIL %s\n",m); fails++;} }while(0)
static UrtCvarIndex g_ix; static UrtCvarEntry g_store[200000];
const UrtCvarIndex *urt_runtime_cvar_index(void){return &g_ix;}
int cr01_gate_verify_exe_file(void){return 1;}
static char g_log[16384];
void cr01_release_log_event(const char*s){strncat(g_log,s,sizeof g_log-strlen(g_log)-1);}
static const char *g_keys[32]; static const char *g_vals[32]; static unsigned g_nkv;
static void setkv(const char*k,const char*v){for(unsigned i=0;i<g_nkv;i++)if(!strcmp(g_keys[i],k)){g_vals[i]=v;return;}g_keys[g_nkv]=k;g_vals[g_nkv++]=v;}
DWORD urt_cfg_get(const WCHAR*key,WCHAR*out,DWORD cap){
  char k[96];unsigned i=0;while(key[i]&&i<95){k[i]=(char)key[i];i++;}k[i]=0;
  for(unsigned j=0;j<g_nkv;j++)if(!strcmp(g_keys[j],k)){
    unsigned n=(unsigned)strlen(g_vals[j]);if(n+1>cap)return 0;
    for(unsigned c=0;c<=n;c++)out[c]=(WCHAR)g_vals[j][c];return n;}
  out[0]=0;return 0;
}
static float f32(uint8_t*p){float f;memcpy(&f,p,4);return f;}
static void putf(uint8_t*p,float f){memcpy(p,&f,4);}
enum { PT=2u, RT=1u, RASTER=0u };
int main(int argc,char**argv){
  PeImage img; if(argc<2||!pe_map_file(argv[1],&img)){puts("FAIL map");return 1;}
  uint8_t*B=img.base;
  const uint32_t R_RNG=0x514c800,R_EXT=0x514c7c8;
  CHECK(urt_cvar_index_build(&g_ix,B,img.size,g_store,200000),"registry index built");
  putf(B+R_RNG,400.0f);putf(B+R_EXT,800.0f);B[0x514f4d8]=1;B[0x51510a0]=0;
  /* snapshot of the whole executable text/data: proves no code byte is ever written */
  uint8_t*copy=malloc(img.size);memcpy(copy,B,img.size);
  CHECK(urt_ptperf501_init(B,img.size),"adapter init");
  CHECK(strstr(g_log,"PT performance control ready (PT scene range)")!=0,"both bindings at pinned RVAs");
  CHECK(urt_ptperf501_reload(),"all-Native preferences");
  for(unsigned m=0;m<3;m++)urt_ptperf501_tick(m,1u);
  CHECK(!memcmp(copy,B,img.size),"Native: not a single byte of the image changed in any renderer");
  setkv("PTObjectDetail","4");setkv("PTSceneRange","3");
  CHECK(urt_ptperf501_reload(),"reload Scene Range=200 m (a stale PTObjectDetail=4 is ignored)");
  urt_ptperf501_tick(RT,1u);urt_ptperf501_tick(RASTER,1u);
  CHECK(f32(B+R_RNG)==400.0f&&f32(B+R_EXT)==800.0f,"Normal RT / raster: nothing owned (isolation)");
  urt_ptperf501_tick(PT,0u);
  CHECK(f32(B+R_RNG)==400.0f,"Master Off: nothing owned");
  urt_ptperf501_tick(PT,1u);
  CHECK(f32(B+R_RNG)==200.0f&&f32(B+R_EXT)==200.0f,"PT: scene box half-size 200 m (both PT box variables)");
  CHECK(f32(B+0x514f628)==f32(copy+0x514f628)&&f32(B+0x514a208)==f32(copy+0x514a208),"the removed Small Object Detail values are never touched");
  {uint8_t*a=B,*b=copy;size_t diff=0;for(size_t i=0;i<img.size;i++)if(a[i]!=b[i]&&!(i>=R_RNG&&i<R_RNG+4)&&!(i>=R_EXT&&i<R_EXT+4))diff++;
   CHECK(diff==0,"only the two owned 4-byte values differ from the original image (no code byte)");}
  urt_ptperf501_tick(PT,1u);
  CHECK(f32(B+R_RNG)==200.0f,"steady state");
  urt_ptperf501_tick(RT,1u);
  CHECK(f32(B+R_RNG)==400.0f&&f32(B+R_EXT)==800.0f,"switch to Normal RT: native restored");
  urt_ptperf501_tick(PT,1u);
  CHECK(f32(B+R_RNG)==200.0f,"back to PT: re-applied");
  putf(B+R_RNG,300.0f);urt_ptperf501_tick(PT,1u);
  CHECK(strstr(g_log,"never overwritten")!=0,"foreign write logged");
  B[0x514f4d8]=0;urt_ptperf501_tick(PT,1u);
  CHECK(f32(B+R_EXT)==800.0f,"PT selected but EnableRT=0: native");
  B[0x514f4d8]=1;putf(B+R_RNG,400.0f);
  setkv("PTSceneRange","9");CHECK(!urt_ptperf501_reload(),"out-of-range value rejected");
  CHECK(f32(B+R_RNG)==400.0f&&f32(B+R_EXT)==800.0f,"invalid input: everything native");
  setkv("PTSceneRange","2");urt_ptperf501_reload();urt_ptperf501_tick(PT,1u);
  CHECK(f32(B+R_RNG)==250.0f,"valid again: 250 m");
  urt_ptperf501_shutdown();
  CHECK(f32(B+R_RNG)==400.0f&&f32(B+R_EXT)==800.0f,"shutdown: all native");
  CHECK(!memcmp(copy,B,img.size),"after shutdown the image is byte-identical to the original");
  free(copy);
  printf("TOTAL %s %d\n",fails?"FAIL":"PASS",fails);
  return fails?1:0;
}
