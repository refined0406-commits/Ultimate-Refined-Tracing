#ifndef CR01_MENU_BRIDGE_H
#define CR01_MENU_BRIDGE_H
#include "cr01_native_execution.h"
#include <stddef.h>
/* Pure, event-driven settings boundary. Witcher menu adapter must provide a
   real menu event, never guessed memory contents. A caller is responsible for
   private durable IO and for verifying all game slot pointers. */
typedef struct {
 CR01NativeController controller;
 uint32_t dirty_generation;
 uint32_t save_ack;
} CR01MenuBridge;
void cr01_menu_bridge_init(CR01MenuBridge*);
/* 0=reject, 1=accepted/applied, 2=accepted/saved but backend pending. */
int cr01_menu_bridge_event(CR01MenuBridge*,const char*key,const char*value);
/* Atomic whole-menu snapshot: all values are parsed/validated before one saved-state
 * transition and one renderer event. No partial file-save generations are applied. */
int cr01_menu_bridge_snapshot(CR01MenuBridge*,const char*const*keys,const char*const*values,unsigned count);
/* Atomic partial snapshot: present[i]!=0 overlays only that persisted key on top
 * of SavedPreferences. Missing/default-pruned menu keys are preserved, not treated
 * as parse failure. Present invalid values still fail the whole transaction. */
int cr01_menu_bridge_snapshot_partial(CR01MenuBridge*,const char*const*keys,const char*const*values,const unsigned char*present,unsigned count);
int cr01_menu_bridge_mode(CR01MenuBridge*,CR01RenderMode);
int cr01_menu_bridge_load(CR01MenuBridge*,const char*,size_t);
int cr01_menu_bridge_save(const CR01MenuBridge*,char*,size_t,size_t*);
int cr01_menu_bridge_should_disable(const CR01MenuBridge*,unsigned feature);
/* Pure bootstrap helpers: a missing/malformed dx12user.settings entry gets
 * the current valid private preference (or the 1.00 default). */
int cr01_menu_bridge_valid_input(const CR01MenuBridge*,const char*,const char*);
int cr01_menu_bridge_value_matches_saved(const CR01MenuBridge*,const char*,const char*);
int cr01_menu_bridge_saved_text(const CR01MenuBridge*,const char*,char*,size_t);
#endif
