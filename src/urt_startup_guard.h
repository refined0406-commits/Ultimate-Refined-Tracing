/* Startup guard for restart-dependent rows under test (pure logic, no Win32; the runtime supplies the small guard file).
 * R12 FIX2 session-state design (replaces the R4-R12 "10 minutes or failed" rule):
 *  - a (row, value) pair applied at a start is recorded PENDING before it takes effect;
 *  - a CLEAN game exit (the process ends normally; URT's DLL is told so) turns this session's PENDING records CLEAN:
 *    an ordinary quit or restart is never treated as a crash, however short the session;
 *  - URT_SG_STABLE_MS of RT/PT rendering with the value in effect turns it PROVEN (never blocked afterwards);
 *  - a start that still finds a record PENDING means the last session ended without a clean exit and before that proof
 *    (crash, hang, killed process): suspected crash. This start does NOT apply the value (normal behaviour), and the record
 *    becomes RETRY: the next start applies it once more (RETRYING). If that session also ends without a clean exit or
 *    proof, the pair becomes FAILED and stays off until Native / Auto is chosen for one start (or another value).
 *    At most two interrupted sessions, never a loop; other settings are never touched. */
#ifndef URT_STARTUP_GUARD_H
#define URT_STARTUP_GUARD_H
#include <stdint.h>
#define URT_SG_MAX 8u
#define URT_SG_STABLE_MS 60000u             /* R12 FIX2: 1 minute of RT/PT rendering proves an RT engine row (was 10 minutes) */
#define URT_SG_PT_FRAMES 300u               /* R12 FIX4: a PT shader coverage row qualifies after 300 PT frames rendered with the variant (~5 s) */
enum { URT_SG_NONE = 0, URT_SG_PENDING = 1, URT_SG_PROVEN = 2, URT_SG_FAILED = 3, URT_SG_CLEAN = 4, URT_SG_RETRY = 5, URT_SG_RETRYING = 6 };
/* what a start decided for one row (for the log) */
enum { URT_SG_EV_NATIVE = 0, URT_SG_EV_NEW, URT_SG_EV_CLEAN, URT_SG_EV_PROVEN, URT_SG_EV_RETRYING, URT_SG_EV_SUSPECTED, URT_SG_EV_FAILED_NOW, URT_SG_EV_BLOCKED, URT_SG_EV_FULL };
typedef struct { char key[40]; uint8_t value; uint8_t state; } UrtSgEntry;
typedef struct { UrtSgEntry e[URT_SG_MAX]; unsigned n; } UrtSgFile;
/* parse the guard file text; malformed lines are ignored (an unreadable file = no records) */
void urt_sg_parse(const char *text, unsigned len, UrtSgFile *out);
/* serialise; returns length (0 if cap too small) */
unsigned urt_sg_format(const UrtSgFile *f, char *out, unsigned cap);
/* decide one guarded row at start. wish = saved option index (0 = Native).
 * Returns 1 = apply (record now PENDING or PROVEN), 0 = do not apply.
 * *newly_failed = 1 when this start turned a PENDING record into FAILED. */
int urt_sg_decide(UrtSgFile *f, const char *key, uint8_t wish, int *newly_failed);
/* the same decision, reporting the event (URT_SG_EV_*) */
int urt_sg_decide_ex(UrtSgFile *f, const char *key, uint8_t wish, int *event);
/* clean exit: the listed keys' PENDING / RETRYING records become CLEAN; returns how many changed */
unsigned urt_sg_mark_clean(UrtSgFile *f, const char *const *keys, unsigned nkeys);
/* the record's state for a key (URT_SG_NONE if none) */
int urt_sg_state(const UrtSgFile *f, const char *key);
/* mark every PENDING / RETRYING record PROVEN; returns how many changed */
unsigned urt_sg_prove_all(UrtSgFile *f);
/* R12 FIX4: PENDING / RETRYING record of one key -> PROVEN; 1 when it changed */
int urt_sg_prove_key(UrtSgFile *f, const char *key);
unsigned urt_sg_pending(const UrtSgFile *f);
/* R12 FIX2: the keys whose values this session applied (only those are marked CLEAN at a clean exit) */
void urt_sg_session_note(const char *key);
void urt_sg_session_clear(void);
unsigned urt_sg_session_keys(const char *const **keys);
/* R12 FIX2: one log sentence per decision event (key and value are added by the caller) */
const char *urt_sg_event_text(int event);
/* R5: helpers to drop records of rows that are no longer guarded */
int urt_sg_key_is(const UrtSgFile *f, unsigned i, const char *key);
void urt_sg_drop_at(UrtSgFile *f, unsigned i);
#endif
