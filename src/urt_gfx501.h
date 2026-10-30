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
/* V5.3.0 rev4: guard-file I/O for the startup guard (read: length or -1; write: bytes written) */
void urt_gfx501_set_guard_io(int (*rd)(char*, unsigned), int (*wr)(const char*, unsigned));
/* V5.3.0 R11: 1 when PT: Second Bounce Coverage was decided at ASI load this session (urt_bounce_boot501.c) */
void urt_gfx501_set_bounce_decided(unsigned decided);
void urt_gfx501_note_variant_frames(unsigned frames);   /* R12 FIX4: PT frames rendered with URT's variant so far */
unsigned urt_gfx501_owned(void);   /* R12 FIX3: engine values URT holds now (diagnostics) */
unsigned urt_gfx501_writes(void);  /* R12 FIX3: engine-variable writes since start (diagnostics, tests) */
#endif
