#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "cr01_indirect_live.h"
static uint32_t fb(float f){union{float f;uint32_t u;}v;v.f=f;return v.u;}
int main(void){
 CR01Settings s;CR01Owner o={0};uint32_t slot=fb(0.83f);
 cr01_defaults(&s);s.master=1;s.v[F_INDIRECT]=150;
 assert(cr01_indirect_shader_exclusive(&o,&slot,&s,CR01_RENDER_PT));
 assert(!o.owned&&slot==fb(0.83f));
 s.v[F_INDIRECT]=125;
 assert(cr01_indirect_shader_exclusive(&o,&slot,&s,CR01_RENDER_PT));
 assert(!o.owned&&slot==fb(0.83f));
 /* Safely restore legacy owned state, never assume native=1.00. */
 assert(cr01_write_owner(&o,&slot,fb(1.40f)));
 assert(o.owned&&slot==fb(1.40f));
 assert(cr01_indirect_shader_exclusive(&o,&slot,&s,CR01_RENDER_PT));
 assert(!o.owned&&!o.conflicted&&slot==fb(0.83f));
 /* A foreign edit during a former ownership lifetime is never clobbered. */
 assert(cr01_write_owner(&o,&slot,fb(1.20f)));
 slot=fb(1.11f);
 assert(!cr01_indirect_shader_exclusive(&o,&slot,&s,CR01_RENDER_PT));
 assert(o.conflicted&&!o.owned&&slot==fb(1.11f));
 assert(!cr01_indirect_shader_exclusive(&o,&slot,&s,CR01_RENDER_RASTER));
 assert(slot==fb(1.11f));
 /* Missing carrier, NaN and foreign pointer are fail-closed. */
 {CR01Owner clean={0};uint32_t nan=0x7fc00000u;
  assert(!cr01_indirect_shader_exclusive(&clean,0,&s,CR01_RENDER_PT));
  assert(!cr01_indirect_shader_exclusive(&clean,&nan,&s,CR01_RENDER_PT));
 }
 puts("PASS R15 exclusive Indirect: no CR01 CVar write, native restore, conflict latch, no foreign overwrite");
 return 0;
}
