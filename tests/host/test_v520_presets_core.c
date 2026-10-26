/* V5.2.0 release: the owner's eight Quick Preset assignments driven through the real ownership core.
 * Preferences carry the STORED option ids that RT_CR01_ApplyQuickPreset writes (0 = Native / Auto), and the
 * expected engine values are the option values of the generated table (spec.py) - never derived from labels.
 * Proves: Performance / Balanced / Extreme apply exactly the table (final: six memory/AS rows Native in all three); Extreme releases the seven
 * Native / Auto rows to the game's own values (no Performance/Balanced override left) and applies GI alpha 4;
 * renderer isolation of GI alpha (Normal RT only); Master Off restores every byte, Master On reapplies; the two
 * RT acceleration-structure rows are owned from boot (before ray tracing initialises, which is when the engine
 * reads them - a change therefore takes effect at the next game start, as the tooltip states). */
#include "../../src/urt_gfx_core.h"
#include <stdio.h>
#include <string.h>
static int err=0;
#define CHECK(c,m) do{ if(!(c)){fprintf(stderr,"FAIL %s\n",m);err++;} else printf("PASS %s\n",m); }while(0)
static unsigned row(const char*k){for(unsigned i=0;i<URT_GX_COUNT;i++)if(!strcmp(urt_gx_defs[i].key,k))return i;return 9999;}
static uint8_t cells[URT_GX_COUNT][16];
static uint32_t val(unsigned i){uint32_t v;memcpy(&v,cells[i]+4,4);return v;}
static void setval(unsigned i,uint32_t v){memcpy(cells[i]+4,&v,4);}
/* owner's table, as stored option ids (Performance, Balanced, Extreme) + expected raw engine values */
static const struct {const char*key;unsigned id[3];uint32_t raw[3];} P[]={
 {"URTX_RTGIAlphaTest",     {4,2,1},{0x41000000u,0x40c00000u,0x40800000u}},   /* 8 / 6 / 4 (mip, float)   */
 {"URTX_TexUnloadSpeed",    {3,2,0},{0x40800000u,0x40000000u,0}},             /* 4x / 2x / Native         */
 {"URTX_MeshStreamLead",    {0,0,0},{0,0,0}},             /* Native in all three      */
 {"URTX_GenericStreamLead", {0,0,0},{0,0,0}},             /* Native in all three      */
 {"URTX_GCDiscardThreshold",{0,0,0},{0,0,0}},                           /* Native in all three      */
 {"URTX_GCIdleFrames",      {0,0,0},{0,0,0}},                          /* Native in all three      */
 {"URTX_RTASDefrag",        {0,0,0},{0,0,0}},                             /* Native in all three      */
 {"URTX_RTASBatch",         {0,0,0},{0,0,0}},                             /* Native in all three      */
};
#define NP (sizeof P/sizeof P[0])
int main(void){
 UrtGxOwner o;UrtGxPrefs p;memset(&p,0,sizeof p);urt_gx_init(&o);
 for(unsigned i=0;i<URT_GX_COUNT;i++){memset(cells[i],0xA5,16);if(urt_gx_defs[i].special)continue;o.slot[i]=cells[i]+4;
  if(urt_gx_defs[i].kind==URT_GX_BOOL)cells[i][4]=(uint8_t)urt_gx_defs[i].def;else setval(i,urt_gx_defs[i].def);}
 uint8_t before[sizeof cells];memcpy(before,cells,sizeof before);
 unsigned glow=row("URTX_PTNoBlueGlow"),gi=row("URTX_RTGIAlphaTest");
 /* table option values == expected raw values (the stored id selects vals[id-1]) */
 int tab=1;for(unsigned k=0;k<NP;k++){unsigned r=row(P[k].key);for(unsigned s=0;s<3;s++)if(P[k].id[s]){
   if(P[k].id[s]>urt_gx_defs[r].nvals||urt_gx_defs[r].vals[P[k].id[s]-1]!=P[k].raw[s])tab=0;}}
 CHECK(tab,"stored option ids select exactly the owner's values in the generated table");
 const char*names[3]={"Performance","Balanced","Extreme"};
 unsigned t=1000;
 for(int pass=0;pass<2;pass++)
 for(unsigned s=0;s<3;s++){
  for(unsigned k=0;k<NP;k++)p.v[row(P[k].key)]=P[k].id[s];
  p.v[glow]=0;                                   /* never set by a preset */
  urt_gx_update(&o,&p,1,1,1,t+=100);             /* Normal RT */
  int ok=1;
  for(unsigned k=0;k<NP;k++){unsigned r=row(P[k].key);uint32_t want=P[k].id[s]?P[k].raw[s]:urt_gx_defs[r].def;if(val(r)!=want)ok=0;}
  ok&=val(glow)==urt_gx_defs[glow].def;
  char m[160];snprintf(m,sizeof m,"%s (pass %d, Normal RT): all eight rows = table; Native rows hold the game's own value; blue glow untouched",names[s],pass+1);
  CHECK(ok,m);
 }
 /* Extreme reached from Performance directly: no Performance override left anywhere */
 for(unsigned k=0;k<NP;k++)p.v[row(P[k].key)]=P[k].id[0];
 urt_gx_update(&o,&p,1,1,1,t+=100);
 for(unsigned k=0;k<NP;k++)p.v[row(P[k].key)]=P[k].id[2];
 urt_gx_update(&o,&p,1,1,1,t+=100);
 int ext=1;for(unsigned k=0;k<NP;k++){unsigned r=row(P[k].key);if(!strcmp(P[k].key,"URTX_RTGIAlphaTest")){if(val(r)!=0x40800000u)ext=0;}
   else if(memcmp(cells[r],before+r*16,16))ext=0;}
 CHECK(ext,"Performance -> Extreme: seven Native rows byte-identical to the game's values, GI alpha-test = 4");
 /* renderer isolation: GI alpha is Normal RT only */
 urt_gx_update(&o,&p,1,1,2,t+=100);
 CHECK(val(gi)==urt_gx_defs[gi].def,"Path Tracing: GI alpha-test released (Normal RT only)");
 urt_gx_update(&o,&p,1,1,0,t+=100);
 CHECK(val(gi)==urt_gx_defs[gi].def,"raster: GI alpha-test not applied");
 /* Master Off / On with Balanced */
 for(unsigned k=0;k<NP;k++)p.v[row(P[k].key)]=P[k].id[1];
 urt_gx_update(&o,&p,1,1,1,t+=100);
 urt_gx_update(&o,&p,0,1,1,t+=100);
 CHECK(!memcmp(before,cells,sizeof cells)&&urt_gx_owned_count(&o)==0,"Master Off: every engine value restored, nothing owned, preferences kept");
 urt_gx_update(&o,&p,1,1,1,t+=100);
 int on=1;for(unsigned k=0;k<NP;k++){unsigned r=row(P[k].key);if(val(r)!=(P[k].id[1]?P[k].raw[1]:urt_gx_defs[r].def))on=0;}
 CHECK(on,"Master On: Balanced reapplied from the kept preferences");
 /* RT AS rows: owned from boot, before a renderer (ray tracing) exists */
 UrtGxOwner o2;urt_gx_init(&o2);memcpy(cells,before,sizeof cells);
 for(unsigned i=0;i<URT_GX_COUNT;i++)if(!urt_gx_defs[i].special)o2.slot[i]=cells[i]+4;
 for(unsigned k=0;k<NP;k++)p.v[row(P[k].key)]=P[k].id[0];
 urt_gx_update(&o2,&p,1,0,0,10);
 CHECK(!memcmp(before,cells,sizeof cells),"boot with Performance (no renderer yet): nothing written - the RT AS rows are Native in every preset");
 /* a MANUAL RT AS choice is still applied at boot, before ray tracing initialises */
 p.v[row("URTX_RTASBatch")]=1;p.v[row("URTX_RTASDefrag")]=1;urt_gx_update(&o2,&p,1,0,0,20);
 CHECK(val(row("URTX_RTASDefrag"))==10u&&val(row("URTX_RTASBatch"))==64u&&val(row("URTX_TexUnloadSpeed"))==urt_gx_defs[row("URTX_TexUnloadSpeed")].def,
       "manual RT AS values (10%, 64) applied at boot only to the RT AS rows, in time for ray-tracing initialisation");
 return err?1:0;
}
