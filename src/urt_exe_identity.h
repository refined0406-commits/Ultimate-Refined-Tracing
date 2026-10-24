#ifndef URT_EXE_IDENTITY_H
#define URT_EXE_IDENTITY_H
#include <stdint.h>
#include <stddef.h>
/* Executable identity tiers (V5).
 *
 *  URT_EXE_EXACT          whole-file SHA-256 equals the protected Patch 5.01 file.
 *  URT_EXE_CODE_IDENTICAL whole file differs, but the "code identity" hash is
 *                         equal: identical section layout, entry point, image size
 *                         and byte-identical raw contents of every section except
 *                         .rsrc (icons/version info). Covers re-signed/repackaged
 *                         builds; every byte URT reads or patches is the same.
 *  URT_EXE_UNKNOWN        anything else: code patches and the PT shader stay off,
 *                         only name-resolved CVar features may run (adaptive mode).
 *
 * The planning functions are pure (no Win32) so they are host-testable. */
enum { URT_EXE_UNKNOWN=0, URT_EXE_CODE_IDENTICAL=1, URT_EXE_EXACT=2 };

#define URT_CODEID_MAX_RANGES 96
typedef struct {
  uint32_t count;
  uint32_t start[URT_CODEID_MAX_RANGES];  /* file offsets, ascending, non-overlapping */
  uint32_t length[URT_CODEID_MAX_RANGES];
  uint8_t  layout[16+2+96*24];            /* serialized layout record hashed first */
  uint32_t layout_len;
} UrtCodeIdPlan;

/* Parse PE headers (first `n` bytes of the file) and plan the code-identity
 * hash. Returns 0 for anything malformed or unusual (fail closed). */
int urt_codeid_plan(const uint8_t *header, size_t n, uint64_t file_size, UrtCodeIdPlan *plan);
/* Bytes of the chunk [off, off+len) that belong to planned ranges are reported
 * through `feed` in ascending order. */
typedef int (*UrtFeed)(void *ctx, const uint8_t *p, uint32_t n);
int urt_codeid_feed_chunk(const UrtCodeIdPlan *plan, uint64_t off, const uint8_t *chunk,
                          uint32_t len, UrtFeed feed, void *ctx);

/* Cached, process-lifetime identity of the running executable (Windows only). */
int urt_exe_identity(void);
const char *urt_exe_identity_name(int tier);
#endif
