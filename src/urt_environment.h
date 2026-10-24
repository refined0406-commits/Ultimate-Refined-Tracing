#ifndef URT_ENVIRONMENT_H
#define URT_ENVIRONMENT_H
#include <stdint.h>
/* V5 environment, compatibility and instance management (Windows).
 * Everything here is read-only towards the game and other mods: file
 * existence/attribute checks, GetModuleHandle of already-loaded modules and
 * one read-only GetPrivateProfileString. Nothing is loaded, patched or written. */

/* 1 = this module may run. 0 = another URT runtime (V5 instance or a legacy
 * V4 RefinedTracing.asi file/module) owns the process; stay dormant. */
int urt_instance_claim(const char **reason);

/* One-shot compatibility report written to the URT log (worker thread). */
void urt_environment_report(void);

/* Documents\The Witcher 3 (no trailing slash). 1 on success. */
int urt_witcher_user_dir(uint16_t *out, unsigned cap);
#endif
