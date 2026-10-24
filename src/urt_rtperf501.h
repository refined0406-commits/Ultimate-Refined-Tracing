#ifndef URT_RTPERF501_H
#define URT_RTPERF501_H
#include <stdint.h>
/* V5.1 Normal RT performance controls (Patch 5.01 adapter). */
int urt_rtperf501_init(uint8_t *base, uint32_t image);
int urt_rtperf501_reload(void);
void urt_rtperf501_tick(unsigned mode, unsigned master);
void urt_rtperf501_shutdown(void);
#endif
