#ifndef CR01_RT_LEGACY501_H
#define CR01_RT_LEGACY501_H
#include <stdint.h>
/* Private legacy-compatible, asynchronous worker integration, NOT a renderer barrier. */
int cr01_rt_legacy501_init(uint8_t*base,uint32_t image);
int cr01_rt_legacy501_reload(const unsigned short*settings_path);
void cr01_rt_legacy501_tick(unsigned mode,unsigned master);
void cr01_rt_legacy501_shutdown(void);
#endif
