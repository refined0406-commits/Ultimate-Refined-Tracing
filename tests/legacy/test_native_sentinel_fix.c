#include "cr01_native_execution.h"
#include "cr01_controls.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
static uint32_t fb(float v){union{float f;uint32_t b;}u;u.f=v;return u.b;}
int main(void){
 const unsigned names[]={F_AO_STRENGTH,F_AO_RADIUS,F_AO_CONTRAST,F_GRASS_DAY,F_GRASS_NIGHT};
 const uint32_t indices[]={5,6,6,5,6};
 CR01NativeController c;CR01Settings s;
 uint32_t backing[5],desired[5];
 cr01_native_init(&c);s=c.saved;s.master=1;
 for(unsigned j=0;j<5;j++){
  backing[j]=0xBF800000u;
  s.v[names[j]]=indices[j];
  assert(cr01_control_native_sane(names[j],backing[j]));
  assert(cr01_control_value(names[j],indices[j],&desired[j]));
  cr01_native_slot(&c,names[j],&backing[j]);
 }
 assert(!cr01_control_native_sane(F_SUN,0xBF800000u));
 assert(!cr01_control_native_sane(F_SKY,0xBF800000u));
 assert(!cr01_control_native_sane(F_AO_STRENGTH,fb(-.5f)));
 assert(!cr01_control_native_sane(F_GRASS_NIGHT,0x7FC00000u));
 assert(cr01_native_set_saved(&c,&s));
 assert(cr01_native_event(&c,CR01_RENDER_PT));
 for(unsigned j=0;j<5;j++){
  assert(backing[j]==desired[j]);
  assert(c.owners[names[j]].owned);
  assert(c.owners[names[j]].native==0xBF800000u);
 }
 assert(cr01_native_event(&c,CR01_RENDER_NORMAL_RT));
 for(unsigned j=0;j<5;j++)assert(backing[j]==0xBF800000u);
 assert(cr01_native_event(&c,CR01_RENDER_PT));
 for(unsigned j=0;j<5;j++)assert(backing[j]==desired[j]);
 assert(cr01_native_event(&c,CR01_RENDER_RASTER));
 for(unsigned j=0;j<5;j++)assert(backing[j]==0xBF800000u);
 assert(cr01_native_event(&c,CR01_RENDER_PT));
 s.master=0;assert(cr01_native_set_saved(&c,&s));
 assert(cr01_native_event(&c,CR01_RENDER_PT));
 for(unsigned j=0;j<5;j++)assert(backing[j]==0xBF800000u);
 s.master=1;assert(cr01_native_set_saved(&c,&s));
 assert(cr01_native_event(&c,CR01_RENDER_PT));
 /* Foreign edit: never restore over another owner's value. */
 backing[1]=fb(8.0f);
 assert(cr01_native_event(&c,CR01_RENDER_NORMAL_RT));
 assert(backing[1]==fb(8.0f));
 for(unsigned j=0;j<5;j++)if(j!=1)assert(backing[j]==0xBF800000u);
 assert(c.owners[F_AO_RADIUS].conflicted);
 assert(cr01_native_shutdown(&c));
 puts("PASS exact -1 PT scalar native sentinel, owned writes, RT/raster and Master restoration, foreign-write protection");
 return 0;
}
