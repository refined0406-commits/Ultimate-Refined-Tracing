#ifndef URT_RUNTIME_H
#define URT_RUNTIME_H
/* V5 runtime capability tiers and PT shader state (see cr01_asi_entry.c). */
enum { URT_TIER_ADAPTIVE=0, URT_TIER_LATE=1, URT_TIER_FULL=2 };
enum { URT_SHADER_NONE=0, URT_SHADER_ACTIVE=1, URT_SHADER_FAILED=3,
       URT_SHADER_LATE=4, URT_SHADER_UNSUPPORTED_EXE=5 };
int urt_runtime_shader_state(void);
int urt_runtime_tier(void);
#endif
