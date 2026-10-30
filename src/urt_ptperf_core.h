#ifndef URT_PTPERF_CORE_H
#define URT_PTPERF_CORE_H
/* V5.2 Path Tracing performance control (PT: Scene Range): pure logic (no Win32,
 * no CRT), host-testable.
 *
 * Evidence for every engine variable used here: private research builds R2-R5 (docs/V5.2_PT_RESEARCH.md there)
 * (Patch 5.01 static analysis). The control defaults to 0 = Native: nothing is
 * written, V5.1.0 behaviour. Engine values are owned ONLY in Path Tracing with
 * Master On and released with compare-and-swap otherwise. No shader and no game
 * code is changed: the PT library stays the V5.1.0 one (identity f1ad5c37), so
 * Orange Hair Fix RT+PT keeps recognising it.
 * V5.2 r2: "PT: Small Object Detail" was removed after RTX 4080 testing showed no
 * FPS change (its four PT-path code bytes are gone too); a saved PTObjectDetail
 * value is never read. */
#include <stdint.h>

/* Menu option indices (group RefinedTracingCR01). */
enum {
  URT_PX_SCENE_RANGE = 0,   /* PTSceneRange: 0 Native(400 m), 1 300, 2 250, 3 200, 4 150 m */
  URT_PX_COUNT
};
extern const char *const urt_px_keys[URT_PX_COUNT];
extern const uint32_t urt_px_option_count[URT_PX_COUNT];
typedef struct { uint32_t v[URT_PX_COUNT]; } UrtPxPrefs;
int urt_px_prefs_valid(const UrtPxPrefs *p);

/* ---- owned engine fields (proximity-TLAS box) --------------------------- */
enum {
  URT_PF_RANGE = 0,   /* ProximityTLASSize            (float, native 400)  */
  URT_PF_RANGE_EXT,   /* ProximityExtendedTLASSize    (float, native 800)  */
  URT_PF_COUNT
};
typedef struct {
  uint32_t *slot[URT_PF_COUNT];
  uint32_t original[URT_PF_COUNT], applied[URT_PF_COUNT];
  uint32_t mask, conflicts;
} UrtPfOwner;
void urt_pf_init(UrtPfOwner *o);
int urt_pf_native_plausible(unsigned field, uint32_t native);
int urt_pf_target(const UrtPxPrefs *p, unsigned field, uint32_t native, uint32_t *out);
/* eligible = Master On && Path Tracing && preferences valid. */
void urt_pf_update(UrtPfOwner *o, const UrtPxPrefs *p, unsigned eligible);
void urt_pf_release(UrtPfOwner *o);
#endif
