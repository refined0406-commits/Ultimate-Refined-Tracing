#ifndef CR01_RTAO_CONFIG_H
#define CR01_RTAO_CONFIG_H
#include <stdint.h>
/* Independent private PT RTAO parameters. Zero selections mean Native Auto
 * mode, 1 reference ray, 1.25m reference radius, V6 0.65 strength.
 * No global renderer CVar and no change to the original 15 PT controls. */
typedef struct { uint32_t mode, rays, radius, strength; } CR01RtaoSettings;
void cr01_rtao_defaults(CR01RtaoSettings *s);
int cr01_rtao_valid(const CR01RtaoSettings *s);
uint32_t cr01_rtao_count(uint32_t choice);
uint32_t cr01_rtao_radius100(uint32_t choice);
uint32_t cr01_rtao_strength100(uint32_t choice);
/* High bits of C17's existing CameraCB[33].x integer transport. May ONLY be
 * applied to the exact already-owned PT shader with verified extra decoding.
 * Bits 16..17 ray index; 18..20 radius index; 21..23 strength index;
 * bit24 activation. Every other bit remains untouched. */
uint32_t cr01_rtao_overlay(const CR01RtaoSettings *s);
uint32_t cr01_rtao_apply(uint32_t c17_packed,const CR01RtaoSettings *s,
                        uint32_t master,uint32_t pt,uint32_t custom_live,
                        uint32_t gate_epoch,uint32_t rtao_epoch);
#endif
