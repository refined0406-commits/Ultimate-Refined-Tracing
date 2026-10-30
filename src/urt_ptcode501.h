#ifndef URT_PTCODE501_H
#define URT_PTCODE501_H
#include <stdint.h>
/* V5.3.0 R7 EXPERIMENTAL: two Path Tracing workload limits that Patch 5.01 hard-codes (no engine variable).
 * Both are single-byte / single-immediate code edits at fingerprinted sites of the exact Patch 5.01
 * executable, written only while the control is chosen and restored byte for byte otherwise.
 *
 * 1. PT light limit. 0x1c1fdd0 copies the frame's PT light list (built by 0x1c717b0, at most 256 lights)
 *    into the PT light buffer and returns the count, which becomes PT constant +0x364. PTRGS* loops over
 *    every light at every path vertex (weighted reservoir selection, then one shadow ray for the chosen
 *    light). The count is min(list, 256): the 256 is the immediate of `mov edi,0x100` at 0x1c20018.
 *    URT replaces that immediate with 128/64/32/16. Every intermediate value of the two-byte write is a
 *    valid cap (the list never holds more than 256 lights and the buffer holds 256), so the edit is safe
 *    while the render thread runs.
 * 2. PT small-object culling. 0x1cf2a20 fills the proximity-TLAS gather filter. In Path Tracing it uses a
 *    constant 1e-4 for both the angular-size threshold and the "always include" size, so every object in
 *    the PT box is added to the TLAS; in Normal RT it uses ProximityTLASObjectAngleSize (0.1) and
 *    ProximityTLASBigObject (15). Four one-byte register-selection edits make the Path Tracing branch use
 *    the two engine variables as Normal RT does (the gather at 0x1cf4830 then skips objects smaller than
 *    AngleSize x distance unless their diagonal exceeds BigObject). URT writes ProximityTLASObjectAngleSize
 *    itself (PT scope, live row); this module only switches the branch while that row is owned. Every
 *    partial state of the four edits keeps the native behaviour or the chosen one.
 * The writer (VirtualProtect + FlushInstructionCache on Windows, plain store in host tests) is registered by
 * the runtime; without it nothing is ever written. */
typedef int (*UrtCodeWriter)(uint8_t *at, const uint8_t *bytes, unsigned len);
void urt_ptcode501_set_writer(UrtCodeWriter w);
/* verifies every site byte-exactly (fingerprints); returns a mask: 1 = light limit available, 2 = culling available */
unsigned urt_ptcode501_init(uint8_t *base, uint32_t image, int known_exe);
/* cap: 0 = native (256), else 1..255; cull: 0 = native branch, 1 = PT uses the engine's RT filter variables */
void urt_ptcode501_tick(unsigned cap, int cull);
void urt_ptcode501_shutdown(void);
typedef struct { unsigned available, cap, cull, faults, writes; } UrtPtCodeStats;
void urt_ptcode501_stats(UrtPtCodeStats *s);
#define URT_PTC_CAP_RVA 0x1c20019u            /* imm32 of mov edi,0x100 */
#define URT_PTC_CULL_RVA0 0x1cf2a6du          /* vmovaps xmm2,xmm1 (ModRM d1 -> d0)        */
#define URT_PTC_CULL_RVA1 0x1cf2a84u          /* vmovaps xmm0,xmm1 (ModRM c1 -> c0)        */
#define URT_PTC_CULL_RVA2 0x1cf2abbu          /* vmovaps xmm0,xmm1 (ModRM c1 -> c2)        */
#define URT_PTC_CULL_RVA3 0x1cf2aceu          /* je +4 -> je +0 (also reaches xmm1=xmm2)   */
#endif
