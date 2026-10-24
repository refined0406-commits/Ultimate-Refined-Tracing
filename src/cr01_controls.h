#ifndef CR01_CONTROLS_H
#define CR01_CONTROLS_H
#include "cr01_contract.h"
/* The exact pure-C selection mapping and native-owner update performed by the
   game runtime. Shader-specific and PT-shadow writes are excluded by contract.
   Native / Auto is represented as no ownership, never guessed zero. */
int cr01_control_value(unsigned feature,uint32_t selection,uint32_t *bits);
int cr01_control_native_sane(unsigned feature,uint32_t raw);
void cr01_sync_simple_pt(const CR01Settings *settings,uint32_t pt_enable,
                         CR01Owner owners[CR01_FEATURE_COUNT],
                         uint32_t *slots[CR01_FEATURE_COUNT]);
#endif
