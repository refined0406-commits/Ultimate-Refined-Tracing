#ifndef CR01_INDIRECT_LIVE_H
#define CR01_INDIRECT_LIVE_H
#include "cr01_native_execution.h"
/* R15: legacy CVar release, safe no-write verification, and one-writer shader authority. */
int cr01_indirect_shader_exclusive(CR01Owner *owner,uint32_t *slot,
                            const CR01Settings *settings,CR01RenderMode mode);
#endif
