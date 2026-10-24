#include "cr01_validation.h"
int cr01_valid_selection(unsigned feature,uint32_t selection){
  if(feature>=CR01_FEATURE_COUNT)return 0;
  switch(feature){
    case F_GEOM: return selection<=6;
    case F_HAIR: return selection<=10;
    case F_AO: case F_SPEC_SS: case F_SPEC_CHECKER:
    case F_BILLBOARDS: case F_SHADOWS: return selection<=2;
    case F_AO_STRENGTH: return selection<=5;
    case F_AO_RADIUS: case F_AO_CONTRAST:
    case F_GRASS_DAY: case F_GRASS_NIGHT: return selection<=6;
    default: return 0;
  }
}
int cr01_valid_float_100(unsigned feature,uint32_t scaled){
  switch(feature){
    case F_INDIRECT: return scaled>=100 && scaled<=150;
    case F_SUN: case F_SKY: return scaled<=300;
    default: return 0;
  }
}
int cr01_should_bind_renderer(const CR01Settings *settings,uint32_t pt_enable){
  return settings&&settings->master==1&&pt_enable==1;
}
int cr01_settings_are_native(const CR01Settings *settings){
  unsigned f;
  if(!settings)return 0;
  for(f=0;f<CR01_FEATURE_COUNT;f++)if(cr01_wants_write(settings,f))return 0;
  return 1;
}

/* These three native CVars are not independently sufficient to update the
   5.01 compiled PT shader. They must not silently masquerade as live controls. */
int cr01_requires_shader_adapter(unsigned feature){
  return feature==F_GEOM||feature==F_HAIR||feature==F_INDIRECT;
}
