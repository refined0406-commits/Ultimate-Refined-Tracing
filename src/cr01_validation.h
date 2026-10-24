#ifndef CR01_VALIDATION_H
#define CR01_VALIDATION_H
#include "cr01_contract.h"
/* Pure host-testable boundaries. Never interpret absent or malformed user data
   as a command to change native rendering. */
int cr01_valid_selection(unsigned feature,uint32_t selection);
int cr01_valid_float_100(unsigned feature,uint32_t scaled);
int cr01_should_bind_renderer(const CR01Settings *settings,uint32_t pt_enable);
int cr01_settings_are_native(const CR01Settings *settings);
int cr01_requires_shader_adapter(unsigned feature);
#endif
