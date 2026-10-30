/* V5.3 private development: the REAL adapter (urt_gfx501.c + core + generated table + compat) against the
 * mapped Patch 5.01 witcher3.exe, for the nine V5.3 rows.
 * Proves: every V5.3 row resolves by name + real category (+48) + flags + registered default to its pinned
 * value RVA, and the game's own descriptor has no save bit for any of them; the startup step writes only the
 * two RT acceleration-structure values; each renderer writes only its in-scope rows and nothing else in the
 * image; Master Off and shutdown give a byte-identical image; a W3LoadFix [Override] line keeps that row
 * unbound (never written). Usage: test_v53_image <witcher3.exe> */
#include "urt_test_build.h"
#include "pe_map.h"
#include "../../src/urt_cvar_index.h"
#include "../../src/urt_gfx501.h"
#include "../../src/urt_gfx_core.h"
#include <string.h>
#include <stdlib.h>
typedef unsigned short WCHAR; typedef unsigned long DWORD;
static int fails;
#define CHECK(c,m) do{ if(c) printf("PASS %s\n",m); else {printf("FAIL %s\n",m); fails++;} }while(0)
static UrtCvarIndex g_ix; static UrtCvarEntry g_store[200000];
const UrtCvarIndex *urt_runtime_cvar_index(void){return &g_ix;}
int cr01_gate_verify_exe_file(void){return 1;}
static char g_log[65536];
void cr01_release_log_event(const char*s){strncat(g_log,s,sizeof g_log-strlen(g_log)-1);}
static const char *g_keys[64]; static char g_vals[64][8]; static unsigned g_nkv;
static void setkv(const char*k,const char*v){for(unsigned i=0;i<g_nkv;i++)if(!strcmp(g_keys[i],k)){strcpy(g_vals[i],v);return;}g_keys[g_nkv]=k;strcpy(g_vals[g_nkv++],v);}
DWORD urt_cfg_get_ext(const WCHAR*key,WCHAR*out,DWORD cap){
  char k[96];unsigned i=0;while(key[i]&&i<95){k[i]=(char)key[i];i++;}k[i]=0;
  for(unsigned j=0;j<g_nkv;j++)if(!strcmp(g_keys[j],k)){
    unsigned n=(unsigned)strlen(g_vals[j]);if(n+1>cap)return 0;
    for(unsigned c=0;c<=n;c++)out[c]=(WCHAR)g_vals[j][c];return n;}
  out[0]=0;return 0;
}
static size_t diff_outside(const uint8_t*a,const uint8_t*b,size_t n,unsigned first){
  size_t bad=0;for(size_t i=0;i<n;i++)if(a[i]!=b[i]){int ok=0;
    for(unsigned r=first;r<URT_GX_COUNT;r++)if(RV(urt_gx_defs[r].rva)&&i>=RV(urt_gx_defs[r].rva)&&i<RV(urt_gx_defs[r].rva)+4u)ok=1;
    if(!ok)bad++;}
  return bad;
}
static uint32_t rd(const uint8_t*B,unsigned r){uint32_t v;memcpy(&v,B+RV(urt_gx_defs[r].rva),4);return v;}
int main(int argc,char**argv){
  PeImage img; if(argc<2||!pe_map_file(argv[1],&img)){puts("FAIL map");return 1;}
  if(!urt_test_build_init(img.size))return 1;
  uint8_t*B=img.base;
  const unsigned first=URT_GX_IX_TEXUNLOADSPEED;
  CHECK(urt_cvar_index_build(&g_ix,B,img.size,g_store,200000),"registry index built");
  /* the file image holds registered defaults only after the game's static initialisers: seed them */
  for(unsigned i=0;i<URT_GX_COUNT;i++){const UrtGxDef*d=&urt_gx_defs[i];if(!RV(d->rva))continue;
    if(d->kind==URT_GX_BOOL)B[RV(d->rva)]=(uint8_t)d->def;else memcpy(B+RV(d->rva),&d->def,4);}
  unsigned bound=0,nosave=0;
  for(unsigned i=first;i<first+9u;i++){const UrtGxDef*d=&urt_gx_defs[i];uint32_t desc=0;
    uint32_t*s=urt_cvar_lookup_cat(&g_ix,d->cvar,d->category,d->flags,d->def,&desc);
    if(s&&(uint8_t*)s==B+RV(d->rva))bound++;else printf("  unresolved %s [%s]\n",d->cvar,d->category);
    if(desc&&!(B[desc+9]&1u))nosave++;else printf("  save flag set %s\n",d->cvar);}
  CHECK(bound==9,"all 9 V5.3 variables resolve to their pinned value RVAs (name + real category + flags + default)");
  CHECK(nosave==9,"the game's descriptors: none of the 9 variables is saved to dx12user.settings");
  uint8_t*copy=malloc(img.size);memcpy(copy,B,img.size);
  CHECK(urt_gfx501_init(B,img.size),"adapter init");
  CHECK(strstr(g_log,"selected graphics controls verified")!=0,"all 18 engine rows of the table bound on the real image");
  CHECK(urt_gfx501_reload(),"all-Native preferences");
  urt_gfx501_boot(1,10);for(unsigned m=0;m<3;m++)urt_gfx501_tick(1,m,1,20+m);
  CHECK(!memcmp(copy,B,img.size),"Native: not a single byte of the image changed (startup step and every renderer)");
  for(unsigned i=first;i<first+9u;i++)setkv(urt_gx_defs[i].key,"1");
  CHECK(urt_gfx501_reload(),"every V5.3 row on option 1");
  urt_gfx501_boot(1,100);
  {unsigned changed=0;for(unsigned i=first;i<first+9u;i++)if(rd(B,i)!=rd(copy,i))changed++;
   CHECK(changed==2&&rd(B,URT_GX_IX_RTASDEFRAG)==10u&&rd(B,URT_GX_IX_RTASBATCH)==64u,"startup step: only the two RT acceleration-structure values written");
   CHECK(strstr(g_log,"RT memory settings applied")!=0,"startup step logged (capture point reported)");}
  uint32_t t=200;
  urt_gfx501_tick(1,0,1,t+=50);
  CHECK(rd(B,URT_GX_IX_RTGIALPHATEST)==rd(copy,URT_GX_IX_RTGIALPHATEST)&&rd(B,URT_GX_IX_PTNOBLUEGLOW)==rd(copy,URT_GX_IX_PTNOBLUEGLOW)&&rd(B,URT_GX_IX_GCIDLEFRAMES)==600u,
        "raster: memory/streaming rows applied, RT-only and PT-only rows untouched");
  urt_gfx501_tick(1,1,1,t+=50);
  CHECK(rd(B,URT_GX_IX_RTGIALPHATEST)==0x40800000u&&rd(B,URT_GX_IX_PTNOBLUEGLOW)==rd(copy,URT_GX_IX_PTNOBLUEGLOW),"Normal RT: GI alpha-test applied, blue-glow row untouched");
  urt_gfx501_tick(1,2,1,t+=50);
  CHECK(rd(B,URT_GX_IX_RTGIALPHATEST)==rd(copy,URT_GX_IX_RTGIALPHATEST)&&rd(B,URT_GX_IX_PTNOBLUEGLOW)==0x41200000u,"PT: blue-glow row applied (mip 10), GI alpha-test restored");
  CHECK(diff_outside(B,copy,img.size,first)==0,"only V5.3 value slots differ (no code byte, no neighbour, no descriptor flag)");
  urt_gfx501_tick(1,2,0,t+=50);
  CHECK(!memcmp(copy,B,img.size),"Master Off: byte-identical image");
  urt_gfx501_tick(1,2,1,t+=50);urt_gfx501_shutdown();
  CHECK(!memcmp(copy,B,img.size),"shutdown: byte-identical image");
  /* W3LoadFix [Override] names MeshOffset and AutoDiscardCount: both rows stay unbound and untouched */
  static const char ov[]="LoadingScreen/FadeInTime=1.0\0Streaming/Sector/MeshOffset=5\0Memory/GC/AutoDiscardCount=800\0\0";
  g_log[0]=0;urt_gfx501_set_external_overrides(ov,sizeof ov);
  CHECK(urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"adapter init with a W3LoadFix [Override] section");
  CHECK(strstr(g_log,"W3LoadFix [Override]")!=0,"deferral logged");
  for(unsigned m=0;m<3;m++){urt_gfx501_boot(1,1000);urt_gfx501_tick(1,m,1,1100+m);}
  CHECK(rd(B,URT_GX_IX_MESHSTREAMLEAD)==rd(copy,URT_GX_IX_MESHSTREAMLEAD)&&rd(B,URT_GX_IX_GCDISCARDTHRESHOLD)==rd(copy,URT_GX_IX_GCDISCARDTHRESHOLD)&&
        rd(B,URT_GX_IX_GENERICSTREAMLEAD)==0x41f00000u,"rows named by W3LoadFix stay W3LoadFix's; other rows still work");
  urt_gfx501_shutdown();urt_gfx501_set_external_overrides(0,0);
  CHECK(!memcmp(copy,B,img.size),"shutdown after deferral: byte-identical image");
  free(copy);
  printf("TOTAL %s %d\n",fails?"FAIL":"PASS",fails);return fails?1:0;
}
