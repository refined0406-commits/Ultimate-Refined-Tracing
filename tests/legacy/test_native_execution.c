#include "cr01_native_execution.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint32_t fb(float v){union {float v;uint32_t b;}x;x.v=v;return x.b;}
int main(void){
 CR01NativeController c;CR01Settings s;uint32_t ao=0, sun=fb(1.0f);
 cr01_native_init(&c);assert(c.initialized);
 assert(c.saved.master==0 && c.saved.v[F_INDIRECT]==100);
 assert(cr01_native_validate(&c.saved));
 cr01_native_slot(&c,F_AO,&ao);cr01_native_slot(&c,F_SUN,&sun);
 assert(cr01_native_event(&c,CR01_RENDER_PT) && ao==0);
 s=c.saved;s.master=1;s.v[F_AO]=CR01_ON;s.v[F_SUN]=150;
 assert(cr01_native_set_saved(&c,&s));
 assert(cr01_native_event(&c,CR01_RENDER_PT));
 assert(ao==1&&sun==fb(1.5f));assert(c.active_mask==(1u<<F_AO | 1u<<F_SUN));
 assert(cr01_native_event(&c,CR01_RENDER_NORMAL_RT) && ao==0 && sun==fb(1));
 assert(cr01_native_event(&c,CR01_RENDER_PT) && ao==1);
 /* Saved preferences survive a live Master Off and On. */
 s.master=0;assert(cr01_native_set_saved(&c,&s));
 assert(cr01_native_event(&c,CR01_RENDER_PT) && ao==0 && c.saved.v[F_AO]==CR01_ON);
 s.master=1;assert(cr01_native_set_saved(&c,&s));
 assert(cr01_native_event(&c,CR01_RENDER_PT) && ao==1);
 /* Shader controls must not be counted as live without a GPU carrier. */
 s.v[F_GEOM]=3;assert(cr01_native_set_saved(&c,&s));
 assert(!cr01_native_event(&c,CR01_RENDER_PT));
 assert((c.pending_mask&(1u<<F_GEOM)) && !(c.active_mask&(1u<<F_GEOM)));
 /* Foreign writes must not be overwritten on Master Off. */
 ao=0; s.master=0;assert(cr01_native_set_saved(&c,&s));
 assert(cr01_native_event(&c,CR01_RENDER_PT)); assert(ao==0);
 assert(cr01_native_shutdown(&c));
 /* Invalid values leave saved configuration unmodified. */
 cr01_native_init(&c);s=c.saved;s.v[F_GEOM]=7;
 assert(!cr01_native_set_saved(&c,&s)&&c.saved.v[F_GEOM]==0);
 assert(!cr01_native_event(&c,(CR01RenderMode)7));
 puts("PASS native game-owned DXR policy, isolation, live master and fail-closed carrier");
 return 0;
}
