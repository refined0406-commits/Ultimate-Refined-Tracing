#ifndef URT_RTPERF_CORE_H
#define URT_RTPERF_CORE_H
/* V5.1 Normal RT performance controls: pure logic (no Win32, no CRT), host-testable.
 *
 * Evidence for every engine variable used here is in docs/V5.1_RT_RESEARCH.md
 * (Patch 5.01 static analysis: registry descriptors, verified read sites).
 * Every control defaults to 0 = Native: nothing is written, V5.0.0 behaviour.
 * Owned values apply ONLY in Normal RT with Master On; anything else releases
 * them with compare-and-swap (a value someone else changed is never clobbered). */
#include <stdint.h>

/* Menu option indices (UltimateRefinedTracing menu, group RefinedTracingCR01).
 * rc3: the three controls kept after gameplay testing. rc1 Probe Ray Distance,
 * Reflection Texture Detail, RTAO Foliage Detail and rc2 Console Probe Grid were
 * removed; their saved values are never read. */
enum {
  URT_RP_REFL_LIGHTS = 0,  /* RTReflLocalLights:     0 Native, 1 Off                        */
  URT_RP_SHADOW_SLOTS,     /* RTShadowLocalLights:   0 Native, 1 6, 2 4, 3 2, 4 Off         */
  URT_RP_SHADOW_FOLIAGE,   /* RTShadowFoliageDetail: 0 Native, 1 Reduced, 2 Low, 3 Very Low */
  URT_RP_COUNT
};
extern const char *const urt_rp_keys[URT_RP_COUNT];
extern const uint32_t urt_rp_option_count[URT_RP_COUNT];

typedef struct { uint32_t v[URT_RP_COUNT]; } UrtRpPrefs;
/* 1 when every index is within its option range. */
int urt_rp_prefs_valid(const UrtRpPrefs *p);

/* ---- owned engine fields ------------------------------------------------ */
enum {
  URT_RF_REFL_LOCAL = 0,      /* bool (low byte), native 1 */
  URT_RF_SHADOW_SLOTS,        /* int 0..8, native 8        */
  URT_RF_SHADOW_ALPHA_MIP,    /* float mip, native 2       */
  URT_RF_COUNT
};
typedef struct {
  uint32_t *slot[URT_RF_COUNT];
  uint32_t original[URT_RF_COUNT], applied[URT_RF_COUNT];
  uint32_t mask;          /* bit i: field i currently owned (slot holds applied[i]) */
  uint32_t conflicts;     /* count of foreign writes seen (diagnostics only)       */
} UrtRfOwner;
void urt_rf_init(UrtRfOwner *o);
/* Desired raw 32-bit target for each field from the preferences and the field's
 * current native value. Returns 0 (no ownership) when Native or when the target
 * would not reduce cost (never raises quality cost above native). */
int urt_rf_target(const UrtRpPrefs *p, unsigned field, uint32_t native, uint32_t *out);
/* Plausibility of a learned native value (refuse to own corrupted/foreign state). */
int urt_rf_native_plausible(unsigned field, uint32_t native);
/* One worker tick. eligible = Master On && Normal RT && prefs valid. */
void urt_rf_update(UrtRfOwner *o, const UrtRpPrefs *p, unsigned eligible);
/* Release every owned field with CAS (never clobbers a foreign value). */
void urt_rf_release(UrtRfOwner *o);

#endif
