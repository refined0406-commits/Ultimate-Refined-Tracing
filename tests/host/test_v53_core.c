/* V5.3 private development: the nine new rows and W3LoadFix [Override] coexistence (pure logic).
 * Proves: exact table identity; no row saved by the game; no overlap with variables owned by other URT
 * modules; Native writes nothing; renderer isolation (GI = Normal RT only, blue glow = PT only, memory and
 * streaming = every renderer); startup rows are applied before a renderer exists and released by Master Off;
 * a foreign write (e.g. an INI or another mod) is never overwritten in a fight; W3LoadFix override parsing. */
#include "../../src/urt_gfx_core.h"
#include "../../src/urt_compat53.h"
#include <stdio.h>
#include <string.h>
static int err=0;
#define CHECK(c,m) do{ if(!(c)){fprintf(stderr,"FAIL %s\n",m);err++;} else printf("PASS %s\n",m); }while(0)
static unsigned row(const char*k){for(unsigned i=0;i<URT_GX_COUNT;i++)if(!strcmp(urt_gx_defs[i].key,k))return i;return 9999;}
static uint8_t cells[URT_GX_COUNT][16];
static uint32_t val(unsigned i){uint32_t v;memcpy(&v,cells[i]+4,4);return v;}
static void setval(unsigned i,uint32_t v){memcpy(cells[i]+4,&v,4);}
static const struct {const char*key,*cvar,*cat;unsigned flags,def,rva,scope,stage,n;} R[]={
 {"URTX_TexUnloadSpeed","TextureUnloadSpeed","Rendering",0x202,0x3f800000,0x514c6e8,URT_GX_ALL,0,3},
 {"URTX_MeshStreamLead","MeshOffset","Streaming/Sector",0x2,0x41200000,0x515e660,URT_GX_ALL,0,3},
 {"URTX_GenericStreamLead","GenericOffset","Streaming/Sector",0x2,0x42340000,0x515e628,URT_GX_ALL,0,3},
 {"URTX_GCDiscardThreshold","AutoDiscardCount","MetadataStore",0x1,400,0x5170158,URT_GX_ALL,0,3},
 {"URTX_GCIdleFrames","IdleFrameCount","Memory/GC",0x1,30,0x516f550,URT_GX_ALL,0,4},
 {"URTX_RTASDefrag","RTConfigDefragThresholdPerCent","Rendering/RT/AS/Config",0x201,30,0x5130388,URT_GX_ALL,1,3},
 {"URTX_RTASBatch","RTConfigMaxBlasesInBatch","Rendering/RayTracing",0x201,256,0x5130468,URT_GX_ALL,1,3},
 {"URTX_RTGIAlphaTest","GIAlphaTestMipLevel","Rendering/RT",0x202,0x40a00000,0x514dec0,URT_GX_RT,0,4},
 {"URTX_PTNoBlueGlow","ReflectionsRadianceMipLevel","Rendering/RT",0x202,0x40800000,0x514de18,URT_GX_PT,0,1},
};
/* engine variables owned by other URT modules (V5.2.0 sources) - a V5.3 row must never share one */
static const char*OTHER[]={"LODBudget","LODDistanceLimit","LocalShadowsSlots","ShadowsAlphaTestMipLevel","RtRadianceShadingLocalLights",
 "RTSTBillboards","RTSTBillboardsProximity","ProximityTLASSize","ProximityExtendedTLASSize","MaxProbesShadedPerFrame",
 "MaxProbesClassificatedPerFrame","NumShadingRaysPerProbe","NumClassificationRaysPerProbe","PTIndirectLightMultiplier",
 "PTSunImpact","PTSkyImpact","PTAOEnable","PTAOStrength","PTAORadius","PTAOFinalValuePower","PTGrassScale","PTGrassNightScale",
 "PTLateSpecularScreenSpaceTrace","PTLateSpecularCheckerboard","AOPretraceQualityLevel","ShadingRaysPerFrameBudget","RTGIPreset",
 "TextureMemoryBudget","TextureMipBias","FoliageDistanceScale","ReflectionsAlphaTestMipLevel","GIRadianceMipLevel"};
int main(void){
 const unsigned N=sizeof R/sizeof R[0];
 unsigned first=row("URTX_TexUnloadSpeed");
 CHECK(URT_GX_COUNT==19&&first==10,"9 V5.3 rows appended after the 10 V5.2 rows");
 unsigned ok=0;
 for(unsigned k=0;k<N;k++){unsigned i=row(R[k].key);if(i>=URT_GX_COUNT){printf("  missing %s\n",R[k].key);continue;}
  const UrtGxDef*d=&urt_gx_defs[i];
  if(!strcmp(d->cvar,R[k].cvar)&&!strcmp(d->category,R[k].cat)&&d->flags==R[k].flags&&d->def==R[k].def&&d->rva==R[k].rva&&
     d->scope==R[k].scope&&d->stage==R[k].stage&&d->nvals==R[k].n&&d->special==URT_GX_SPECIAL_NONE)ok++;
  else printf("  identity mismatch %s\n",R[k].key);}
 CHECK(ok==N,"exact engine identity of all 9 rows (name, real category, flags, registered default, pinned RVA, scope, stage)");
 {unsigned saved=0;for(unsigned i=first;i<URT_GX_COUNT;i++)saved+=urt_gx_row_saved(i)?1u:0u;
  CHECK(saved==0,"no V5.3 variable is saved by the game to dx12user.settings (no 0x100 flag)");}
 {unsigned clash=0;for(unsigned i=first;i<URT_GX_COUNT;i++)for(unsigned k=0;k<sizeof OTHER/sizeof OTHER[0];k++)if(!strcmp(urt_gx_defs[i].cvar,OTHER[k]))clash++;
  for(unsigned i=0;i<URT_GX_COUNT;i++)for(unsigned j=i+1;j<URT_GX_COUNT;j++)if(urt_gx_defs[i].cvar[0]&&!strcmp(urt_gx_defs[i].cvar,urt_gx_defs[j].cvar))clash++;
  CHECK(clash==0,"no duplicated variable ownership (other URT modules, retired rows, foliage-shadow detail, reflection alpha test)");}
 CHECK(urt_gx_is_startup(row("URTX_RTASDefrag"))&&urt_gx_is_startup(row("URTX_RTASBatch"))&&!urt_gx_is_startup(row("URTX_TerrainPreload"))&&!urt_gx_is_startup(row("URTX_FGMenuResources")),
  "only the two RT acceleration-structure rows are startup rows");
 /* ---- ownership ---- */
 UrtGxOwner o;UrtGxPrefs p;memset(&p,0,sizeof p);urt_gx_init(&o);
 for(unsigned i=0;i<URT_GX_COUNT;i++){memset(cells[i],0xA5,16);if(urt_gx_defs[i].special)continue;o.slot[i]=cells[i]+4;
  if(urt_gx_defs[i].kind==URT_GX_BOOL)cells[i][4]=(uint8_t)urt_gx_defs[i].def;else setval(i,urt_gx_defs[i].def);}
 uint8_t before[sizeof cells];memcpy(before,cells,sizeof before);
 urt_gx_update(&o,&p,1,0,0,10);for(unsigned m=0;m<3;m++)urt_gx_update(&o,&p,1,1,m,20+m);
 CHECK(!memcmp(before,cells,sizeof cells),"all Native: nothing written before or after the renderer starts");
 unsigned d=row("URTX_RTASDefrag"),b=row("URTX_RTASBatch"),gi=row("URTX_RTGIAlphaTest"),glow=row("URTX_PTNoBlueGlow"),
          tu=row("URTX_TexUnloadSpeed"),gc=row("URTX_GCIdleFrames");
 for(unsigned i=first;i<URT_GX_COUNT;i++)p.v[i]=1;
 urt_gx_update(&o,&p,1,0,0,100);   /* boot: renderer not known yet */
 CHECK(val(d)==10u&&val(b)==64u,"startup rows applied before any renderer exists (defrag 10 %, batch 64)");
 CHECK(val(tu)==urt_gx_defs[tu].def&&val(gi)==urt_gx_defs[gi].def&&val(glow)==urt_gx_defs[glow].def,"live rows never applied without a renderer");
 urt_gx_update(&o,&p,1,1,0,200);
 CHECK(val(tu)==0x3f000000u&&val(gc)==600u&&val(gi)==urt_gx_defs[gi].def&&val(glow)==urt_gx_defs[glow].def,"raster: memory/streaming applied; GI (RT) and blue glow (PT) not");
 urt_gx_update(&o,&p,1,1,1,300);
 CHECK(val(gi)==0x40800000u&&val(glow)==urt_gx_defs[glow].def,"Normal RT: GI alpha-test applied; PT-only blue-glow row not");
 urt_gx_update(&o,&p,1,1,2,400);
 CHECK(val(gi)==urt_gx_defs[gi].def&&val(glow)==0x41200000u,"PT: blue-glow row applied (mip 10); GI alpha-test released");
 CHECK(val(d)==10u&&val(b)==64u,"startup rows stay applied across renderer changes");
 urt_gx_update(&o,&p,1,0,0,500);
 CHECK(val(d)==10u&&val(tu)==urt_gx_defs[tu].def,"renderer lost: live rows released, startup rows kept");
 urt_gx_update(&o,&p,0,1,2,600);
 CHECK(!memcmp(before,cells,sizeof cells)&&urt_gx_owned_count(&o)==0,"Master Off: every byte restored, nothing owned");
 /* Native on one row releases only that row */
 urt_gx_update(&o,&p,1,1,2,700);p.v[d]=0;urt_gx_update(&o,&p,1,1,2,800);
 CHECK(val(d)==urt_gx_defs[d].def&&val(b)==64u,"Native/Auto on one startup row restores the game's value of that row only");
 /* foreign write (an INI loaded later, or another mod): released, retaken next tick, given up after 3 fights */
 p.v[d]=1;urt_gx_update(&o,&p,1,1,2,900);
 setval(b,256u);urt_gx_update(&o,&p,1,1,2,1000);
 CHECK(val(b)==256u&&o.conflicts>=1,"foreign write detected and not overwritten in the same tick");
 urt_gx_update(&o,&p,1,1,2,1050);CHECK(val(b)==64u,"non-saved row retaken on the next tick (e.g. INI loaded after URT)");
 setval(b,256u);urt_gx_update(&o,&p,1,1,2,1100);urt_gx_update(&o,&p,1,1,2,1150);
 setval(b,256u);urt_gx_update(&o,&p,1,1,2,1200);urt_gx_update(&o,&p,1,1,2,1250);urt_gx_update(&o,&p,1,1,2,1300);
 CHECK(val(b)==256u&&o.yields>=1,"a value rewritten 3 times within 30 s is left to the other writer (no fight)");
 urt_gx_release_all(&o);
 /* ---- W3LoadFix [Override] ---- */
 static const char sec[]="; comment MeshOffset=1\0LoadingScreen/FadeInTime=1.0\0  streaming/sector/meshoffset = 5\0Memory/GC/AutoDiscardCount=800\0#IdleFrameCount=5\0\0";
 unsigned L=sizeof sec;
 CHECK(urt_w3lf_override_mentions(sec,L,"Streaming/Sector","MeshOffset"),"override: exact Group/Name (case-insensitive, blanks) matches");
 CHECK(urt_w3lf_override_mentions(sec,L,"MetadataStore","AutoDiscardCount"),"override: another group spelling of the same name matches");
 CHECK(!urt_w3lf_override_mentions(sec,L,"Memory/GC","IdleFrameCount"),"override: commented line ignored");
 CHECK(!urt_w3lf_override_mentions(sec,L,"Streaming/Sector","GenericOffset"),"override: unrelated variable not matched");
 CHECK(!urt_w3lf_override_mentions(sec,L,"Rendering","TextureUnloadSpeedX")&&!urt_w3lf_override_mentions(0,0,"a","b"),"override: no partial-name or empty matches");
 {static const char s2[]="LoadingScreen/FadeInTime=1.0\0\0";unsigned hits=0;
  for(unsigned i=0;i<URT_GX_COUNT;i++)if(urt_gx_defs[i].cvar[0])hits+=(unsigned)urt_w3lf_override_mentions(s2,sizeof s2,urt_gx_defs[i].category,urt_gx_defs[i].cvar);
  CHECK(hits==0,"W3LoadFix's own loading-screen fades never collide with a URT row");}
 printf("v53 core: %d error(s)\n",err);
 return err?1:0;
}
