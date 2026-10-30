/* V5.3.0 R11: PT Second Bounce Coverage + Orange Hair Fix Path-A patch, on the REAL native PT library of
 * witcher3.exe (LLVM 18 text extracted by tools/research/dxil_extract.py from the exe's DXIL part).
 *   1. URT's bounce-selector edit (the exact lines cr01_pt_shared_dxil.c inserts) is reproduced on the text;
 *   2. the coverage pass and the hair pass apply to both PT exports, every anchor unique, and refuse a second run;
 *   3. the result re-assembles with llvm-as (valid SSA, dominance, types) - the DXIL validator runs in game (DXC);
 *   4. no TraceRay is added or removed; the bounce loop and its limit test are the native ones;
 *   5. the pixel pattern: coverage 1/2 and 1/4, every pixel gets its full bounces once every 2 / 4 frames;
 *   6. the hair pass inserts exactly Orange Hair Fix's RELEASE lines (patch_pt.py) at its three anchor kinds.
 * Usage: test_bounce501 <PTRGSFinal library .ll> <scratch dir> */
#include "../../src/urt_bounce501.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails;
#define CHECK(c, m) do { if (c) printf("PASS %s\n", m); else { printf("FAIL %s\n", m); fails++; } } while (0)
static unsigned occ(const char *t, const char *q) { unsigned c = 0; for (const char *p = t; (p = strstr(p, q)); p++) c++; return c; }
static char *readall(const char *path, unsigned *n) {
  FILE *f = fopen(path, "rb"); if (!f) return 0;
  fseek(f, 0, SEEK_END); long m = ftell(f); fseek(f, 0, SEEK_SET);
  char *b = malloc((size_t)m + 1); if (fread(b, 1, (size_t)m, f) != (size_t)m) { fclose(f); free(b); return 0; }
  fclose(f); b[m] = 0; *n = (unsigned)m; return b;
}
/* URT's own bounce-selector edit (cr01_pt_shared_dxil.c rpt_patch_one_function_parity, selector part) */
static char *urt_selector(const char *src, unsigned n, unsigned *outn) {
  char *out = malloc((size_t)n + 65536); unsigned u = 0; const char *p = src;
  for (int fn = 0; fn < 2; fn++) {
    const char *def = strstr(p, fn ? "define void @\"\\01?PTRGSWithSERFinal@@" : "define void @\"\\01?PTRGSFinal@@");
    if (!def) return 0;
    const char *ann = strstr(def, "%dx.types.ResourceProperties { i32 13, i32 1632 })");
    const char *sel = strstr(def, ", i32 4, i32 2\n");
    if (!ann || !sel) return 0;
    const char *al = ann; while (al > def && al[-1] != '\n') al--;
    char cam[32]; sscanf(al, " %31s", cam);
    const char *sl = sel; while (sl[-1] != '\n') sl--;
    char bounce[32], hair[32]; if (sscanf(sl, " %31s = select i1 %31[^,]", bounce, hair) != 2) return 0;
    memcpy(out + u, p, (size_t)(sl - p)); u += (unsigned)(sl - p);
    u += (unsigned)sprintf(out + u,
      "  %%rpt_raw_cb = call %%dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %%dx.types.Handle %s, i32 33)\n"
      "  %%rpt_raw = extractvalue %%dx.types.CBufRet.i32 %%rpt_raw_cb, 0\n  %%rpt_nb = and i32 %%rpt_raw, 255\n"
      "  %%rpt_nh_cb = call %%dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %%dx.types.Handle %s, i32 56)\n"
      "  %%rpt_nh = extractvalue %%dx.types.CBufRet.i32 %%rpt_nh_cb, 0\n  %s = select i1 %s, i32 %%rpt_nh, i32 %%rpt_nb\n",
      cam, cam, bounce, hair);
    p = sel + strlen(", i32 4, i32 2\n");
  }
  unsigned rest = (unsigned)(src + n - p); memcpy(out + u, p, rest); u += rest; out[u] = 0; *outn = u;
  return out;
}
static int assembles(const char *dir, const char *name, const char *t, unsigned n) {
  char ll[1024], cmd[2300]; snprintf(ll, sizeof ll, "%s/%s.ll", dir, name);
  FILE *f = fopen(ll, "wb"); if (!f) return 0; fwrite(t, 1, n, f); fclose(f);
  snprintf(cmd, sizeof cmd, "llvm-as '%s' -o '%s/%s.bc' 2>'%s/%s.err'", ll, dir, name, dir, name);
  return system(cmd) == 0;
}
int main(int argc, char **argv) {
  unsigned n = 0, un = 0;
  char *nat = argc > 2 ? readall(argv[1], &n) : 0;
  if (!nat) { puts("FAIL input"); return 1; }
  CHECK(occ(nat, "define void @") == 2 && occ(nat, ", i32 4, i32 2\n") == 2, "native PT library: PTRGSFinal + PTRGSWithSERFinal, native bounce selector select(hair, 4, 2) in each");
  char *urt = urt_selector(nat, n, &un);
  CHECK(urt && occ(urt, ", i32 %rpt_nh, i32 %rpt_nb\n") == 2, "URT's bounce selector (CameraCB rows 33/56) reproduced in both exports");
  CHECK(assembles(argv[2], "r11_urt", urt, un), "URT-selector text assembles (baseline for the passes)");
  uint32_t cap = un + 262144u, c1 = 0, c2 = 0, h1 = 0, h2 = 0; unsigned sites = 0, s2 = 0;
  char *t1 = malloc(cap), *t2 = malloc(cap), *t3 = malloc(cap);
  CHECK(urt_bc_patch_coverage(urt, un, t1, cap, &c1), "coverage pass applies to both exports");
  CHECK(occ(t1, "%urt_bc_nb = select i1 %urt_bc_sel, i32 %rpt_nb, i32 %urt_bc_lim\n") == 2 && occ(t1, ", i32 %rpt_nh, i32 %urt_bc_nb\n") == 2 &&
        occ(t1, ", i32 %rpt_nh, i32 %rpt_nb\n") == 0, "per-pixel limit min(bounces, 1) for unselected non-hair pixels; hair keeps %rpt_nh");
  CHECK(occ(t1, "i32 13)\n  %urt_bc_f = extractvalue %dx.types.CBufRet.i32 %urt_bc_fcb, 3") == 2,
        "frame index = CameraCB row 13 .w (the game's own checkerboard uses it: (x + y + cb13.w) & 1)");
  CHECK(!urt_bc_patch_coverage(t1, c1, t2, cap, &c2), "coverage pass refuses an already patched library");
  CHECK(!urt_bc_patch_coverage(nat, n, t2, cap, &c2), "coverage pass refuses a library without URT's selector (native)");
  CHECK(assembles(argv[2], "r11_cov", t1, c1), "coverage-patched text assembles (llvm-as: SSA, dominance, types)");
  CHECK(urt_ohf_bake(t1, c1, t3, cap, &h1, &sites) && sites >= 2, "Orange Hair Fix Path-A patch applies to both exports");
  printf("  hair strength sites: %u\n", sites);
  CHECK(!urt_ohf_bake(t3, h1, t2, cap, &h2, &s2), "hair pass refuses an already hair-patched library (Orange Hair Fix's own rule)");
  CHECK(assembles(argv[2], "r11_variant", t3, h1), "full variant text (coverage + hair) assembles");
  CHECK(occ(t3, "@dx.op.traceRay") == occ(nat, "@dx.op.traceRay") && occ(t3, "!llvm.loop") == occ(nat, "!llvm.loop"),
        "no TraceRay and no loop added or removed: the native bounce loop decides how many bounce rays run");
  /* the exact Orange Hair Fix RELEASE insertion (patch_pt.py with RELEASE = True) */
  static const char *const rel[] = {"  %hlf_mode = extractvalue %dx.types.CBufRet.i32 %hlf_cb, 1\n  %hlf_m77 = icmp eq i32 %hlf_mode, 77\n"
    "  %hlf_m25 = icmp eq i32 %hlf_mode, 25\n  %hlf_m75 = icmp eq i32 %hlf_mode, 75\n  %hlf_m90 = icmp eq i32 %hlf_mode, 90\n  %hlf_c3 = icmp eq i32 ",
    ", 3\n  %hlf_hair = and i1 %hlf_c3, true\n  %hlf_diag = and i1 %hlf_m90, true\n"
    "  %hlf_s1 = select i1 %hlf_m75, float 7.500000e-01, float 5.000000e-01\n  %hlf_s2 = select i1 %hlf_m25, float 2.500000e-01, float %hlf_s1\n"
    "  %hlf_s3 = select i1 %hlf_diag, float 1.000000e+00, float %hlf_s2\n  %hlf_str = select i1 %hlf_m77, float 0.000000e+00, float %hlf_s3\n"
    "  %hlf_m0 = icmp eq i32 %hlf_mode, 0\n  %hlf_off = or i1 %hlf_m77, %hlf_m0\n  %hlf_en = xor i1 %hlf_off, true\n  %hlf_on = and i1 %hlf_hair, %hlf_en\n"};
  CHECK(occ(t3, rel[0]) == 2 && occ(t3, rel[1]) == 2 && occ(t3, ", i32 338)  ; HairLightFix cvar\n") == 2,
        "hair pass inserts Orange Hair Fix's RELEASE lines exactly (cb12[338].y; 0/77 off, 25/50/75/90)");
  CHECK(occ(t3, " = select i1 %hlf_hl, i32 1, i32 %hlf_zs\n") == 2 && occ(t3, " = select i1 %hlf_hair, float %hlf_str, float %hlf_sat") == sites,
        "hair pass: flag select at the skin&&lobe zext and one strength select per site");
  /* the pixel pattern of the coverage edit (same integer arithmetic as the inserted IR) */
  {
    int ok = 1;
    for (unsigned k = 0; k <= 2; k++) {
      unsigned mask = (1u << k) - 1u, sel = 0, tot = 0;
      for (unsigned y = 0; y < 64; y++) for (unsigned x = 0; x < 64; x++) {
        unsigned per = 0;
        for (unsigned f = 0; f < (1u << k); f++) { unsigned s = x + y + (y >> 1) + f; if (((s & mask) == 0)) per++; }
        if (per != 1) ok = 0;
        unsigned s0 = x + y + (y >> 1) + 7; sel += (s0 & mask) == 0; tot++;
      }
      if (sel * (1u << k) != tot) ok = 0;
    }
    CHECK(ok, "pattern: level 0 = every pixel; 1/2 and 1/4 of the pixels per frame; each pixel once every 2 / 4 frames");
  }
  /* R12 module 2: first-bounce diffuse coverage, after module 1 and the hair patch (the variant's order) */
  {
    uint32_t d1 = 0, d2 = 0; char *t4 = malloc(cap);
    CHECK(urt_dc_patch_diffuse(t3, h1, t4, cap, &d1), "R12: first-bounce diffuse pass applies to both exports");
    CHECK(!urt_dc_patch_diffuse(t4, d1, t2, cap, &d2), "R12: refuses an already patched library");
    CHECK(!urt_dc_patch_diffuse(urt, un, t2, cap, &d2), "R12: refuses a library without module 1 (the variant's order)");
    CHECK(assembles(argv[2], "r12_variant", t4, d1), "R12: full variant text (both coverage edits + hair) assembles (llvm-as)");
    CHECK(occ(t4, "@dx.op.traceRay") == occ(nat, "@dx.op.traceRay") && occ(t4, "!llvm.loop") == occ(nat, "!llvm.loop") &&
          occ(t4, "@dx.op.textureStore") == occ(nat, "@dx.op.textureStore"), "R12: no TraceRay, loop or output store added or removed");
    CHECK(occ(t4, "  br i1 %urt_dc_skip, label %urt_dc_exit, label %urt_dc_go\n") == 2 && occ(t4, "\nurt_dc_exit:\n  br label %") == 2 &&
          occ(t4, ", %urt_dc_exit ]") >= 20, "R12: the dropped sample takes the loop exit; every exit phi gets the new edge");
    /* the skip condition: iteration 0 AND the diffuse-lobe phi of the lobe block (STATIC: rand < diffuse energy fraction) */
    {
      int ok = 1;
      for (const char *p = t4; (p = strstr(p, "  %urt_dc_a = and i1 ")); p++) {
        char i0[32], lobe[32];
        if (sscanf(p, "  %%urt_dc_a = and i1 %31[^,], %31s", i0, lobe) != 2) { ok = 0; break; }
        char q[96]; snprintf(q, sizeof q, "  %s = icmp eq i32 ", i0); if (!strstr(t4, q)) ok = 0;
        snprintf(q, sizeof q, "  %s = phi i1 [ ", lobe); if (!strstr(t4, q)) ok = 0;
      }
      CHECK(ok && occ(t4, "  %urt_dc_a = and i1 ") == 2, "R12: skip = iteration 0 (counter == 0) AND the lobe phi i1");
    }
    CHECK(occ(t4, "%urt_dc_l0 = lshr i32 %rpt_raw, 18\n  %urt_dc_l = and i32 %urt_dc_l0, 3\n") == 2,
          "R12: level from bits 18-19 of URT's CameraCB bounce word (URT's normal library reads bits 0-15, module 1 bits 16-17)");
    /* only the diffuse (RTGBufferA) colour of the normal branch is scaled; RTGBufferB (specular) untouched */
    {
      int ok = 1; unsigned na = 0;
      for (const char *p = t4; (p = strstr(p, "float %urt_dc_o0, float %urt_dc_o1, float %urt_dc_o2, float ")); p++) na++;
      const char *pa = strstr(t4, "@\"\\01?RTGBufferA@@");
      ok = pa != 0 && na == 2;
      CHECK(ok, "R12: one scaled colour store per export (the diffuse buffer A, normal branch)");
    }
    /* the pixel pattern (same integer arithmetic as the inserted IR): kept share, every pixel kept/dropped in turn, unbiased weight */
    {
      int ok = 1;
      for (unsigned lvl = 1; lvl <= 2; lvl++) {
        const float w = lvl == 1 ? 4.0f / 3.0f : 2.0f;
        unsigned kept = 0, tot = 0;
        for (unsigned y = 0; y < 64; y++) for (unsigned x = 0; x < 64; x++) {
          unsigned kept_f = 0, frames = lvl == 1 ? 4 : 2;
          for (unsigned f = 0; f < frames; f++) {
            unsigned drop = lvl == 1 ? (((x & 1) | ((y & 1) << 1)) == (f & 3)) : (((x ^ y ^ f) & 1) != 0);
            kept_f += !drop; if (f == 0) { kept += !drop; tot++; }
          }
          if ((float)kept_f * w != (float)frames) ok = 0;            /* E[w * kept] = 1 for every pixel over its cycle */
        }
        if (lvl == 1 && kept * 4 != tot * 3) ok = 0;
        if (lvl == 2 && kept * 2 != tot) ok = 0;
      }
      CHECK(ok, "R12 pattern: level 1 keeps 3/4 (2x2 quad, one dropped in turn), level 2 keeps 1/2 (checkerboard, alternating); weight x keep = 1 per pixel");
    }
    /* R12 FIX2 module 3: bounce-vertex sun / moon shadow-ray coverage, after module 2 */
    uint32_t e1 = 0, e2 = 0; char *t5 = malloc(cap + 65536);
    CHECK(urt_sc_patch_shadow(t4, d1, t5, cap + 65536, &e1), "FIX2: sun-shadow pass applies to both exports");
    CHECK(!urt_sc_patch_shadow(t5, e1, t2, cap, &e2) && !urt_sc_patch_shadow(urt, un, t2, cap, &e2), "FIX2: refuses a second run and a library without module 1");
    CHECK(assembles(argv[2], "fix2_variant", t5, e1), "FIX2: full variant (modules 1-3 + hair) assembles (llvm-as)");
    CHECK(occ(t5, "@dx.op.traceRay") == occ(nat, "@dx.op.traceRay") && occ(t5, "!llvm.loop") == occ(nat, "!llvm.loop"), "FIX2: no TraceRay or loop added or removed");
    CHECK(occ(t5, ", i32 %urt_sc_mask, i32 0, i32 4, i32 0, float ") == 6 && occ(t5, ", float 0x4589D971E0000000, ") == occ(nat, ", float 0x4589D971E0000000, "),
          "FIX2: exactly the three directional (tmax 1e28) shadow rays per export get the per-pixel instance mask");
    CHECK(occ(t5, ", float %urt_sc_w, float 0.000000e+00\n") == 6 && occ(t5, "%urt_sc_l0 = lshr i32 %rpt_raw, 20\n") == 2,
          "FIX2: each of their visibility factors becomes select(miss, w, 0); level from bits 20-21");
    CHECK(occ(t5, ", i32 %urt_bc_") == occ(t4, ", i32 %urt_bc_") && occ(t5, "%urt_dc_") == occ(t4, "%urt_dc_"), "FIX2: modules 1 and 2 untouched by module 3 (the R12 FIX1 50% edit is kept as it is)");
    {
      int ok = 1;   /* unbiased per pixel: over 4 frames, kept 2 x weight 2; with module 2 at 50 %, half of the kept-diffuse frames */
      for (unsigned y = 0; y < 32; y++) for (unsigned x = 0; x < 32; x++) {
        float sum = 0, sumd = 0; unsigned nd = 0;
        for (unsigned f = 0; f < 4; f++) {
          unsigned skip = ((x ^ (y >> 1) ^ (f >> 1)) & 1u) != 0; float w = skip ? 0.0f : 2.0f; sum += w;
          if (((x ^ y ^ f) & 1u) == 0) { sumd += w; nd++; }
        }
        if (sum != 4.0f || nd != 2 || sumd != 2.0f) ok = 0;
      }
      CHECK(ok, "FIX2 pattern: each pixel traced 2 of 4 frames (weight 2); also unbiased within module 2's kept frames");
    }
    /* R12 FIX3 module 4: bounce-vertex local-light shadow coverage, after module 3 (built only when its option is set) */
    uint32_t g1 = 0, g2 = 0; char *t6 = malloc(cap + 65536);
    CHECK(urt_lc_patch_local(t5, e1, t6, cap + 65536, &g1), "FIX3: local-light pass applies to both exports");
    CHECK(!urt_lc_patch_local(t6, g1, t2, cap, &g2) && !urt_lc_patch_local(urt, un, t2, cap, &g2), "FIX3: refuses a second run and a library without module 1");
    CHECK(assembles(argv[2], "fix3_variant", t6, g1), "FIX3: full variant (modules 1-4 + hair) assembles (llvm-as: SSA, dominance, phi predecessors)");
    CHECK(occ(t6, "@dx.op.traceRay") == occ(nat, "@dx.op.traceRay") && occ(t6, "!llvm.loop") == occ(nat, "!llvm.loop"), "FIX3: no TraceRay or loop added or removed");
    CHECK(occ(t6, ", i32 %urt_lc_mask, i32 0, i32 4, i32 0, float ") == 4 && occ(t6, ", i32 %urt_sc_mask, i32 0, i32 4, i32 0, float ") == 6,
          "FIX3: exactly the two local-light shadow rays per export (flags 12 / 28, tmax = light distance) get the module 4 mask; the sun rays keep module 3's");
    CHECK(occ(t6, ", float %urt_lc_w, float 0.000000e+00\n") == 2 && occ(t6, "%urt_lc_go = and i1 ") == 2 && occ(t6, ", %urt_lc_keep\n") == 2 &&
          occ(t6, "%urt_lc_wb = phi float ") == 2 && occ(t6, " = fmul fast float %urt_lc_q, %urt_lc_wb\n") == 2 && occ(t6, "%urt_lc_l0 = lshr i32 %rpt_raw, 22\n") == 2,
          "FIX3: select site weighted; branch site skips the light evaluation when not traced and weights its result factor through a phi (1 on untraced paths); level from bits 22-23");
    CHECK(occ(t6, "%urt_sc_") == occ(t5, "%urt_sc_") && occ(t6, "%urt_dc_") == occ(t5, "%urt_dc_") && occ(t6, ", i32 %urt_bc_") == occ(t5, ", i32 %urt_bc_"),
          "FIX3: modules 1-3 untouched by module 4 (the measured R12 FIX2 shader text is a strict subset)");
    {
      int ok = 1;   /* unbiased per pixel over 4 frames (2 x weight 2), and within module 2's kept diffuse frames */
      for (unsigned y = 0; y < 32; y++) for (unsigned x = 0; x < 32; x++) {
        float sum = 0, sumd = 0;
        for (unsigned f = 0; f < 4; f++) {
          const unsigned skip = (((x >> 1) ^ y ^ (f >> 1)) & 1u) != 0; const float w = skip ? 0.0f : 2.0f; sum += w;
          if (((x ^ y ^ f) & 1u) == 0) sumd += w;
        }
        if (sum != 4.0f || sumd != 2.0f) ok = 0;
      }
      CHECK(ok, "FIX3 pattern: each pixel traces its local-light shadow 2 of 4 frames (weight 2); unbiased within module 2's kept frames");
    }
    { FILE *f = fopen(argv[4], "wb"); if (f) { fwrite(t6, 1, g1, f); fclose(f); } }
    /* R12 fix: the same pipeline on the game DXC's disassembly syntax ("; <label>:N", typed pointers; argv[3], made by
       dxc37_style.py from the same text) must make exactly the same edits (compared after the same rewrite). */
    if (argc > 4) {
      unsigned n37 = 0, nexp = 0; char *s37 = readall(argv[3], &n37), *exp37 = 0;
      char cmd[2048]; snprintf(cmd, sizeof cmd, "python3 -I '%s' '%s' '%s.37'", argv[5], argv[4], argv[4]);
      int conv = system(cmd) == 0;
      char p37[1100]; snprintf(p37, sizeof p37, "%s.37", argv[4]); exp37 = conv ? readall(p37, &nexp) : 0;
      uint32_t ua = 0, a1 = 0, a2 = 0, a3 = 0; char *u37 = s37 ? urt_selector(s37, n37, &ua) : 0;
      char *b1 = malloc(cap + 65536), *b2 = malloc(cap + 65536), *b3 = malloc(cap + 65536); unsigned st = 0;
      uint32_t a4 = 0;
      int ok = u37 && urt_bc_patch_coverage(u37, ua, b1, cap + 65536, &a1) && urt_ohf_bake(b1, a1, b2, cap + 65536, &a2, &st) &&
               urt_dc_patch_diffuse(b2, a2, b3, cap + 65536, &a3) && urt_sc_patch_shadow(b3, a3, b1, cap + 65536, &a4) &&
               urt_lc_patch_local(b1, a4, b3, cap + 65536, &a3);
      CHECK(ok, "R12 fix / FIX3: all five passes (modules 1-4 + hair) apply to the DXC-syntax text (labels \"; <label>:N\", typed pointers)");
      CHECK(ok && exp37 && a3 == nexp && !memcmp(b3, exp37, a3), "R12 fix: DXC-syntax result == LLVM-18 result in DXC syntax (identical edits)");
      free(s37); free(exp37); free(u37); free(b1); free(b2); free(b3);
    } else CHECK(0, "R12 fix: DXC-syntax input given");
    free(t4); free(t5); free(t6);
  }
  free(nat); free(urt); free(t1); free(t2); free(t3);
  printf("TOTAL %s %d\n", fails ? "FAIL" : "PASS", fails);
  return fails ? 1 : 0;
}
