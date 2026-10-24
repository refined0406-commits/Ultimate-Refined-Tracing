#include "cr01_pt_carrier501.h"
#include "cr01_validation.h"
static int is_neutral(const CR01Settings*s){
 return !s->master || (s->v[F_GEOM]==0&&s->v[F_HAIR]==0&&
      s->v[F_INDIRECT]==100&&s->v[F_SHADOWS]==0&&s->v[F_BILLBOARDS]==0);
}
/* Use native equivalent shader values ONLY as neutral parameters in our own
 * serializer. These must not be written into unrelated game CVar storage. */
static uint32_t geometry(const CR01Settings*s){return s->v[F_GEOM]?s->v[F_GEOM]:2u;}
static uint32_t hair(const CR01Settings*s){return s->v[F_HAIR]?s->v[F_HAIR]:4u;}
int cr01_pt501_init(CR01PTCarrier501*c,const CR01PTCarrier501Ops*ops){
 if(!c||!ops||!ops->verify_build||!ops->verify_native_shader||
    !ops->verify_camera_site||!ops->verify_rollback||
    !ops->commit_native_pt_carrier||!ops->restore_native_pt_carrier||
    !ops->publish_camera)return 0;
 for(unsigned i=0;i<sizeof(*c);i++)((unsigned char*)c)[i]=0;c->ops=*ops;cr01_defaults(&c->saved);
 c->stage=CR01_CARRIER_COLD;c->mode=CR01_RENDER_RASTER;return 1;
}
static int restore(CR01PTCarrier501*c){
 /* Publish known native-equivalent settings BEFORE releasing carrier. Game
  * resource ownership is not relinquished unless both actions succeed. */
 if(!c->carrier_owned){c->stage=CR01_CARRIER_NATIVE;return 1;}
 if(!c->ops.publish_camera(c->ops.context,2u,4u,100u))goto fail;
 if(c->applied_shadows&&(!c->ops.set_pt_shadows||
    !c->ops.set_pt_shadows(c->ops.context,0u)))goto fail;
 if(c->applied_billboards&&(!c->ops.set_pt_billboards||
    !c->ops.set_pt_billboards(c->ops.context,0u)))goto fail;
 if(!c->ops.restore_native_pt_carrier(c->ops.context))goto fail;
 c->carrier_owned=0;c->applied_camera=0;
 c->applied_shadows=c->applied_billboards=0;c->stage=CR01_CARRIER_NATIVE;
 return 1;
fail:
 c->stage=CR01_CARRIER_FAILED;c->fault=1;
 return 0;
}
int cr01_pt501_release(CR01PTCarrier501*c){return c&&restore(c);}
int cr01_pt501_update(CR01PTCarrier501*c,const CR01Settings*s,CR01RenderMode mode){
 if(!c||!s||c->stage==CR01_CARRIER_FAILED||mode>CR01_RENDER_PT||
    !cr01_native_validate(s))return 0;
 /* Save preferences even when inactive. No settings are applied outside PT. */
 if(mode!=CR01_RENDER_PT||is_neutral(s)){
  if(!restore(c))return 0;
  c->saved=*s;c->mode=mode;c->revision++;return 1;
 }
 if(!c->carrier_owned){
  c->stage=CR01_CARRIER_PREFLIGHT;
  if(!c->ops.verify_build(c->ops.context)||
     !c->ops.verify_native_shader(c->ops.context)||
     !c->ops.verify_camera_site(c->ops.context)||
     !c->ops.verify_rollback(c->ops.context)||
     (s->v[F_SHADOWS]&&(!c->ops.verify_shadow_site||
       !c->ops.verify_shadow_site(c->ops.context))))goto refuse;
  if(!c->ops.commit_native_pt_carrier(c->ops.context))goto refuse;
  c->carrier_owned=1;c->stage=CR01_CARRIER_ACTIVE;
 }
 if(!c->ops.publish_camera(c->ops.context,geometry(s),hair(s),s->v[F_INDIRECT]))goto refuse;
 c->applied_camera=1;
 if(s->v[F_SHADOWS]){
  if(!c->ops.set_pt_shadows||!c->ops.set_pt_shadows(c->ops.context,s->v[F_SHADOWS]))goto refuse;
  c->applied_shadows=1;
 }else if(c->applied_shadows){
  if(!c->ops.set_pt_shadows||!c->ops.set_pt_shadows(c->ops.context,0))goto refuse;
  c->applied_shadows=0;
 }
 if(s->v[F_BILLBOARDS]){
  if(!c->ops.set_pt_billboards||!c->ops.set_pt_billboards(c->ops.context,s->v[F_BILLBOARDS]))goto refuse;
  c->applied_billboards=1;
 }else if(c->applied_billboards){
  if(!c->ops.set_pt_billboards||!c->ops.set_pt_billboards(c->ops.context,0))goto refuse;
  c->applied_billboards=0;
 }
 c->saved=*s;c->mode=mode;c->revision++;return 1;
refuse:
 /* Every unsupported signature or partial transaction is native-only; a
  * failure to reverse is sticky and may never be interpreted as applied. */
 if(!restore(c)){c->stage=CR01_CARRIER_FAILED;return 0;}
 c->stage=CR01_CARRIER_FAILED;c->fault=1;return 0;
}
