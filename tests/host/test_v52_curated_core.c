/* V5.2 curated, selected controls: mode isolation, ownership, Master off and conflicts.
 * RC6: 16 engine rows + FG; saved rows (flag 0x100) are detached from the game settings save. */
#include "../../src/urt_gfx_core.h"
#include <stdio.h>
#include <string.h>
static int err=0;
#define CHECK(c,m) do{ if(!(c)){fprintf(stderr,"FAIL %s\n",m);err++;} }while(0)
static unsigned row(const char*k){for(unsigned i=0;i<URT_GX_COUNT;i++)if(!strcmp(urt_gx_defs[i].key,k))return i;return 9999;}
static uint8_t cells[URT_GX_COUNT][16];
static uint8_t flagb[URT_GX_COUNT];
int main(void){
 UrtGxOwner o;UrtGxPrefs p={{0}};unsigned specials=0;uint8_t idx;
 CHECK(URT_GX_COUNT==19&&URT_GX_IX_FGMENURESOURCES==9,"RC8: 9 engine rows + FG at indices 0..9 (caustics and tree distance removed); V5.3 dev rows appended after them");
 CHECK(row("URTX_TreeDistance")==9999,"RC8: no Tree and Bush Draw Distance row in the runtime table");
 for(unsigned i=0;i<URT_GX_COUNT;i++)CHECK(strcmp(urt_gx_defs[i].cvar,"FoliageDistanceScale"),"RC8: FoliageDistanceScale never owned");
 CHECK(row("URTX_PTCaustics")==9999&&row("URTX_PTCausticsArea")==9999,"RC7: no caustics row in the runtime table");
 for(unsigned i=0;i<URT_GX_COUNT;i++){
  CHECK(!strncmp(urt_gx_defs[i].key,"URTX_",5),"expected hidden key namespace");
  if(urt_gx_defs[i].special)specials++;
  for(unsigned j=i+1;j<URT_GX_COUNT;j++){
   CHECK(strcmp(urt_gx_defs[i].key,urt_gx_defs[j].key),"no duplicate key");
   if(!urt_gx_defs[i].special&&!urt_gx_defs[j].special)CHECK(strcmp(urt_gx_defs[i].cvar,urt_gx_defs[j].cvar),"no duplicate variable ownership");
  }
 }
 CHECK(specials==1,"only FG menu experimental special retained");
 CHECK(row("URTX_PTAOQuality")<URT_GX_COUNT,"PT AO quality present");
 CHECK(row("URTX_RTHBIL")<URT_GX_COUNT,"RT HBIL present");
 CHECK(row("URTX_TextureBudget")==9999,"texture budget excluded: native texture preset owned");
 CHECK(row("URTX_TextureSharpness")==9999,"mip bias excluded: native texture preset owned");
 CHECK(row("URTX_PTTextureDetail")==9999,"unused PT texture detail excluded");
 CHECK(row("URTX_FGMenuResources")<URT_GX_COUNT,"FG special present");
 CHECK(row("URTX_RTAnimatedBVH")==9999,"rejected animated BVH absent");
 CHECK(urt_gx_parse_index("",0,&idx)&&idx==0,"native parsing");
 CHECK(!urt_gx_parse_index("-1",0,&idx)&&!urt_gx_parse_index("99",0,&idx),"invalid values rejected");
 urt_gx_init(&o);
 for(unsigned i=0;i<URT_GX_COUNT;i++){
  memset(cells[i],0,16);memset(cells[i],0xC3,4);memset(cells[i]+8,0xB9,8);
  if(urt_gx_defs[i].special)continue;
  o.slot[i]=cells[i]+4;
  if(urt_gx_row_saved(i)){flagb[i]=0x03;o.savebyte[i]=&flagb[i];}
  if(urt_gx_defs[i].kind==URT_GX_BOOL)cells[i][4]=(uint8_t)urt_gx_defs[i].def;
  else memcpy(cells[i]+4,&urt_gx_defs[i].def,4);
 }
 uint8_t before[sizeof cells];memcpy(before,cells,sizeof before);
 for(unsigned mode=0;mode<3;mode++)urt_gx_update(&o,&p,1,1,mode,100+mode);
 CHECK(!memcmp(before,cells,sizeof cells),"native default never writes all renderer states");
 unsigned ao=row("URTX_PTAOQuality"),rt=row("URTX_RTHBIL"),stream=row("URTX_TextureStreams");
 p.v[ao]=2;p.v[rt]=1;p.v[stream]=2;
 urt_gx_update(&o,&p,1,1,0,200);
 { uint32_t yy; memcpy(&yy,cells[stream]+4,4);
   CHECK(yy==urt_gx_defs[stream].def&&flagb[stream]==0x02,"saved row: save bit cleared first, value NOT written in the same tick"); }
 urt_gx_update(&o,&p,1,1,0,200+URT_GX_DETACH_SETTLE_MS-1);
 { uint32_t yy; memcpy(&yy,cells[stream]+4,4); CHECK(yy==urt_gx_defs[stream].def,"saved row: still not written before the settle time"); }
 urt_gx_update(&o,&p,1,1,0,200+URT_GX_DETACH_SETTLE_MS);
 uint32_t x,y;
 memcpy(&x,cells[ao]+4,4);memcpy(&y,cells[stream]+4,4);
 CHECK(x==urt_gx_defs[ao].def&&y==urt_gx_defs[stream].vals[1],"raster: PT not applied, shared streaming applied");
 CHECK(cells[rt][4]==(uint8_t)urt_gx_defs[rt].def,"raster: normal RT-only setting untouched");
 urt_gx_update(&o,&p,1,1,2,300);
 memcpy(&x,cells[ao]+4,4);CHECK(x==urt_gx_defs[ao].vals[1],"PT: AO quality applied");
 urt_gx_update(&o,&p,1,1,1,400);
 CHECK(cells[rt][4]==(uint8_t)urt_gx_defs[rt].def,"RT: saved HBIL row not written in the detach tick");
 urt_gx_update(&o,&p,1,1,1,400+URT_GX_DETACH_SETTLE_MS);
 memcpy(&x,cells[ao]+4,4);
 CHECK(x==urt_gx_defs[ao].def&&cells[rt][4]==(uint8_t)urt_gx_defs[rt].vals[0],"RT: PT restored, RT HBIL applied");
 urt_gx_update(&o,&p,0,1,1,500);
 CHECK(!memcmp(before,cells,sizeof cells),"Master Off: original bytes restored");
 CHECK(flagb[stream]==0x03&&urt_gx_detached_count(&o)==0,"Master Off: save bit restored after the value");
 CHECK(urt_gx_owned_count(&o)==0,"Master Off: no owned fields");
 urt_gx_update(&o,&p,1,1,1,600);urt_gx_update(&o,&p,1,1,1,600+URT_GX_DETACH_SETTLE_MS);
 uint32_t external=23u;memcpy(cells[stream]+4,&external,4);
 urt_gx_update(&o,&p,1,1,1,1000);
 CHECK(flagb[stream]==0x03,"foreign write: save bit restored at once (the game's value may be saved)");
 CHECK(o.conflicts>=1,"foreign write detected");
 urt_gx_update(&o,&p,0,1,1,800);
 memcpy(&y,cells[stream]+4,4);CHECK(y==external,"foreign setting change preserved when Master Off");
 /* ---- RC6 save-bit contract, every saved row ---- */
 urt_gx_init(&o);memset(&p,0,sizeof p);
 for(unsigned i=0;i<URT_GX_COUNT;i++){
  memset(cells[i],0,16);
  if(urt_gx_defs[i].special)continue;
  o.slot[i]=cells[i]+4;
  if(urt_gx_defs[i].kind==URT_GX_BOOL)cells[i][4]=(uint8_t)urt_gx_defs[i].def;else memcpy(cells[i]+4,&urt_gx_defs[i].def,4);
  if(urt_gx_row_saved(i)){flagb[i]=0x01;o.savebyte[i]=&flagb[i];}
  p.v[i]=1;
 }
 unsigned nsaved=0;for(unsigned i=0;i<URT_GX_COUNT;i++)nsaved+=urt_gx_row_saved(i)?1u:0u;
 CHECK(nsaved==6,"6 saved rows: AO screen-space, HBIL, AO pre-trace, SSR assist, texture streams, terrain preload");
 CHECK(urt_gx_row_saved(row("URTX_TerrainPreload"))&&!urt_gx_row_saved(row("URTX_RTReflFog"))&&!urt_gx_row_saved(row("URTX_PTSkinBounceDesat")),"saved-flag classification of the RC6/RC7 rows");
 for(unsigned t=0;t<4;t++){
  urt_gx_update(&o,&p,1,1,1,5000+t*100);
  /* invariant checked after every tick: a saved variable never holds a non-native value while its save bit is set */
  for(unsigned i=0;i<URT_GX_COUNT;i++)if(urt_gx_row_saved(i)&&o.slot[i]){
   uint32_t v=0;if(urt_gx_defs[i].kind==URT_GX_BOOL)v=cells[i][4];else memcpy(&v,cells[i]+4,4);
   CHECK(!((flagb[i]&1u)&&v!=urt_gx_defs[i].def),"invariant: save bit set => native value");
  }
 }
 unsigned rh=row("URTX_RTHBIL");
 CHECK(cells[rh][4]==1u&&flagb[rh]==0x00,"HBIL On applied after settle with the save bit cleared");
 urt_gx_release_all(&o);
 for(unsigned i=0;i<URT_GX_COUNT;i++)if(urt_gx_row_saved(i))CHECK(flagb[i]==0x01,"release_all: every save bit restored");
 CHECK(urt_gx_detached_count(&o)==0,"release_all: nothing detached");
 /* a save byte that is not in the expected state: the row is never owned */
 urt_gx_init(&o);unsigned ts=row("URTX_TextureStreams");
 o.slot[ts]=cells[ts]+4;o.savebyte[ts]=&flagb[ts];flagb[ts]=0x00;
 uint32_t nat=12u;memcpy(cells[ts]+4,&nat,4);
 for(unsigned t=0;t<5;t++)urt_gx_update(&o,&p,1,1,1,9000+t*300);
 memcpy(&nat,cells[ts]+4,4);
 CHECK(nat==12u&&flagb[ts]==0x00&&o.sb_conflicts>=1,"save bit already clear (not ours): value never written, flags untouched");
 /* a saved row without a bound save byte is never owned */
 urt_gx_init(&o);o.slot[ts]=cells[ts]+4;
 for(unsigned t=0;t<5;t++)urt_gx_update(&o,&p,1,1,1,9000+t*300);
 memcpy(&nat,cells[ts]+4,4);CHECK(nat==12u,"no save byte bound: stays native");
 /* renderer leaves scope during the settle period: never written, bit restored */
 urt_gx_init(&o);o.slot[rh]=cells[rh]+4;o.savebyte[rh]=&flagb[rh];flagb[rh]=0x01;cells[rh][4]=0;
 urt_gx_update(&o,&p,1,1,1,100);CHECK(flagb[rh]==0x00,"detached");
 urt_gx_update(&o,&p,1,1,2,150);CHECK(flagb[rh]==0x01&&cells[rh][4]==0,"PT before settle: reattached, never written");
 /* RC7: the game (graphics menu) writes a saved row URT owns: URT releases, reattaches and waits for the next game save */
 {unsigned tr=row("URTX_TerrainPreload");uint32_t nat,game=0x42480000u /*50*/;float f;
  urt_gx_init(&o);memset(&p,0,sizeof p);p.v[tr]=4;  /* 200 m */
  o.slot[tr]=cells[tr]+4;o.savebyte[tr]=&flagb[tr];flagb[tr]=0x01;nat=urt_gx_defs[tr].def;memcpy(cells[tr]+4,&nat,4);
  urt_gx_update(&o,&p,1,1,1,1000);urt_gx_update(&o,&p,1,1,1,1000+URT_GX_DETACH_SETTLE_MS);
  memcpy(&f,cells[tr]+4,4);CHECK(f==200.0f&&flagb[tr]==0x00,"Terrain Preload 200 m applied session-only (save bit cleared)");
  memcpy(cells[tr]+4,&game,4);                       /* the game or another mod writes the variable */
  for(unsigned t=0;t<20;t++)urt_gx_update(&o,&p,1,1,1,2000+t*300);
  memcpy(&nat,cells[tr]+4,4);
  CHECK(nat==game&&flagb[tr]==0x01&&!o.owned[tr],"native change: released, save bit back, URT waits (player's value is what the next save writes)");
  urt_gx_settings_saved(&o);                         /* the game wrote dx12user.settings */
  urt_gx_update(&o,&p,1,1,1,9000);urt_gx_update(&o,&p,1,1,1,9000+URT_GX_DETACH_SETTLE_MS);
  memcpy(&f,cells[tr]+4,4);CHECK(f==200.0f&&o.original[tr]==game,"after the game saved: URT applies again, the new game value is the native value it restores");
  urt_gx_update(&o,&p,0,1,1,9900);memcpy(&nat,cells[tr]+4,4);
  CHECK(nat==game&&flagb[tr]==0x01,"Master Off: the game's current value restored, save bit back");}
 fprintf(stderr,"curated core test: %d error(s)\n",err);return err?1:0;
}
