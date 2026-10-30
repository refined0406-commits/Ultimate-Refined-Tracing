#ifndef URT_FOLIAGE501_H
#define URT_FOLIAGE501_H
#include <stdint.h>
#include <stddef.h>
/* V5.3.0 R8 EXPERIMENTAL "RT/PT: Solid Distant Foliage" (Patch 5.01).
 *
 * What the game does (verified, see docs/v530/V5.3.0_R8_FOLIAGE_INVESTIGATION.md):
 *  - Every Path Tracing TraceRay (PTRGSFinal, 16 sites) uses the GBuffer payload, ray-contribution 0, multiplier 4,
 *    so a ray that meets a SpeedTree leaf card runs the hit group AHS_GBuffer_Trees = AnyHitTrees.
 *    Normal RT reflection / GI rays use the same hit groups.
 *  - AnyHitTrees fetches the triangle's UVs (4 buffer loads), samples the leaf texture's alpha
 *    (mip min(10 x hit distance, CB19.z)) and calls IgnoreHit for a fully transparent texel (alpha < 1e-4).
 *    A shadow ray (ACCEPT_FIRST_HIT) through a canopy therefore runs this shader for every leaf card it
 *    crosses until one texel is opaque; a bounce ray runs it for every card it crosses before the nearest hit.
 *  - The library is registered from the static descriptor AHS_GBuffer_Trees (RVA 0x322c790); the generic
 *    loader (0x500040) reads the blob pointer at +0x28 (RVA 0x322c7b8: -> [u64 size][DXBC]) when ray tracing
 *    builds its state objects.
 * What this changes: one comparison. A leaf texel is skipped only if it is transparent AND the hit is within
 * the chosen distance of the ray origin; beyond that distance every leaf card counts as solid, so the ray
 * stops at the first distant leaf instead of running the any-hit shader through the canopy.
 * The modified library is built at startup with the game's own dxcompiler.dll / dxil.dll (disassemble ->
 * one text edit -> assemble -> original container parts -> validate + sign), exactly like URT's PT library,
 * and published by swapping that one descriptor pointer before ray tracing initialises. Session-fixed
 * (applies at the next start), guarded by the startup guard, never written after ray tracing started.
 * The Path Tracing ray-generation library (and Orange Hair Fix's patches of it) is not touched. */
#define URT_FOL_ENTRY_RVA   0x322c790u   /* descriptor AHS_GBuffer_Trees */
#define URT_FOL_FIELD_RVA   0x322c7b8u   /* +0x28: blob header pointer   */
#define URT_FOL_HEADER_RVA  0x339a620u   /* [u64 size][DXBC]             */
#define URT_FOL_NATIVE_SIZE 0x11a8u      /* 4520-byte container          */
/* Pure text edit of a DXC disassembly (also used by the host tests on the LLVM 18 text).
 * dist_index 1..4 = 30 m, 20 m, 10 m, 5 m. Returns 1 and the patched text, or 0 if any anchor is not unique. */
int urt_fol_patch_ir(const char *src, uint32_t n, unsigned dist_index, char *out, uint32_t cap, uint32_t *outn);
/* Read-only identity check of the descriptor, its strings and the native container (digest included). */
int urt_fol_verify(const uint8_t *base, uint32_t image);
#if defined(_WIN32)
#define URT_FOL_CC __stdcall
#else
#define URT_FOL_CC
#endif
typedef long (URT_FOL_CC *UrtDxcCreate)(const void *, const void *, void **);
/* Build the validated, signed container (Windows: game DXC). Output buffer = [u64 size][DXBC], never freed. */
int urt_fol_build(const uint8_t *base, UrtDxcCreate create, unsigned dist_index, uint8_t **hdr_out);
/* Publish: compare-and-swap the descriptor pointer from the native header to hdr. writer makes the 8 bytes
 * writable/restores protection (Windows: VirtualProtect); returns 1 when the swap is in place. */
typedef int (*UrtFolProtect)(void *at, size_t n, int make_writable);
int urt_fol_publish(uint8_t *base, const uint8_t *hdr, UrtFolProtect prot);
#endif
