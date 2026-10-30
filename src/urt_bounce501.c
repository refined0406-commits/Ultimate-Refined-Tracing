/* V5.3.0 R11 PT Bounce Optimisation Layer, module 1 (Patch 5.01). See urt_bounce501.h. */
#include "urt_bounce501.h"
#if defined(_WIN32)
#define BWIN __stdcall
__declspec(dllimport) void *BWIN VirtualAlloc(void *, size_t, unsigned long, unsigned long);
__declspec(dllimport) int BWIN VirtualFree(void *, size_t, unsigned long);
static void *b_alloc(size_t n) { return n ? VirtualAlloc(0, n, 0x1000 | 0x2000, 0x04) : 0; }
static void b_free(void *p) { if (p) (void)VirtualFree(p, 0, 0x8000); }
#else
#define BWIN
#include <stdlib.h>
static void *b_alloc(size_t n) { return n ? calloc(1, n) : 0; }
static void b_free(void *p) { free(p); }
#endif
#define NOPOS 0xffffffffu
/* R12 fix: the exact stage and reason of the last failure (logged by the ASI entry; one build per process) */
static char g_diag[480];
const char *urt_bc_diag(void) { return g_diag; }
static void dg_put(unsigned *k, const char *t) { while (t && *t && *k + 1 < sizeof g_diag) g_diag[(*k)++] = *t++; g_diag[*k] = 0; }
static void dg_num(unsigned *k, unsigned long v, int hex) {
  char d[24]; unsigned dn = 0, b = hex ? 16u : 10u;
  if (hex) dg_put(k, "0x");
  do { unsigned x = (unsigned)(v % b); d[dn++] = (char)(x < 10 ? '0' + x : 'a' + x - 10); v /= b; } while (v && dn < 20);
  while (dn && *k + 1 < sizeof g_diag) g_diag[(*k)++] = d[--dn];
  g_diag[*k] = 0;
}
/* "<stage>: <what> (urt_bounce501.c:<line>)" - the first failure is kept */
static int dg(const char *stage, const char *what, unsigned line) {
  if (g_diag[0]) return 0;
  unsigned k = 0; dg_put(&k, stage); dg_put(&k, ": "); dg_put(&k, what); dg_put(&k, " (urt_bounce501.c:"); dg_num(&k, line, 0); dg_put(&k, ")");
  return 0;
}
static uint32_t slen(const char *s) { uint32_t n = 0; while (s && s[n]) n++; return n; }
static int eq_at(const char *b, uint32_t n, uint32_t at, const char *q) {
  for (uint32_t i = 0; q[i]; i++) if (at + i >= n || b[at + i] != q[i]) return 0;
  return 1;
}
static uint32_t find(const char *b, uint32_t n, const char *q, uint32_t from, uint32_t to) {
  if (to > n) to = n;
  for (uint32_t i = from; i < to; i++) if (eq_at(b, n, i, q)) return i;
  return NOPOS;
}
static uint32_t count(const char *b, uint32_t n, const char *q, uint32_t from, uint32_t to, uint32_t *first) {
  uint32_t c = 0, p = from;
  *first = NOPOS;
  while ((p = find(b, n, q, p, to)) != NOPOS) { if (!c) *first = p; c++; p++; }
  return c;
}
static uint32_t line_start(const char *b, uint32_t p) { while (p && b[p - 1] != '\n') p--; return p; }
static uint32_t line_end(const char *b, uint32_t n, uint32_t p) { while (p < n && b[p] != '\n') p++; return p; }
static int put(char *out, uint32_t cap, uint32_t *u, const char *p, uint32_t n) {
  if (*u + n + 1 > cap) return 0;
  for (uint32_t i = 0; i < n; i++) out[(*u)++] = p[i];
  out[*u] = 0;
  return 1;
}
static int puts_(char *out, uint32_t cap, uint32_t *u, const char *s) { return put(out, cap, u, s, slen(s)); }
/* "%name" defined on the line [ls,le) ("  %name = ...") */
static int def_name(const char *b, uint32_t ls, uint32_t le, char *out, uint32_t cap) {
  uint32_t p = ls, k = 0;
  while (p < le && (b[p] == ' ' || b[p] == '\t')) p++;
  if (p >= le || b[p] != '%') return 0;
  while (p < le && b[p] != ' ' && k + 1 < cap) out[k++] = b[p++];
  out[k] = 0;
  return k > 1 && eq_at(b, le, p, " = ");
}
static const char *const k_fns[2] = {"define void @\"\\01?PTRGSFinal@@YAXXZ\"", "define void @\"\\01?PTRGSWithSERFinal@@YAXXZ\""};
static int fn_range(const char *s, uint32_t n, const char *fn, uint32_t *fs, uint32_t *fe) {
  uint32_t a = find(s, n, fn, 0, n);
  if (a == NOPOS || line_start(s, a) != a || find(s, n, fn, a + 1, n) != NOPOS) return 0;
  uint32_t e = find(s, n, "\n}", a, n);
  if (e == NOPOS) return 0;
  *fs = a; *fe = e + 2;
  return 1;
}
/* the unique line in [fs,fe) containing q; its defined name */
static int unique_def(const char *s, uint32_t n, uint32_t fs, uint32_t fe, const char *q, char *name, uint32_t cap) {
  uint32_t first;
  if (count(s, n, q, fs, fe, &first) != 1) return 0;
  uint32_t ls = line_start(s, first);
  return def_name(s, ls, line_end(s, n, first), name, cap);
}

/* ---------------- module 1: second-bounce coverage ---------------- */
static int cov_one(const char *s, uint32_t n, const char *fn, char *out, uint32_t cap, uint32_t *outn) {
  uint32_t fs, fe, first, u = 0;
  char cam[48], x[48], y[48];
  if (!fn_range(s, n, fn, &fs, &fe)) return dg("module 1 (second-bounce coverage) text pass", "anchor not found or not unique", __LINE__);
  if (!unique_def(s, n, fs, fe, "%dx.types.ResourceProperties { i32 13, i32 1632 })", cam, sizeof cam) ||
      !unique_def(s, n, fs, fe, "@dx.op.dispatchRaysIndex.i32(i32 145, i8 0)", x, sizeof x) ||
      !unique_def(s, n, fs, fe, "@dx.op.dispatchRaysIndex.i32(i32 145, i8 1)", y, sizeof y)) return dg("module 1 (second-bounce coverage) text pass", "anchor not found or not unique", __LINE__);
  /* URT's PT library: its CameraCB word and its bounce selector, each exactly once */
  if (count(s, n, "  %rpt_raw = extractvalue %dx.types.CBufRet.i32 %rpt_raw_cb, 0", fs, fe, &first) != 1) return dg("module 1 (second-bounce coverage) text pass", "anchor not found or not unique", __LINE__);
  const char *sel = ", i32 %rpt_nh, i32 %rpt_nb";
  if (count(s, n, sel, fs, fe, &first) != 1) return dg("module 1 (second-bounce coverage) text pass", "anchor not found or not unique", __LINE__);
  uint32_t sl = line_start(s, first), se = line_end(s, n, first), tail = first + slen(sel);
  if (first < find(s, n, "  %rpt_raw = ", fs, fe) || find(s, n, " = select i1 ", sl, first) == NOPOS) return dg("module 1 (second-bounce coverage) text pass", "anchor not found or not unique", __LINE__);
  if (!(tail == se || (tail + 1 == se && s[tail] == '\r'))) return dg("module 1 (second-bounce coverage) text pass", "anchor not found or not unique", __LINE__);   /* nothing after the operand */
  if (!put(out, cap, &u, s, sl)) return dg("module 1 (second-bounce coverage) text pass", "anchor not found or not unique", __LINE__);
  if (!puts_(out, cap, &u, "  %urt_bc_k0 = lshr i32 %rpt_raw, 16\n  %urt_bc_k = and i32 %urt_bc_k0, 3\n"
                           "  %urt_bc_kh = icmp ugt i32 %urt_bc_k, 2\n  %urt_bc_kk = select i1 %urt_bc_kh, i32 2, i32 %urt_bc_k\n"
                           "  %urt_bc_fcb = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle ") ||
      !puts_(out, cap, &u, cam) ||
      !puts_(out, cap, &u, ", i32 13)\n  %urt_bc_f = extractvalue %dx.types.CBufRet.i32 %urt_bc_fcb, 3\n  %urt_bc_yh = lshr i32 ") ||
      !puts_(out, cap, &u, y) || !puts_(out, cap, &u, ", 1\n  %urt_bc_s0 = add i32 ") || !puts_(out, cap, &u, x) ||
      !puts_(out, cap, &u, ", ") || !puts_(out, cap, &u, y) ||
      !puts_(out, cap, &u, "\n  %urt_bc_s1 = add i32 %urt_bc_s0, %urt_bc_yh\n  %urt_bc_s = add i32 %urt_bc_s1, %urt_bc_f\n"
                           "  %urt_bc_m = shl i32 1, %urt_bc_kk\n  %urt_bc_mm = add i32 %urt_bc_m, -1\n"
                           "  %urt_bc_r = and i32 %urt_bc_s, %urt_bc_mm\n  %urt_bc_sel = icmp eq i32 %urt_bc_r, 0\n"
                           "  %urt_bc_gt1 = icmp ugt i32 %rpt_nb, 1\n  %urt_bc_lim = select i1 %urt_bc_gt1, i32 1, i32 %rpt_nb\n"
                           "  %urt_bc_nb = select i1 %urt_bc_sel, i32 %rpt_nb, i32 %urt_bc_lim\n")) return dg("module 1 (second-bounce coverage) text pass", "anchor not found or not unique", __LINE__);
  /* the selector line itself, with its non-hair operand replaced */
  if (!put(out, cap, &u, s + sl, first - sl) || !puts_(out, cap, &u, ", i32 %rpt_nh, i32 %urt_bc_nb")) return dg("module 1 (second-bounce coverage) text pass", "anchor not found or not unique", __LINE__);
  if (!put(out, cap, &u, s + tail, n - tail)) return dg("module 1 (second-bounce coverage) text pass", "anchor not found or not unique", __LINE__);
  *outn = u;
  return 1;
}
int urt_bc_patch_coverage(const char *src, uint32_t n, char *out, uint32_t cap, uint32_t *outn) {
  if (!src || !out || !outn || !n) return 0;
  *outn = 0;
  if (find(src, n, "%urt_bc_", 0, n) != NOPOS) return dg("module 1 text pass", "library already carries module 1", __LINE__);   /* never twice */
  char *mid = (char *)b_alloc(cap);
  uint32_t mn = 0;
  int ok = mid && cov_one(src, n, k_fns[0], mid, cap, &mn) && cov_one(mid, mn, k_fns[1], out, cap, outn);
  b_free(mid);
  if (!ok) *outn = 0;
  return ok;
}

/* ---------------- Orange Hair Fix "Path A": its PT hair patch (patch_pt.py, RELEASE), ported 1:1 ---------------- */
typedef struct { const char *p; uint32_t n; } Ln;
/* parse "%<digits>" at p (within line), returns length or 0 */
static uint32_t num_tok(const char *p, uint32_t n) {
  if (!n || p[0] != '%') return 0;
  uint32_t k = 1;
  while (k < n && p[k] >= '0' && p[k] <= '9') k++;
  return k > 1 ? k : 0;
}
static uint32_t skip_ws(const char *p, uint32_t n) { uint32_t k = 0; while (k < n && (p[k] == ' ' || p[k] == '\t')) k++; return k; }
/* exact-line matcher: line == ws + "%D" + lit1 + "%D" + lit2 (+ optional \r); returns the two tokens */
static int match2(Ln l, const char *lit1, const char *lit2, char *t1, char *t2) {
  uint32_t i = skip_ws(l.p, l.n), k;
  uint32_t n = l.n; if (n && l.p[n - 1] == '\r') n--;
  if (!(k = num_tok(l.p + i, n - i)) || k > 30) return 0;
  for (uint32_t j = 0; j < k; j++) { t1[j] = l.p[i + j]; }
  t1[k] = 0; i += k;
  if (!eq_at(l.p, n, i, lit1)) return 0;
  i += slen(lit1);
  if (!(k = num_tok(l.p + i, n - i)) || k > 30) return 0;
  for (uint32_t j = 0; j < k; j++) { t2[j] = l.p[i + j]; }
  t2[k] = 0; i += k;
  if (!eq_at(l.p, n, i, lit2)) return 0;
  i += slen(lit2);
  return i == n;
}
static int contains(Ln l, const char *q) { return find(l.p, l.n, q, 0, l.n) != NOPOS; }
#define MAXL 60000u
#define DESAT_DOT "float 0x3FD3333340000000, float 5.000000e-01, float 0x3FC99999A0000000"
typedef struct { Ln *v; uint32_t n, cap; char *pool; uint32_t pu, pcap; } Lines;
static int push(Lines *L, uint32_t at, const char *p, uint32_t n) {        /* insert a line copy at index */
  if (L->n >= L->cap || L->pu + n + 1 > L->pcap) return 0;
  char *d = L->pool + L->pu;
  for (uint32_t i = 0; i < n; i++) d[i] = p[i];
  L->pu += n + 1;
  for (uint32_t i = L->n; i > at; i--) L->v[i] = L->v[i - 1];
  L->v[at].p = d; L->v[at].n = n; L->n++;
  return 1;
}
static int pushf(Lines *L, uint32_t at, const char *a, const char *b, const char *c, const char *d, const char *e) {
  char buf[256]; uint32_t u = 0;
  const char *parts[5] = {a, b, c, d, e};
  for (int i = 0; i < 5; i++) if (parts[i]) { uint32_t k = slen(parts[i]); if (u + k >= sizeof buf) return 0; for (uint32_t j = 0; j < k; j++) buf[u++] = parts[i][j]; }
  return push(L, at, buf, u);
}
static int ohf_fn(Lines *L, uint32_t a, uint32_t *b_io, unsigned *nsites) {
  uint32_t b = *b_io;                                                       /* function lines [a,b) */
  char spc[32] = {0}, cam[32] = {0}, t1[32], t2[32];
  for (uint32_t i = a; i < b; i++) {                                        /* first 5456 / 1632 annotate */
    Ln l = L->v[i]; uint32_t w = skip_ws(l.p, l.n), k = num_tok(l.p + w, l.n - w);
    const char *mid = " = call %dx.types.Handle @dx.op.annotateHandle(i32 216, %dx.types.Handle ";
    if (!k || !eq_at(l.p, l.n, w + k, mid)) continue;
    uint32_t q = w + k + slen(mid), k2 = num_tok(l.p + q, l.n - q);
    if (!k2) continue;
    q += k2;
    char *dst = 0;
    if (eq_at(l.p, l.n, q, ", %dx.types.ResourceProperties { i32 13, i32 5456 }")) dst = spc;
    else if (eq_at(l.p, l.n, q, ", %dx.types.ResourceProperties { i32 13, i32 1632 }")) dst = cam;
    if (dst && !dst[0] && k < 31) { for (uint32_t j = 0; j < k; j++) dst[j] = l.p[w + j]; dst[k] = 0; }
  }
  if (!spc[0] || !cam[0]) return 0;
  /* A) the material-id skin compare */
  uint32_t ai = NOPOS, nc = 0; char skin[32], id7[32];
  for (uint32_t i = a; i < b; i++) {
    if (!match2(L->v[i], " = icmp eq i32 ", ", 5", t1, t2)) continue;
    int ok = 0;
    for (uint32_t j = (i >= a + 6 ? i - 6 : a); j < i; j++) {
      char u1[32], u2[32];
      if (match2(L->v[j], " = and i32 ", ", 7", u1, u2) && slen(u1) == slen(t2) && eq_at(u1, slen(u1), 0, t2)) ok = 1;
    }
    if (ok) { nc++; ai = i; for (uint32_t j = 0; j <= slen(t1); j++) skin[j] = t1[j]; for (uint32_t j = 0; j <= slen(t2); j++) id7[j] = t2[j]; }
  }
  if (nc != 1) return 0;
  const char *ins_fmt[] = {
    "  %hlf_cb = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle ", spc, ", i32 338)  ; HairLightFix cvar", 0,
    "  %hlf_mode = extractvalue %dx.types.CBufRet.i32 %hlf_cb, 1", 0,
    "  %hlf_m77 = icmp eq i32 %hlf_mode, 77", 0,
    "  %hlf_m25 = icmp eq i32 %hlf_mode, 25", 0,
    "  %hlf_m75 = icmp eq i32 %hlf_mode, 75", 0,
    "  %hlf_m90 = icmp eq i32 %hlf_mode, 90", 0,
    "  %hlf_c3 = icmp eq i32 ", id7, ", 3", 0,
    "  %hlf_hair = and i1 %hlf_c3, true", 0,
    "  %hlf_diag = and i1 %hlf_m90, true", 0,
    "  %hlf_s1 = select i1 %hlf_m75, float 7.500000e-01, float 5.000000e-01", 0,
    "  %hlf_s2 = select i1 %hlf_m25, float 2.500000e-01, float %hlf_s1", 0,
    "  %hlf_s3 = select i1 %hlf_diag, float 1.000000e+00, float %hlf_s2", 0,
    "  %hlf_str = select i1 %hlf_m77, float 0.000000e+00, float %hlf_s3", 0,
    "  %hlf_m0 = icmp eq i32 %hlf_mode, 0", 0,
    "  %hlf_off = or i1 %hlf_m77, %hlf_m0", 0,
    "  %hlf_en = xor i1 %hlf_off, true", 0,
    "  %hlf_on = and i1 %hlf_hair, %hlf_en", 0, (const char *)1};
  uint32_t at = ai + 1;
  for (unsigned i = 0; ins_fmt[i] != (const char *)1;) {
    const char *p[5] = {0, 0, 0, 0, 0}; unsigned k = 0;
    while (ins_fmt[i]) { if (k < 5) p[k++] = ins_fmt[i]; i++; }
    i++;
    if (!pushf(L, at++, p[0], p[1], p[2], p[3], p[4])) return 0;
    b++;
  }
  /* B) the skin && lobe gate, its branch, and the flag zext after the CameraCB[82] block */
  uint32_t gi = NOPOS, ng = 0; char gv[32], lobe[32];
  for (uint32_t i = a; i < b; i++) {
    char lit[64]; uint32_t u = 0;
    const char *p1 = " = and i1 "; for (uint32_t j = 0; p1[j]; j++) lit[u++] = p1[j];
    for (uint32_t j = 0; skin[j]; j++) lit[u++] = skin[j];
    lit[u++] = ','; lit[u++] = ' '; lit[u] = 0;
    if (match2(L->v[i], lit, "", t1, t2)) { /* "%G = and i1 %skin, %L" with %skin fixed inside lit */
      ng++; gi = i; for (uint32_t j = 0; j <= slen(t1); j++) gv[j] = t1[j]; for (uint32_t j = 0; j <= slen(t2); j++) lobe[j] = t2[j];
    }
  }
  if (ng != 1 || gi + 1 >= b) return 0;
  {
    Ln l = L->v[gi + 1]; uint32_t w = skip_ws(l.p, l.n);
    char pre[48]; uint32_t u = 0; const char *p1 = "br i1 "; for (uint32_t j = 0; p1[j]; j++) pre[u++] = p1[j];
    for (uint32_t j = 0; gv[j]; j++) pre[u++] = gv[j];
    const char *p2 = ", label %"; for (uint32_t j = 0; p2[j]; j++) pre[u++] = p2[j]; pre[u] = 0;
    if (!eq_at(l.p, l.n, w, pre)) return 0;
  }
  uint32_t zi = NOPOS;
  for (uint32_t j = gi; j < gi + 40 && j < b; j++)
    if (match2(L->v[j], " = zext i1 ", " to i32", t1, t2) && j > a && contains(L->v[j - 1], "phi i1")) { zi = j; break; }
  if (zi == NOPOS) return 0;
  {
    char q[64]; uint32_t u = 0; const char *p1 = "%dx.types.Handle "; for (uint32_t j = 0; p1[j]; j++) q[u++] = p1[j];
    for (uint32_t j = 0; cam[j]; j++) q[u++] = cam[j];
    const char *p2 = ", i32 82)"; for (uint32_t j = 0; p2[j]; j++) q[u++] = p2[j]; q[u] = 0;
    int seen = 0; for (uint32_t j = gi; j < zi; j++) if (contains(L->v[j], q)) seen = 1;
    if (!seen) return 0;
  }
  match2(L->v[zi], " = zext i1 ", " to i32", t1, t2);
  {
    char zv[32], pv[32]; for (uint32_t j = 0; j <= slen(t1); j++) zv[j] = t1[j]; for (uint32_t j = 0; j <= slen(t2); j++) pv[j] = t2[j];
    for (uint32_t i = zi; i + 1 < L->n; i++) L->v[i] = L->v[i + 1];     /* drop the zext line */
    L->n--;
    if (!pushf(L, zi, "  %hlf_zs = zext i1 ", pv, " to i32", 0, 0) ||
        !pushf(L, zi + 1, "  %hlf_hl = and i1 %hlf_on, ", lobe, 0, 0, 0) ||
        !pushf(L, zi + 2, "  ", zv, " = select i1 %hlf_hl, i32 1, i32 %hlf_zs", 0, 0)) return 0;
    b += 2;
  }
  /* C) the strength sites: saturate(CameraCB[82].w) right after the (0.3, 0.5, 0.2) dot */
  char q82[64]; { uint32_t u = 0; const char *p1 = "%dx.types.Handle "; for (uint32_t j = 0; p1[j]; j++) q82[u++] = p1[j];
    for (uint32_t j = 0; cam[j]; j++) { q82[u++] = cam[j]; }
    const char *p2 = ", i32 82)";
    for (uint32_t j = 0; p2[j]; j++) { q82[u++] = p2[j]; }
    q82[u] = 0; }
  unsigned ns = 0;
  for (uint32_t i = a; i < b; i++) {
    if (!contains(L->v[i], "@dx.op.dot3.f32") || !contains(L->v[i], DESAT_DOT)) continue;
    for (uint32_t j = i + 1; j < i + 5 && j < b; j++) {
      Ln l = L->v[j]; uint32_t w = skip_ws(l.p, l.n), k = num_tok(l.p + w, l.n - w);
      const char *m = " = call float @dx.op.unary.f32(i32 7, float ";
      if (!k || !eq_at(l.p, l.n, w + k, m)) continue;
      uint32_t q = w + k + slen(m), k2 = num_tok(l.p + q, l.n - q);
      if (!k2 || q + k2 >= l.n || l.p[q + k2] != ')') continue;
      char sv[32], ev[32]; for (uint32_t z = 0; z < k; z++) sv[z] = l.p[w + z]; sv[k] = 0;
      for (uint32_t z = 0; z < k2; z++) { ev[z] = l.p[q + z]; }
      ev[k2] = 0;
      int has82 = 0, hasev = 0;
      char eq[96]; { uint32_t u = 0; for (uint32_t z = 0; ev[z]; z++) eq[u++] = ev[z];
        const char *p1 = " = extractvalue %dx.types.CBufRet.f32 %"; for (uint32_t z = 0; p1[z]; z++) eq[u++] = p1[z]; eq[u] = 0; }
      for (uint32_t z = i + 1; z < j; z++) {
        if (contains(L->v[z], q82)) has82 = 1;
        uint32_t f = find(L->v[z].p, L->v[z].n, eq, 0, L->v[z].n);
        if (f != NOPOS) { uint32_t r = f + slen(eq); while (r < L->v[z].n && L->v[z].p[r] >= '0' && L->v[z].p[r] <= '9') r++;
          if (r > f + slen(eq) && eq_at(L->v[z].p, L->v[z].n, r, ", 3")) hasev = 1; }
      }
      if (has82 && hasev) {
        ns++;
        char nm[24]; { const char *p1 = "%hlf_sat"; uint32_t u = 0; for (uint32_t z = 0; p1[z]; z++) nm[u++] = p1[z];
          char d[8]; uint32_t dn = 0, v = ns; do { d[dn++] = (char)('0' + v % 10); v /= 10; } while (v); while (dn) nm[u++] = d[--dn]; nm[u] = 0; }
        /* line j: "<ws>%sv = call ..." becomes "<ws>%hlf_satN = call ..." (first occurrence), then the select */
        char nl[512]; uint32_t u = 0;
        if (l.n + 32 > sizeof nl) return 0;
        for (uint32_t z = 0; z < w; z++) nl[u++] = l.p[z];
        for (uint32_t z = 0; nm[z]; z++) nl[u++] = nm[z];
        for (uint32_t z = w + k; z < l.n; z++) nl[u++] = l.p[z];
        for (uint32_t z = j; z + 1 < L->n; z++) L->v[z] = L->v[z + 1];
        L->n--;
        if (!push(L, j, nl, u) || !pushf(L, j + 1, "  ", sv, " = select i1 %hlf_hair, float %hlf_str, float ", nm, 0)) return 0;
        b += 1;
      }
      break;
    }
  }
  if (!ns) return 0;
  *nsites += ns;
  *b_io = b;
  return 1;
}
int urt_ohf_bake(const char *src, uint32_t n, char *out, uint32_t cap, uint32_t *outn, unsigned *sites) {
  if (!src || !out || !outn || !n) return 0;
  *outn = 0; if (sites) *sites = 0;
  if (find(src, n, "%hlf_", 0, n) != NOPOS) return 0;                  /* never twice (Orange Hair Fix's own rule) */
  Lines L = {0};
  L.cap = 0;
  for (uint32_t i = 0; i < n; i++) if (src[i] == '\n') L.cap++;
  L.cap += 1024u;
  L.v = (Ln *)b_alloc(sizeof(Ln) * L.cap);
  L.pcap = 1u << 20; L.pool = (char *)b_alloc(L.pcap);
  int ok = 0; unsigned ns = 0, nf = 0;
  if (!L.v || !L.pool) goto done;
  for (uint32_t p = 0; p < n;) { uint32_t e = line_end(src, n, p); L.v[L.n].p = src + p; L.v[L.n].n = e - p; L.n++; p = e + 1; if (e >= n) break; }
  for (uint32_t i = 0; i < L.n; i++) {
    if (!(L.v[i].n >= 13 && eq_at(L.v[i].p, L.v[i].n, 0, "define void @"))) continue;
    uint32_t j = i; while (j < L.n && !(L.v[j].n >= 1 && L.v[j].p[0] == '}' && (L.v[j].n == 1 || L.v[j].p[1] == '\r'))) j++;
    if (j >= L.n) goto done;
    int pt = contains(L.v[i], "PTRGSFinal@@") || contains(L.v[i], "PTRGSWithSERFinal@@");
    if (!pt) { i = j; continue; }
    uint32_t b = j + 1; unsigned before = ns;
    if (!ohf_fn(&L, i, &b, &ns) || ns == before) { (void)dg("Orange Hair Fix hair patch (Path A)", "its anchors were not found in a PT export", __LINE__); goto done; }
    nf++; i = b - 1;
  }
  if (nf != 2) { (void)dg("Orange Hair Fix hair patch (Path A)", "not exactly two PT exports patched", __LINE__); goto done; }
  {
    uint32_t u = 0;
    for (uint32_t i = 0; i < L.n; i++) { if (!put(out, cap, &u, L.v[i].p, L.v[i].n)) goto done; if (i + 1 < L.n && !put(out, cap, &u, "\n", 1)) goto done; }
    if (n && src[n - 1] == '\n' && !put(out, cap, &u, "\n", 1)) goto done;
    *outn = u; ok = 1;
  }
done:
  if (sites) *sites = ok ? ns : 0;
  b_free(L.v); b_free(L.pool);
  return ok;
}

/* ---------------- module 2 (R12): first-bounce diffuse coverage ---------------- */
/* An edit list over one text: [pos, pos+del) is replaced by txt (sorted, non-overlapping). */
typedef struct { uint32_t pos, del; const char *txt; uint32_t tn; } DcEd;
typedef struct { DcEd e[512]; uint32_t n; char pool[65536]; uint32_t pu; } DcEds;
static const char *dc_str(DcEds *E, const char *a, uint32_t an, const char *b, uint32_t bn, const char *c, uint32_t cn, uint32_t *tn) {
  if (E->pu + an + bn + cn + 1 > sizeof E->pool) return 0;
  char *t = E->pool + E->pu;
  for (uint32_t i = 0; i < an; i++) t[i] = a[i];
  for (uint32_t i = 0; i < bn; i++) t[an + i] = b[i];
  for (uint32_t i = 0; i < cn; i++) t[an + bn + i] = c[i];
  *tn = an + bn + cn; E->pu += *tn + 1; t[*tn] = 0;
  return t;
}
static int dc_add(DcEds *E, uint32_t pos, uint32_t del, const char *txt, uint32_t tn) {
  if (!txt || E->n >= 512) return 0;
  E->e[E->n].pos = pos; E->e[E->n].del = del; E->e[E->n].txt = txt; E->e[E->n].tn = tn; E->n++;
  return 1;
}
/* end of the line's content (before an optional '\r') */
static uint32_t dc_le(const char *b, uint32_t n, uint32_t p) { uint32_t e = line_end(b, n, p); return (e > p && b[e - 1] == '\r') ? e - 1 : e; }
/* "%tok" starting at p: its length (letters, digits, '_', '.') */
static uint32_t dc_tok(const char *b, uint32_t n, uint32_t p) {
  if (p >= n || b[p] != '%') return 0;
  uint32_t k = 1;
  while (p + k < n && ((b[p + k] >= '0' && b[p + k] <= '9') || (b[p + k] >= 'a' && b[p + k] <= 'z') || (b[p + k] >= 'A' && b[p + k] <= 'Z') || b[p + k] == '_' || b[p + k] == '.')) k++;
  return k > 1 ? k : 0;
}
/* the line in [fs,fe) whose content (after indentation) starts with "<name> = " + q; NOPOS if none or several */
static uint32_t dc_def(const char *s, uint32_t n, uint32_t fs, uint32_t fe, const char *name, uint32_t nn, const char *q) {
  char pat[96]; uint32_t k = 0, first, c;
  if (nn + 8 + slen(q) >= sizeof pat) return NOPOS;
  pat[k++] = '\n'; pat[k++] = ' '; pat[k++] = ' ';
  for (uint32_t i = 0; i < nn; i++) pat[k++] = name[i];
  pat[k++] = ' '; pat[k++] = '='; pat[k++] = ' ';
  for (uint32_t i = 0; q[i]; i++) pat[k++] = q[i];
  pat[k] = 0;
  c = count(s, n, pat, fs, fe, &first);
  return c == 1 ? first + 1 : NOPOS;
}
/* R12 fix: block labels are printed "908:" by LLVM 18 (llvm-dis, the offline tests) but "; <label>:908" by the game's
 * DXC (LLVM 3.7 printer, the runtime). Both spellings are accepted everywhere a label is located. */
static uint32_t dc_label_digits(const char *s, uint32_t ls, uint32_t *ds) {   /* digits of a label line; 0 = none */
  uint32_t q = ls;
  if (s[q] == ';') { const char *t = "; <label>:"; for (uint32_t i = 0; t[i]; i++) if (s[q + i] != t[i]) return 0; q += 10; }
  uint32_t d = q;
  while (s[q] >= '0' && s[q] <= '9') q++;
  if (q == d) return 0;
  if (s[ls] == ';') { if (!(s[q] == ' ' || s[q] == '\n' || s[q] == '\r' || s[q] == 0)) return 0; }
  else if (s[q] != ':') return 0;
  *ds = d; return q - d;
}
/* the line before the unique label line of block <name> ("%908"): returns the position of its preceding '\n' */
static uint32_t dc_label(const char *s, uint32_t n, uint32_t fs, uint32_t fe, const char *name) {
  uint32_t nl = slen(name) - 1, hit = NOPOS, c = 0;
  for (uint32_t p = fs; (p = find(s, n, "\n", p, fe)) != NOPOS; p++) {
    uint32_t ds, dl = dc_label_digits(s, p + 1, &ds);
    if (dl == nl && eq_at(s, n, ds, name + 1)) { hit = p; c++; }
  }
  return c == 1 ? hit : NOPOS;
}
/* label of the block that contains position p (either spelling) */
static uint32_t dc_block(const char *s, uint32_t fs, uint32_t p, char *lab, uint32_t cap) {
  uint32_t ls = line_start(s, p);
  while (ls > fs) {
    uint32_t ds, dl = dc_label_digits(s, ls, &ds), k = 0;
    if (dl) {
      if (dl + 2 > cap) return NOPOS;
      lab[k++] = '%'; for (uint32_t i = 0; i < dl; i++) lab[k++] = s[ds + i]; lab[k] = 0;
      return ls;
    }
    if (ls < 2) break;
    ls = line_start(s, ls - 1);
  }
  return NOPOS;
}
static int dc_same(const char *s, uint32_t a, uint32_t b, uint32_t l) { for (uint32_t i = 0; i < l; i++) if (s[a + i] != s[b + i]) return 0; return 1; }
static int dc_one(const char *s, uint32_t n, const char *fn, DcEds *E) {
  uint32_t fs, fe, first;
  char cam[48], x[48], y[48], B[48], T[48], L[48], C[48], I0[48], X[48], S[48], P[48], A[48], R[48], LB[48], H[48];
  if (!fn_range(s, n, fn, &fs, &fe)) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
  if (!unique_def(s, n, fs, fe, "%dx.types.ResourceProperties { i32 13, i32 1632 })", cam, sizeof cam) ||
      !unique_def(s, n, fs, fe, "@dx.op.dispatchRaysIndex.i32(i32 145, i8 0)", x, sizeof x) ||
      !unique_def(s, n, fs, fe, "@dx.op.dispatchRaysIndex.i32(i32 145, i8 1)", y, sizeof y) ||
      !unique_def(s, n, fs, fe, ", i32 %rpt_nh, i32 %urt_bc_nb", B, sizeof B)) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
  uint32_t bline = line_start(s, find(s, n, ", i32 %rpt_nh, i32 %urt_bc_nb", fs, fe));
  /* bounce-loop latch: "<L> = icmp ult i32 <T>, <B>" exactly once; <T> = add nuw nsw i32 <C>, 1 */
  uint32_t lat = NOPOS, nl = 0, bn = slen(B);
  for (uint32_t p = fs; (p = find(s, n, " = icmp ult i32 %", p, fe)) != NOPOS; p++) {
    uint32_t le = dc_le(s, n, p);
    if (!(le >= bn + 2 && eq_at(s, n, le - bn - 2, ", ") && eq_at(s, n, le - bn, B))) continue;
    /* the compare that drives the loop back edge: its branch carries !llvm.loop */
    char ln_[48], bp_[64]; uint32_t j_ = 0, f_;
    if (!def_name(s, line_start(s, p), le, ln_, sizeof ln_)) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    const char *br_ = "  br i1 "; while (*br_) bp_[j_++] = *br_++;
    for (uint32_t i = 0; ln_[i] && j_ + 2 < sizeof bp_; i++) bp_[j_++] = ln_[i];
    bp_[j_++] = ','; bp_[j_] = 0;
    if (count(s, n, bp_, fs, fe, &f_) == 1 && find(s, n, ", !llvm.loop ", f_, dc_le(s, n, f_)) != NOPOS) { lat = p; nl++; }
  }
  if (nl != 1) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
  {
    uint32_t ls = line_start(s, lat), le = dc_le(s, n, lat);
    if (!def_name(s, ls, le, L, sizeof L)) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    uint32_t tp = lat + slen(" = icmp ult i32 "), tk = dc_tok(s, n, tp);
    if (!tk || tk + 1 > sizeof T) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    for (uint32_t i = 0; i < tk; i++) T[i] = s[tp + i]; T[tk] = 0;
  }
  uint32_t td = dc_def(s, n, fs, fe, T, slen(T), "add nuw nsw i32 %");
  if (td == NOPOS) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
  {
    uint32_t cp = td + 2 + slen(T) + slen(" = add nuw nsw i32 "), ck = dc_tok(s, n, cp);
    if (!ck || ck + 1 > sizeof C || !eq_at(s, n, cp + ck, ", 1")) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    for (uint32_t i = 0; i < ck; i++) C[i] = s[cp + i]; C[ck] = 0;
  }
  /* latch branch "br i1 <L>, label %<H>, label %<X>": X = the loop exit */
  {
    char pat[80]; uint32_t k = 0;
    const char *a = "  br i1 "; for (uint32_t i = 0; a[i]; i++) pat[k++] = a[i];
    for (uint32_t i = 0; L[i]; i++) pat[k++] = L[i];
    pat[k++] = ','; pat[k] = 0;
    if (count(s, n, pat, fs, fe, &first) != 1) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    uint32_t le = dc_le(s, n, first), q = first + k;
    if (!eq_at(s, n, q, " label %")) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    q += 7; uint32_t hk = dc_tok(s, n, q); if (!hk || hk + 1 > sizeof H) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    for (uint32_t i = 0; i < hk; i++) H[i] = s[q + i]; H[hk] = 0;
    q += hk; if (!eq_at(s, n, q, ", label %")) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    q += 8; uint32_t xk = dc_tok(s, n, q); if (!xk || q + xk + slen(", !llvm.loop") > le + 1 || xk + 1 > sizeof X) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    for (uint32_t i = 0; i < xk; i++) X[i] = s[q + i]; X[xk] = 0;
    if (!eq_at(s, n, q + xk, ", !llvm.loop ")) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
  }
  /* iteration-0 flag: "<I0> = icmp eq i32 <C>, 0" */
  {
    char q[64]; uint32_t k = 0; const char *a = "icmp eq i32 ";
    for (uint32_t i = 0; a[i]; i++) q[k++] = a[i];
    for (uint32_t i = 0; C[i]; i++) q[k++] = C[i];
    q[k++] = ','; q[k++] = ' '; q[k++] = '0'; q[k++] = '\n'; q[k] = 0;
    char pat[80]; uint32_t j = 0; const char *b2 = " = "; for (uint32_t i = 0; b2[i]; i++) pat[j++] = b2[i];
    for (uint32_t i = 0; q[i]; i++) pat[j++] = q[i]; pat[j] = 0;
    if (count(s, n, pat, fs, fe, &first) != 1) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    uint32_t ls = line_start(s, first);
    if (!def_name(s, ls, dc_le(s, n, first), I0, sizeof I0)) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
  }
  /* the first exit to X inside the loop: "br i1 %c, label X, label %d" in block A; A's predecessor ends with
     "br i1 <S>, label A, label <R>" (the lobe decision block LB), <S> = and i1 %q, <P>, <P> = phi i1 (specular lobe) */
  {
    char pat[80]; uint32_t k = 0; const char *a = ", label ";
    for (uint32_t i = 0; a[i]; i++) pat[k++] = a[i];
    for (uint32_t i = 0; X[i]; i++) pat[k++] = X[i];
    const char *b2 = ", label %"; for (uint32_t i = 0; b2[i]; i++) pat[k++] = b2[i]; pat[k] = 0;
    uint32_t hdr = find(s, n, "\n", find(s, n, "\n", fs, fe) + 1, fe);       /* after the define line */
    uint32_t hl;  /* header label line "<H>:" */
    {
      if ((hl = dc_label(s, n, fs, fe, H)) == NOPOS) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    }
    (void)hdr;
    uint32_t e1 = find(s, n, pat, hl, fe);
    if (e1 == NOPOS || find(s, n, "  br i1 %", line_start(s, e1), e1) != line_start(s, e1)) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    if (dc_block(s, fs, e1, A, sizeof A) == NOPOS) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
  }
  uint32_t lt = NOPOS;
  {
    char pat[64]; uint32_t k = 0; const char *a = ", label ";
    for (uint32_t i = 0; a[i]; i++) pat[k++] = a[i];
    for (uint32_t i = 0; A[i]; i++) pat[k++] = A[i];
    pat[k++] = ','; pat[k++] = ' '; pat[k] = 0;
    if (count(s, n, pat, fs, fe, &first) != 1) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    lt = line_start(s, first);
    if (!eq_at(s, n, lt, "  br i1 %")) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    uint32_t sk = dc_tok(s, n, lt + 8); if (!sk || sk + 1 > sizeof S || lt + 8 + sk != first) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    for (uint32_t i = 0; i < sk; i++) S[i] = s[lt + 8 + i]; S[sk] = 0;
    uint32_t rp = first + k; if (!eq_at(s, n, rp, "label %")) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    rp += 6; uint32_t rk = dc_tok(s, n, rp); if (!rk || rk + 1 > sizeof R || rp + rk != dc_le(s, n, rp)) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    for (uint32_t i = 0; i < rk; i++) R[i] = s[rp + i]; R[rk] = 0;
  }
  uint32_t lbl = dc_block(s, fs, lt, LB, sizeof LB);
  if (lbl == NOPOS) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
  {
    uint32_t sd = dc_def(s, n, fs, fe, S, slen(S), "and i1 %");
    if (sd == NOPOS || sd < lbl || sd > lt) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    uint32_t le = dc_le(s, n, sd), q = le;
    while (q > sd && s[q - 1] != ' ') q--;
    uint32_t pk = dc_tok(s, n, q); if (!pk || q + pk != le || pk + 1 > sizeof P) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    for (uint32_t i = 0; i < pk; i++) P[i] = s[q + i]; P[pk] = 0;
    uint32_t pd = dc_def(s, n, fs, fe, P, pk, "phi i1 [ ");
    if (pd == NOPOS || pd < lbl || pd > lt) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
  }
  /* 1. per-pixel decision, before the loop (same place as module 1's) */
  {
    /* build the decision block piece by piece into the pool */
    char buf[2048]; uint32_t k = 0;
#define DC_A(str) do { const char *z_ = (str); while (*z_) { if (k + 1 >= sizeof buf) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__); buf[k++] = *z_++; } } while (0)
    DC_A("  %urt_dc_l0 = lshr i32 %rpt_raw, 18\n  %urt_dc_l = and i32 %urt_dc_l0, 3\n");
    DC_A("  %urt_dc_fcb = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle "); DC_A(cam);
    DC_A(", i32 13)\n  %urt_dc_f = extractvalue %dx.types.CBufRet.i32 %urt_dc_fcb, 3\n");
    DC_A("  %urt_dc_x1 = and i32 "); DC_A(x); DC_A(", 1\n  %urt_dc_y1 = and i32 "); DC_A(y); DC_A(", 1\n");
    DC_A("  %urt_dc_y2 = shl i32 %urt_dc_y1, 1\n  %urt_dc_q = or i32 %urt_dc_x1, %urt_dc_y2\n");
    DC_A("  %urt_dc_f3 = and i32 %urt_dc_f, 3\n  %urt_dc_d1 = icmp eq i32 %urt_dc_q, %urt_dc_f3\n");
    DC_A("  %urt_dc_c0 = xor i32 "); DC_A(x); DC_A(", "); DC_A(y); DC_A("\n");
    DC_A("  %urt_dc_c1 = xor i32 %urt_dc_c0, %urt_dc_f\n  %urt_dc_c = and i32 %urt_dc_c1, 1\n  %urt_dc_d2 = icmp ne i32 %urt_dc_c, 0\n");
    DC_A("  %urt_dc_is1 = icmp eq i32 %urt_dc_l, 1\n  %urt_dc_is2 = icmp eq i32 %urt_dc_l, 2\n");
    DC_A("  %urt_dc_e1 = and i1 %urt_dc_is1, %urt_dc_d1\n  %urt_dc_e2 = and i1 %urt_dc_is2, %urt_dc_d2\n");
    DC_A("  %urt_dc_drop = or i1 %urt_dc_e1, %urt_dc_e2\n");
    DC_A("  %urt_dc_w1 = select i1 %urt_dc_is1, float 0x3FF5555560000000, float 1.000000e+00\n");
    DC_A("  %urt_dc_w = select i1 %urt_dc_is2, float 2.000000e+00, float %urt_dc_w1\n");
    uint32_t tn; const char *t = dc_str(E, buf, k, "", 0, "", 0, &tn);
    if (!dc_add(E, bline, 0, t, tn)) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    /* 2. the lobe decision: iteration 0, diffuse lobe, dropped pixel -> the loop exit with nothing traced */
    k = 0;
    /* P = "rand < diffuse energy fraction": the diffuse lobe (STATIC, see section 5) */
    DC_A("  %urt_dc_a = and i1 "); DC_A(I0); DC_A(", "); DC_A(P); DC_A("\n");
    DC_A("  %urt_dc_skip = and i1 %urt_dc_a, %urt_dc_drop\n  br i1 %urt_dc_skip, label %urt_dc_exit, label %urt_dc_go\n\n");
    DC_A("urt_dc_exit:\n  br label "); DC_A(X); DC_A("\n\nurt_dc_go:\n  br i1 "); DC_A(S); DC_A(", label "); DC_A(A);
    DC_A(", label "); DC_A(R);
    t = dc_str(E, buf, k, "", 0, "", 0, &tn);
    if (!dc_add(E, lt, dc_le(s, n, lt) - lt, t, tn)) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    /* 3. the exit block's phis gain the new predecessor with the values they take from A (nothing traced yet) */
    {
      uint32_t j = 0, xl = dc_label(s, n, fs, fe, X);
      if (xl == NOPOS) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
      uint32_t p = find(s, n, "\n", xl + 1, fe) + 1, np = 0;
      char ap[56]; j = 0; ap[j++] = ','; ap[j++] = ' '; for (uint32_t i = 0; A[i]; i++) ap[j++] = A[i]; ap[j++] = ' '; ap[j++] = ']'; ap[j] = 0;
      while (eq_at(s, n, p, "  %") && find(s, n, " = phi ", p, dc_le(s, n, p)) != NOPOS) {
        uint32_t le = dc_le(s, n, p), ae = find(s, n, ap, p, le);
        if (ae == NOPOS) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
        uint32_t ab = ae; while (ab > p && s[ab - 1] != '[') ab--;
        if (ab == p || s[ab] != ' ') return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
        const char *t2 = dc_str(E, ", [", 3, s + ab, ae - ab, ", %urt_dc_exit ]", 16, &tn);
        if (!dc_add(E, le, 0, t2, tn)) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
        np++; p = line_end(s, n, p) + 1;
      }
      if (!np) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    }
    /* 4. phis of the two former successors now come from urt_dc_go */
    for (unsigned w = 0; w < 2; w++) {
      const char *blk = w ? R : A;
      uint32_t j = 0, bl = dc_label(s, n, fs, fe, blk);
      if (bl == NOPOS) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
      char op[56]; j = 0; op[j++] = ','; op[j++] = ' '; for (uint32_t i = 0; LB[i]; i++) op[j++] = LB[i]; op[j++] = ' '; op[j++] = ']'; op[j] = 0;
      uint32_t p = find(s, n, "\n", bl + 1, fe) + 1;
      while (eq_at(s, n, p, "  %") && find(s, n, " = phi ", p, dc_le(s, n, p)) != NOPOS) {
        uint32_t le = dc_le(s, n, p), q = find(s, n, op, p, le);
        if (q == NOPOS || find(s, n, op, q + 1, le) != NOPOS) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
        const char *t2 = dc_str(E, ", %urt_dc_go ]", 14, "", 0, "", 0, &tn);
        if (!dc_add(E, q, slen(op), t2, tn)) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
        p = line_end(s, n, p) + 1;
      }
    }
  }
  /* 5. the diffuse output. STATIC: the lobe flag (P, zext'd into the loop's lobe word) is "rand < diffuse energy
     fraction", and the post-loop code routes that lobe's radiance and hit distance to RTGBufferA, the other lobe's to
     RTGBufferB. The store of A whose colour differs from B's (the normal, non-debug branch) gets the scaled colour. */
  {
    uint32_t pos[2][4], len[2][4]; unsigned cnt[2] = {0, 0};
    for (unsigned w = 0; w < 2; w++) {
      char hg[48], pat[96];
      /* "load %dx.types.Handle, ptr @..." (LLVM 18) or "load %dx.types.Handle, %dx.types.Handle* @..." (DXC) */
      {
        const char *g = w ? "@\"\\01?RTGBufferB@@" : "@\"\\01?RTGBufferA@@";
        uint32_t hits = 0, at = NOPOS;
        for (uint32_t q = fs; (q = find(s, n, g, q, fe)) != NOPOS; q++) {
          uint32_t ls = line_start(s, q);
          if (find(s, n, " = load %dx.types.Handle, ", ls, q) != NOPOS) { hits++; at = ls; }
        }
        if (hits != 1 || !def_name(s, at, dc_le(s, n, at), hg, sizeof hg)) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
      }
      uint32_t k = 0; const char *a = "@dx.op.createHandleForLib.dx.types.Handle(i32 160, %dx.types.Handle ";
      for (uint32_t i = 0; a[i]; i++) pat[k++] = a[i];
      for (uint32_t i = 0; hg[i]; i++) pat[k++] = hg[i];
      pat[k++] = ')'; pat[k] = 0;
      for (uint32_t p = fs; (p = find(s, n, pat, p, fe)) != NOPOS; p++) {
        uint32_t ls = line_start(s, p); char h1[48], h2[48];
        if (!def_name(s, ls, dc_le(s, n, p), h1, sizeof h1)) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
        uint32_t al = line_end(s, n, p) + 1;
        if (!def_name(s, al, dc_le(s, n, al), h2, sizeof h2) || find(s, n, h1, al, dc_le(s, n, al)) == NOPOS) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
        char sp[96]; uint32_t j = 0; const char *b2 = "@dx.op.textureStore.f32(i32 67, %dx.types.Handle ";
        for (uint32_t i = 0; b2[i]; i++) sp[j++] = b2[i];
        for (uint32_t i = 0; h2[i]; i++) sp[j++] = h2[i];
        sp[j++] = ','; sp[j] = 0;
        for (uint32_t q = al; (q = find(s, n, sp, q, fe)) != NOPOS; q++) {
          uint32_t le = dc_le(s, n, q);
          if (find(s, n, "float 6.550400e+04, i8 15)", q, le) != NOPOS) continue;   /* the early debug / sky stores */
          uint32_t f = find(s, n, "i32 undef, float ", q, le); if (f == NOPOS) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
          f += slen("i32 undef, ");                                                  /* at "float %r, float %g, float %b" */
          uint32_t g = f;
          for (unsigned c = 0; c < 3; c++) { if (!eq_at(s, n, g, "float ")) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__); g += 6; uint32_t tk = dc_tok(s, n, g); if (!tk) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__); g += tk; if (c < 2) { if (!eq_at(s, n, g, ", ")) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__); g += 2; } }
          if (cnt[w] >= 4) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
          pos[w][cnt[w]] = f; len[w][cnt[w]] = g - f; cnt[w]++;
        }
      }
    }
#define DC_SAME(p1, l1, p2, l2) ((l1) == (l2) && dc_same(s, (p1), (p2), (l1)))
    if (cnt[0] != 2 || cnt[1] != 2 || !DC_SAME(pos[1][0], len[1][0], pos[1][1], len[1][1])) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);   /* both B stores: one colour */
    int s0 = DC_SAME(pos[0][0], len[0][0], pos[1][0], len[1][0]), s1 = DC_SAME(pos[0][1], len[0][1], pos[1][0], len[1][0]);
    if (s0 == s1) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);                                                         /* exactly one A store is B's colour */
#undef DC_SAME
    const uint32_t pick = s0 ? pos[0][1] : pos[0][0], plen = s0 ? len[0][1] : len[0][0];
    /* its three colour operands; the last is defined by a phi, the scaled values go right after that phi group */
    char D[3][48]; uint32_t g = pick;
    for (unsigned c = 0; c < 3; c++) { g += 6; uint32_t tk = dc_tok(s, n, g); if (!tk || tk + 1 > 48) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__); for (uint32_t i = 0; i < tk; i++) D[c][i] = s[g + i]; D[c][tk] = 0; g += tk + 2; }
    uint32_t d2line = dc_def(s, n, fs, fe, D[2], slen(D[2]), "phi float [ ");
    if (d2line == NOPOS) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    uint32_t after = line_end(s, n, d2line) + 1;
    if (find(s, n, " = phi ", after, dc_le(s, n, after)) != NOPOS || after > pick) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    char buf[512]; uint32_t k = 0;
    for (unsigned c = 0; c < 3; c++) {
      DC_A("  %urt_dc_o");
      if (k + 2 >= sizeof buf) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
      buf[k++] = (char)('0' + c); DC_A(" = fmul fast float "); DC_A(D[c]); DC_A(", %urt_dc_w\n");
    }
    uint32_t tn; const char *t = dc_str(E, buf, k, "", 0, "", 0, &tn);
    if (!dc_add(E, after, 0, t, tn)) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
    const char *t2 = dc_str(E, "float %urt_dc_o0, float %urt_dc_o1, float %urt_dc_o2", 52, "", 0, "", 0, &tn);
    if (!dc_add(E, pick, plen, t2, tn)) return dg("module 2 (first-bounce diffuse coverage) text pass", "anchor not found or not unique", __LINE__);
#undef DC_A
  }
  return 1;
}
static int dc_cmp_apply(const char *s, uint32_t n, DcEds *E, char *out, uint32_t cap, uint32_t *outn) {
  for (uint32_t i = 1; i < E->n; i++) {            /* insertion sort by position (stable) */
    DcEd t = E->e[i]; uint32_t j = i;
    while (j && E->e[j - 1].pos > t.pos) { E->e[j] = E->e[j - 1]; j--; }
    E->e[j] = t;
  }
  uint32_t u = 0, p = 0;
  for (uint32_t i = 0; i < E->n; i++) {
    if (E->e[i].pos < p || E->e[i].pos + E->e[i].del > n) return dg("module 2 text pass", "overlapping edits", __LINE__);
    if (!put(out, cap, &u, s + p, E->e[i].pos - p) || !put(out, cap, &u, E->e[i].txt, E->e[i].tn)) return dg("module 2 text pass", "output buffer too small", __LINE__);
    p = E->e[i].pos + E->e[i].del;
  }
  if (!put(out, cap, &u, s + p, n - p)) return 0;
  *outn = u;
  return 1;
}
/* ---------------- module 3 (R12 FIX2): bounce-vertex sun/moon shadow-ray coverage ---------------- */
/* STATIC: at a bounce hit the PT ray generation traces its directional-light (sun / moon) shadow rays with tmax 1e28 and
 * turns the result into a visibility factor V = select(miss, 1.0, 0.0) that only multiplies that light's contribution.
 * Module 3: for pixels outside this frame's pattern the shadow ray is traced with instance mask 0 (no instance can be
 * hit, nothing to traverse) and V = 0; traced pixels use V x 2. Unbiased per pixel over its 4-frame cycle, also together
 * with module 2 (the pattern alternates every 2 frames). Level in bits 20-21; 0 = mask 255 and weight 1 (unchanged). */
static int sc_one(const char *s, uint32_t n, const char *fn, DcEds *E) {
#define SCF(w) return dg("module 3 (sun shadow coverage) text pass", w, __LINE__)
  uint32_t fs, fe;
  char cam[48], x[48], y[48];
  if (!fn_range(s, n, fn, &fs, &fe)) SCF("PT export not found");
  if (!unique_def(s, n, fs, fe, "%dx.types.ResourceProperties { i32 13, i32 1632 })", cam, sizeof cam) ||
      !unique_def(s, n, fs, fe, "@dx.op.dispatchRaysIndex.i32(i32 145, i8 0)", x, sizeof x) ||
      !unique_def(s, n, fs, fe, "@dx.op.dispatchRaysIndex.i32(i32 145, i8 1)", y, sizeof y)) SCF("camera / dispatch index anchors");
  uint32_t sel = find(s, n, ", i32 %rpt_nh, i32 %urt_bc_nb", fs, fe);
  if (sel == NOPOS) SCF("module 1 selector not found");
  unsigned sites = 0;
  for (uint32_t p = fs; (p = find(s, n, "@dx.op.traceRay.struct.GbufferPayload(i32 157, %dx.types.Handle ", p, fe)) != NOPOS; p++) {
    uint32_t le = dc_le(s, n, p);
    if (find(s, n, ", float 0x4589D971E0000000, ", p, le) == NOPOS) continue;                       /* directional: tmax 1e28 */
    uint32_t m = find(s, n, ", i32 255, i32 0, i32 4, i32 0, float ", p, le);
    if (m == NOPOS) continue;
    /* the next five lines: load payload, element 3 (hit T), bitcast, "< 0" (miss), select(miss, 1.0, 0.0) */
    uint32_t l1 = line_end(s, n, p) + 1, l2 = line_end(s, n, l1) + 1, l3 = line_end(s, n, l2) + 1, l4 = line_end(s, n, l3) + 1, l5 = line_end(s, n, l4) + 1;
    if (find(s, n, " = load <4 x i32>, ", l1, dc_le(s, n, l1)) == NOPOS || find(s, n, ", i32 3", l2, dc_le(s, n, l2)) == NOPOS ||
        find(s, n, " = extractelement <4 x i32> ", l2, dc_le(s, n, l2)) == NOPOS || find(s, n, " = bitcast i32 ", l3, dc_le(s, n, l3)) == NOPOS ||
        find(s, n, " = fcmp fast olt float ", l4, dc_le(s, n, l4)) == NOPOS || find(s, n, ", 0.000000e+00", l4, dc_le(s, n, l4)) == NOPOS)
      continue;
    uint32_t q = find(s, n, ", float 1.000000e+00, float 0.000000e+00", l5, dc_le(s, n, l5));
    if (q == NOPOS || find(s, n, " = select i1 %", l5, q) == NOPOS || q + slen(", float 1.000000e+00, float 0.000000e+00") != dc_le(s, n, l5)) continue;
    uint32_t tn; const char *t;
    static const char nm[] = ", i32 %urt_sc_mask, i32 0, i32 4, i32 0, float ", om[] = ", i32 255, i32 0, i32 4, i32 0, float ";
    static const char nv[] = ", float %urt_sc_w, float 0.000000e+00", ov[] = ", float 1.000000e+00, float 0.000000e+00";
    t = dc_str(E, nm, slen(nm), "", 0, "", 0, &tn);
    if (!dc_add(E, m, slen(om), t, tn)) SCF("edit list full");
    t = dc_str(E, nv, slen(nv), "", 0, "", 0, &tn);
    if (!dc_add(E, q, slen(ov), t, tn)) SCF("edit list full");
    sites++;
  }
  if (sites != 3) SCF("not exactly three directional shadow-ray sites (trace + visibility select) in the export");
  char buf[1600]; uint32_t k = 0;
#define SC_A(str) do { const char *z_ = (str); while (*z_) { if (k + 1 >= sizeof buf) SCF("buffer"); buf[k++] = *z_++; } } while (0)
  SC_A("  %urt_sc_l0 = lshr i32 %rpt_raw, 20\n  %urt_sc_l = and i32 %urt_sc_l0, 3\n  %urt_sc_on = icmp eq i32 %urt_sc_l, 1\n");
  SC_A("  %urt_sc_fcb = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle "); SC_A(cam);
  SC_A(", i32 13)\n  %urt_sc_f = extractvalue %dx.types.CBufRet.i32 %urt_sc_fcb, 3\n");
  SC_A("  %urt_sc_yh = lshr i32 "); SC_A(y); SC_A(", 1\n  %urt_sc_fh = lshr i32 %urt_sc_f, 1\n");
  SC_A("  %urt_sc_a = xor i32 "); SC_A(x); SC_A(", %urt_sc_yh\n  %urt_sc_b = xor i32 %urt_sc_a, %urt_sc_fh\n");
  SC_A("  %urt_sc_c = and i32 %urt_sc_b, 1\n  %urt_sc_odd = icmp ne i32 %urt_sc_c, 0\n  %urt_sc_skip = and i1 %urt_sc_on, %urt_sc_odd\n");
  SC_A("  %urt_sc_mask = select i1 %urt_sc_skip, i32 0, i32 255\n");
  SC_A("  %urt_sc_w0 = select i1 %urt_sc_on, float 2.000000e+00, float 1.000000e+00\n");
  SC_A("  %urt_sc_w = select i1 %urt_sc_skip, float 0.000000e+00, float %urt_sc_w0\n");
#undef SC_A
  uint32_t tn; const char *t = dc_str(E, buf, k, "", 0, "", 0, &tn);
  if (!dc_add(E, line_start(s, sel), 0, t, tn)) SCF("edit list full");
  return 1;
#undef SCF
}
int urt_sc_patch_shadow(const char *src, uint32_t n, char *out, uint32_t cap, uint32_t *outn) {
  if (!src || !out || !outn || !n) return 0;
  *outn = 0;
  if (find(src, n, "%urt_sc_", 0, n) != NOPOS || find(src, n, "%urt_bc_nb", 0, n) == NOPOS)
    return dg("module 3 text pass", "library already carries module 3, or module 1 is missing", __LINE__);
  DcEds *E = (DcEds *)b_alloc(sizeof(DcEds));
  int ok = E && sc_one(src, n, k_fns[0], E) && sc_one(src, n, k_fns[1], E) && dc_cmp_apply(src, n, E, out, cap, outn);
  b_free(E);
  if (!ok) *outn = 0;
  return ok;
}

/* ---------------- module 4 (R12 FIX3): bounce-vertex local-light shadow coverage ---------------- */
/* STATIC: at a bounce hit each PT export traces one local-light (point / spot) shadow ray per shading path, with tmax =
 * the light distance (not the directional 1e28 or the bounce ray's limit):
 *   site S (flags 28): V = select(miss, 1.0, 0.0) multiplies that light's contribution (like module 3);
 *   site B (flags 12): "br miss, %LIT, %MERGE" - the whole light evaluation runs only when visible; its one result
 *     factor (the line "%F = fdiv fast float %I, %P" right after "%P = max(..)" at the top of %LIT) scales every output
 *     of that block and is used exactly once.
 * Module 4, for pixels outside this frame's pattern: instance mask 0 (nothing traversed) and the light counts as not
 * visible (site S: V = 0; site B: branch to %MERGE, so the light evaluation is skipped as well). Traced pixels weight the
 * light x2 (site S: V = 2; site B: F x 2 through a phi that is 1 for %LIT's other, untraced predecessors). Pattern
 * ((x >> 1) ^ y ^ (frame >> 1)) & 1: 2 of 4 frames per pixel, unbiased also within module 2's kept frames. Level in bits
 * 22-23 (1 = 50 %); level 0 = mask 255, weight 1, branch unchanged. Built only when the option is set. */
static int lc_one(const char *s, uint32_t n, const char *fn, DcEds *E) {
#define LCF(w) return dg("module 4 (local-light shadow coverage) text pass", w, __LINE__)
  uint32_t fs, fe;
  char cam[48], x[48], y[48], tb[48], lit[48], mrg[48], cmp[48];
  if (!fn_range(s, n, fn, &fs, &fe)) LCF("PT export not found");
  if (!unique_def(s, n, fs, fe, "%dx.types.ResourceProperties { i32 13, i32 1632 })", cam, sizeof cam) ||
      !unique_def(s, n, fs, fe, "@dx.op.dispatchRaysIndex.i32(i32 145, i8 0)", x, sizeof x) ||
      !unique_def(s, n, fs, fe, "@dx.op.dispatchRaysIndex.i32(i32 145, i8 1)", y, sizeof y)) LCF("camera / dispatch index anchors");
  uint32_t sel = find(s, n, ", i32 %rpt_nh, i32 %urt_bc_nb", fs, fe);
  if (sel == NOPOS) LCF("module 1 selector not found");
  unsigned ns = 0, nb = 0;
  static const char m12[] = ", i32 12, i32 255, i32 0, i32 4, i32 0, float ", m28[] = ", i32 28, i32 255, i32 0, i32 4, i32 0, float ";
  for (uint32_t p = fs; (p = find(s, n, "@dx.op.traceRay.struct.GbufferPayload(i32 157, %dx.types.Handle ", p, fe)) != NOPOS; p++) {
    uint32_t le = dc_le(s, n, p);
    if (find(s, n, ", float 0x4589D971E0000000, ", p, le) != NOPOS || find(s, n, ", float 0x415FFFFFC0000000, ", p, le) != NOPOS) continue;
    uint32_t m = find(s, n, m12, p, le); const int is12 = m != NOPOS;
    if (!is12) m = find(s, n, m28, p, le);
    if (m == NOPOS) continue;
    uint32_t l1 = line_end(s, n, p) + 1, l2 = line_end(s, n, l1) + 1, l3 = line_end(s, n, l2) + 1, l4 = line_end(s, n, l3) + 1, l5 = line_end(s, n, l4) + 1;
    if (find(s, n, " = load <4 x i32>, ", l1, dc_le(s, n, l1)) == NOPOS || find(s, n, ", i32 3", l2, dc_le(s, n, l2)) == NOPOS ||
        find(s, n, " = extractelement <4 x i32> ", l2, dc_le(s, n, l2)) == NOPOS || find(s, n, " = bitcast i32 ", l3, dc_le(s, n, l3)) == NOPOS ||
        find(s, n, " = fcmp fast olt float ", l4, dc_le(s, n, l4)) == NOPOS || find(s, n, ", 0.000000e+00", l4, dc_le(s, n, l4)) == NOPOS)
      LCF("local-light shadow ray not followed by the miss test");
    uint32_t c0 = l4; while (s[c0] == ' ') c0++;
    uint32_t cl = dc_tok(s, n, c0);
    if (!cl || cl + 1 > sizeof cmp) LCF("miss-test name");
    for (uint32_t i = 0; i < cl; i++) cmp[i] = s[c0 + i]; cmp[cl] = 0;
    uint32_t tn; const char *t;
    static const char nm[] = ", i32 %urt_lc_mask, i32 0, i32 4, i32 0, float ";
    char nmk[64]; uint32_t k = 0; { const char *fl = is12 ? ", i32 12" : ", i32 28"; for (uint32_t i = 0; fl[i]; i++) nmk[k++] = fl[i]; for (uint32_t i = 0; nm[i]; i++) nmk[k++] = nm[i]; nmk[k] = 0; }
    t = dc_str(E, nmk, k, "", 0, "", 0, &tn);
    if (!dc_add(E, m, slen(m12), t, tn)) LCF("edit list full");
    if (!is12) {                                                          /* site S */
      uint32_t q = find(s, n, ", float 1.000000e+00, float 0.000000e+00", l5, dc_le(s, n, l5));
      if (q == NOPOS || find(s, n, " = select i1 ", l5, q) == NOPOS || find(s, n, cmp, l5, q) == NOPOS || q + slen(", float 1.000000e+00, float 0.000000e+00") != dc_le(s, n, l5))
        LCF("site S: visibility select not found");
      static const char nv[] = ", float %urt_lc_w, float 0.000000e+00";
      t = dc_str(E, nv, slen(nv), "", 0, "", 0, &tn);
      if (!dc_add(E, q, slen(", float 1.000000e+00, float 0.000000e+00"), t, tn)) LCF("edit list full");
      ns++;
      continue;
    }
    /* site B: "br i1 %cmp, label %LIT, label %MERGE" */
    uint32_t b0 = l5; while (s[b0] == ' ') b0++;
    if (!eq_at(s, n, b0, "br i1 ") || !eq_at(s, n, b0 + 6, cmp) || !eq_at(s, n, b0 + 6 + cl, ", label "))
      LCF("site B: branch on the miss test not found");
    uint32_t lp = b0 + 6 + cl + 8, ll = dc_tok(s, n, lp);
    if (!ll || ll + 1 > sizeof lit || !eq_at(s, n, lp + ll, ", label ")) LCF("site B: branch targets");
    for (uint32_t i = 0; i < ll; i++) lit[i] = s[lp + i]; lit[ll] = 0;
    uint32_t mp = lp + ll + 8, ml = dc_tok(s, n, mp);
    if (!ml || ml + 1 > sizeof mrg || mp + ml != dc_le(s, n, b0)) LCF("site B: branch targets");
    for (uint32_t i = 0; i < ml; i++) mrg[i] = s[mp + i]; mrg[ml] = 0;
    if (dc_block(s, fs, p, tb, sizeof tb) == NOPOS) LCF("site B: trace block label");
    /* the branch: go = miss AND kept */
    char g1[160]; uint32_t gk = 0;
    { const char *a = "  %urt_lc_go = and i1 "; for (uint32_t i = 0; a[i]; i++) g1[gk++] = a[i];
      for (uint32_t i = 0; i < cl; i++) g1[gk++] = cmp[i];
      a = ", %urt_lc_keep\n"; for (uint32_t i = 0; a[i]; i++) g1[gk++] = a[i]; }
    t = dc_str(E, g1, gk, "", 0, "", 0, &tn);
    if (!dc_add(E, line_start(s, b0), 0, t, tn)) LCF("edit list full");
    t = dc_str(E, "%urt_lc_go", 10, "", 0, "", 0, &tn);
    if (!dc_add(E, b0 + 6, cl, t, tn)) LCF("edit list full");
    /* %LIT: weight phi first, then the result factor F = I / P becomes (I / P) x w */
    uint32_t lab = dc_label(s, n, fs, fe, lit);
    if (lab == NOPOS) LCF("site B: light block label");
    uint32_t lline = lab + 1, lend = dc_le(s, n, lline), pr = find(s, n, "; preds = ", lline, lend);
    if (pr == NOPOS) LCF("site B: light block predecessors");
    char ph[1024]; uint32_t pk = 0; unsigned npred = 0, self = 0;
    { const char *a = "  %urt_lc_wb = phi float "; for (uint32_t i = 0; a[i]; i++) ph[pk++] = a[i]; }
    for (uint32_t q = pr + 10; q < lend;) {
      uint32_t tl = dc_tok(s, n, q);
      if (!tl || pk + tl + 40 > sizeof ph) LCF("site B: predecessor list");
      const int me = tl == slen(tb) && eq_at(s, n, q, tb);
      const char *a = npred ? ", [ " : "[ "; for (uint32_t i = 0; a[i]; i++) ph[pk++] = a[i];
      a = me ? "%urt_lc_w2, " : "1.000000e+00, "; for (uint32_t i = 0; a[i]; i++) ph[pk++] = a[i];
      for (uint32_t i = 0; i < tl; i++) ph[pk++] = s[q + i];
      ph[pk++] = ' '; ph[pk++] = ']';
      npred++; self += me ? 1u : 0u; q += tl;
      if (q < lend && s[q] == ',') q++;
      while (q < lend && s[q] == ' ') q++;
    }
    if (self != 1u || npred < 2u) LCF("site B: the trace block is not exactly one predecessor of the light block");
    ph[pk++] = '\n';
    t = dc_str(E, ph, pk, "", 0, "", 0, &tn);
    uint32_t body = line_end(s, n, lline) + 1;
    if (!dc_add(E, body, 0, t, tn)) LCF("edit list full");
    uint32_t i1 = body, i2 = line_end(s, n, i1) + 1;
    if (find(s, n, " = call float @dx.op.binary.f32(i32 35, float ", i1, dc_le(s, n, i1)) == NOPOS || find(s, n, " = fdiv fast float ", i2, dc_le(s, n, i2)) == NOPOS)
      LCF("site B: light block does not start with the result factor");
    uint32_t f0 = i2; while (s[f0] == ' ') f0++;
    uint32_t fl = dc_tok(s, n, f0);
    if (!fl || !eq_at(s, n, f0 + fl, " = fdiv fast float ")) LCF("site B: result factor");
    { char fn_[48]; if (fl + 2 > sizeof fn_) LCF("site B: result factor"); for (uint32_t i = 0; i < fl; i++) fn_[i] = s[f0 + i]; fn_[fl] = 0;
      /* used exactly once besides its definition (whole-token count) */
      unsigned uses = 0;
      for (uint32_t q = fs; (q = find(s, n, fn_, q, fe)) != NOPOS; q += fl) {
        const char c = s[q + fl];
        if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '.') continue;
        uses++;
      }
      if (uses != 2u) LCF("site B: the result factor is not used exactly once");
      t = dc_str(E, "%urt_lc_q", 9, "", 0, "", 0, &tn);
      if (!dc_add(E, f0, fl, t, tn)) LCF("edit list full");
      char ml_[96]; uint32_t mk = 0;
      { const char *a = "  "; for (uint32_t i = 0; a[i]; i++) ml_[mk++] = a[i]; for (uint32_t i = 0; i < fl; i++) ml_[mk++] = fn_[i];
        a = " = fmul fast float %urt_lc_q, %urt_lc_wb\n"; for (uint32_t i = 0; a[i]; i++) ml_[mk++] = a[i]; }
      t = dc_str(E, ml_, mk, "", 0, "", 0, &tn);
      if (!dc_add(E, line_end(s, n, i2) + 1, 0, t, tn)) LCF("edit list full"); }
    (void)mrg;
    nb++;
  }
  if (ns != 1u || nb != 1u) LCF("not exactly one local-light shadow site of each kind (select / branch) in the export");
  char buf[1600]; uint32_t k = 0;
#define LC_A(str) do { const char *z_ = (str); while (*z_) { if (k + 1 >= sizeof buf) LCF("buffer"); buf[k++] = *z_++; } } while (0)
  LC_A("  %urt_lc_l0 = lshr i32 %rpt_raw, 22\n  %urt_lc_l = and i32 %urt_lc_l0, 3\n  %urt_lc_on = icmp eq i32 %urt_lc_l, 1\n");
  LC_A("  %urt_lc_fcb = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle "); LC_A(cam);
  LC_A(", i32 13)\n  %urt_lc_f = extractvalue %dx.types.CBufRet.i32 %urt_lc_fcb, 3\n");
  LC_A("  %urt_lc_xh = lshr i32 "); LC_A(x); LC_A(", 1\n  %urt_lc_fh = lshr i32 %urt_lc_f, 1\n");
  LC_A("  %urt_lc_a = xor i32 %urt_lc_xh, "); LC_A(y); LC_A("\n  %urt_lc_b = xor i32 %urt_lc_a, %urt_lc_fh\n");
  LC_A("  %urt_lc_c = and i32 %urt_lc_b, 1\n  %urt_lc_odd = icmp ne i32 %urt_lc_c, 0\n  %urt_lc_skip = and i1 %urt_lc_on, %urt_lc_odd\n");
  LC_A("  %urt_lc_keep = xor i1 %urt_lc_skip, true\n");
  LC_A("  %urt_lc_mask = select i1 %urt_lc_skip, i32 0, i32 255\n");
  LC_A("  %urt_lc_w2 = select i1 %urt_lc_on, float 2.000000e+00, float 1.000000e+00\n");
  LC_A("  %urt_lc_w = select i1 %urt_lc_skip, float 0.000000e+00, float %urt_lc_w2\n");
#undef LC_A
  uint32_t tn; const char *t = dc_str(E, buf, k, "", 0, "", 0, &tn);
  if (!dc_add(E, line_start(s, sel), 0, t, tn)) LCF("edit list full");
  return 1;
#undef LCF
}
int urt_lc_patch_local(const char *src, uint32_t n, char *out, uint32_t cap, uint32_t *outn) {
  if (!src || !out || !outn || !n) return 0;
  *outn = 0;
  if (find(src, n, "%urt_lc_", 0, n) != NOPOS || find(src, n, "%urt_bc_nb", 0, n) == NOPOS)
    return dg("module 4 text pass", "library already carries module 4, or module 1 is missing", __LINE__);
  DcEds *E = (DcEds *)b_alloc(sizeof(DcEds));
  int ok = E && lc_one(src, n, k_fns[0], E) && lc_one(src, n, k_fns[1], E) && dc_cmp_apply(src, n, E, out, cap, outn);
  b_free(E);
  if (!ok) *outn = 0;
  return ok;
}
int urt_dc_patch_diffuse(const char *src, uint32_t n, char *out, uint32_t cap, uint32_t *outn) {
  if (!src || !out || !outn || !n) return 0;
  *outn = 0;
  if (find(src, n, "%urt_dc_", 0, n) != NOPOS || find(src, n, "%urt_bc_nb", 0, n) == NOPOS)   /* once; after module 1 */
    return dg("module 2 text pass", "library already carries module 2, or module 1 is missing", __LINE__);
  DcEds *E = (DcEds *)b_alloc(sizeof(DcEds));
  int ok = E && dc_one(src, n, k_fns[0], E) && dc_one(src, n, k_fns[1], E) && dc_cmp_apply(src, n, E, out, cap, outn);
  b_free(E);
  if (!ok) *outn = 0;
  return ok;
}

/* ---------------- DXC pipeline (same interfaces as URT's PT library build) ---------------- */
static uint32_t rd32(const uint8_t *s) { return (uint32_t)s[0] | (uint32_t)s[1] << 8 | (uint32_t)s[2] << 16 | (uint32_t)s[3] << 24; }
static void wr32(uint8_t *s, uint32_t v) { for (unsigned i = 0; i < 4; i++) s[i] = (uint8_t)(v >> (8 * i)); }
typedef struct { uint32_t d1; uint16_t d2, d3; uint8_t d4[8]; } BGUID;
typedef struct BBlob BBlob; typedef struct BLib BLib; typedef struct BComp BComp; typedef struct BAsm BAsm; typedef struct BVal BVal; typedef struct BOp BOp;
struct BBlobV { long (BWIN *qi)(BBlob *, const BGUID *, void **); unsigned long (BWIN *add)(BBlob *); unsigned long (BWIN *rel)(BBlob *);
  void *(BWIN *ptr)(BBlob *); size_t (BWIN *size)(BBlob *); };
struct BBlob { struct BBlobV *v; };
struct BLibV { void *qi, *add; unsigned long (BWIN *rel)(BLib *); void *setmalloc, *fromblob, *fromfile;
  long (BWIN *pinned)(BLib *, const void *, uint32_t, uint32_t, BBlob **); };
struct BLib { struct BLibV *v; };
struct BCompV { void *qi, *add; unsigned long (BWIN *rel)(BComp *); void *compile, *pre; long (BWIN *dis)(BComp *, BBlob *, BBlob **); };
struct BComp { struct BCompV *v; };
struct BAsmV { void *qi, *add; unsigned long (BWIN *rel)(BAsm *); long (BWIN *assemble)(BAsm *, BBlob *, BOp **); };
struct BAsm { struct BAsmV *v; };
struct BValV { void *qi, *add; unsigned long (BWIN *rel)(BVal *); long (BWIN *validate)(BVal *, BBlob *, uint32_t, BOp **); };
struct BVal { struct BValV *v; };
struct BOpV { void *qi, *add; unsigned long (BWIN *rel)(BOp *); long (BWIN *status)(BOp *, long *); long (BWIN *result)(BOp *, BBlob **);
               long (BWIN *errors)(BOp *, BBlob **); };   /* IDxcOperationResult: GetStatus, GetResult, GetErrorBuffer */
struct BOp { struct BOpV *v; };
static const BGUID C_LIB = {0x6245d6af, 0x66e0, 0x48fd, {0x80, 0xb4, 0x4d, 0x27, 0x17, 0x96, 0x74, 0x8c}};
static const BGUID I_LIB = {0xe5204dc7, 0xd18c, 0x4c3c, {0xbd, 0xfb, 0x85, 0x16, 0x73, 0x98, 0x0f, 0xe7}};
static const BGUID C_COMP = {0x73e22d93, 0xe6ce, 0x47f3, {0xb5, 0xbf, 0xf0, 0x66, 0x4f, 0x39, 0xc1, 0xb0}};
static const BGUID I_COMP = {0x8c210bf3, 0x011f, 0x4422, {0x8d, 0x70, 0x6f, 0x9a, 0xcb, 0x8d, 0xb6, 0x17}};
static const BGUID C_ASM = {0xd728db68, 0xf903, 0x4f80, {0x94, 0xcd, 0xdc, 0xcf, 0x76, 0xec, 0x71, 0x51}};
static const BGUID I_ASM = {0x091f7a26, 0x1c1f, 0x4948, {0x90, 0x4b, 0xe6, 0xe3, 0xa8, 0xa7, 0x71, 0xd5}};
static const BGUID C_VAL = {0x8ca3e215, 0xf728, 0x4cf3, {0x8c, 0xdd, 0x88, 0xaf, 0x91, 0x75, 0x87, 0xa1}};
static const BGUID I_VAL = {0xa6e82bd2, 0x1fd7, 0x4826, {0x98, 0x11, 0x28, 0x57, 0xe7, 0x97, 0xf4, 0x9a}};
#define FOURCC(a, b, c, d) ((uint32_t)(a) | (uint32_t)(b) << 8 | (uint32_t)(c) << 16 | (uint32_t)(d) << 24)
static int part(const uint8_t *c, uint32_t n, uint32_t cc, const uint8_t **out, uint32_t *outn) {
  if (n < 32 || rd32(c + 24) != n) return 0;
  uint32_t k = rd32(c + 28);
  if (!k || k > 64 || 32u + 4u * k > n) return 0;
  for (uint32_t i = 0; i < k; i++) {
    uint32_t o = rd32(c + 32 + 4 * i);
    if (o > n - 8 || rd32(c + o + 4) > n - o - 8) return 0;
    if (rd32(c + o) == cc) { *out = c + o + 8; *outn = rd32(c + o + 4); return 1; }
  }
  return 0;
}
/* the input container with its DXIL and HASH parts taken from the assembled one; every other part byte-exact */
static uint8_t *compose(const uint8_t *orig, uint32_t on, const uint8_t *asmb, uint32_t an, uint32_t *outn) {
  const uint8_t *hp, *dp; uint32_t hn, dn;
  if (on < 32 || rd32(orig + 24) != on) return 0;
  if (!part(asmb, an, FOURCC('H', 'A', 'S', 'H'), &hp, &hn) || !part(asmb, an, FOURCC('D', 'X', 'I', 'L'), &dp, &dn)) return 0;
  uint32_t k = rd32(orig + 28), total = (32u + 4u * k + 3u) & ~3u, seen = 0;
  if (!k || k > 64) return 0;
  for (uint32_t i = 0; i < k; i++) {
    uint32_t o = rd32(orig + 32 + 4 * i), cc = rd32(orig + o), sz = rd32(orig + o + 4);
    if (cc == FOURCC('H', 'A', 'S', 'H')) { sz = hn; seen |= 1; } else if (cc == FOURCC('D', 'X', 'I', 'L')) { sz = dn; seen |= 2; }
    total += (8u + sz + 3u) & ~3u;
  }
  if (seen != 3 || total > (1u << 20)) return 0;
  uint8_t *b = (uint8_t *)b_alloc(total);
  if (!b) return 0;
  for (unsigned i = 0; i < 4; i++) b[i] = orig[i];
  for (unsigned i = 20; i < 24; i++) b[i] = orig[i];
  wr32(b + 24, total); wr32(b + 28, k);
  uint32_t cur = (32u + 4u * k + 3u) & ~3u;
  for (uint32_t i = 0; i < k; i++) {
    uint32_t o = rd32(orig + 32 + 4 * i), cc = rd32(orig + o), sz = rd32(orig + o + 4);
    const uint8_t *src = orig + o + 8;
    if (cc == FOURCC('H', 'A', 'S', 'H')) { src = hp; sz = hn; } else if (cc == FOURCC('D', 'X', 'I', 'L')) { src = dp; sz = dn; }
    wr32(b + 32 + 4 * i, cur); wr32(b + cur, cc); wr32(b + cur + 4, sz);
    for (uint32_t j = 0; j < sz; j++) b[cur + 8 + j] = src[j];
    cur += (8u + sz + 3u) & ~3u;
  }
  *outn = total;
  return b;
}
static int same_kept_parts(const uint8_t *a, uint32_t an, const uint8_t *b, uint32_t bn) {
  uint32_t k = rd32(a + 28);
  if (k != rd32(b + 28)) return 0;
  for (uint32_t i = 0; i < k; i++) {
    uint32_t oa = rd32(a + 32 + 4 * i), ob = rd32(b + 32 + 4 * i);
    if (oa > an - 8 || ob > bn - 8 || rd32(a + oa) != rd32(b + ob)) return 0;
    uint32_t cc = rd32(a + oa), sa = rd32(a + oa + 4), sb = rd32(b + ob + 4);
    if (cc == FOURCC('H', 'A', 'S', 'H') || cc == FOURCC('D', 'X', 'I', 'L')) continue;
    if (sa != sb || sa > an - oa - 8 || sb > bn - ob - 8) return 0;
    for (uint32_t j = 0; j < sa; j++) if (a[oa + 8 + j] != b[ob + 8 + j]) return 0;
  }
  return 1;
}
/* both exports carry both edits after assembly, signing and validation */
static int verified(const char *c, uint32_t n, int lc) {
  static const struct { const char *q; unsigned want; const char *what; } k[] = {
    {"%urt_bc_nb = select i1 %urt_bc_sel, i32 %rpt_nb, i32 %urt_bc_lim", 1, "module 1 per-pixel bounce limit"},
    {", i32 %rpt_nh, i32 %urt_bc_nb", 1, "module 1 selector operand"},
    {"%hlf_on = and i1 %hlf_hair, %hlf_en", 1, "Orange Hair Fix hair patch"},
    {"%urt_dc_skip = and i1 %urt_dc_a, %urt_dc_drop", 1, "module 2 drop condition"},
    {"br i1 %urt_dc_skip, label %urt_dc_exit, label %urt_dc_go", 1, "module 2 drop branch"},
    {"%urt_dc_o2 = fmul fast float ", 1, "module 2 diffuse weight"},
    {"float %urt_dc_o0, float %urt_dc_o1, float %urt_dc_o2", 1, "module 2 scaled diffuse store"},
    {"%rpt_p2_apply", 1, "URT's PT library edits"},
    {"%urt_sc_mask = select i1 %urt_sc_skip, i32 0, i32 255", 1, "module 3 shadow-ray mask"},
    {", i32 %urt_sc_mask, i32 0, i32 4, i32 0, float ", 3, "module 3 masked shadow rays"},
    {", float %urt_sc_w, float 0.000000e+00", 3, "module 3 weighted visibility"},
    /* R12 FIX3 module 4 (only when built): checked when lc */
    {"%urt_lc_mask = select i1 %urt_lc_skip, i32 0, i32 255", 1, "module 4 shadow-ray mask"},
    {", i32 %urt_lc_mask, i32 0, i32 4, i32 0, float ", 2, "module 4 masked local-light shadow rays"},
    {", float %urt_lc_w, float 0.000000e+00", 1, "module 4 weighted visibility (select site)"},
    {"%urt_lc_go = and i1 ", 1, "module 4 skipped light evaluation (branch site)"},
    {" = fmul fast float %urt_lc_q, %urt_lc_wb", 1, "module 4 weighted light result (branch site)"}};
  const unsigned nk = lc ? (unsigned)(sizeof k / sizeof k[0]) : 11u;
  for (unsigned f = 0; f < 2; f++) {
    uint32_t fs, fe, first;
    if (!fn_range(c, n, k_fns[f], &fs, &fe)) return dg("10 re-check", f ? "PTRGSWithSERFinal export not found" : "PTRGSFinal export not found", __LINE__);
    for (unsigned i = 0; i < nk; i++) {
      unsigned got = count(c, n, k[i].q, fs, fe, &first);
      if (i >= 11u && !lc) break;
      if (i == 7 ? got < 1 : got != k[i].want) return dg(f ? "10 re-check (PTRGSWithSERFinal)" : "10 re-check (PTRGSFinal)", k[i].what, __LINE__);
    }
    /* the drop branch's exit block jumps straight to the bounce loop's exit (nothing traced for a dropped sample) */
    uint32_t x = find(c, n, "\nurt_dc_exit:", fs, fe);
    if (x == NOPOS || find(c, n, "\n  br label %", x + 1, fe) != find(c, n, "\n", x + 1, fe))
      return dg(f ? "10 re-check (PTRGSWithSERFinal)" : "10 re-check (PTRGSFinal)", "module 2 exit block", __LINE__);
  }
  return 1;
}
/* the DXC operation's error text (assembler / validator messages), first line(s), into the diagnostic */
static void dg_dxc(const char *stage, const char *what, long hr, BOp *op, unsigned line) {
  if (g_diag[0]) return;
  unsigned k = 0; dg_put(&k, stage); dg_put(&k, ": "); dg_put(&k, what); dg_put(&k, " (HRESULT "); dg_num(&k, (unsigned long)(uint32_t)hr, 1); dg_put(&k, ")");
  BBlob *e = 0;
  if (op && op->v->errors && op->v->errors(op, &e) >= 0 && e) {
    const char *t = (const char *)e->v->ptr(e); size_t tn = e->v->size(e);
    if (t && tn) {
      dg_put(&k, ": ");
      for (size_t i = 0; i < tn && t[i] && k + 1 < 400u; i++) g_diag[k++] = (t[i] == '\r' || t[i] == '\n') ? ' ' : t[i];
      g_diag[k] = 0;
    }
    e->v->rel(e);
  }
  dg_put(&k, " (urt_bounce501.c:"); dg_num(&k, line, 0); dg_put(&k, ")");
}
#define DG(stage, what) do { (void)dg(stage, what, __LINE__); goto done; } while (0)
int urt_bc_build_variant(const uint8_t *orig, uint32_t on, UrtBcDxcCreate create, uint8_t **out, uint32_t *outn) {
  return urt_bc_build_variant_lv(orig, on, create, out, outn, 0u);
}
/* R12 FIX3: the packed level decides whether module 4 is built (bits 6-7 not 0); modules 1-3 as in R12 FIX2 */
int urt_bc_build_variant_lv(const uint8_t *orig, uint32_t on, UrtBcDxcCreate create, uint8_t **out, uint32_t *outn, unsigned level) {
  const int lc = ((level >> 6) & 3u) != 0u;
  BLib *lib = 0; BComp *comp = 0; BAsm *as = 0; BVal *val = 0;
  BBlob *src = 0, *dis = 0, *ir = 0, *asmb = 0, *cb = 0, *signedb = 0, *check = 0; BOp *op = 0, *vop = 0;
  char *t1 = 0, *t2 = 0; uint8_t *composed = 0; int ok = 0; long st = 0, hr;
  g_diag[0] = 0;
  if (!out || !outn) return dg("input", "no output", __LINE__);
  *out = 0; *outn = 0;
  if (!orig || !create || on < 65536u || on > 1048576u || orig[0] != 'D' || orig[1] != 'X' || orig[2] != 'B' || orig[3] != 'C' || rd32(orig + 24) != on)
    return dg("input", "URT's PT library is not a DXBC container of the expected size", __LINE__);
  if (create(&C_LIB, &I_LIB, (void **)&lib) < 0 || !lib) DG("1 compiler", "DxcCreateInstance(DxcLibrary) failed");
  if (create(&C_COMP, &I_COMP, (void **)&comp) < 0 || !comp) DG("1 compiler", "DxcCreateInstance(DxcCompiler) failed");
  if (create(&C_ASM, &I_ASM, (void **)&as) < 0 || !as) DG("1 compiler", "DxcCreateInstance(DxcAssembler) failed");
  if (create(&C_VAL, &I_VAL, (void **)&val) < 0 || !val) DG("1 compiler", "DxcCreateInstance(DxcValidator) failed");
  if (lib->v->pinned(lib, orig, on, 0, &src) < 0 || !src) DG("2 disassembly", "CreateBlobWithEncodingFromPinned failed");
  if ((hr = comp->v->dis(comp, src, &dis)) < 0 || !dis) { dg_dxc("2 disassembly", "Disassemble of URT's PT library failed", hr, 0, __LINE__); goto done; }
  {
    const char *d = (const char *)dis->v->ptr(dis); size_t dn = dis->v->size(dis);
    if (!d || dn < 4096 || dn > (1u << 26)) DG("2 disassembly", "disassembly text missing or of implausible size");
    while (dn && d[dn - 1] == 0) dn--;
    uint32_t cap = (uint32_t)dn + 262144u, n1 = 0, n2 = 0, n3 = 0; unsigned sites = 0;
    t1 = (char *)b_alloc(cap); t2 = (char *)b_alloc(cap);
    if (!t1 || !t2) DG("3 text passes", "out of memory");
    if (!urt_bc_patch_coverage(d, (uint32_t)dn, t1, cap, &n1)) { (void)dg("3 text passes", "module 1 failed", __LINE__); goto done; }
    if (!urt_ohf_bake(t1, n1, t2, cap, &n2, &sites) || sites < 2) { (void)dg("4 Orange Hair Fix hair patch", "failed", __LINE__); goto done; }
    /* R12 module 2 after the hair patch (Orange Hair Fix's anchors see exactly the text its own tool would see) */
    if (!urt_dc_patch_diffuse(t2, n2, t1, cap, &n3)) { (void)dg("5 module 2 text pass", "failed", __LINE__); goto done; }
    uint32_t n4 = 0;   /* R12 FIX2: module 3 after module 2 */
    if (!urt_sc_patch_shadow(t1, n3, t2, cap, &n4)) { (void)dg("5b module 3 text pass", "failed", __LINE__); goto done; }
    if (lc) {   /* R12 FIX3: module 4 only when PT: Bounce Local Light Shadow Coverage is set (else the R12 FIX2 text exactly) */
      uint32_t n5 = 0;
      if (!urt_lc_patch_local(t2, n4, t1, cap, &n5)) { (void)dg("5c module 4 text pass", "failed", __LINE__); goto done; }
      char *tt = t1; t1 = t2; t2 = tt; n4 = n5;
    }
    if (lib->v->pinned(lib, t2, n4, 65001u, &ir) < 0 || !ir) DG("6 assembly", "blob for the edited text failed");
  }
  if ((hr = as->v->assemble(as, ir, &op)) < 0 || !op) { dg_dxc("6 assembly", "AssembleToContainer call failed", hr, op, __LINE__); goto done; }
  if (op->v->status(op, &st) < 0 || st < 0) { dg_dxc("6 assembly", "the edited text did not assemble", st, op, __LINE__); goto done; }
  if (op->v->result(op, &asmb) < 0 || !asmb) DG("6 assembly", "no assembled container");
  {
    const uint8_t *a = (const uint8_t *)asmb->v->ptr(asmb); size_t an = asmb->v->size(asmb); uint32_t cn = 0;
    if (!a || an < 65536 || an > (1u << 20) || rd32(a + 24) != (uint32_t)an) DG("6 assembly", "assembled container of implausible size");
    composed = compose(orig, on, a, (uint32_t)an, &cn);
    if (!composed) DG("7 container", "could not compose the variant from URT's library parts");
    if (!same_kept_parts(orig, on, composed, cn)) DG("7 container", "a kept container part differs from URT's library");
    if (lib->v->pinned(lib, composed, cn, 0, &cb) < 0 || !cb) DG("7 container", "blob for the composed container failed");
  }
  if ((hr = val->v->validate(val, cb, 1u /* in-place sign */, &vop)) < 0 || !vop) { dg_dxc("8 validation", "Validate call failed", hr, vop, __LINE__); goto done; }
  if (vop->v->status(vop, &st) < 0 || st < 0) { dg_dxc("8 validation", "the game's DXC validator rejected the variant", st, vop, __LINE__); goto done; }
  if (vop->v->result(vop, &signedb) < 0 || !signedb) DG("8 validation", "no signed container returned");
  {
    const uint8_t *s = (const uint8_t *)signedb->v->ptr(signedb); size_t sn = signedb->v->size(signedb);
    if (!s || sn < 65536 || sn > (1u << 20) || s[0] != 'D' || s[1] != 'X' || s[2] != 'B' || s[3] != 'C' || rd32(s + 24) != (uint32_t)sn)
      DG("9 signed container", "not a DXBC container of plausible size");
    if (!same_kept_parts(orig, on, s, (uint32_t)sn)) DG("9 signed container", "a kept container part changed during signing");
    int digest = 0;
    for (unsigned i = 4; i < 20; i++) digest |= s[i];
    if (!digest) DG("9 signed container", "unsigned (D3D12 would reject it)");
    if ((hr = comp->v->dis(comp, signedb, &check)) < 0 || !check) { dg_dxc("10 re-check", "disassembly of the signed variant failed", hr, 0, __LINE__); goto done; }
    const char *c = (const char *)check->v->ptr(check); size_t cn = check->v->size(check);
    if (!c || cn > (1u << 26) || !verified(c, (uint32_t)cn, lc))
      DG("10 re-check", "the signed variant does not carry every edit exactly once in both exports");
    uint8_t *h = (uint8_t *)b_alloc(sn);                       /* kept for the whole process */
    if (!h) DG("10 re-check", "out of memory");
    for (size_t i = 0; i < sn; i++) h[i] = s[i];
    *out = h; *outn = (uint32_t)sn; ok = 1;
  }
done:
  if (check) check->v->rel(check);
  if (signedb) signedb->v->rel(signedb);
  if (vop) vop->v->rel(vop);
  if (cb) cb->v->rel(cb);
  if (asmb) asmb->v->rel(asmb);
  if (op) op->v->rel(op);
  if (ir) ir->v->rel(ir);
  if (dis) dis->v->rel(dis);
  if (src) src->v->rel(src);
  if (val) val->v->rel(val);
  if (as) as->v->rel(as);
  if (comp) comp->v->rel(comp);
  if (lib) lib->v->rel(lib);
  if (composed) b_free(composed);
  b_free(t1); b_free(t2);
  return ok;
}
#undef DG
