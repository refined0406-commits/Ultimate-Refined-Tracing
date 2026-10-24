#ifndef CR01_NATIVE_EXECUTION_H
#define CR01_NATIVE_EXECUTION_H
#include "cr01_contract.h"
/* Game-owned DXR model. This layer never allocates an SBT, records a GPU
 * command, changes a DXR state object, or installs a breakpoint. The host
 * supplies verified, writable PT-only CVar slots; absence means no write. */
typedef enum {
 CR01_RENDER_RASTER=0, CR01_RENDER_NORMAL_RT=1, CR01_RENDER_PT=2
} CR01RenderMode;
typedef struct {
 CR01Settings saved;
 CR01Owner owners[CR01_FEATURE_COUNT];
 uint32_t *slots[CR01_FEATURE_COUNT];
 CR01RenderMode mode;
 uint32_t adapter_status; /* bit0: shader carrier; bit1: independent shadow carrier */
 uint32_t active_mask; /* only independently backed PT native controls */
 uint32_t pending_mask; /* requested, but no validated backend exists */
 uint32_t initialized;
} CR01NativeController;
/* Must be called only by an event owner who knows the current game mode.
 * Adapter statuses are game-evidence inputs, NOT inferred by this module.
 * This file intentionally neither activates nor claims shader controls. */
void cr01_native_init(CR01NativeController *r);
int cr01_native_set_saved(CR01NativeController *r,const CR01Settings *s);
void cr01_native_slot(CR01NativeController *r,unsigned f,uint32_t *verified_slot);
int cr01_native_event(CR01NativeController *r,CR01RenderMode mode);
int cr01_native_shutdown(CR01NativeController *r);
int cr01_native_validate(const CR01Settings *s);
#endif
