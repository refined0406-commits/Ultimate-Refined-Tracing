#ifndef URT_INI_H
#define URT_INI_H
#include <stdint.h>
#include <stddef.h>
/* Coherent in-memory snapshot of ONE section of an INI file (pure C).
 *
 * V4.3.1 read each of ~45 keys with its own GetPrivateProfileStringW call,
 * i.e. ~45 open/parse cycles of dx12user.settings per menu event, each able to
 * observe a different moment of a concurrent game save. V5 reads the file
 * once per event and answers every key from that single snapshot.
 *
 * Lookup semantics follow GetPrivateProfileString for the ASCII content URT
 * uses: section and key names compare case-insensitively after trimming
 * spaces/tabs; the first matching section and the first matching key win;
 * the value is trimmed and one pair of matching surrounding quotes removed;
 * results are truncated to cap-1 characters. The runtime additionally
 * cross-checks this against the real API at startup (see runtime). */
#define URT_INI_MAX_KEYS 192
#define URT_INI_MAX_TEXT 16384
typedef struct {
  uint32_t count, valid, text_used;
  uint16_t key_off[URT_INI_MAX_KEYS], key_len[URT_INI_MAX_KEYS];
  uint16_t val_off[URT_INI_MAX_KEYS], val_len[URT_INI_MAX_KEYS];
  char text[URT_INI_MAX_TEXT];
} UrtIniSection;

/* Parse raw file bytes (ASCII/UTF-8, optional UTF-8 BOM, or UTF-16LE with BOM).
 * Returns 1 when parsed (the section may be absent: count==0), 0 when the
 * buffer is unusable (e.g. non-ASCII UTF-16 content inside the section or the
 * section exceeds the fixed capacity) - callers then use the Windows API. */
int urt_ini_parse(const uint8_t *data, size_t n, const char *section, UrtIniSection *out);
/* GetPrivateProfileStringW-like result: number of characters copied (0 when absent). */
unsigned urt_ini_get(const UrtIniSection *s, const char *key, char *out, unsigned cap);
#endif
