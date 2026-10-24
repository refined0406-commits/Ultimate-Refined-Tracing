#ifndef CR01_SHADOW501_H
#define CR01_SHADOW501_H
#include <stdint.h>
#include "cr01_contract.h"
/* Patch 5.01 only, mirror only PT shadow gates, never native Shadow CVar. */
int cr01_shadow501_prepare(uint8_t*exe,uint32_t image_size);
int cr01_shadow501_update(const CR01Settings*,unsigned pt_active);
int cr01_shadow501_ready(void);
#endif
