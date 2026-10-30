/* V5.3.0 R8 EXPERIMENTAL "RT/PT: Solid Distant Foliage" (Patch 5.01). See urt_foliage501.h. */
#include "urt_foliage501.h"
#if defined(_WIN32)
#define FWIN __stdcall
__declspec(dllimport) void *FWIN VirtualAlloc(void *, size_t, unsigned long, unsigned long);
__declspec(dllimport) int FWIN VirtualFree(void *, size_t, unsigned long);
static void *f_alloc(size_t n) { return n ? VirtualAlloc(0, n, 0x1000 | 0x2000, 0x04) : 0; }
static void f_free(void *p) { if (p) (void)VirtualFree(p, 0, 0x8000); }
#else
#define FWIN
#include <stdlib.h>
static void *f_alloc(size_t n) { return n ? calloc(1, n) : 0; }
static void f_free(void *p) { free(p); }
#endif

static uint32_t rd32(const uint8_t *s) { return (uint32_t)s[0] | (uint32_t)s[1] << 8 | (uint32_t)s[2] << 16 | (uint32_t)s[3] << 24; }
static uint64_t rd64(const uint8_t *s) { return (uint64_t)rd32(s) | (uint64_t)rd32(s + 4) << 32; }
static void wr32(uint8_t *s, uint32_t v) { for (unsigned i = 0; i < 4; i++) s[i] = (uint8_t)(v >> (8 * i)); }
static uint32_t slen(const char *s) { uint32_t n = 0; while (s && s[n]) n++; return n; }
static int eq_at(const char *b, uint32_t n, uint32_t at, const char *q) {
  for (uint32_t i = 0; q[i]; i++) if (at + i >= n || b[at + i] != q[i]) return 0;
  return 1;
}
#define NOPOS 0xffffffffu
static uint32_t find(const char *b, uint32_t n, const char *q, uint32_t from, uint32_t to) {
  if (to > n) to = n;
  for (uint32_t i = from; i < to; i++) if (eq_at(b, n, i, q)) return i;
  return NOPOS;
}
static uint32_t line_start(const char *b, uint32_t p) { while (p && b[p - 1] != '\n') p--; return p; }
static uint32_t line_end(const char *b, uint32_t n, uint32_t p) { while (p < n && b[p] != '\n') p++; return p; }
/* the SSA name defined on a line "  %name = ..." (writes "%name") */
static int def_name(const char *b, uint32_t ls, uint32_t le, char *out, uint32_t cap) {
  uint32_t p = ls, k = 0;
  while (p < le && (b[p] == ' ' || b[p] == '\t')) p++;
  if (p >= le || b[p] != '%') return 0;
  while (p < le && b[p] != ' ' && k + 1 < cap) out[k++] = b[p++];
  out[k] = 0;
  return k > 1 && eq_at(b, le, p, " = ");
}
static int put(char *out, uint32_t cap, uint32_t *u, const char *p, uint32_t n) {
  if (*u + n + 1 > cap) return 0;
  for (uint32_t i = 0; i < n; i++) out[(*u)++] = p[i];
  out[*u] = 0;
  return 1;
}
static int puts_(char *out, uint32_t cap, uint32_t *u, const char *s) { return put(out, cap, u, s, slen(s)); }

static const char *const k_dist[5] = {0, "0x403E000000000000", "0x4034000000000000", "0x4024000000000000", "0x4014000000000000"};
#define ALPHA_K ", 0x3F1A36E2E0000000"
int urt_fol_patch_ir(const char *src, uint32_t n, unsigned di, char *out, uint32_t cap, uint32_t *outn) {
  if (!src || !out || !outn || di < 1 || di > 4) return 0;
  *outn = 0;
  if (find(src, n, "%urt_fol", 0, n) != NOPOS) return 0;              /* already patched / foreign */
  const char *fn = "define void @\"\\01?AnyHitTrees@@";
  uint32_t fs = find(src, n, fn, 0, n);
  if (fs == NOPOS || find(src, n, fn, fs + 1, n) != NOPOS) return 0;
  if (line_start(src, fs) != fs) return 0;
  uint32_t fe = find(src, n, "\n}", fs, n);
  if (fe == NOPOS) return 0;
  /* exactly one ray-distance read and exactly one transparent-texel test in the function */
  uint32_t t = find(src, n, " = call float @dx.op.rayTCurrent.f32(i32 154)", fs, fe);
  if (t == NOPOS || find(src, n, " = call float @dx.op.rayTCurrent.f32(i32 154)", t + 1, fe) != NOPOS) return 0;
  uint32_t a = find(src, n, ALPHA_K, fs, fe);
  if (a == NOPOS || find(src, n, ALPHA_K, a + 1, fe) != NOPOS || a < t) return 0;
  uint32_t tl = line_start(src, t), al = line_start(src, a), ae = line_end(src, n, a);
  char tn[48], rn[48], opnd[48];
  if (!def_name(src, tl, line_end(src, n, t), tn, sizeof tn) || !def_name(src, al, ae, rn, sizeof rn)) return 0;
  uint32_t op = find(src, n, " = fcmp fast olt float ", al, ae);
  if (op == NOPOS) return 0;
  op += slen(" = fcmp fast olt float ");
  uint32_t k = 0;
  while (op < a && k + 1 < sizeof opnd) opnd[k++] = src[op++];
  opnd[k] = 0;
  if (op != a || k < 2 || opnd[0] != '%') return 0;
  /* the line must end right after the constant (no metadata we would drop) */
  if (a + slen(ALPHA_K) != ae && !(a + slen(ALPHA_K) + 1 == ae && src[ae - 1] == '\r')) return 0;
  uint32_t u = 0;
  if (!put(out, cap, &u, src, al)) return 0;
  if (!puts_(out, cap, &u, "  %urt_fol_a = fcmp fast olt float ") || !puts_(out, cap, &u, opnd) ||
      !puts_(out, cap, &u, ALPHA_K "\n  %urt_fol_near = fcmp fast ole float ") || !puts_(out, cap, &u, tn) ||
      !puts_(out, cap, &u, ", ") || !puts_(out, cap, &u, k_dist[di]) || !puts_(out, cap, &u, "\n  ") ||
      !puts_(out, cap, &u, rn) || !puts_(out, cap, &u, " = and i1 %urt_fol_a, %urt_fol_near")) return 0;
  if (!put(out, cap, &u, src + ae, n - ae)) return 0;
  *outn = u;
  return 1;
}

static const uint8_t k_digest[16] = {0x9e, 0xee, 0x01, 0xb4, 0x30, 0xd9, 0x0f, 0x82, 0x31, 0xb8, 0xfd, 0x71, 0x0a, 0x1b, 0xb7, 0x4e};
static int wstr_is(const uint8_t *base, uint32_t image, uint64_t va, const char *s) {
  uint64_t b = (uint64_t)(uintptr_t)base;
  if (va < b || va - b >= image) return 0;
  const uint8_t *p = base + (va - b);
  for (uint32_t i = 0; ; i++) {
    if ((va - b) + 2u * i + 1u >= image) return 0;
    if (p[2 * i] != (uint8_t)s[i] || p[2 * i + 1] != 0) return 0;
    if (!s[i]) return 1;
  }
}
int urt_fol_verify(const uint8_t *base, uint32_t image) {
  if (!base || image < URT_FOL_HEADER_RVA + 8u + URT_FOL_NATIVE_SIZE) return 0;
  const uint8_t *e = base + URT_FOL_ENTRY_RVA;
  const uint64_t b = (uint64_t)(uintptr_t)base;
  if (!wstr_is(base, image, rd64(e), "AHS_GBuffer_Trees") || !wstr_is(base, image, rd64(e + 0x10), "AnyHitTrees")) return 0;
  if (rd64(e + 0x20) != 0xffffffffu || e[0x40] != 1 || e[0x48] != 1) return 0;
  if (rd64(e + 0x28) != b + URT_FOL_HEADER_RVA) return 0;            /* native pointer still in place */
  const uint8_t *h = base + URT_FOL_HEADER_RVA, *c = h + 8;
  if (rd64(h) != URT_FOL_NATIVE_SIZE || c[0] != 'D' || c[1] != 'X' || c[2] != 'B' || c[3] != 'C' || rd32(c + 24) != URT_FOL_NATIVE_SIZE) return 0;
  for (unsigned i = 0; i < 16; i++) if (c[4 + i] != k_digest[i]) return 0;
  return 1;
}

int urt_fol_publish(uint8_t *base, const uint8_t *hdr, UrtFolProtect prot) {
  if (!base || !hdr || !prot) return 0;
  uint64_t *field = (uint64_t *)(void *)(base + URT_FOL_FIELD_RVA);
  uint64_t expect = (uint64_t)(uintptr_t)(base + URT_FOL_HEADER_RVA);
  if (!prot(field, 8, 1)) return 0;
  int ok = __atomic_compare_exchange_n(field, &expect, (uint64_t)(uintptr_t)hdr, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
  if (!prot(field, 8, 0)) ok = 0;
  return ok && __atomic_load_n(field, __ATOMIC_ACQUIRE) == (uint64_t)(uintptr_t)hdr;
}

/* ---- DXC (game's dxcompiler.dll / dxil.dll), same interfaces as URT's PT library build ---- */
typedef struct { uint32_t d1; uint16_t d2, d3; uint8_t d4[8]; } FGUID;
typedef struct FBlob FBlob; typedef struct FLib FLib; typedef struct FComp FComp; typedef struct FAsm FAsm;
typedef struct FVal FVal; typedef struct FOp FOp;
struct FBlobV { long (FWIN *qi)(FBlob *, const FGUID *, void **); unsigned long (FWIN *add)(FBlob *); unsigned long (FWIN *rel)(FBlob *);
  void *(FWIN *ptr)(FBlob *); size_t (FWIN *size)(FBlob *); };
struct FBlob { struct FBlobV *v; };
struct FLibV { void *qi, *add; unsigned long (FWIN *rel)(FLib *); void *setmalloc, *fromblob, *fromfile;
  long (FWIN *pinned)(FLib *, const void *, uint32_t, uint32_t, FBlob **); };
struct FLib { struct FLibV *v; };
struct FCompV { void *qi, *add; unsigned long (FWIN *rel)(FComp *); void *compile, *pre; long (FWIN *dis)(FComp *, FBlob *, FBlob **); };
struct FComp { struct FCompV *v; };
struct FAsmV { void *qi, *add; unsigned long (FWIN *rel)(FAsm *); long (FWIN *assemble)(FAsm *, FBlob *, FOp **); };
struct FAsm { struct FAsmV *v; };
struct FValV { void *qi, *add; unsigned long (FWIN *rel)(FVal *); long (FWIN *validate)(FVal *, FBlob *, uint32_t, FOp **); };
struct FVal { struct FValV *v; };
struct FOpV { void *qi, *add; unsigned long (FWIN *rel)(FOp *); long (FWIN *status)(FOp *, long *); long (FWIN *result)(FOp *, FBlob **); };
struct FOp { struct FOpV *v; };
static const FGUID C_LIB = {0x6245d6af, 0x66e0, 0x48fd, {0x80, 0xb4, 0x4d, 0x27, 0x17, 0x96, 0x74, 0x8c}};
static const FGUID I_LIB = {0xe5204dc7, 0xd18c, 0x4c3c, {0xbd, 0xfb, 0x85, 0x16, 0x73, 0x98, 0x0f, 0xe7}};
static const FGUID C_COMP = {0x73e22d93, 0xe6ce, 0x47f3, {0xb5, 0xbf, 0xf0, 0x66, 0x4f, 0x39, 0xc1, 0xb0}};
static const FGUID I_COMP = {0x8c210bf3, 0x011f, 0x4422, {0x8d, 0x70, 0x6f, 0x9a, 0xcb, 0x8d, 0xb6, 0x17}};
static const FGUID C_ASM = {0xd728db68, 0xf903, 0x4f80, {0x94, 0xcd, 0xdc, 0xcf, 0x76, 0xec, 0x71, 0x51}};
static const FGUID I_ASM = {0x091f7a26, 0x1c1f, 0x4948, {0x90, 0x4b, 0xe6, 0xe3, 0xa8, 0xa7, 0x71, 0xd5}};
static const FGUID C_VAL = {0x8ca3e215, 0xf728, 0x4cf3, {0x8c, 0xdd, 0x88, 0xaf, 0x91, 0x75, 0x87, 0xa1}};
static const FGUID I_VAL = {0xa6e82bd2, 0x1fd7, 0x4826, {0x98, 0x11, 0x28, 0x57, 0xe7, 0x97, 0xf4, 0x9a}};
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
/* original container with its DXIL and HASH parts taken from the assembled one; every other part kept byte-exact */
static uint8_t *compose(const uint8_t *orig, uint32_t on, const uint8_t *asmb, uint32_t an, uint32_t *outn) {
  const uint8_t *hp, *dp; uint32_t hn, dn;
  if (on < 32 || rd32(orig + 24) != on) return 0;
  if (!part(asmb, an, FOURCC('H', 'A', 'S', 'H'), &hp, &hn) || !part(asmb, an, FOURCC('D', 'X', 'I', 'L'), &dp, &dn)) return 0;
  uint32_t k = rd32(orig + 28), total = (32u + 4u * k + 3u) & ~3u, seen = 0;
  for (uint32_t i = 0; i < k; i++) {
    uint32_t o = rd32(orig + 32 + 4 * i), cc = rd32(orig + o), sz = rd32(orig + o + 4);
    if (cc == FOURCC('H', 'A', 'S', 'H')) { sz = hn; seen |= 1; } else if (cc == FOURCC('D', 'X', 'I', 'L')) { sz = dn; seen |= 2; }
    total += (8u + sz + 3u) & ~3u;
  }
  if (seen != 3 || total > (1u << 20)) return 0;
  uint8_t *b = (uint8_t *)f_alloc(total);
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
int urt_fol_build(const uint8_t *base, UrtDxcCreate create, unsigned di, uint8_t **hdr_out) {
  FLib *lib = 0; FComp *comp = 0; FAsm *as = 0; FVal *val = 0;
  FBlob *src = 0, *dis = 0, *ir = 0, *asmb = 0, *cb = 0, *signedb = 0, *check = 0; FOp *op = 0, *vop = 0;
  char *txt = 0; uint8_t *composed = 0; int ok = 0; long st;
  if (!hdr_out) return 0;
  *hdr_out = 0;
  if (!base || !create || di < 1 || di > 4) return 0;
  const uint8_t *orig = base + URT_FOL_HEADER_RVA + 8; const uint32_t on = URT_FOL_NATIVE_SIZE;
  if (create(&C_LIB, &I_LIB, (void **)&lib) < 0 || !lib || create(&C_COMP, &I_COMP, (void **)&comp) < 0 || !comp ||
      create(&C_ASM, &I_ASM, (void **)&as) < 0 || !as || create(&C_VAL, &I_VAL, (void **)&val) < 0 || !val) goto done;
  if (lib->v->pinned(lib, orig, on, 0, &src) < 0 || !src || comp->v->dis(comp, src, &dis) < 0 || !dis) goto done;
  {
    const char *d = (const char *)dis->v->ptr(dis); size_t dn = dis->v->size(dis);
    if (!d || dn < 512 || dn > (1u << 22)) goto done;
    while (dn && d[dn - 1] == 0) dn--;
    uint32_t cap = (uint32_t)dn + 4096u, tn = 0;
    txt = (char *)f_alloc(cap);
    if (!txt || !urt_fol_patch_ir(d, (uint32_t)dn, di, txt, cap, &tn)) goto done;
    if (lib->v->pinned(lib, txt, tn, 65001u, &ir) < 0 || !ir) goto done;
  }
  if (as->v->assemble(as, ir, &op) < 0 || !op || op->v->status(op, &st) < 0 || st < 0 || op->v->result(op, &asmb) < 0 || !asmb) goto done;
  {
    const uint8_t *a = (const uint8_t *)asmb->v->ptr(asmb); size_t an = asmb->v->size(asmb); uint32_t cn = 0;
    if (!a || an < 64 || an > (1u << 20) || rd32(a + 24) != (uint32_t)an) goto done;
    composed = compose(orig, on, a, (uint32_t)an, &cn);
    if (!composed || !same_kept_parts(orig, on, composed, cn)) goto done;
    if (lib->v->pinned(lib, composed, cn, 0, &cb) < 0 || !cb) goto done;
  }
  if (val->v->validate(val, cb, 1u /* in-place sign */, &vop) < 0 || !vop || vop->v->status(vop, &st) < 0 || st < 0 ||
      vop->v->result(vop, &signedb) < 0 || !signedb) goto done;
  {
    const uint8_t *s = (const uint8_t *)signedb->v->ptr(signedb); size_t sn = signedb->v->size(signedb);
    if (!s || sn < 64 || sn > (1u << 20) || s[0] != 'D' || s[1] != 'X' || s[2] != 'B' || s[3] != 'C' || rd32(s + 24) != (uint32_t)sn ||
        !same_kept_parts(orig, on, s, (uint32_t)sn)) goto done;
    int digest = 0;
    for (unsigned i = 4; i < 20; i++) digest |= s[i];
    if (!digest) goto done;                                   /* unsigned container: D3D12 would reject it */
    /* the signed container must still carry the edit, inside AnyHitTrees */
    if (comp->v->dis(comp, signedb, &check) < 0 || !check) goto done;
    const char *c = (const char *)check->v->ptr(check); size_t cn = check->v->size(check);
    if (!c || cn > (1u << 22) || find(c, (uint32_t)cn, "%urt_fol_near = fcmp fast ole float ", 0, (uint32_t)cn) == NOPOS) goto done;
    uint8_t *h = (uint8_t *)f_alloc(8u + sn);                 /* kept for the whole process (state-object lifetime) */
    if (!h) goto done;
    for (unsigned i = 0; i < 8; i++) h[i] = (uint8_t)((uint64_t)sn >> (8 * i));
    for (size_t i = 0; i < sn; i++) h[8 + i] = s[i];
    *hdr_out = h; ok = 1;
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
  if (composed) f_free(composed);
  if (txt) f_free(txt);
  return ok;
}
