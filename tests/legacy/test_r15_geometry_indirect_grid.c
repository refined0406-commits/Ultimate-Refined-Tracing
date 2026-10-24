/* Targeted changed-code regression, not an exhaustive multi-control simulation. */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "cr01_gate_live_adapter.h"
#include "cr01_gate_core.h"
static unsigned read32(const unsigned char*p){return p[0]|((unsigned)p[1]<<8)|((unsigned)p[2]<<16)|((unsigned)p[3]<<24);}
int main(void){
 unsigned cases=0; CR01GateState gate={0}; CR01Settings s;
 for(unsigned geom=0;geom<=6;geom++)for(unsigned comp=100;comp<=150;comp+=5)
  for(unsigned hair=0;hair<=6;hair++){
   unsigned char dst[1024]; cr01_defaults(&s);s.master=1;
   s.v[F_GEOM]=geom;s.v[F_HAIR]=hair;s.v[F_INDIRECT]=comp;
   assert(cr01_gate_publish_e06(&gate,&s,1,1,1,++cases));
   memset(dst,0xa5,sizeof dst);
   int got=cr01_gate_post_serialize(&gate,dst,1);
   int changed=((geom?geom:2)!=2)||((hair?hair:4)!=4)||comp!=100;
   assert(got==changed);
   if(got){
    assert(read32(dst+CR01_GEOM_OFFSET)==((geom?geom:2)|(comp<<8)));
    assert(read32(dst+CR01_HAIR_OFFSET)==(hair?hair:4));
   }
   assert(cr01_gate_publish_e06(&gate,&s,0,1,1,++cases));
   memset(dst,0xa5,sizeof dst);
   assert(!cr01_gate_post_serialize(&gate,dst,1));
   assert(read32(dst+CR01_GEOM_OFFSET)==0xa5a5a5a5u);
   assert(cr01_gate_publish_e06(&gate,&s,1,1,1,++cases));
   memset(dst,0xa5,sizeof dst);
   assert(!cr01_gate_post_serialize(&gate,dst,0));
  }
 puts("PASS R15 changed-code bridge: geometry native/1..6 x hair native/1..6 x indirect 1.00..1.50, PT-only, no diagnostic stretch");
 return 0;
}
