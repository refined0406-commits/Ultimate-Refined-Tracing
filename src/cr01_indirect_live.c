#include "cr01_indirect_live.h"
/* R15: release legacy CR01 CVar ownership BEFORE publishing any shader gain.
 * A foreign change/ambiguous capture is a hard fail-closed condition. Do not
 * claim ownership of a pre-existing engine value, and never set it to 1.00.
 * This prevents TWO non-neutral CR01-controlled multipliers. */
#include <stdint.h>
static int native_sane(uint32_t raw){
 union { uint32_t u; float f; }v; v.u=raw;
 return v.f>=0.0f&&v.f<=64.0f;
}
int cr01_indirect_shader_exclusive(CR01Owner *owner,uint32_t *slot,
                            const CR01Settings *settings,CR01RenderMode mode){
 (void)mode;
 if(!owner||!settings||owner->conflicted)return 0;
 if(owner->owned){
   /* Losing the installed value is foreign ownership. No native overwrite. */
   if(!owner->ptr || (slot&&(uintptr_t)slot!=owner->ptr)){
     owner->conflicted=1;return 0;
   }
   uint32_t *owned=(uint32_t*)owner->ptr;
   if(*(volatile uint32_t*)owned!=owner->installed){
     owner->owned=0;owner->conflicted=1;return 0;
   }
   if(!cr01_release_owner(owner)||owner->conflicted||owner->owned)return 0;
 }
 /* A missing/invalid CVar discovery never authorises non-neutral shader gain.
  * This check must run before gate publication on startup AND menu events. */
 if(!slot || !native_sane(*(volatile uint32_t*)slot))return 0;
 return 1;
}
