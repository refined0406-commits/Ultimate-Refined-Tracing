#ifndef URT_FG501_H
#define URT_FG501_H
#include <stdint.h>
/* V5.2 r2 "Frame Generation: Keep Resources in Menus" (Patch 5.01).
 *
 * Patch 5.01 sets DLSS Frame Generation (Streamline DLSS-G) to off when a full-screen
 * menu opens and back on when it closes. With the options it passes, Streamline frees
 * every Frame Generation resource on "off" and allocates them again on "on". Streamline's
 * documented flag DLSSGFlags::eRetainResourcesWhenOff (bit 3 of DLSSGOptions::flags)
 * tells it to keep them. This control adds exactly that flag to the game's own
 * slDLSSGSetOptions calls whose mode is eOff, and only while the player has Frame
 * Generation enabled in the game's settings (DLSSGMode > 0). Nothing else in the options
 * (mode, frame count, Reflex, V-Sync, frame limit, presets) is changed, no other
 * Streamline/NGX/Reflex/NVAPI function is touched and no NVIDIA App setting is read or
 * written.
 *
 * Mechanism: the game resolves slDLSSGSetOptions lazily into ONE global function
 * pointer (RVA 0x5ca8140) that is used only by its option-setter (0x1b81530). URT swaps
 * that pointer, with compare-and-swap, for a pass-through wrapper while the control is
 * On, and swaps it back when it is Off/Native or Master is Off. Exact executable only
 * (the setter's three accesses to the pointer are fingerprinted, read-only). If anything
 * else replaces the pointer, URT re-wraps the new one up to 3 times, then leaves it alone. */
int urt_fg501_init(uint8_t *base, uint32_t image, int known_exe);
void urt_fg501_tick(int want);
void urt_fg501_shutdown(void);
/* diagnostics */
typedef struct { uint32_t installed, installs, replaced, calls, off_calls, flagged; int available; } UrtFgStats;
void urt_fg501_stats(UrtFgStats *s);
/* exposed for host tests: the wrapper and the option-struct check */
int urt_fg501_wrapper(const void *viewport, const void *options);
int urt_fg501_should_retain(const void *options, int fg_user_enabled);
#endif
