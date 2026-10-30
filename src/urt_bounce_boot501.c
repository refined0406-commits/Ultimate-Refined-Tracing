/* V5.3.0 R11: PT: Second Bounce Coverage - startup decision at ASI load (Windows; PT shader preparation time).
 * The PT library is prepared before the URT runtime starts, so this unit reads the one saved menu value itself
 * (Documents\The Witcher 3\dx12user.settings, [URTExtended] URTX_ExpPTSecondBounce, R12 FIX2 URTX_PTDiffuseCoverage and URTX_ExpPTSunShadowCoverage;
 * read only) and applies the
 * startup guard to it (UltimateRefinedTracing.startupguard, same file and rules as every restart row):
 * a start with the value set records it PENDING (applied from the first PT frame). A clean game exit records it CLEAN,
 * 300 PT frames rendered with the variant record it PROVEN (R12 FIX4). A session that ends without either is suspected:
 * the next start runs native once, the one after retries; a second interruption turns it off until Native / Auto. */
#include "urt_bounce_boot501.h"
#include "urt_startup_guard.h"
#include "cr01_release_log.h"
#include <stdint.h>
#define WIN __stdcall
#define IMP __declspec(dllimport)
typedef void *HANDLE; typedef void *HMODULE; typedef unsigned long DWORD; typedef int BOOL; typedef unsigned short WCHAR;
IMP HMODULE WIN LoadLibraryW(const WCHAR *);
IMP void *WIN GetProcAddress(HMODULE, const char *);
IMP DWORD WIN GetPrivateProfileStringW(const WCHAR *, const WCHAR *, const WCHAR *, WCHAR *, DWORD, const WCHAR *);
IMP HANDLE WIN CreateFileW(const WCHAR *, DWORD, DWORD, void *, DWORD, DWORD, HANDLE);
IMP BOOL WIN ReadFile(HANDLE, void *, DWORD, DWORD *, void *);
IMP BOOL WIN WriteFile(HANDLE, const void *, DWORD, DWORD *, void *);
IMP BOOL WIN CloseHandle(HANDLE);
#define INVALID_H ((HANDLE)(intptr_t)-1)
static WCHAR g_dir[1024], g_settings[1100], g_guard[1100];
static unsigned g_level_active, g_decided;
static unsigned wlen(const WCHAR *s) { unsigned n = 0; while (s[n]) n++; return n; }
static int wcat(WCHAR *d, unsigned cap, const WCHAR *s) {
  unsigned n = wlen(d), k = 0;
  while (s[k]) { if (n + 1 >= cap) return 0; d[n++] = s[k++]; }
  d[n] = 0; return 1;
}
static int paths(void) {
  static const WCHAR shell[] = {'s','h','e','l','l','3','2','.','d','l','l',0};
  static const WCHAR tail[] = {'\\','T','h','e',' ','W','i','t','c','h','e','r',' ','3',0};
  static const WCHAR set[] = {'\\','d','x','1','2','u','s','e','r','.','s','e','t','t','i','n','g','s',0};
  static const WCHAR grd[] = {'\\','U','l','t','i','m','a','t','e','R','e','f','i','n','e','d','T','r','a','c','i','n','g','.',
                              's','t','a','r','t','u','p','g','u','a','r','d',0};
  HMODULE sh = LoadLibraryW(shell);
  if (!sh) return 0;
  typedef long (WIN *GETDOC)(void *, int, void *, DWORD, WCHAR *);
  GETDOC getdoc = (GETDOC)(void *)GetProcAddress(sh, "SHGetFolderPathW");
  if (!getdoc || getdoc(0, 5, 0, 0, g_dir) < 0 || !wcat(g_dir, 1024, tail)) return 0;
  g_settings[0] = 0; g_guard[0] = 0;
  return wcat(g_settings, 1100, g_dir) && wcat(g_settings, 1100, set) && wcat(g_guard, 1100, g_dir) && wcat(g_guard, 1100, grd);
}
static int guard_read(char *b, unsigned cap) {
  HANDLE h = CreateFileW(g_guard, 0x80000000ul, 1ul, 0, 3ul /* OPEN_EXISTING */, 0x80ul, 0);
  if (h == INVALID_H || !h) return 0;
  DWORD r = 0; BOOL ok = ReadFile(h, b, cap, &r, 0); CloseHandle(h);
  return ok ? (int)r : 0;
}
static int guard_write(const char *b, unsigned n) {
  HANDLE h = CreateFileW(g_guard, 0x40000000ul, 0ul, 0, 2ul /* CREATE_ALWAYS */, 0x80ul, 0);
  if (h == INVALID_H || !h) return 0;
  DWORD w = 0; BOOL ok = WriteFile(h, b, n, &w, 0); CloseHandle(h);
  return ok && w == n;
}
/* R11 module 1 (R12 FIX3: retired option, level always 0), R12 module 2 and R12 FIX2 module 3 share one variant library and
 * one decision; each option keeps its own guard record. */
/* R12 FIX2: First Bounce Diffuse Coverage moved to the PT section under a new key, URTX_PTDiffuseCoverage (saved 0 = Native /
 * Auto, 1 = 50 %). The game stores an OPTIONS row as its option index, so the R12 key (saved 1 = the retired 75 %) is no longer read. */
static const char *const kKey[4] = {"URTX_ExpPTSecondBounce", "URTX_PTDiffuseCoverage", "URTX_ExpPTSunShadowCoverage", "URTX_ExpPTLocalShadowCoverage"};   /* R12 FIX3: + module 4 */
static const char kOldDiffuse[] = "URTX_ExpPTDiffuseCoverage";
static void log_level(const char *pre, const char *key, unsigned v, const char *post) {
  char b[400]; unsigned n = 0;
  for (const char *q = pre; *q && n < 200; q++) b[n++] = *q;
  for (const char *q = key; *q && n < 260; q++) b[n++] = *q;
  if (key[0]) b[n++] = '=';
  if (key[0] || pre[0]) b[n++] = (char)('0' + (v % 10u));
  for (const char *q = post; *q && n < 398; q++) b[n++] = *q;
  b[n] = 0; cr01_release_log_event(b);
}
static unsigned read_level(const WCHAR *key) {
  static const WCHAR sect[] = {'U','R','T','E','x','t','e','n','d','e','d',0};
  WCHAR v[8] = {0};
  DWORD c = GetPrivateProfileStringW(sect, key, (const WCHAR *)0, v, 8, g_settings);
  unsigned level = (c == 1 && v[0] >= '0' && v[0] <= '9') ? (unsigned)(v[0] - '0') : 0u;
  return level > 2u ? 0u : level;                            /* out of range: Native */
}
/* returns bits 0-1 = Second Bounce Coverage level, bits 2-3 = First Bounce Diffuse Coverage level (0 = build nothing) */
/* R12 FIX2: the guard file paths are prepared outside DllMain (no LoadLibrary at process exit) */
int urt_bc_prepare_paths(void) { return g_guard[0] ? 1 : paths(); }
unsigned urt_bc_startup_level(void) {
  static const WCHAR k0[] = {'U','R','T','X','_','E','x','p','P','T','S','e','c','o','n','d','B','o','u','n','c','e',0};
  static const WCHAR k1[] = {'U','R','T','X','_','P','T','D','i','f','f','u','s','e','C','o','v','e','r','a','g','e',0};
  static const WCHAR k1old[] = {'U','R','T','X','_','E','x','p','P','T','D','i','f','f','u','s','e','C','o','v','e','r','a','g','e',0};
  static const WCHAR k2[] = {'U','R','T','X','_','E','x','p','P','T','S','u','n','S','h','a','d','o','w','C','o','v','e','r','a','g','e',0};
  static const WCHAR k3[] = {'U','R','T','X','_','E','x','p','P','T','L','o','c','a','l','S','h','a','d','o','w','C','o','v','e','r','a','g','e',0};
  g_decided = 1;
  if (!urt_bc_prepare_paths()) return 0;
  unsigned lv[4] = {read_level(k0), read_level(k1), read_level(k2), read_level(k3)}, out = 0;
  /* R12 FIX3: PT: Second Bounce Coverage is retired. A saved value builds nothing (level 0: module 1, the shared base of
   * modules 2 and 3, keeps the native bounce count); deciding it as 0 below clears its guard record. */
  if (lv[0]) {
    lv[0] = 0;
    cr01_release_log_event("URT: PT: Second Bounce Coverage (V5.3.0 test builds only) is retired; its saved value is ignored (no effect on any other setting)\r\n");
  }
  if (lv[1] > 1u) lv[1] = 0;                                 /* diffuse coverage: Native / Auto or 50 % (saved 1) only */
  if (lv[2] > 1u) lv[2] = 0;                                 /* module 3: Native / Auto or 50 % (saved 1) only */
  if (lv[3] > 1u) lv[3] = 0;                                 /* module 4: Native / Auto or 50 % (saved 1) only */
  /* R12 FIX2: the R12 key is retired. Whatever it holds (75 % or 50 %) builds nothing; its guard record is cleared. */
  if (read_level(k1old))
    cr01_release_log_event("URT: PT: First Bounce Diffuse Coverage: a value saved by a V5.3.0 test build (URTX_ExpPTDiffuseCoverage) is ignored; choose 50% in PT: First Bounce Diffuse Coverage (Path Tracing section)\r\n");
  char t[1024]; int n = guard_read(t, sizeof t - 1u);
  UrtSgFile g; urt_sg_parse(t, n > 0 ? (unsigned)n : 0u, &g);
  { int nf = 0; (void)urt_sg_decide(&g, kOldDiffuse, 0, &nf); }
  int apply[4] = {0, 0, 0, 0}, ev[4] = {0, 0, 0, 0};
  for (unsigned i = 0; i < 4; i++) apply[i] = urt_sg_decide_ex(&g, kKey[i], (uint8_t)lv[i], &ev[i]);
  char o[1024]; unsigned on = urt_sg_format(&g, o, sizeof o);
  if ((lv[0] || lv[1] || lv[2] || lv[3] || n > 0) && !(on && guard_write(o, on))) {
    if (lv[0] || lv[1] || lv[2] || lv[3]) cr01_release_log_event("URT: startup guard file could not be written; the PT shader coverage options stay native this session\r\n");
    return 0;
  }
  static const char *const req[4] = {"URT: PT: Second Bounce Coverage requested (level ", "URT: PT: First Bounce Diffuse Coverage 50% requested (setting ",
                                      "URT: PT: Bounce Sun Shadow Coverage 50% requested (setting ", "URT: PT: Bounce Local Light Shadow Coverage 50% requested (setting "};
  for (unsigned i = 0; i < 4; i++) {
    if (!lv[i]) continue;
    log_level("URT: startup guard: ", kKey[i], lv[i], urt_sg_event_text(ev[i]));
    if (!apply[i]) continue;
    urt_sg_session_note(kKey[i]);
    log_level(req[i], "", lv[i],
              "): building URT's PT library variant (coverage edits + Orange Hair Fix hair patch)\r\n");
    out |= (i == 1u ? 2u : lv[i]) << (2u * i);                 /* diffuse: setting 1 = shader level 2 (50 %, as R12 FIX1) */
  }
  return out;
}
/* R12 FIX2: clean game exit (DLL_PROCESS_DETACH during normal process termination). Only file I/O; the records of the
 * values this session applied become CLEAN, so an ordinary quit or restart is never counted as a crash. */
void urt_bc_clean_exit(void) {
  const char *const *keys; unsigned nk = urt_sg_session_keys(&keys);
  if (!nk || !g_guard[0]) return;
  char t[1024]; int n = guard_read(t, sizeof t - 1u);
  if (n <= 0) return;
  UrtSgFile g; urt_sg_parse(t, (unsigned)n, &g);
  if (!urt_sg_mark_clean(&g, keys, nk)) return;
  char o[1024]; unsigned on = urt_sg_format(&g, o, sizeof o);
  if (on && guard_write(o, on)) cr01_release_log_event("URT: clean game exit: the settings under test of this session are recorded as safe (they apply at the next start)\r\n");
}
static void put_num(char *b, unsigned *n, unsigned x) { char d[12]; unsigned dn = 0; do { d[dn++] = (char)('0' + x % 10u); x /= 10u; } while (x && dn < 11u); while (dn) b[(*n)++] = d[--dn]; }
void urt_bc_note_result(unsigned level, int ok, unsigned bytes, const char *why) {
  if (ok) {
    g_level_active = level;
    static const char *const pct[4][3] = {{"", "50%", "25%"}, {"", "75%", "50%"}, {"", "50%", ""}, {"", "50%", ""}};
    for (unsigned i = 0; i < 4; i++) {
      unsigned l = (level >> (2u * i)) & 3u;
      if (!l || l > 2u) continue;
      char b[520]; unsigned n = 0;
      const char *a = i == 3 ? "URT: PT: Bounce Local Light Shadow Coverage " : i == 2 ? "URT: PT: Bounce Sun Shadow Coverage " : i ? "URT: PT: First Bounce Diffuse Coverage " : "URT: PT: Second Bounce Coverage ";
      for (const char *q = a; *q; q++) b[n++] = *q;
      for (const char *q = pct[i][l]; *q; q++) b[n++] = *q;
      for (const char *q = " active: variant PT library validated ("; *q; q++) b[n++] = *q;
      put_num(b, &n, bytes);
      for (const char *q = i == 3 ? " bytes) and installed; re-check of the signed shader: the local-light shadow ray of both shading paths at bounce hits runs for half of the pixels per frame (the skipped light evaluation included) and that light is weighted, in both PT exports\r\n"
                         : i == 2 ? " bytes) and installed; re-check of the signed shader: the three sun / moon shadow rays at bounce hits run for half of the pixels per frame and their visibility is weighted, in both PT exports\r\n"
                         : i ? " bytes) and installed; re-check of the signed shader: dropped diffuse samples exit the bounce loop before tracing and the kept diffuse output is weighted, in both PT exports; Orange Hair Fix's PT hair patch is built in (its Path A)\r\n"
                             : " bytes) and installed; re-check of the signed shader: per-pixel bounce limit in both PT exports; Orange Hair Fix's PT hair patch is built in (its Path A)\r\n"; *q && n < 516u; q++) b[n++] = *q;
      b[n] = 0; cr01_release_log_event(b);
    }
    return;
  }
  /* the variant could not be built: nothing changed, so the guard records are dropped again (no false crash later) */
  char t[1024]; int n = guard_read(t, sizeof t - 1u);
  UrtSgFile g; urt_sg_parse(t, n > 0 ? (unsigned)n : 0u, &g);
  for (unsigned i = 0; i < 4; i++) if ((level >> (2u * i)) & 3u) { int nf = 0; (void)urt_sg_decide(&g, kKey[i], 0, &nf); }
  urt_sg_session_clear();
  char o[1024]; unsigned on = urt_sg_format(&g, o, sizeof o);
  if (on) (void)guard_write(o, on);
  {
    char b[640]; unsigned k = 0;
    for (const char *q = "URT: PT shader coverage variant NOT built - stage "; *q; q++) b[k++] = *q;
    for (const char *q = why && why[0] ? why : "unknown"; *q && k < 600u; q++) b[k++] = *q;
    for (const char *q = "; URT's normal PT library is used this session (nothing else changed)\r\n"; *q && k < 638u; q++) b[k++] = *q;
    b[k] = 0; cr01_release_log_event(b);
  }
}
unsigned urt_bc_active_level(void) { return g_level_active; }
unsigned urt_bc_decided(void) { return g_decided; }
