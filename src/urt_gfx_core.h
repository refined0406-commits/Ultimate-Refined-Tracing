#ifndef URT_GFX_CORE_H
#define URT_GFX_CORE_H
/* V5.2 selected controls: pure ownership logic (no Win32,
 * no CRT), host-testable. One menu option -> one engine variable (table in
 * urt_gfx_table.c, generated from tools/v52_selected/spec.py).
 *
 * Contract (same as every URT live control since V5.1, plus a fight limiter):
 *  - option 0 = Native / Auto: the variable is never written;
 *  - a value is owned only while Master is On and the renderer is in the row's scope;
 *    otherwise it is released with compare-and-swap (applied -> the value it had);
 *  - bools are owned with a 1-byte CAS (the neighbouring bytes are never written);
 *  - a foreign write (the game's own graphics menu, a graphics preset, another mod) is
 *    detected and the slot is released without overwriting it; ownership resumes on
 *    the next tick using the new value as the native one. A field that is rewritten by
 *    someone else 3 times within 30 seconds is given up until its URT value is changed
 *    again (never fights a mod that keeps writing it);
 *  - special rows (frame generation) are handled by their own adapter and ignored here.
 *
 * RC6 - settings-file protection for "saved" engine variables (descriptor flag 0x100):
 *  The game's settings save copies the LIVE value of every variable whose save bit
 *  (byte +9 of the descriptor = bit 0 of the 0x100 flag byte) is set into the user
 *  settings document and writes dx12user.settings. A raw URT value would therefore end
 *  up in the player's file. For such rows URT now:
 *   1. clears that one bit with a 1-byte CAS ("detach"), and only after
 *      URT_GX_DETACH_SETTLE_MS writes its value, so a save already past the bit test
 *      can never read URT's value;
 *   2. on release, restores the value first and then sets the bit back with a CAS
 *      ("reattach"), so the next save writes the game's own value again.
 *  While detached, the game skips the variable when saving: the player's line in
 *  dx12user.settings is left exactly as it was. A row whose save byte cannot be bound
 *  or is not in the expected state is never owned (stays Native).
 * RC7 - player changes win: when the game (e.g. its own graphics menu) or another mod writes a
 *  saved row's variable, URT releases it, gives the save bit back and does NOT take the row again
 *  until the game has written dx12user.settings (urt_gx_settings_saved, called on the next
 *  settings-file change). The player's own new value therefore reaches the file first. */
#include <stdint.h>
#include "urt_gfx_table.h"

typedef struct { uint8_t v[URT_GX_COUNT]; } UrtGxPrefs;   /* option index per row */
int urt_gx_prefs_valid(const UrtGxPrefs *p);
/* Parses a menu value ("" = missing = Native). Returns 1 and the index, 0 when invalid. */
int urt_gx_parse_index(const char *s, unsigned row, uint8_t *out);
/* 1 when the row's engine variable is saved to dx12user.settings by the game. */
int urt_gx_row_saved(unsigned row);

#define URT_GX_FIGHT_LIMIT 3u
#define URT_GX_FIGHT_WINDOW_MS 30000u
#define URT_GX_DETACH_SETTLE_MS 250u
enum { URT_GX_SB_ATTACHED = 0, URT_GX_SB_DETACHED = 1 };
typedef struct {
  uint8_t *slot[URT_GX_COUNT];
  uint8_t *savebyte[URT_GX_COUNT];     /* descriptor flag byte (saved rows only)      */
  uint8_t sb_orig[URT_GX_COUNT];       /* its value before URT cleared the save bit    */
  uint8_t sb_state[URT_GX_COUNT];
  uint32_t sb_t0[URT_GX_COUNT];        /* time the bit was cleared                    */
  uint32_t original[URT_GX_COUNT], applied[URT_GX_COUNT];
  uint8_t owned[URT_GX_COUNT], yielded[URT_GX_COUNT], yield_choice[URT_GX_COUNT];
  uint8_t fights[URT_GX_COUNT];
  uint8_t await_save[URT_GX_COUNT];   /* saved row written by someone else: wait for the next game save */
  uint32_t fight_t0[URT_GX_COUNT];
  uint32_t conflicts, yields, writes, detaches, reattaches, sb_conflicts;
  uint8_t boot_done;                   /* V5.3.0: boot rows were decided for this session */
} UrtGxOwner;
void urt_gx_init(UrtGxOwner *o);
/* 1 when the renderer mode (0 raster, 1 Normal RT, 2 PT) is in the row's scope. */
int urt_gx_in_scope(unsigned row, unsigned mode);
/* Desired raw value of a row for a preference (0 = Native: no ownership). */
int urt_gx_target(const UrtGxPrefs *p, unsigned row, uint32_t native, uint32_t *out);
/* V5.3.0: 1 for a row written once before the renderer exists and then left alone for the session (stage BOOT):
 * urt_gx_update and urt_gx_release_all never touch it; urt_gx_boot_apply decides it exactly once. */
int urt_gx_is_boot(unsigned row);
/* Applies every BOOT row whose preference is not Native (Master On, valid preferences) and marks the session
 * decided; later calls do nothing. Returns the number of rows applied. */
unsigned urt_gx_boot_apply(UrtGxOwner *o, const UrtGxPrefs *p, unsigned master);
/* 1 for a row the engine reads once when ray tracing initialises (stage STARTUP, V5.3). */
int urt_gx_is_startup(unsigned row);
/* One worker tick. master=0 releases everything; mode_valid=0 releases every live row (startup rows stay
 * owned while Master is On, see urt_gx_is_startup). */
void urt_gx_update(UrtGxOwner *o, const UrtGxPrefs *p, unsigned master, unsigned mode_valid,
                   unsigned mode, uint32_t now_ms);
/* The game wrote dx12user.settings: rows waiting for that save may be owned again. */
void urt_gx_settings_saved(UrtGxOwner *o);
/* Releases every value and reattaches every save bit (value first, then the bit). */
void urt_gx_release_all(UrtGxOwner *o);
/* Number of rows currently owned (diagnostics). */
unsigned urt_gx_owned_count(const UrtGxOwner *o);
/* Number of save bits currently cleared by URT (must be 0 after release_all). */
unsigned urt_gx_detached_count(const UrtGxOwner *o);
#endif
