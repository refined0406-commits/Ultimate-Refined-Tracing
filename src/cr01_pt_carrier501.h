#ifndef CR01_PT_CARRIER501_H
#define CR01_PT_CARRIER501_H
#include "cr01_native_execution.h"
#include <stdint.h>
/* Private Patch 5.01 renderer bridge policy. All callbacks are verified,
 * game-facing operations owned by an independent native carrier. Callback
 * success must represent actual state, not merely offline compilation.
 * No private DXR state objects, SBTs, queues, or GPU command submission. */
enum { CR01_CARRIER_COLD, CR01_CARRIER_PREFLIGHT, CR01_CARRIER_NATIVE,
       CR01_CARRIER_ACTIVE, CR01_CARRIER_FAILED };
typedef struct {
 void *context;
 /* Validate exact supported EXE, native shader bytes and signed replacement
  * before allowing a game-facing write. All are read-only. */
 int (*verify_build)(void *);
 int (*verify_native_shader)(void *);
 int (*verify_camera_site)(void *);
 int (*verify_shadow_site)(void *);
 int (*verify_rollback)(void *);
 /* Game-native shader delivery is backed by a known-good carrier transaction;
  * it must return 1 only after verified commit. */
 int (*commit_native_pt_carrier)(void *);
 /* This operation must be safe, idempotent, and restore all owned state. */
 int (*restore_native_pt_carrier)(void *);
 /* Applies the three explicit PT controls in CameraCB using a VERIFIED
  * native game serializer. Not a replacement DispatchRays implementation. */
 int (*publish_camera)(void *,uint32_t geometry,uint32_t hair,uint32_t indirect);
 /* Optional independent PT shadow and billboard owners, no shared CVar. */
 int (*set_pt_shadows)(void *,uint32_t selection);
 int (*set_pt_billboards)(void *,uint32_t selection);
} CR01PTCarrier501Ops;
typedef struct {
 CR01PTCarrier501Ops ops;
 CR01Settings saved;
 CR01RenderMode mode;
 uint32_t stage, applied_camera, applied_shadows, applied_billboards;
 uint32_t carrier_owned, revision, fault;
} CR01PTCarrier501;
int cr01_pt501_init(CR01PTCarrier501 *,const CR01PTCarrier501Ops *);
int cr01_pt501_update(CR01PTCarrier501 *,const CR01Settings *,CR01RenderMode);
int cr01_pt501_release(CR01PTCarrier501 *);
#endif
