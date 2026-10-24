#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "cr01_validation.h"
#include "cr01_gate_core.h"
int main(void){
 CR01GateState g={0}; CR01GateFields f={0};
 unsigned char dst[0x400]; uint32_t hair,geom;
 f.geometry=2; f.hair=4; f.comp=100; f.master=1; f.pt=1; f.renderer_ready=1; f.shader_ready=1;
 for(unsigned i=0;i<=10;i++) assert(cr01_valid_selection(F_HAIR,i));
 assert(!cr01_valid_selection(F_HAIR,11));
 for(unsigned i=0;i<=6;i++) assert(cr01_valid_selection(F_GEOM,i));
 for(unsigned i=7;i<=10;i++) assert(!cr01_valid_selection(F_GEOM,i));
 assert(cr01_gate_publish(&g,f,1));
 memset(dst,0x5A,sizeof(dst));assert(!cr01_gate_post_serialize(&g,dst,1));
 f.hair=10;assert(cr01_gate_publish(&g,f,2));
 memset(dst,0x5A,sizeof(dst));assert(!cr01_gate_post_serialize(&g,dst,0));
 assert(dst[CR01_HAIR_OFFSET]==0x5A);
 f.master=0;assert(cr01_gate_publish(&g,f,3));
 assert(!cr01_gate_post_serialize(&g,dst,1));
 f.master=1;f.pt=0;assert(cr01_gate_publish(&g,f,4));
 assert(!cr01_gate_post_serialize(&g,dst,1));
 f.pt=1;
 for(unsigned i=1;i<=10;i++){
   f.hair=(uint8_t)i;
   assert(cr01_gate_publish(&g,f,5+i));
   memset(dst,0x5A,sizeof(dst));
   int changed=cr01_gate_post_serialize(&g,dst,1);
   if(i==4) assert(!changed); else assert(changed);
   memcpy(&hair,dst+CR01_HAIR_OFFSET,sizeof(hair));
   if(i!=4)assert(hair==i);
   if(i!=4){memcpy(&geom,dst+CR01_GEOM_OFFSET,sizeof(geom));assert((geom&0xffu)==2);}
 }
 f.hair=11;assert(!cr01_gate_publish(&g,f,20));
 f.hair=8;f.geometry=7;assert(!cr01_gate_publish(&g,f,21));
 f.geometry=6;assert(cr01_gate_publish(&g,f,22));
 memset(dst,0x5A,sizeof(dst));assert(cr01_gate_post_serialize(&g,dst,1));
 memcpy(&geom,dst+CR01_GEOM_OFFSET,sizeof(geom));assert((geom&255u)==6);
 memcpy(&hair,dst+CR01_HAIR_OFFSET,sizeof(hair));assert(hair==8);
 puts("PASS HairBounces 0..10 validated and 7..10 staged in camera scratch; geometry stays 0..6; Master Off/PT Off/shader absent remain neutral; no GPU validation");
 return 0;
}
