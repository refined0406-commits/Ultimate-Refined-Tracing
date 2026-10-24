#ifndef REFINED_PT_SHADOW_EXT_501_H
#define REFINED_PT_SHADOW_EXT_501_H
#include <stddef.h>
#include <stdint.h>
/* Extends existing PT-only Shadow mirror, preserving CDPR native state outside PT. */
int refined_pt_shadow_ext_install(uint8_t *game_base, const volatile uint32_t *native_shadow, volatile uint32_t *pt_mirror);
int refined_pt_shadow_ext_restore(void);
unsigned refined_pt_shadow_ext_redirects(void);
#endif
