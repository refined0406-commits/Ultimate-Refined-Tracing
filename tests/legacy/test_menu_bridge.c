#include "cr01_menu_bridge.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
int main(void){
 CR01MenuBridge a,b;uint32_t slot=0;char bytes[1000];size_t n;
 cr01_menu_bridge_init(&a);assert(a.controller.saved.master==0);
 assert(cr01_menu_bridge_event(&a,"MasterEnabled","0")==1);
 assert(a.dirty_generation==0); /* Duplicate notifications cannot write. */
 assert(cr01_menu_bridge_event(&a,"MasterEnabled","false"));
 assert(a.dirty_generation==0);
 assert(cr01_menu_bridge_event(&a,"MasterEnabled","true"));
 assert(a.controller.saved.master==1);
 assert(cr01_menu_bridge_event(&a,"MasterEnabled","0"));
 cr01_native_slot(&a.controller,F_AO,&slot);
 assert(cr01_menu_bridge_should_disable(&a,F_AO));
 assert(cr01_menu_bridge_event(&a,"AmbientOcclusion","1"));
 assert(slot==0 && a.controller.saved.v[F_AO]==1);
 assert(cr01_menu_bridge_event(&a,"MasterEnabled","1"));
 assert(cr01_menu_bridge_should_disable(&a,F_AO)==0);
 assert(cr01_menu_bridge_mode(&a,CR01_RENDER_PT));assert(slot==1);
 assert(cr01_menu_bridge_mode(&a,CR01_RENDER_NORMAL_RT));assert(slot==0);
 assert(cr01_menu_bridge_mode(&a,CR01_RENDER_PT));assert(slot==1);
 assert(cr01_menu_bridge_event(&a,"MasterEnabled","0"));
 assert(slot==0&&a.controller.saved.v[F_AO]==1);
 assert(!cr01_menu_bridge_event(&a,"GeometryBounces","7"));
 assert(!cr01_menu_bridge_event(&a,"SunImpact","3.001"));
 assert(!cr01_menu_bridge_event(&a,"NotOurSetting","1"));
 assert(!cr01_menu_bridge_event(&a,"IndirectLight","nan"));
 assert(cr01_menu_bridge_event(&a,"IndirectLight","1.00"));
 assert(!cr01_menu_bridge_event(&a,"IndirectLight","0.99"));
 assert(cr01_menu_bridge_event(&a,"MasterEnabled","1"));
 assert(cr01_menu_bridge_mode(&a,CR01_RENDER_PT));
 assert(cr01_menu_bridge_event(&a,"GeometryBounces","3")==2);
 assert((a.controller.pending_mask&(1u<<F_GEOM))!=0);
 assert(cr01_menu_bridge_event(&a,"MasterEnabled","0"));
 {
  const char*keys[]={"PTShadows","PTBillboards","GeometryBounces","HairBounces",
    "IndirectLight","SunImpact","SkyImpact","AmbientOcclusion","AOStrength",
    "AORadius","AOContrast","GrassDay","GrassNight","SpecularScreenSpace",
    "SpecularCheckerboard"};
  const char*values[]={"0","1","3","4","1.00","1.25","0.90","1","2",
    "2","3","4","5","1","2"};
  for(unsigned i=0;i<15;i++) assert(cr01_menu_bridge_event(&a,keys[i],values[i]));
 }
 assert(cr01_menu_bridge_save(&a,bytes,sizeof(bytes),&n));
 cr01_menu_bridge_init(&b);
 assert(cr01_menu_bridge_load(&b,bytes,n));
 assert(b.controller.saved.master==0&&b.controller.saved.v[F_AO]==1);
 assert(b.controller.saved.v[F_INDIRECT]==100);
 assert(!cr01_menu_bridge_load(&b,bytes,n-1));

 {
  CR01MenuBridge snap; uint32_t aoSlot=0;
  const char*keys16[]={"MasterEnabled","PTShadows","PTBillboards","GeometryBounces","HairBounces",
    "IndirectLight","SunImpact","SkyImpact","AmbientOcclusion","AOStrength",
    "AORadius","AOContrast","GrassDay","GrassNight","SpecularScreenSpace",
    "SpecularCheckerboard"};
  const char*vals16[]={"1","0","1","3","4","1.20","1.25","0.90","1","2",
    "2","3","4","5","1","2"};
  const char*bad16[]={"1","0","1","7","4","1.20","1.25","0.90","1","2",
    "2","3","4","5","1","2"};
  cr01_menu_bridge_init(&snap);
  cr01_native_slot(&snap.controller,F_AO,&aoSlot);
  assert(cr01_menu_bridge_mode(&snap,CR01_RENDER_PT));
  assert(cr01_menu_bridge_snapshot(&snap,keys16,vals16,16)==2);
  assert(snap.dirty_generation==1);
  assert(snap.controller.saved.master==1&&snap.controller.saved.v[F_INDIRECT]==120);
  assert(aoSlot==1);
  assert(cr01_menu_bridge_snapshot(&snap,keys16,vals16,16)==1);
  assert(snap.dirty_generation==1);
  assert(!cr01_menu_bridge_snapshot(&snap,keys16,bad16,16));
  assert(snap.dirty_generation==1&&snap.controller.saved.v[F_GEOM]==3);
  assert(cr01_native_shutdown(&snap.controller));
 }

 {
  CR01MenuBridge partial; uint32_t aoSlot=0;
  const char*keys16[]={"MasterEnabled","PTShadows","PTBillboards","GeometryBounces","HairBounces",
    "IndirectLight","SunImpact","SkyImpact","AmbientOcclusion","AOStrength",
    "AORadius","AOContrast","GrassDay","GrassNight","SpecularScreenSpace",
    "SpecularCheckerboard"};
  const char*vals16[]={"1","0","0","0","0","1.00","1.00","1.00","2","0","0","0","0","0","0","0"};
  unsigned char present16[16]={1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0};
  cr01_menu_bridge_init(&partial);
  cr01_native_slot(&partial.controller,F_AO,&aoSlot);
  assert(cr01_menu_bridge_mode(&partial,CR01_RENDER_PT));
  assert(cr01_menu_bridge_event(&partial,"MasterEnabled","1"));
  assert(cr01_menu_bridge_event(&partial,"GeometryBounces","5")==2);
  assert(partial.controller.saved.v[F_GEOM]==5);
  assert(cr01_menu_bridge_snapshot_partial(&partial,keys16,vals16,present16,16));
  assert(partial.controller.saved.master==1);
  assert(partial.controller.saved.v[F_AO]==2);
  assert(partial.controller.saved.v[F_GEOM]==5); /* missing key preserves saved preference */
  assert(aoSlot==0);
  vals16[8]="99";
  assert(!cr01_menu_bridge_snapshot_partial(&partial,keys16,vals16,present16,16));
  assert(partial.controller.saved.v[F_AO]==2);
  assert(cr01_native_shutdown(&partial.controller));
 }
 assert(cr01_native_shutdown(&a.controller));
 puts("PASS private menu bridge, 16-key control validation, live master and reload");
 return 0;
}
