#include "cr01_gate_live_adapter.h"
#include "cr01_validation.h"
int cr01_gate_publish_e06(CR01GateState *out,const CR01Settings *saved,
                           uint32_t pt,uint32_t completed,uint32_t custom,uint32_t epoch){
 CR01GateFields f;
 if(!out||!saved||saved->master>1||pt>1||completed>1||custom>1)return 0;
 for(unsigned i=0;i<CR01_FEATURE_COUNT;i++){
  if(i==F_INDIRECT||i==F_SUN||i==F_SKY){
   if(!cr01_valid_float_100(i,saved->v[i]))return 0;
  } else if(!cr01_valid_selection(i,saved->v[i]))return 0;
 }
 /* Independently preserve native-default selection=0 without changing saved
    preferences. In particular indirect=100 is always neutral (1.00). */
 f.geometry=(uint8_t)(saved->v[F_GEOM]?saved->v[F_GEOM]:CR01_NATIVE_GEOM);
 f.hair=(uint8_t)(saved->v[F_HAIR]?saved->v[F_HAIR]:CR01_NATIVE_HAIR);
 f.comp=(uint8_t)saved->v[F_INDIRECT];
 f.master=(uint8_t)saved->master;
 f.pt=(uint8_t)pt;
 f.renderer_ready=(uint8_t)completed;
 f.shader_ready=(uint8_t)custom;
 return cr01_gate_publish(out,f,epoch);
}
