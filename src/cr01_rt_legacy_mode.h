#ifndef CR01_RT_LEGACY_MODE_H
#define CR01_RT_LEGACY_MODE_H
#include "cr01_native_execution.h"
/* One shared renderer enum is authoritative for legacy Normal RT eligibility.
 * Raster=0 / Normal RT=1 / PT=2. Never duplicate literal mode IDs here. */
static inline unsigned cr01_rt_legacy_mode_eligible(CR01RenderMode mode){
 return mode==CR01_RENDER_NORMAL_RT;
}
#endif
