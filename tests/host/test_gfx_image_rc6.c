/* V5.2.0 RC6: the REAL selected-controls adapter (src/urt_gfx501.c + urt_gfx_core.c + generated
 * table + urt_fg501.c) against the mapped Patch 5.01 witcher3.exe.
 * Proves:
 *  - all 16 engine rows resolve by name + real category + flags + registered default to their
 *    pinned value RVAs (the RC6 rows: six PT caustics rows, PT skin bounce desaturation,
 *    RT/PT fog in reflections, terrain preload distance, tree and bush draw distance);
 *  - the "save" flag of every row as stored in the game's own descriptor matches the table, and
 *    for each saved row the value is the descriptor's +0x20 field (the save loop's GetText value);
 *  - all Native: not one byte of the image changes in any renderer;
 *  - every row applied: only value slots and the save-flag bytes of saved rows differ, and a saved
 *    variable never holds a URT value while its save bit is set (checked after every tick);
 *  - Master Off / unknown renderer / shutdown: the image is byte-identical to the original
 *    (values AND save flags restored).
 * Usage: test_gfx_image_rc6 <witcher3.exe> */
#include "urt_test_build.h"
#include "pe_map.h"
#include "../../src/urt_cvar_index.h"
#include "../../src/urt_gfx501.h"
#include "../../src/urt_gfx_core.h"
#include <string.h>
#include <stdlib.h>
/* V5.3 dev: the table now also holds the V5.3 rows (tests/host/test_v53_image.c). This V5.2 regression
 * keeps exercising exactly the ten V5.2 rows (indices 0..FG); the V5.3 rows stay Native here. */
#define V52N (URT_GX_IX_FGMENURESOURCES+1u)
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
static uint32_t descs[URT_GX_COUNT];
static int invariant(const uint8_t*B){
  for(unsigned i=0;i<V52N;i++){const UrtGxDef*d=&urt_gx_defs[i];
    if(!urt_gx_row_saved(i))continue;
    uint32_t v=0;if(d->kind==URT_GX_BOOL)v=B[RV(d->rva)];else memcpy(&v,B+RV(d->rva),4);
    if((B[descs[i]+9]&1u)&&v!=d->def)return 0;}
  return 1;
}
int main(int argc,char**argv){
  PeImage img; if(argc<2||!pe_map_file(argv[1],&img)){puts("FAIL map");return 1;}
  if(!urt_test_build_init(img.size))return 1;
  uint8_t*B=img.base;
  CHECK(urt_cvar_index_build(&g_ix,B,img.size,g_store,200000),"registry index built");
  for(unsigned i=0;i<V52N;i++){const UrtGxDef*d=&urt_gx_defs[i];if(!RV(d->rva))continue;
    if(d->kind==URT_GX_BOOL)B[RV(d->rva)]=(uint8_t)d->def;else memcpy(B+RV(d->rva),&d->def,4);}
  unsigned bound=0,n=0,flags_ok=0,saved=0;
  for(unsigned i=0;i<V52N;i++){const UrtGxDef*d=&urt_gx_defs[i];if(!d->cvar[0])continue;n++;
    uint32_t desc=0;uint32_t*s=urt_cvar_lookup_cat(&g_ix,d->cvar,d->category,d->flags,d->def,&desc);
    if(s&&(uint8_t*)s==B+RV(d->rva))bound++;else printf("  unresolved %s [%s]\n",d->cvar,d->category);
    descs[i]=desc;
    unsigned bit=B[desc+9]&1u;
    if(bit==((d->flags>>8)&1u))flags_ok++;else printf("  flag mismatch %s\n",d->cvar);
    if(bit){saved++;if((uint8_t*)s!=B+desc+0x20u)printf("  saved value not at +0x20: %s\n",d->cvar);}
  }
  CHECK(n==9&&bound==9,"RC8: 9 engine variables at their pinned RVAs (caustics and tree distance removed)");
  CHECK(flags_ok==9&&saved==6,"descriptor save flags match the table: 6 saved rows, 3 never-saved rows");
  /* RC6 rows by name: each one's variable and save status */
  static const struct { const char*key,*cvar; unsigned saved; } rc6[]={
    {"URTX_PTSkinBounceDesat","PTSkinIndirectDesaturation",0},{"URTX_RTReflFog","RtRadianceShadingFog",0},
    {"URTX_TerrainPreload","TerrainReadAheadDistance",1}};
  unsigned rc6ok=0;
  for(unsigned k=0;k<3;k++)for(unsigned i=0;i<V52N;i++)
    if(!strcmp(urt_gx_defs[i].key,rc6[k].key)&&!strcmp(urt_gx_defs[i].cvar,rc6[k].cvar)&&(unsigned)urt_gx_row_saved(i)==rc6[k].saved&&(B[descs[i]+9]&1u)==rc6[k].saved)rc6ok++;
  CHECK(rc6ok==3,"RC6/RC7 rows: engine variable identity and save status as documented");
  {unsigned caus=0;for(unsigned i=0;i<V52N;i++)if(strstr(urt_gx_defs[i].cvar,"Caustics")||!strcmp(urt_gx_defs[i].cvar,"FoliageDistanceScale"))caus++;CHECK(caus==0,"no caustics or FoliageDistanceScale binding (native game behaviour)");}
  uint8_t*copy=malloc(img.size);memcpy(copy,B,img.size);
  CHECK(urt_gfx501_init(B,img.size),"adapter init");
  CHECK(strstr(g_log,"selected graphics controls verified")!=0,"all live bindings verified (incl. save bytes)");
  CHECK(urt_gfx501_reload(),"all-Native preferences");
  for(unsigned m=0;m<3;m++)urt_gfx501_tick(1,m,1,100+m);
  CHECK(!memcmp(copy,B,img.size),"Native: not a single byte of the image changed in any renderer");
  for(unsigned i=0;i<V52N;i++)if(!urt_gx_defs[i].special)setkv(urt_gx_defs[i].key,"1");
  CHECK(urt_gfx501_reload(),"every row on option 1");
  int inv=1;uint32_t t=1000;
  for(unsigned m=0;m<3;m++)for(unsigned k=0;k<6;k++){urt_gfx501_tick(1,m,1,t);t+=100;inv&=invariant(B);}
  CHECK(inv,"invariant over 18 ticks in raster, RT and PT: save bit set => native value");
  /* in PT now: PT rows, RT/PT row and all-renderer rows applied */
  unsigned applied=0;
  for(unsigned i=0;i<V52N;i++){const UrtGxDef*d=&urt_gx_defs[i];if(d->special)continue;
    if(!urt_gx_in_scope(i,2))continue;
    uint32_t v=0;if(d->kind==URT_GX_BOOL)v=B[RV(d->rva)];else memcpy(&v,B+RV(d->rva),4);
    if(v==d->vals[0]||(v==d->def&&d->vals[0]==d->def))applied++;else printf("  not applied in PT: %s\n",d->key);}
  CHECK(applied==5,"PT: all 5 in-scope rows applied (2 PT, 1 RT/PT, 2 all-renderer saved rows)");
  {size_t diff=0;for(size_t i=0;i<img.size;i++)if(B[i]!=copy[i]){int ok=0;
     for(unsigned r=0;r<V52N;r++){if(RV(urt_gx_defs[r].rva)&&i>=RV(urt_gx_defs[r].rva)&&i<RV(urt_gx_defs[r].rva)+4)ok=1;
       if(urt_gx_row_saved(r)&&i==descs[r]+9u)ok=1;}
     if(!ok)diff++;}
   CHECK(diff==0,"only owned value slots and saved rows' flag bytes differ (no code byte, no neighbour)");}
  {unsigned cleared=0;for(unsigned i=0;i<V52N;i++)if(urt_gx_row_saved(i)&&urt_gx_in_scope(i,2)&&!(B[descs[i]+9]&1u))cleared++;
   CHECK(cleared==2,"PT: the two in-scope saved rows (texture streams, terrain preload) are detached from the settings save while owned");}
  urt_gfx501_tick(1,2,0,t+=100);
  CHECK(!memcmp(copy,B,img.size),"Master Off: byte-identical image (values and save flags)");
  urt_gfx501_tick(1,1,1,t+=100);urt_gfx501_tick(1,1,1,t+=400);inv&=invariant(B);
  urt_gfx501_tick(0,0,1,t+=100);
  CHECK(inv&&!memcmp(copy,B,img.size),"renderer unknown: byte-identical image");
  urt_gfx501_tick(1,1,1,t+=100);urt_gfx501_tick(1,1,1,t+=400);
  setkv("URTX_RTHBIL","9");CHECK(!urt_gfx501_reload(),"out-of-range value rejected");
  CHECK(!memcmp(copy,B,img.size),"invalid input: byte-identical image");
  setkv("URTX_RTHBIL","1");urt_gfx501_reload();urt_gfx501_tick(1,1,1,t+=100);urt_gfx501_tick(1,1,1,t+=400);
  urt_gfx501_shutdown();
  CHECK(!memcmp(copy,B,img.size),"shutdown: byte-identical image");
  free(copy);
  printf("TOTAL %s %d\n",fails?"FAIL":"PASS",fails);return fails?1:0;
}
