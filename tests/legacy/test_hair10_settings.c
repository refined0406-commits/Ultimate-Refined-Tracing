#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cr01_menu_bridge.h"
#include "cr01_settings_codec.h"
int main(void){
 CR01MenuBridge b;CR01Settings restored;char storage[4096];size_t n=0;
 cr01_menu_bridge_init(&b);
 assert(cr01_menu_bridge_valid_input(&b,"HairBounces","0"));
 for(unsigned i=1;i<=10;i++){
  char val[20];snprintf(val,sizeof(val),"%u",i);
  assert(cr01_menu_bridge_valid_input(&b,"HairBounces",val));
  assert(cr01_menu_bridge_event(&b,"HairBounces",val));
  assert(b.controller.saved.v[F_HAIR]==i);
  assert(cr01_settings_serialize(&b.controller.saved,storage,sizeof(storage),&n));
  assert(cr01_settings_parse(storage,n,&restored));
  assert(restored.v[F_HAIR]==i && restored.v[F_GEOM]==0);
 }
 assert(!cr01_menu_bridge_valid_input(&b,"HairBounces","11"));
 assert(!cr01_menu_bridge_event(&b,"HairBounces","11"));
 for(unsigned i=7;i<=10;i++){
  char val[20];snprintf(val,sizeof(val),"%u",i);
  assert(!cr01_menu_bridge_valid_input(&b,"GeometryBounces",val));
 }
 assert(cr01_menu_bridge_valid_input(&b,"GeometryBounces","6"));
 assert(b.controller.saved.v[F_HAIR]==10);
 assert(cr01_menu_bridge_saved_text(&b,"HairBounces",storage,sizeof(storage)));
 assert(strcmp(storage,"10")==0);
 puts("PASS menu validation 0..10, private settings codec roundtrip for 1..10, saved-value text 10, geometry 7..10 rejected");
 return 0;
}
