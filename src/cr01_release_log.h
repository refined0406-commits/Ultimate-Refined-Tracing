#ifndef CR01_RELEASE_LOG_H
#define CR01_RELEASE_LOG_H
/* Crash-tolerant session-local support diagnostics. No background worker. */
int cr01_release_log_begin(void);
void cr01_release_log_event(const char *text);
#endif
