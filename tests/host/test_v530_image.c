/* V5.3.0 PRIVATE: the REAL adapter against the mapped Patch 5.01 witcher3.exe, for the eight new rows
 * (rev2: + async GI classification and async AS compaction (BOOT), animated-geometry trace mode (STARTUP)).
 * Proves: each row resolves by name + real category + flags + registered default to its pinned value RVA and is
 * not saved by the game; the two async-compute flags (stage BOOT) are written once, before any renderer, and are
 * then left alone all session (renderer changes, Native/Auto, Master Off and shutdown do not touch them - the
 * engine reads them while recording each frame); the hidden blend-distance row follows the tree-distance choice;
 * the SHaRC density row writes only in Path Tracing, the tree rows only with ray tracing (RT or PT); Master Off
 * restores every live row byte for byte. Usage: test_v530_image <witcher3.exe> */
#include "urt_test_build.h"
#include "pe_map.h"
#include "../../src/urt_cvar_index.h"
#include "../../src/urt_gfx501.h"
#include "../../src/urt_gfx_core.h"
#include "../../src/urt_startup_guard.h"
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
static size_t fast_diff_outside(const uint8_t*a,const uint8_t*b,size_t n){   /* R12 FIX3: 64 KB blocks, inspect only differing ones */
  size_t bad=0;unsigned savebits=0,nsaved=0;for(unsigned r=0;r<URT_GX_COUNT;r++)nsaved+=urt_gx_row_saved(r)?1u:0u;for(size_t c=0;c<n;c+=65536){size_t e=c+65536<n?c+65536:n;
    if(memcmp(a+c,b+c,e-c))for(size_t i=c;i<e;i++)if(a[i]!=b[i]){int ok=0;
      for(unsigned r=0;r<URT_GX_COUNT;r++)if(RV(urt_gx_defs[r].rva)&&i>=RV(urt_gx_defs[r].rva)&&i<RV(urt_gx_defs[r].rva)+4u)ok=1;
      if(!ok&&(b[i]&1u)&&a[i]==(uint8_t)(b[i]&~1u)&&++savebits<=nsaved)ok=1;   /* a saved row's descriptor save bit, cleared while URT owns it */
      if(!ok)bad++;}}
  return bad;
}
static const char*const dig_[]={"0","1","2","3","4","5","6"};
static uint32_t rd(const uint8_t*B,unsigned r){uint32_t v;memcpy(&v,B+RV(urt_gx_defs[r].rva),4);return v;}
/* rev4: in-memory startup guard file */
static char g_gf[1024];static int g_gfn=-1;
static int gf_read(char*b,unsigned cap){if(g_gfn<0)return 0;unsigned n=(unsigned)g_gfn<cap?(unsigned)g_gfn:cap;memcpy(b,g_gf,n);return (int)n;}
static int gf_write(const char*b,unsigned n){if(n>=sizeof g_gf)return -1;memcpy(g_gf,b,n);g_gf[n]=0;g_gfn=(int)n;return (int)n;}
int main(int argc,char**argv){
  PeImage img; if(argc<2||!pe_map_file(argv[1],&img)){puts("FAIL map");return 1;}
  if(!urt_test_build_init(img.size))return 1;
  uint8_t*B=img.base;
  const unsigned first=URT_GX_IX_PTRADIANCECACHEDENSITY,N=11,NV=8;   /* R12 FIX3: Second Bounce Coverage retired, Local Light Shadow Coverage added */   /* R11: every R6-R10 EXPERIMENTAL row retired; R11 + R12: two shader-only rows */
  const uint32_t AC=RV(0x4067290u);   /* R5: Rendering/Async/RT/UseAsyncCompact value slot (row removed; must never be written) */
  const unsigned sh=URT_GX_IX_PTRADIANCECACHEDENSITY,ao=URT_GX_IX_RTASYNCAO,gi=URT_GX_IX_RTASYNCGISHADING,
                 td=URT_GX_IX_RTTREEWINDDISTANCE,tb=URT_GX_IX_RTTREEWINDBLEND,
                 gc=URT_GX_IX_RTASYNCGICLASSIFY,ft=URT_GX_IX_RTASFASTTRACE,mo=URT_GX_IX_PTMOONLIGHT;
  CHECK(first+N==URT_GX_COUNT&&urt_gx_defs[tb].lead==td,"eleven V5.3.0 rows appended (R12 FIX3: - PT: Second Bounce Coverage, + PT: Bounce Local Light Shadow Coverage; R12 FIX2: + PT: Bounce Sun Shadow Coverage; R5: compaction removed; R11: the R6-R10 EXPERIMENTAL rows retired, + PT: Second Bounce Coverage; R12: + PT: First Bounce Diffuse Coverage); blend row follows tree distance");
  CHECK(urt_cvar_index_build(&g_ix,B,img.size,g_store,200000),"registry index built");
  for(unsigned i=0;i<URT_GX_COUNT;i++){const UrtGxDef*d=&urt_gx_defs[i];if(!RV(d->rva))continue;
    if(d->kind==URT_GX_BOOL)B[RV(d->rva)]=(uint8_t)d->def;else memcpy(B+RV(d->rva),&d->def,4);}
  unsigned bound=0,nosave=0;
  for(unsigned i=first;i<first+N;i++){const UrtGxDef*d=&urt_gx_defs[i];uint32_t desc=0;if(d->special!=URT_GX_SPECIAL_NONE)continue;
    uint32_t*s=urt_cvar_lookup_cat(&g_ix,d->cvar,d->category,d->flags,d->def,&desc);
    if(s&&(uint8_t*)s==B+RV(d->rva))bound++;else printf("  unresolved %s [%s]\n",d->cvar,d->category);
    if(desc&&!(B[desc+9]&1u))nosave++;else printf("  save flag set %s\n",d->cvar);}
  CHECK(bound==NV,"all 8 variables resolve to their pinned value RVAs (name + real category + flags + registered default)");
  CHECK(nosave==NV,"none of the 8 variables is saved by the game to dx12user.settings");
  uint8_t*copy=malloc(img.size);memcpy(copy,B,img.size);
  urt_gfx501_set_guard_io(gf_read,gf_write);urt_gfx501_set_bounce_decided(1);
  CHECK(urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"adapter init, all Native");
  urt_gfx501_boot(1,10);for(unsigned m=0;m<3;m++)urt_gfx501_tick(1,m,1,20+m);
  CHECK(!memcmp(copy,B,img.size),"Native: not a single byte of the image changed");
  urt_gfx501_shutdown();
  /* all five on: async flags On, SHaRC factor 8, tree distance 10 m (blend follows: 7.5 m) */
  setkv("URTX_RTAsyncAO","1");setkv("URTX_RTAsyncGIShading","1");setkv("URTX_RTAsyncGIClassify","1");setkv("URTX_RTASAsyncCompact","1");setkv("URTX_RTASFastTrace","1");setkv("URTX_PTMoonLight","4");setkv("URTX_PTRadianceCacheDensity","3");setkv("URTX_RTTreeWindDistance","2");
  g_log[0]=0;
  /* R5: a guard file left by R4 with the removed row recorded */
  gf_write("; old\r\nURTX_RTASAsyncCompact=1 failed\r\n",strlen("; old\r\nURTX_RTASAsyncCompact=1 failed\r\n"));
  {int found=0;for(unsigned i=0;i<URT_GX_COUNT;i++)if(!strcmp(urt_gx_defs[i].key,"URTX_RTASAsyncCompact")||!strcmp(urt_gx_defs[i].cvar,"UseAsyncCompact"))found=1;
   CHECK(!found,"R5: no async-compaction row (key or engine variable) in the runtime table");}
  CHECK(urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"adapter init, options set (a stale saved async-compaction On is present)");
  urt_gfx501_boot(1,100);
  CHECK(B[RV(urt_gx_defs[ao].rva)]==1&&B[RV(urt_gx_defs[gi].rva)]==1&&strstr(g_log,"async compute settings applied")!=0,
        "boot (no renderer yet): both async-compute flags written once and logged");
  CHECK(B[RV(urt_gx_defs[gc].rva)]==1,"boot: async GI classification written once");
  CHECK(B[AC]==copy[AC]&&B[AC]==0&&!strstr(g_log,"URTX_RTASAsyncCompact"),"R5: a saved async-compaction On is ignored: UseAsyncCompact never written, never logged as applied");
  CHECK(B[RV(urt_gx_defs[ft].rva)]==1&&strstr(g_log,"startup override applied: URTX_RTASFastTrace=1")!=0,
        "boot (AS config still at its pre-init value 128): Fast Trace written once before ray tracing initialises, logged by name");
  CHECK(strstr(g_gf,"URTX_RTASFastTrace=1 pending")&&strstr(g_gf,"URTX_RTAsyncGIClassify=1 pending")&&!strstr(g_gf,"URTX_RTASAsyncCompact")
        &&!strstr(g_gf,"URTX_RTAsyncAO"),"startup guard: the two rows under test recorded PENDING; the removed row's old record dropped; approved rows not guarded");
  CHECK(urt_gx_defs[gc].stage==URT_GX_STAGE_BOOT&&urt_gx_defs[ft].stage==URT_GX_STAGE_BOOT,
        "stages: classification and trace mode session-fixed (boot)");
  CHECK(rd(B,sh)==rd(copy,sh)&&rd(B,td)==rd(copy,td)&&rd(B,tb)==rd(copy,tb),"boot: live rows untouched before a renderer exists");
  uint32_t t=200;
  urt_gfx501_tick(1,0,1,t+=50);
  CHECK(rd(B,sh)==rd(copy,sh)&&rd(B,td)==rd(copy,td),"raster: SHaRC and tree rows stay native");
  urt_gfx501_tick(1,1,1,t+=50);
  CHECK(rd(B,td)==0x41200000u&&rd(B,tb)==0x40f00000u&&rd(B,sh)==rd(copy,sh),"Normal RT: tree distance 10 m with blend 7.5 m (follower); SHaRC untouched");
  CHECK(rd(B,mo)==rd(copy,mo)&&rd(copy,mo)==0x3f800000u,"raster and Normal RT: PT: Moon Light stays at the game value 1.0");
  urt_gfx501_tick(1,2,1,t+=50);
  CHECK(rd(B,sh)==8u&&rd(B,td)==0x41200000u,"Path Tracing: SHaRC update density factor 8; tree rows kept");
  CHECK(rd(B,mo)==0x3fc00000u,"Path Tracing: PT: Moon Light 1.50 written live to PTMoonImpact");
  /* Native on the tree control releases both variables */
  setkv("URTX_RTTreeWindDistance","0");CHECK(urt_gfx501_reload(),"tree control back to Native");
  urt_gfx501_tick(1,2,1,t+=50);
  CHECK(rd(B,td)==rd(copy,td)&&rd(B,tb)==rd(copy,tb),"Native on the tree control: both variables restored byte for byte");
  /* async flags are session-fixed: Native in the menu, renderer changes and Master Off do not touch them */
  setkv("URTX_RTAsyncAO","0");CHECK(urt_gfx501_reload(),"async AO set back to Native in the menu");
  urt_gfx501_boot(1,t+=50);for(unsigned m=0;m<3;m++)urt_gfx501_tick(1,m,1,t+=50);
  CHECK(B[RV(urt_gx_defs[ao].rva)]==1&&B[RV(urt_gx_defs[gi].rva)]==1,"menu change mid-session: async flags unchanged until the next start (no second boot step)");
  setkv("URTX_RTAsyncGIClassify","0");setkv("URTX_RTASAsyncCompact","0");CHECK(urt_gfx501_reload(),"classification and compaction set to Native mid-session");
  for(unsigned m=0;m<3;m++)urt_gfx501_tick(1,m,1,t+=50);
  CHECK(B[RV(urt_gx_defs[gc].rva)]==1&&B[AC]==0,"mid-session Native: classification not flipped live (next start); compaction never touched");
  setkv("URTX_RTASFastTrace","0");CHECK(urt_gfx501_reload(),"trace mode set to Native mid-session");
  for(unsigned m=0;m<3;m++)urt_gfx501_tick(1,m,1,t+=50);
  CHECK(B[RV(urt_gx_defs[ft].rva)]==1,"mid-session Native: Fast Trace is NOT switched back live (a BLAS refit with other flags is never caused)");
  setkv("URTX_RTASFastTrace","1");CHECK(urt_gfx501_reload(),"trace mode Fast Trace again");
  urt_gfx501_tick(1,2,0,t+=50);
  CHECK(B[RV(urt_gx_defs[ao].rva)]==1&&rd(B,sh)==rd(copy,sh)&&rd(B,td)==rd(copy,td),"Master Off: live rows restored; async flags stay for this session");
  CHECK(rd(B,mo)==rd(copy,mo),"Master Off: PT: Moon Light restored byte for byte");
  CHECK(B[RV(urt_gx_defs[ft].rva)]==1,"Master Off: trace mode stays for this session (session-fixed; native again at the next start)");
  {size_t bad=0;for(size_t i=0;i<img.size;i++)if(B[i]!=copy[i]&&i!=RV(urt_gx_defs[ao].rva)&&i!=RV(urt_gx_defs[gi].rva)&&i!=RV(urt_gx_defs[gc].rva)&&i!=RV(urt_gx_defs[ft].rva))bad++;
   CHECK(bad==0,"Master Off: the image differs only in the four session-fixed flag bytes");}
  urt_gfx501_shutdown();
  /* next session ("restart") with async AO Native and Master On: only GI shading written */
  B[RV(urt_gx_defs[ao].rva)]=0;B[RV(urt_gx_defs[gi].rva)]=0;B[RV(urt_gx_defs[gc].rva)]=0;B[AC]=0;B[RV(urt_gx_defs[ft].rva)]=0;
  CHECK(urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"next start");
  urt_gfx501_boot(1,10);
  CHECK(B[RV(urt_gx_defs[ao].rva)]==0&&B[RV(urt_gx_defs[gi].rva)]==1,"next start: the menu choice takes effect (AO native, GI shading on)");
  CHECK(B[RV(urt_gx_defs[gc].rva)]==0&&B[AC]==0,"next start: classification Native (not written); compaction never written");
  CHECK(B[RV(urt_gx_defs[ft].rva)]==0&&strstr(g_log,"startup guard: URTX_RTASFastTrace=1 NOT applied this start")!=0&&strstr(g_log,"suspected start-up failure")&&strstr(g_gf,"URTX_RTASFastTrace=1 retry\r")
        &&!strstr(g_gf,"URTX_RTAsyncGIClassify")&&!strstr(g_gf,"URTX_RTASAsyncCompact"),
        "FIX2: next start after a session with no clean exit and no proof: Fast Trace skipped once, logged as a suspected crash; Native rows' records cleared");
  CHECK(B[RV(urt_gx_defs[gi].rva)]==1,"guard is narrow: the approved async GI shading row is still applied");
  urt_gfx501_shutdown();
  /* R12 FIX2: the start after the skipped one retries it automatically (no user action needed) */
  B[RV(urt_gx_defs[gi].rva)]=0;g_log[0]=0;
  CHECK(urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"FIX2: following start");urt_gfx501_boot(1,10);
  CHECK(B[RV(urt_gx_defs[ft].rva)]==1&&strstr(g_gf,"URTX_RTASFastTrace=1 retrying")&&strstr(g_log,"applied once more after a suspected crash"),
        "FIX2: retried automatically once (RETRYING), logged");
  urt_gfx501_shutdown();B[RV(urt_gx_defs[ft].rva)]=0;
  /* R12 FIX2: the clean-exit mark (urt_bc_clean_exit at DLL_PROCESS_DETACH, Windows) makes the next start apply it at once */
  {UrtSgFile cf;urt_sg_parse(g_gf,(unsigned)strlen(g_gf),&cf);static const char*const ck[]={"URTX_RTASFastTrace"};
   CHECK(urt_sg_mark_clean(&cf,ck,1)==1,"FIX2: clean exit after a short session marks the record CLEAN");
   char ct[1024];unsigned cn=urt_sg_format(&cf,ct,sizeof ct);gf_write(ct,cn);}
  B[RV(urt_gx_defs[gi].rva)]=0;g_log[0]=0;
  CHECK(urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"FIX2: immediate restart after a clean exit");urt_gfx501_boot(1,10);
  CHECK(B[RV(urt_gx_defs[ft].rva)]==1&&strstr(g_log,"ended with a clean game exit")&&strstr(g_gf,"URTX_RTASFastTrace=1 pending"),
        "FIX2: a clean exit is never a crash - applied at once, however short the last session");
  urt_gfx501_shutdown();B[RV(urt_gx_defs[ft].rva)]=0;
  /* retry after one Native start, then prove it with 10 minutes of ray-traced rendering */
  setkv("URTX_RTASFastTrace","0");B[RV(urt_gx_defs[gi].rva)]=0;
  CHECK(urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"start with trace mode Native");urt_gfx501_boot(1,10);
  CHECK(!strstr(g_gf,"URTX_RTASFastTrace")&&B[RV(urt_gx_defs[ft].rva)]==0,"Native for one start clears the blocked record; nothing written");
  urt_gfx501_shutdown();setkv("URTX_RTASFastTrace","1");B[RV(urt_gx_defs[gi].rva)]=0;g_log[0]=0;
  CHECK(urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"start with Fast Trace chosen again");urt_gfx501_boot(1,10);
  CHECK(B[RV(urt_gx_defs[ft].rva)]==1,"chosen again: retried");
  {uint32_t tt=100;for(unsigned k=0;k<20;k++)urt_gfx501_tick(1,1,1,tt+=2000);}
  CHECK(strstr(g_gf,"URTX_RTASFastTrace=1 pending")!=0,"after under 1 minute of RT: still pending");
  {uint32_t tt=500000;for(unsigned k=0;k<120;k++)urt_gfx501_tick(1,2,1,tt+=2000);}
  CHECK(strstr(g_gf,"URTX_RTASFastTrace=1 proven")&&strstr(g_log,"recorded as stable"),"after 1 minute of RT/PT rendering: proven and logged");
  urt_gfx501_shutdown();B[RV(urt_gx_defs[ft].rva)]=0;B[RV(urt_gx_defs[gi].rva)]=0;
  CHECK(urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"start after a stable session");urt_gfx501_boot(1,10);
  CHECK(B[RV(urt_gx_defs[ft].rva)]==1,"a proven value is applied at every later start");
  urt_gfx501_shutdown();B[RV(urt_gx_defs[ft].rva)]=0;B[RV(urt_gx_defs[gi].rva)]=0;
  /* ray tracing already initialised (AS config no longer at its pre-init value): never applied late */
  {uint32_t v=256,keep;memcpy(&keep,B+RV(0x405af14u),4);memcpy(B+RV(0x405af14u),&v,4);g_log[0]=0;
   CHECK(urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"start where ray tracing is already initialised");urt_gfx501_boot(1,10);
   CHECK(B[RV(urt_gx_defs[ft].rva)]==0&&strstr(g_log,"Trace Mode not applied this session")!=0&&strstr(g_gf,"URTX_RTASFastTrace=1 proven"),
         "late start: Fast Trace not applied (would mix build flags), logged; its guard record untouched");
   urt_gfx501_shutdown();memcpy(B+RV(0x405af14u),&keep,4);B[RV(urt_gx_defs[gi].rva)]=0;}
  /* no guard-file I/O: rows under test fail safe */
  urt_gfx501_set_guard_io(0,0);setkv("URTX_RTASFastTrace","1");setkv("URTX_RTAsyncGIClassify","1");
  CHECK(urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"start without a writable guard file");urt_gfx501_boot(1,10);
  CHECK(B[RV(urt_gx_defs[ft].rva)]==0&&B[RV(urt_gx_defs[gc].rva)]==0&&B[RV(urt_gx_defs[gi].rva)]==1,"no guard file: rows under test stay native (fail safe); approved rows unaffected");
  urt_gfx501_shutdown();B[RV(urt_gx_defs[gi].rva)]=0;setkv("URTX_RTASFastTrace","0");setkv("URTX_RTAsyncGIClassify","0");
  /* a fresh start with trace mode chosen mid-session never writes it live */
  urt_gfx501_set_guard_io(gf_read,gf_write);
  CHECK(urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"start Native");urt_gfx501_boot(1,10);
  setkv("URTX_RTASFastTrace","1");CHECK(urt_gfx501_reload(),"Fast Trace chosen in the menu mid-session");
  {uint32_t tt=100;for(unsigned m=0;m<3;m++)urt_gfx501_tick(1,m,1,tt+=50);}
  CHECK(B[RV(urt_gx_defs[ft].rva)]==0,"mid-session choice: not written live (applies at the next start)");
  urt_gfx501_shutdown();setkv("URTX_RTASFastTrace","0");
  /* Master Off at start: nothing written */
  B[RV(urt_gx_defs[gi].rva)]=0;urt_gfx501_shutdown();
  CHECK(urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"start with Master Off");
  urt_gfx501_boot(0,10);urt_gfx501_tick(1,2,0,20);
  CHECK(!memcmp(copy,B,img.size),"Master Off at start: byte-identical image");
  /* R11: every R6-R10 EXPERIMENTAL row retired; stale saved values are ignored and their engine variables / code never touched */
  {memcpy(B,copy,img.size);g_gfn=-1;g_gf[0]=0;
   static const char*const gone[]={"URTX_ExpRTGrassDensity","URTX_ExpHBILSamples","URTX_ExpHBILDirections","URTX_ExpPTLightLimit",
     "URTX_ExpPTObjectCulling","URTX_ExpPTRadianceCache","URTX_ExpRTSolidFoliage","URTX_ExpRTGrassWind","URTX_ExpRTStaticTrees","URTX_ExpRTBillboardUpdates"};
   int absent=1;for(unsigned k=0;k<10;k++){for(unsigned i=0;i<URT_GX_COUNT;i++)if(!strcmp(urt_gx_defs[i].key,gone[k]))absent=0;setkv(gone[k],"2");}
   CHECK(absent,"R11: the ten retired EXPERIMENTAL rows (R8, R9, R11) are not in the runtime table");
   g_log[0]=0;CHECK(urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"R11: start with stale saved values for every retired row");urt_gfx501_boot(1,10);
   {uint32_t tt=100;for(unsigned m=0;m<3;m++)urt_gfx501_tick(1,m,1,tt+=50);urt_gfx501_tick(1,2,1,tt+=50);}
   const uint32_t V[]={RV(0x406c858u),RV(0x4067418u),RV(0x40673e0u),RV(0x514f628u),RV(0x5149b40u),RV(0x1c20018u),RV(0x1c20019u),RV(0x1cf2a6du),RV(0x1cf2a84u),RV(0x1cf2abbu),RV(0x1cf2aceu)};
   int same=1;for(unsigned k=0;k<sizeof V/sizeof V[0];k++)if(memcmp(B+V[k],copy+V[k],4))same=0;
   CHECK(same,"R11: grass density, HBIL, proximity culling, PTUseSHARC, light-cap and culling code: not a byte written in any renderer");
   CHECK(strstr(g_log,"diagnostic: the game's PT radiance cache (SHaRC) is off")!=0&&!strstr(g_log,"URTX_ExpPTRadianceCache"),"R11: the read-only SHaRC diagnostic stays; no SHaRC override");
   urt_gfx501_shutdown();for(unsigned k=0;k<10;k++)setkv(gone[k],"0");}
  /* R12 FIX3: PT: Second Bounce Coverage retired - not a runtime row; a saved value and its guard record are dropped,
     other settings untouched (the boot decision in urt_bounce_boot501.c, Windows, also ignores the saved value) */
  {memcpy(B,copy,img.size);const char*old="URTX_ExpPTSecondBounce=2 proven\r\nURTX_RTASFastTrace=1 proven\r\n";gf_write(old,strlen(old));
   setkv("URTX_ExpPTSecondBounce","2");setkv("URTX_RTASFastTrace","1");
   int absent=1;for(unsigned i=0;i<URT_GX_COUNT;i++)if(!strcmp(urt_gx_defs[i].key,"URTX_ExpPTSecondBounce"))absent=0;
   g_log[0]=0;CHECK(absent&&urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"FIX3: start with an old Second Bounce Coverage value saved");urt_gfx501_boot(1,10);
   CHECK(!strstr(g_gf,"URTX_ExpPTSecondBounce")&&!strstr(g_log,"URTX_ExpPTSecondBounce")&&strstr(g_gf,"URTX_RTASFastTrace=1 proven")&&B[RV(urt_gx_defs[ft].rva)]==1,
         "FIX3: retired row: record dropped, never decided; an unrelated guarded row keeps its record and applies");
   urt_gfx501_shutdown();B[RV(urt_gx_defs[ft].rva)]=0;B[RV(urt_gx_defs[gi].rva)]=0;setkv("URTX_ExpPTSecondBounce","0");setkv("URTX_RTASFastTrace","0");g_gfn=-1;g_gf[0]=0;}
  /* R12: PT: First Bounce Diffuse Coverage - the same kind of row (same variant library, its own guard record) */
  {const unsigned dc=URT_GX_IX_PTDIFFUSECOVERAGE;memcpy(B,copy,img.size);
   CHECK(urt_gx_defs[dc].special==URT_GX_SPECIAL_BOUNCE&&urt_gx_defs[dc].stage==URT_GX_STAGE_BOOT&&!urt_gx_defs[dc].rva&&!urt_gx_defs[dc].cvar[0]&&
         urt_gx_defs[dc].scope==URT_GX_PT&&urt_gx_defs[dc].nvals==1,"FIX2: first-bounce diffuse coverage is a boot-stage, PT, shader-only row with 1 level (50%)");
   const char*two="URTX_ExpPTSunShadowCoverage=1 pending\r\nURTX_PTDiffuseCoverage=1 pending\r\n";
   gf_write(two,strlen(two));setkv("URTX_ExpPTSunShadowCoverage","1");setkv("URTX_PTDiffuseCoverage","1");
   urt_gfx501_set_bounce_decided(1);g_log[0]=0;
   CHECK(urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"R12: start, both coverage rows decided at ASI load");urt_gfx501_boot(1,10);
   CHECK(strstr(g_gf,"URTX_PTDiffuseCoverage=1 pending")&&strstr(g_gf,"URTX_ExpPTSunShadowCoverage=1 pending")&&!strstr(g_log,"startup override applied: URTX_ExpPTDiffuseCoverage"),
         "R12 boot: both ASI-load records kept as they are; the rows write nothing");
   {uint32_t tt=100;for(unsigned k=0;k<330;k++)urt_gfx501_tick(1,2,1,tt+=2000);}
   CHECK(strstr(g_gf,"URTX_PTDiffuseCoverage=1 pending")&&strstr(g_gf,"URTX_ExpPTSunShadowCoverage=1 pending"),
         "FIX4: time alone never qualifies a coverage row (11 minutes of PT without frames rendered with the variant: still pending)");
   urt_gfx501_note_variant_frames(299);urt_gfx501_tick(1,2,1,800000);
   CHECK(strstr(g_gf,"URTX_PTDiffuseCoverage=1 pending")!=0,"FIX4: 299 variant frames: not yet qualified");
   g_log[0]=0;urt_gfx501_note_variant_frames(300);urt_gfx501_tick(1,2,1,800050);
   CHECK(strstr(g_gf,"URTX_PTDiffuseCoverage=1 proven")&&strstr(g_gf,"URTX_ExpPTSunShadowCoverage=1 proven")&&strstr(g_log,"rendered 300 Path Tracing frames"),
         "FIX4: 300 Path Tracing frames rendered with the variant (about 5 s) qualify both coverage rows, logged");
   urt_gfx501_note_variant_frames(0);
   urt_gfx501_shutdown();
   gf_write("URTX_PTDiffuseCoverage=1 retrying\r\n",strlen("URTX_PTDiffuseCoverage=1 retrying\r\n"));setkv("URTX_ExpPTSunShadowCoverage","0");setkv("URTX_PTDiffuseCoverage","1");
   urt_gfx501_set_bounce_decided(0);g_log[0]=0;
   CHECK(urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"R12: start without the ASI-load decision");urt_gfx501_boot(1,10);
   CHECK(strstr(g_gf,"URTX_PTDiffuseCoverage=1 failed")&&strstr(g_log,"turned OFF"),"FIX2: an undecided start after an interrupted retry turns it OFF (no crash loop)");
   urt_gfx501_shutdown();urt_gfx501_set_bounce_decided(1);setkv("URTX_PTDiffuseCoverage","0");g_gfn=-1;g_gf[0]=0;}
  /* R12 FIX2: the R12 key URTX_ExpPTDiffuseCoverage (1 = the retired 75 %) is not a runtime row; its saved value and record are dropped */
  {memcpy(B,copy,img.size);const char*old="URTX_ExpPTDiffuseCoverage=1 pending\r\n";gf_write(old,strlen(old));setkv("URTX_ExpPTDiffuseCoverage","1");
   int absent=1;for(unsigned i=0;i<URT_GX_COUNT;i++)if(!strcmp(urt_gx_defs[i].key,"URTX_ExpPTDiffuseCoverage"))absent=0;
   CHECK(absent&&urt_gfx501_init(B,img.size)&&urt_gfx501_reload(),"FIX2: start with the R12 75% value saved under the retired key");urt_gfx501_boot(1,10);
   CHECK(!strstr(g_gf,"URTX_ExpPTDiffuseCoverage")&&!strstr(g_log,"URTX_ExpPTDiffuseCoverage"),"FIX2: retired key: its guard record dropped and never decided (no shader level can come from it)");
   urt_gfx501_shutdown();setkv("URTX_ExpPTDiffuseCoverage","0");g_gfn=-1;g_gf[0]=0;}

  /* ===== R12 FIX3 lifecycle stress (OFFLINE, real image of this executable): 30 sessions ("restarts") x 600 random steps of
     menu changes, repeated settings callbacks, renderer changes (raster / Normal RT / PT), Master On/Off and clean exits.
     Invariants checked during the run (A every 100 steps on the full image; B, C, E every step; D, F every session):
       A  no byte outside URT's own engine-variable slots (and saved rows' descriptor flag bytes) ever differs from the game;
       B  Master Off or a row's renderer out of scope: every live row is byte-identical to the game's value;
       C  a repeated settings callback / tick with nothing changed writes nothing (no duplicated work, no accumulation);
       D  owned values never exceed the row count; after shutdown only session-fixed rows (boot / startup) may differ;
       E  the shader-only coverage rows never write an engine byte; no record of a retired key ever appears;
       F  with every session ending in a clean exit, a chosen restart row is applied at every start (no false blocking). */
  {extern unsigned urt_gfx501_writes(void);extern unsigned urt_gfx501_owned(void);
   uint32_t rng=0x5eed1234u;
#define RN(n) ((rng=rng*1103515245u+12345u),(unsigned)((rng>>8)%(unsigned)(n)))
   unsigned bad_a=0,bad_b=0,bad_c=0,bad_d=0,bad_e=0,bad_f=0,steps=0,events=0,mchanges=0,mtoggles=0,restarts=0;
   static const char*const dig[]={"0","1","2","3","4","5","6"};
   g_gfn=-1;g_gf[0]=0;urt_gfx501_set_guard_io(gf_read,gf_write);urt_gfx501_set_bounce_decided(1);
   for(unsigned r=0;r<URT_GX_COUNT;r++)setkv(urt_gx_defs[r].key,"0");
   setkv("URTX_RTASFastTrace","1");setkv("URTX_PTDiffuseCoverage","1");setkv("URTX_ExpPTSunShadowCoverage","1");setkv("URTX_ExpPTLocalShadowCoverage","1");
   uint32_t t=1000,slot0[URT_GX_COUNT];
   for(unsigned sess=0;sess<30;sess++){
    memcpy(B,copy,img.size);g_log[0]=0;restarts++;                      /* a new game process: the image is native again */
    if(!urt_gfx501_init(B,img.size)||!urt_gfx501_reload()){bad_d++;break;}
    urt_gfx501_boot(1,t);
    if(B[RV(urt_gx_defs[ft].rva)]!=1)bad_f++;                            /* F: Fast Trace chosen, last session ended cleanly */
    unsigned mode=0,master=1,mv=1;
    for(unsigned st=0;st<600;st++,steps++){
     const unsigned a=RN(12);t+=1+RN(2500);
     if(a<=3){unsigned r=RN(URT_GX_COUNT);
      if(urt_gx_defs[r].special==URT_GX_SPECIAL_NONE&&urt_gx_defs[r].lead==URT_GX_NO_LEAD&&r!=ft){setkv(urt_gx_defs[r].key,dig[RN(urt_gx_defs[r].nvals+1u)]);(void)urt_gfx501_reload();events++;}}
     else if(a<=6){unsigned m=RN(3);if(m!=mode)mchanges++;mode=m;}
     else if(a==7){master=!master;mtoggles++;}
     else if(a==8)mv=RN(8)!=0;                                           /* renderer briefly not detectable (loading) */
     urt_gfx501_tick(mv,mode,master,t);
     if(st%100u==99u&&fast_diff_outside(B,copy,img.size))bad_a++;
     for(unsigned r=0;r<URT_GX_COUNT;r++){
      if(!RV(urt_gx_defs[r].rva)){continue;}
      const int live=urt_gx_defs[r].stage==URT_GX_STAGE_LIVE&&urt_gx_defs[r].special==URT_GX_SPECIAL_NONE;
      if(live&&mv&&(!master||!urt_gx_in_scope(r,mode))&&rd(B,r)!=rd(copy,r))bad_b++;
     }
     if(a>=9){                                                           /* C: the same settings callback / tick again */
      for(unsigned r=0;r<URT_GX_COUNT;r++)slot0[r]=RV(urt_gx_defs[r].rva)?rd(B,r):0u;const unsigned w0=urt_gfx501_writes();
      (void)urt_gfx501_reload();urt_gfx501_tick(mv,mode,master,t+1);urt_gfx501_tick(mv,mode,master,t+2);t+=2;
      int same=urt_gfx501_writes()==w0;for(unsigned r=0;r<URT_GX_COUNT;r++)if(RV(urt_gx_defs[r].rva)&&rd(B,r)!=slot0[r])same=0;
      if(!same)bad_c++;
     }
     if(urt_gfx501_owned()>URT_GX_COUNT)bad_d++;
     if(strstr(g_gf,"URTX_ExpPTSecondBounce")||strstr(g_gf,"URTX_ExpPTDiffuseCoverage"))bad_e++;
    }
    for(unsigned r=0;r<URT_GX_COUNT;r++)if(urt_gx_defs[r].special==URT_GX_SPECIAL_BOUNCE&&RV(urt_gx_defs[r].rva))bad_e++;
    /* clean game exit: DLL_PROCESS_DETACH marks this session's applied keys (urt_bc_clean_exit, Windows) */
    {UrtSgFile cf;const char*const*ks;unsigned nk=urt_sg_session_keys(&ks);urt_sg_parse(g_gf,g_gfn>0?(unsigned)g_gfn:0u,&cf);
     (void)urt_sg_mark_clean(&cf,ks,nk);char ct[1024];unsigned cn=urt_sg_format(&cf,ct,sizeof ct);if(cn)gf_write(ct,cn);urt_sg_session_clear();}
    urt_gfx501_tick(1,mode,0,t+=10);urt_gfx501_shutdown();
    {size_t bad=0;for(size_t c=0;c<img.size;c+=65536){size_t e=c+65536<img.size?c+65536:img.size;if(!memcmp(B+c,copy+c,e-c))continue;
      for(size_t i=c;i<e;i++)if(B[i]!=copy[i]){int fixed=0;
       for(unsigned r=0;r<URT_GX_COUNT;r++)if(urt_gx_defs[r].stage!=URT_GX_STAGE_LIVE&&RV(urt_gx_defs[r].rva)&&i>=RV(urt_gx_defs[r].rva)&&i<RV(urt_gx_defs[r].rva)+4u)fixed=1;
       if(!fixed)bad++;}}
     if(bad)bad_d++;}
   }
#undef RN
   printf("  stress: %u restarts, %u steps, %u settings events, %u renderer changes, %u Master toggles\n",restarts,steps,events,mchanges,mtoggles);
   CHECK(!bad_a,"FIX3 stress A: no byte outside URT's own engine-variable slots (and saved rows' save bits) ever changed (every 100 steps, 180 full-image scans)");
   CHECK(!bad_b,"FIX3 stress B: Master Off / renderer out of scope: every live row byte-identical to the game's value (every step)");
   CHECK(!bad_c,"FIX3 stress C: a repeated settings callback or tick writes nothing (no duplicated work, nothing accumulates)");
   CHECK(!bad_d,"FIX3 stress D: owned values bounded; after every shutdown only session-fixed rows differ from the game");
   CHECK(!bad_e,"FIX3 stress E: coverage rows never write an engine byte; no record of a retired key ever appears");
   CHECK(!bad_f,"FIX3 stress F: clean exits + immediate restarts: the chosen restart row is applied at every one of the 30 starts");
   urt_gfx501_set_guard_io(0,0);}


  /* R12 FIX4 interruption decisions for a PT coverage row (deterministic): before vs after qualification */
  {g_gfn=-1;g_gf[0]=0;urt_gfx501_set_guard_io(gf_read,gf_write);urt_gfx501_set_bounce_decided(1);int ev=0,go;uint32_t t=900000;
#define START(want) do{UrtSgFile g_;urt_sg_parse(g_gf,g_gfn>0?(unsigned)g_gfn:0u,&g_);go=urt_sg_decide_ex(&g_,"URTX_PTDiffuseCoverage",(uint8_t)(want),&ev); \
     char o_[1024];unsigned on_=urt_sg_format(&g_,o_,sizeof o_);gf_write(o_,on_);memcpy(B,copy,img.size); \
     setkv("URTX_PTDiffuseCoverage",(want)?"1":"0");(void)urt_gfx501_init(B,img.size);(void)urt_gfx501_reload();urt_gfx501_boot(1,t);}while(0)
#define PLAY(frames) do{for(unsigned k_=0;k_<40u;k_++){t+=50;urt_gfx501_note_variant_frames(go?(frames)*(k_+1u)/40u:0u);urt_gfx501_tick(1,2,1,t);}}while(0)
#define STOP() do{urt_gfx501_shutdown();urt_gfx501_note_variant_frames(0);}while(0)
   START(1);int a1=go&&ev==URT_SG_EV_NEW;PLAY(120);STOP();                                 /* crash after 2 s, before qualifying */
   START(1);int a2=!go&&ev==URT_SG_EV_SUSPECTED;PLAY(0);STOP();                            /* runs native once (logged) ... */
   START(1);int a3=go&&ev==URT_SG_EV_RETRYING;PLAY(400);STOP();                            /* ... retried; qualifies; later crash */
   START(1);int a4=go&&ev==URT_SG_EV_PROVEN;STOP();                                        /* not treated as a shader failure */
   CHECK(a1&&a2&&a3&&a4,"FIX4: interrupted before qualifying -> native once (logged), retried at the next start; interrupted after 300 variant frames -> still applied (not a start-up failure)");
   g_gfn=-1;g_gf[0]=0;
   START(1);PLAY(100);STOP();START(1);STOP();START(1);int b1=go;PLAY(50);STOP();START(1);int b2=!go&&ev==URT_SG_EV_FAILED_NOW;STOP();
   START(1);int b3=!go&&ev==URT_SG_EV_BLOCKED;STOP();START(0);STOP();START(1);int b4=go&&ev==URT_SG_EV_NEW;STOP();
   CHECK(b1&&b2&&b3&&b4,"FIX4: a shader that fails at start-up twice is turned off (no crash loop), stays off and logged; one Native / Auto start re-enables it");
#undef START
#undef PLAY
#undef STOP
   setkv("URTX_PTDiffuseCoverage","0");urt_gfx501_set_guard_io(0,0);g_gfn=-1;g_gf[0]=0;}
  /* ===== R12 FIX4 rapid experimentation (OFFLINE, real image): 240 cycles of "change settings -> start -> load into
     gameplay for 0-10 s (or quit at the menu) -> clean exit -> start again at once", then 120 cycles with random
     interruptions. The ASI-load decision for the PT coverage rows is the same urt_sg_decide_ex call urt_bounce_boot501.c
     makes (Windows); the RT restart rows go through urt_gfx501_boot as in game. */
  {static const char*const CK[3]={"URTX_PTDiffuseCoverage","URTX_ExpPTSunShadowCoverage","URTX_ExpPTLocalShadowCoverage"};
   uint32_t rng=0xc0ffee11u;
#define RQ(n) ((rng=rng*1103515245u+12345u),(unsigned)((rng>>8)%(unsigned)(n)))
   unsigned want[3]={0,0,0},wft=0,wgc=0,miss=0,bad_state=0,starts=0,quits_menu=0,qual=0,t=5000;
   int cov_go[3];
   g_gfn=-1;g_gf[0]=0;urt_gfx501_set_guard_io(gf_read,gf_write);urt_gfx501_set_bounce_decided(1);
   for(unsigned r=0;r<URT_GX_COUNT;r++)setkv(urt_gx_defs[r].key,"0");
#define ASI_DECIDE() do{UrtSgFile g_;urt_sg_parse(g_gf,g_gfn>0?(unsigned)g_gfn:0u,&g_);urt_sg_session_clear(); \
     for(unsigned c_=0;c_<3;c_++){int ev_=0;cov_go[c_]=urt_sg_decide_ex(&g_,CK[c_],(uint8_t)want[c_],&ev_);if(cov_go[c_])urt_sg_session_note(CK[c_]);} \
     char o_[1024];unsigned on_=urt_sg_format(&g_,o_,sizeof o_);gf_write(o_,on_);}while(0)
#define CLEAN_EXIT() do{UrtSgFile cf_;const char*const*ks_;unsigned nk_=urt_sg_session_keys(&ks_);urt_sg_parse(g_gf,g_gfn>0?(unsigned)g_gfn:0u,&cf_); \
     (void)urt_sg_mark_clean(&cf_,ks_,nk_);char ct_[1024];unsigned cn_=urt_sg_format(&cf_,ct_,sizeof ct_);if(cn_)gf_write(ct_,cn_);}while(0)
   for(unsigned cyc=0;cyc<360;cyc++){
    const int interrupt_phase=cyc>=240;
    /* the user changes 1-4 settings in the menu (coverage rows, restart rows, anything else) */
    for(unsigned c=0,nc=1+RQ(4);c<nc;c++){const unsigned w=RQ(6);
     if(w<3){want[w]=RQ(2);setkv(CK[w],want[w]?"1":"0");}
     else if(w==3){wft=RQ(2);setkv("URTX_RTASFastTrace",wft?"1":"0");}
     else if(w==4){wgc=RQ(2);setkv("URTX_RTAsyncGIClassify",wgc?"1":"0");}
     else{unsigned r=RQ(URT_GX_COUNT);if(urt_gx_defs[r].special==URT_GX_SPECIAL_NONE&&urt_gx_defs[r].lead==URT_GX_NO_LEAD&&r!=ft&&r!=gc)setkv(urt_gx_defs[r].key,dig_[RQ(urt_gx_defs[r].nvals+1u)]);}}
    /* start: ASI load (coverage rows), then the runtime (restart rows) */
    memcpy(B,copy,img.size);g_log[0]=0;starts++;
    const char*prev_state_text=0;(void)prev_state_text;
    UrtSgFile before;urt_sg_parse(g_gf,g_gfn>0?(unsigned)g_gfn:0u,&before);
    ASI_DECIDE();
    if(!urt_gfx501_init(B,img.size)||!urt_gfx501_reload()){bad_state++;break;}
    urt_gfx501_boot(1,t);
    /* every chosen option must be applied at this start unless the guard has a recorded reason (suspected / failed) */
    for(unsigned c=0;c<3;c++){const int st=urt_sg_state(&before,CK[c]);
     const int excused=st==URT_SG_PENDING||st==URT_SG_RETRYING||st==URT_SG_FAILED;
     if(want[c]&&!cov_go[c]&&!(interrupt_phase&&excused))miss++;}
    {const int st=urt_sg_state(&before,"URTX_RTASFastTrace");const int ex=st==URT_SG_PENDING||st==URT_SG_RETRYING||st==URT_SG_FAILED;
     if(wft&&B[RV(urt_gx_defs[ft].rva)]!=1&&!(interrupt_phase&&ex))miss++;}
    {const int st=urt_sg_state(&before,"URTX_RTAsyncGIClassify");const int ex=st==URT_SG_PENDING||st==URT_SG_RETRYING||st==URT_SG_FAILED;
     if(wgc&&B[RV(urt_gx_defs[gc].rva)]!=1&&!(interrupt_phase&&ex))miss++;}
    if(!interrupt_phase&&(strstr(g_gf," retry")||strstr(g_gf," failed")))bad_state++;   /* clean exits only: never a suspicion */
    /* load into gameplay for 0-10 s of Path Tracing (or quit at the main menu), frames rendered with the variant */
    const unsigned secs=RQ(4)==0?0u:RQ(11);if(!secs)quits_menu++;
    const int any_cov=cov_go[0]||cov_go[1]||cov_go[2];unsigned frames=0;
    for(unsigned k=0;k<secs*20u;k++){t+=50;if(any_cov)frames+=3u;urt_gfx501_note_variant_frames(frames);urt_gfx501_tick(1,2,1,t);}
    if(frames>=300u)qual++;
    /* exit: clean (experimenting) or, in the second phase, sometimes interrupted */
    const int clean=!interrupt_phase||RQ(3)!=0;
    if(clean)CLEAN_EXIT();
    urt_gfx501_shutdown();urt_gfx501_note_variant_frames(0);t+=1000;
   }
#undef ASI_DECIDE
#undef CLEAN_EXIT
#undef RQ
   printf("  rapid experimentation: %u starts (%u quit at the menu), %u sessions reached 300 variant frames\n",starts,quits_menu,qual);
   CHECK(!miss,"FIX4 rapid experimentation: with clean exits every chosen option is applied at every start, however short the session (no artificial wait); after an interruption only the guard's recorded, logged reason holds one back");
   CHECK(!bad_state,"FIX4 rapid experimentation: 240 change-restart-exit cycles with clean exits never create a suspected or failed record");
   urt_gfx501_set_guard_io(0,0);}
  urt_gfx501_shutdown();free(copy);
  printf("TOTAL %s %d\n",fails?"FAIL":"PASS",fails);return fails?1:0;
}
