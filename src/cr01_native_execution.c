#include "cr01_native_execution.h"
#include "cr01_controls.h"
#include "cr01_validation.h"

static int needs_shader(unsigned f){return f==F_GEOM||f==F_HAIR||f==F_INDIRECT;}
static int needs_shadow(unsigned f){return f==F_SHADOWS;}
int cr01_native_validate(const CR01Settings *s){
 unsigned i;
 if(!s || s->master>1||!cr01_rtao_valid(&s->rtao))return 0;
 for(i=0;i<CR01_FEATURE_COUNT;i++){
  if(i==F_INDIRECT||i==F_SUN||i==F_SKY){
   if(!cr01_valid_float_100(i,s->v[i]))return 0;
  } else if(!cr01_valid_selection(i,s->v[i]))return 0;
 }
 return 1;
}
void cr01_native_init(CR01NativeController*r){
 if(!r)return;
 volatile unsigned char *raw=(volatile unsigned char*)r;
 for(unsigned i=0;i<sizeof(*r);i++)raw[i]=0;
 cr01_defaults(&r->saved);
 r->mode=CR01_RENDER_RASTER;r->initialized=1;
}
int cr01_native_set_saved(CR01NativeController*r,const CR01Settings*s){
 if(!r||!r->initialized||!cr01_native_validate(s))return 0;
 /* Policy: retain saved preferences even when master is Off; the game-facing
  * event callback then releases only the values we demonstrably own. */
 r->saved=*s;
 return 1;
}
void cr01_native_slot(CR01NativeController*r,unsigned f,uint32_t*verified_slot){
 if(!r||!r->initialized||f>=CR01_FEATURE_COUNT)return;
 /* Never accept shader/Shadow slots via the generic native CVar path. */
 if(needs_shader(f)||needs_shadow(f))return;
 if(r->owners[f].owned && r->slots[f]!=verified_slot){
  /* Do not rebind ownership to a different slot until release has succeeded.
   * Wrong-pointer calls cannot silently change native baselines. */
  (void)cr01_release_owner(&r->owners[f]);
 }
 if(!r->owners[f].owned)r->slots[f]=verified_slot;
}
int cr01_native_event(CR01NativeController*r,CR01RenderMode mode){
 unsigned f;
 if(!r||!r->initialized||mode>CR01_RENDER_PT)return 0;
 r->mode=mode;r->active_mask=r->pending_mask=0;
 /* In RT/raster, or with Master Off, no CR01-controlled PT writes remain. */
 if(mode!=CR01_RENDER_PT || !r->saved.master){
  for(f=0;f<CR01_FEATURE_COUNT;f++){
   if(!cr01_release_owner(&r->owners[f]))return 0;
  }
  return 1;
 }
 /* First release overrides the current preferences no longer request. */
 for(f=0;f<CR01_FEATURE_COUNT;f++){
  uint32_t target;
  uint32_t requested=cr01_wants_write(&r->saved,f);
  if(!requested){if(!cr01_release_owner(&r->owners[f]))return 0;continue;}
  if(needs_shader(f)||needs_shadow(f)){
   /* No lie: these carriers require independent shader/shadow authorization.
    * Mark pending rather than pretending a CVar write changes GPU shaders. */
   r->pending_mask|=1u<<f;continue;
  }
  if(!r->slots[f]||!cr01_control_value(f,r->saved.v[f],&target)){
   r->pending_mask|=1u<<f;continue;
  }
  if(!r->owners[f].owned && !cr01_control_native_sane(f,*(volatile uint32_t*)r->slots[f])){
   r->pending_mask|=1u<<f;continue;
  }
  if(!cr01_write_owner(&r->owners[f],r->slots[f],target)){
   r->pending_mask|=1u<<f;continue;
  }
  r->active_mask|=1u<<f;
 }
 return r->pending_mask==0;
}
int cr01_native_shutdown(CR01NativeController*r){
 unsigned i;int ok=1;
 if(!r||!r->initialized)return 0;
 for(i=0;i<CR01_FEATURE_COUNT;i++)if(!cr01_release_owner(&r->owners[i]))ok=0;
 if(ok){r->active_mask=0;r->pending_mask=0;r->initialized=0;}
 return ok;
}
