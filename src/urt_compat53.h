#ifndef URT_COMPAT53_H
#define URT_COMPAT53_H
/* V5.3 (private development): pure helpers for optional coexistence with W3LoadFix (Nexus 13881).
 * No Win32, no CRT; host-testable. URT never requires, loads, patches or writes W3LoadFix files.
 *
 * W3LoadFix's w3loadfix.ini [Override] section sets engine variables in memory ("Group/Name=value") and
 * re-applies them every 2 seconds. When such a line names a variable that a URT row could own, URT leaves
 * that row to W3LoadFix for the session (no write, no fight) instead of relying on the conflict limiter. */

/* `sec` is the section text as returned by GetPrivateProfileSection ("k=v\0k=v\0\0"), already reduced to
 * ASCII. Returns 1 when an active (not ';'/'#' commented) line's key is exactly `cvar`, `category/cvar`, or
 * ends with "/cvar" (any group spelling); comparison is case-insensitive and ignores surrounding blanks. */
int urt_w3lf_override_mentions(const char *sec, unsigned len, const char *category, const char *cvar);
#endif
