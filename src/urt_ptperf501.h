#ifndef URT_PTPERF501_H
#define URT_PTPERF501_H
#include <stdint.h>
/* V5.2 Path Tracing performance control: PT: Scene Range (Patch 5.01 adapter). */
int urt_ptperf501_init(uint8_t *base, uint32_t image);
int urt_ptperf501_reload(void);
void urt_ptperf501_tick(unsigned mode, unsigned master);
void urt_ptperf501_shutdown(void);
#endif
