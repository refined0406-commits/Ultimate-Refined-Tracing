#ifndef URT_BOUNCE_BOOT501_H
#define URT_BOUNCE_BOOT501_H
/* V5.3.0 R11 PT: Second Bounce Coverage - startup decision (see urt_bounce_boot501.c). Windows only. */
unsigned urt_bc_startup_level(void);            /* bits 0-1 second bounce, 2-3 first-bounce diffuse; 0 = build nothing */
void urt_bc_note_result(unsigned level, int ok, unsigned bytes, const char *why);   /* why: stage + reason of a failure */
unsigned urt_bc_active_level(void);             /* packed levels of the variant installed this session (0 = none) */
unsigned urt_bc_decided(void);                  /* 1 once the decision ran this session */
int urt_bc_prepare_paths(void);                  /* R12 FIX2: guard file path (call outside DllMain) */
void urt_bc_clean_exit(void);                   /* R12 FIX2: DLL_PROCESS_DETACH at normal process exit */
#endif
