#ifndef CR01_SETTINGS_STORE_WINDOWS_H
#define CR01_SETTINGS_STORE_WINDOWS_H
#include "cr01_settings_store.h"
#include <stdint.h>
/* Private settings migration: canonical new path, old path is read-only.
 * An absent new file may import the old file exactly once after codec validation;
 * neither the old private file nor game-owned settings are ever deleted. */
typedef struct {
 uint16_t final_path[1024];   /* UltimateRefinedTracing.settings (V5)          */
 uint16_t temp_path[1024];    /* .tmp, written + flushed before replace         */
 uint16_t legacy_path[1024];  /* RefinedTracing.settings (V4.x), read-only     */
 uint32_t initialized;
 uint32_t legacy_loaded;      /* nonzero: the last load came from a legacy file */
 uint16_t legacy2_path[1024]; /* RefinedTracingCR01.settings (early V4), read-only */
 uint16_t backup_path[1024];  /* .bak: last known-good copy kept by ReplaceFileW */
 uint16_t corrupt_path[1024]; /* .corrupt: invalid primary moved aside (diagnostics) */
 uint32_t primary_bytes_read; /* primary was read successfully (content may be invalid) */
 uint32_t commit_blocked;     /* primary unreadable (I/O): never overwrite it this run */
} CR01WindowsStore;
int cr01_store_windows_init(CR01WindowsStore*,CR01PrivateStore*,const uint16_t*witcher_settings_dir);
#endif
