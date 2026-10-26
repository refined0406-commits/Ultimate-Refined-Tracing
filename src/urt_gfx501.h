#ifndef URT_GFX501_H
#define URT_GFX501_H
#include <stdint.h>
int urt_gfx501_init(uint8_t *base, uint32_t image);
int urt_gfx501_reload(void);
void urt_gfx501_tick(unsigned mode_valid, unsigned mode, unsigned master, uint32_t now_ms);
void urt_gfx501_shutdown(void);
/* V5.3 */
void urt_gfx501_set_external_overrides(const char *text, unsigned len);
void urt_gfx501_boot(unsigned master, uint32_t now_ms);
#endif
