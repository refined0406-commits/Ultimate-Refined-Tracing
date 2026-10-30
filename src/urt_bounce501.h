#ifndef URT_BOUNCE501_H
#define URT_BOUNCE501_H
#include <stdint.h>
#include <stddef.h>
/* V5.3.0 R11 PT Bounce Optimisation Layer (Patch 5.01), module 1: "PT: Second Bounce Coverage".
 * See docs/v530/V5.3.0_R11_BOUNCE_LAYER.md.
 *
 * What the game does (STATIC, PTRGSFinal / PTRGSWithSERFinal):
 *   the path-tracing ray generation runs one loop iteration per bounce (counter from 0, exit test
 *   "counter + 1 < bounces"), and the bounce count is the select(hair, 4, 2) that URT's PT library already turns
 *   into select(hair, %rpt_nh, %rpt_nb) (CameraCB row 33 / 56, written by URT's serializer gate).
 *   Each extra iteration traces one more bounce ray, shades its hit and traces its light/shadow rays.
 *   Owner-measured cost of the second bounce at Kaer Morhen: 2 bounces 47-48 FPS, 1 bounce 57-58 FPS.
 * What the module changes: per pixel, the non-hair bounce limit becomes
 *   selected ? %rpt_nb : min(%rpt_nb, 1)
 *   with "selected" = ((x + y + y/2 + frame) & (2^k - 1)) == 0, k = coverage level carried in bits 16-17 of the
 *   same CameraCB word (0 = all pixels = unchanged behaviour). Unselected pixels render exactly what Geometry
 *   Bounces 1 renders for them; selected pixels keep every bounce; the pattern moves every frame so DLSS Ray
 *   Reconstruction's temporal accumulation sees every pixel's second bounce over 2 (k=1) or 4 (k=2) frames.
 *   Hair keeps its own bounce count (Orange Hair Fix territory).
 * Orange Hair Fix compatibility (its documented "Path A"): the variant library also carries Orange Hair Fix's
 *   own PT hair patch (ported 1:1 from its patch_pt.py, RELEASE build), so hair stays fixed although Orange Hair
 *   Fix no longer recognises the library by hash. Without the hair mod its menu value is 0 = off (no change).
 * The variant is built only when the option is not Native at startup; Native keeps URT's unchanged PT library. */
#define URT_BC_LEVEL_MAX 2u
/* text passes over a DXC disassembly of URT's PT library (both exports); 1 = done, 0 = an anchor was not unique */
int urt_bc_patch_coverage(const char *src, uint32_t n, char *out, uint32_t cap, uint32_t *outn);
/* R12 module 2, "PT: First Bounce Diffuse Coverage": on iteration 0, a pixel whose sample took the diffuse lobe and
 * falls outside this frame's pattern (level 1: 3 of 4 pixels kept, 2x2 quad, one dropped per frame in turn; level 2:
 * checkerboard alternating per frame) leaves the loop before anything is traced, exactly as the game's own invalid-
 * normal exit does (zero radiance, hit distance 0). Kept pixels' diffuse output is scaled by 4/3 or 2 (unbiased per
 * pixel over 4 / 2 frames). Level in bits 18-19 of the CameraCB bounce word; 0 = no change. Runs after module 1. */
int urt_dc_patch_diffuse(const char *src, uint32_t n, char *out, uint32_t cap, uint32_t *outn);
/* R12 FIX2 module 3, "PT: Bounce Sun Shadow Coverage": the three directional (sun / moon) shadow rays of each PT export,
 * traced at bounce hits, run for half of the pixels per frame (instance mask 0 = no traversal for the others) with their
 * visibility weighted x2 (0 when not traced). Level in bits 20-21. Runs after module 2. */
int urt_sc_patch_shadow(const char *src, uint32_t n, char *out, uint32_t cap, uint32_t *outn);
/* R12 FIX3 module 4, "PT: Bounce Local Light Shadow Coverage": the local-light (point / spot) shadow ray of each shading
 * path at bounce hits runs for half of the pixels per frame (mask 0 = no traversal for the others; at the branch site the
 * light evaluation is skipped too); traced pixels weight that light x2. Level in bits 22-23. Runs after module 3. */
int urt_lc_patch_local(const char *src, uint32_t n, char *out, uint32_t cap, uint32_t *outn);
int urt_ohf_bake(const char *src, uint32_t n, char *out, uint32_t cap, uint32_t *outn, unsigned *sites);
#if defined(_WIN32)
#define URT_BC_CC __stdcall
#else
#define URT_BC_CC
#endif
typedef long (URT_BC_CC *UrtBcDxcCreate)(const void *, const void *, void **);
/* DXC: disassemble URT's validated library, both passes, assemble, keep every other container part, validate +
 * sign, disassemble again and check both edits in both exports. *out is allocated (kept for the process). */
/* R12 fix: the exact stage and reason of the last failure of the passes or urt_bc_build_variant (empty after success) */
const char *urt_bc_diag(void);
/* R12 FIX3: as urt_bc_build_variant; module 4 is added when the packed level's bits 6-7 are set */
int urt_bc_build_variant_lv(const uint8_t *orig, uint32_t on, UrtBcDxcCreate create, uint8_t **out, uint32_t *outn, unsigned level);
int urt_bc_build_variant(const uint8_t *urt_lib, uint32_t n, UrtBcDxcCreate create, uint8_t **out, uint32_t *outn);
#endif
