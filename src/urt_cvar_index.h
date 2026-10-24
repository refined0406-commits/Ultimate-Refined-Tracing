#ifndef URT_CVAR_INDEX_H
#define URT_CVAR_INDEX_H
#include <stdint.h>
#include <stddef.h>
/* One-pass index of Witcher 3 engine CVar registry descriptors.
 *
 * A registry descriptor is a qword-aligned record inside a writable image
 * section whose first qword points at the CVar name (a NUL-terminated
 * printable string in a readable section) and whose qword at +16 points at
 * the writable backing storage of the value. This is exactly the shape the
 * V4.x resolver (find_cvar) accepted; V4.x re-scanned the whole image for
 * every requested name, this module scans writable sections once.
 *
 * Pure C: no Win32, no CRT. The caller supplies entry storage. */
typedef struct {
  uint32_t name_rva;   /* RVA of the name string the descriptor points at */
  uint32_t value_rva;  /* RVA of the writable value storage (+16 pointer) */
  uint32_t desc_rva;   /* RVA of the descriptor itself                     */
  uint32_t hash;       /* FNV-1a of the name, for fast rejection           */
} UrtCvarEntry;

typedef struct {
  uint8_t *base;
  uint32_t image_size;
  UrtCvarEntry *entries;
  uint32_t count, capacity;
  uint32_t overflow;   /* 1 if more descriptors existed than capacity: lookups fail closed */
} UrtCvarIndex;

/* Returns 1 on success. Fails closed on malformed headers or overflow. */
int urt_cvar_index_build(UrtCvarIndex *ix, uint8_t *base, uint32_t image_size,
                         UrtCvarEntry *storage, uint32_t capacity);
/* Same contract as V4.x find_cvar: the unique writable value storage for an
 * exact name, or NULL when absent or ambiguous (two different storages). */
uint32_t *urt_cvar_lookup(const UrtCvarIndex *ix, const char *name);
/* Category-scoped lookup: the string immediately following the name string
 * (after its NUL padding) must equal `category`, and the descriptor qword at
 * +8 must equal `kind` when kind!=0. Used where two registry groups register
 * the same short name (e.g. RTXGI MaxProbesShadedPerFrame). */
uint32_t *urt_cvar_lookup_scoped(const UrtCvarIndex *ix, const char *name,
                                 const char *category, uint64_t kind,
                                 uint32_t *desc_rva_out);
/* Descriptor default (u32 at +24) for a resolved unique name, for identity checks. */
int urt_cvar_descriptor(const UrtCvarIndex *ix, const char *name, uint32_t *desc_rva_out);
/* V5.1: lookup by the descriptor's real category (+48), flags (+8) and default (+24). */
uint32_t *urt_cvar_lookup_cat(const UrtCvarIndex *ix, const char *name, const char *category,
                              uint64_t flags, uint32_t default_lo, uint32_t *desc_out);
uint32_t urt_fnv1a(const char *s);
#endif
