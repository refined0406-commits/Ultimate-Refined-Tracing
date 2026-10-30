/* R12 FIX3 (OFFLINE): PT coverage level persistence through URT's real CameraCB gate logic (cr01_gate_core.c, the pure
 * core shared with cr01_gate_windows.c). Simulates 400 000 frames of the render-thread sequence
 *   native serializer store (via URT's in-function bridge) -> SHaRC safety net -> post-serialize -> next in-function word
 * with random menu publishes (Geometry Bounces incl. Native 2, Hair, Indirect-Light, Master On/Off, renderer PT or not),
 * SHaRC toggles and library-live changes, and checks on EVERY frame that the word the PT shader reads carries the
 * coverage level exactly when URT's Path Tracing words are in effect, with the staged bounce values, and is the game's
 * native word otherwise. Also: the accepted level table (retired Second Bounce Coverage bits and the retired 75 %). */
#include "../../src/cr01_gate_core.h"
#include <stdio.h>
#include <string.h>
static int fails;
#define CHECK(c,m) do{ if(c) printf("PASS %s\n",m); else {printf("FAIL %s\n",m); fails++;} }while(0)
static uint32_t rng = 0x1234567u;
static unsigned rn(unsigned n) { rng = rng * 1103515245u + 12345u; return (rng >> 8) % n; }
static uint32_t rd32(const uint8_t *p) { uint32_t v; memcpy(&v, p, 4); return v; }
int main(void) {
  /* level table */
  int table = 1;
  for (unsigned lv = 0; lv < 512; lv++) {
    const unsigned sbc = lv & 3u, dc = (lv >> 2) & 3u, sc = (lv >> 4) & 3u, lc = (lv >> 6) & 3u;
    const int want = lv < 256u && sbc == 0u && (dc == 0u || dc == 2u) && sc <= 1u && lc <= 1u;
    if (cr01_gate_level_valid(lv) != want) table = 0;
  }
  CHECK(table && cr01_gate_level_valid(8u | 16u | 64u) && !cr01_gate_level_valid(1u) && !cr01_gate_level_valid(4u) && !cr01_gate_level_valid(32u) && !cr01_gate_level_valid(128u),
        "level table: diffuse 50% (2<<2), sun shadow 50% (1<<4), local-light shadow 50% (1<<6) accepted; retired Second Bounce Coverage bits, the retired 75% and unknown levels refused");
  static const unsigned levels[] = {8u, 16u, 24u, 88u, 24u};   /* diffuse, sun, both, all three; both with the library not live */
  unsigned long frames = 0, eligible = 0, carried = 0, wrong = 0, leaked = 0, badvals = 0, native_ok = 0, edge_native = 0, toggles = 0;
  for (unsigned li = 0; li < 5; li++) {
    const unsigned level = levels[li];
    CR01GateState g = {0};
    uint32_t infunc = 2u;            /* the in-function bridge's armed word (cr01_gate_windows.c g_infunc_packed) */
    const int custom_live = li != 4u;   /* PT shader slots: installed once at ASI load and never cleared after the commit */
    unsigned sharc = 0, epoch = 1, sharc_edge = 0;
    CR01GateFields f = {2, 4, 100, 1, 1, 1, 1};
    (void)cr01_gate_publish(&g, f, epoch);
    infunc = cr01_gate_infunc_word(cr01_gate_snapshot(&g), custom_live, sharc, level);
    for (unsigned long fr = 0; fr < 100000ul; fr++, frames++) {
      /* worker-thread events between frames (settings publish = cr01_gate_win_publish_settings) */
      const unsigned ev = rn(40);
      if (ev < 6) {
        f.geometry = (uint8_t)(rn(4) ? 1u + rn(6) : 2u);   /* often Native 2 */
        f.hair = (uint8_t)(rn(3) ? 1u + rn(10) : 4u);
        f.comp = (uint8_t)(rn(3) ? 100u + rn(51) : 100u);
        if (ev == 0) f.master = (uint8_t)!f.master;
        if (ev == 1) f.pt = (uint8_t)!f.pt;                 /* renderer change PT <-> other */
        (void)cr01_gate_publish(&g, f, ++epoch);
        infunc = cr01_gate_infunc_word(cr01_gate_snapshot(&g), custom_live, sharc, level);
      } else if (ev == 6) { const unsigned ns = !sharc && rn(4) == 0; if (ns != sharc) { sharc = ns; sharc_edge = 1; toggles++; } }   /* the game's SHaRC on for a while */
      /* render thread: one CameraCB serialization */
      uint8_t dst[0x400];
      memset(dst, 0xcd, sizeof dst);
      memcpy(dst + CR01_GEOM_OFFSET, &infunc, 4);          /* native store through URT's bridge (mov eax,[packed]) */
      { const uint32_t h4 = 4u; memcpy(dst + CR01_HAIR_OFFSET, &h4, 4); }
      const int custom = cr01_gate_effective_custom(custom_live, sharc);
      if (sharc) { const uint32_t g2 = 2u, h4 = 4u; memcpy(dst + CR01_GEOM_OFFSET, &g2, 4); memcpy(dst + CR01_HAIR_OFFSET, &h4, 4); }
      (void)cr01_gate_post_serialize_level(&g, dst, custom, level);
      infunc = cr01_gate_infunc_word(cr01_gate_snapshot(&g), custom_live, sharc, level);
      /* what the PT shader reads this frame */
      const uint32_t w = rd32(dst + CR01_GEOM_OFFSET), h = rd32(dst + CR01_HAIR_OFFSET);
      const int ok_vals = f.geometry >= 1 && f.geometry <= 6 && f.hair >= 1 && f.hair <= 10 && f.comp >= 100 && f.comp <= 150;
      const int el = custom_live && !sharc && f.master && f.pt && ok_vals;
      if (sharc_edge) {   /* the first frame after the game switches SHaRC: the bridge word armed one frame earlier is used once;
                             URT's safety net makes it native when SHaRC turns on, so the only effect is one native frame */
        sharc_edge = 0;
        if (el && w == 2u && h == 4u) { edge_native++; continue; }
      }
      if (el) {
        eligible++;
        if (((w >> 16) & 255u) == level) carried++; else wrong++;
        if ((w & 0xffu) != f.geometry || ((w >> 8) & 0xffu) != f.comp || h != f.hair) badvals++;
      } else {
        if ((w >> 16) != 0u) leaked++;
        else if (w == 2u && h == 4u) native_ok++;
      }
    }
  }
  printf("  %lu frames, %lu with URT's PT words in effect, %lu native; %lu SHaRC switches (%lu first frames native)\n", frames, eligible, frames - eligible - edge_native, toggles, edge_native);
  CHECK(eligible > 40000ul && carried == eligible && !wrong,
        "every frame with URT's PT words in effect carries the coverage level - whatever Geometry Bounces (incl. Native), Hair and Indirect-Light are set to, across publishes, renderer changes and Master toggles");
  CHECK(!badvals, "those frames carry exactly the staged Geometry Bounces / Indirect-Light / Hair values");
  CHECK(!leaked && native_ok == frames - eligible - edge_native && edge_native <= toggles,
        "Master Off, renderer not PT, SHaRC on or library not live: the game's native word (2 / 4), never a coverage bit (renderer isolation)");
  printf("TOTAL %s %d\n", fails ? "FAIL" : "PASS", fails);
  return fails ? 1 : 0;
}
