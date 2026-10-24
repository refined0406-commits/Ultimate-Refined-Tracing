#ifndef CR01_SHADER_SLOTS_H
#define CR01_SHADER_SLOTS_H
#include <stdint.h>
#include <stddef.h>
int cr01_shader_slots_install(uint8_t *exe,const uint8_t *custom,size_t custom_size);
int cr01_shader_slots_commit(void);
int cr01_shader_slots_rollback(void);
int cr01_shader_slots_active(void);
#endif
