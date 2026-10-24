#ifndef CR01_CONTRACT_H
#define CR01_CONTRACT_H
#include <stdint.h>
#include <stddef.h>
#include "cr01_rtao_config.h"
/* Settings are isolated under a new versioned config group. 0 = pass through. */
enum { CR01_NATIVE=0, CR01_ON=1, CR01_OFF=2 };
enum { F_GEOM, F_HAIR, F_INDIRECT, F_SUN, F_SKY, F_AO, F_AO_STRENGTH,
       F_AO_RADIUS, F_AO_CONTRAST, F_GRASS_DAY, F_GRASS_NIGHT,
       F_SPEC_SS, F_SPEC_CHECKER, F_BILLBOARDS, F_SHADOWS, CR01_FEATURE_COUNT };
typedef struct { uint32_t master; uint32_t v[CR01_FEATURE_COUNT]; CR01RtaoSettings rtao; } CR01Settings;
typedef struct { uintptr_t ptr; uint32_t native, installed; uint8_t owned, conflicted; } CR01Owner;
/* Integer option zero = Native/Auto; neutral numeric sliders use 100 (1.00).
   Sun/Sky numeric zero is a valid deliberate 0.00 override, not Native. */
void cr01_defaults(CR01Settings *s);
int cr01_wants_write(const CR01Settings *s,unsigned feature);
int cr01_write_owner(CR01Owner *owner,uint32_t *destination,uint32_t desired);
int cr01_release_owner(CR01Owner *owner);
void cr01_release_all(CR01Owner owners[CR01_FEATURE_COUNT]);
#endif
