#ifndef CR01_BONE_HOOK501_H
#define CR01_BONE_HOOK501_H
#include <stdint.h>
/* Only called by existing single settings-worker thread, never DllMain. */
int cr01_bone501_set(unsigned on);
/* Conservative Off; restore only if our exact pointer is still installed. */
void cr01_bone501_shutdown(void);
#endif
