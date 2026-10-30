/* V5.3.0 R11: per-build address translation (pure C, host-testable). See urt_build.h. */
#include "urt_build.h"
#include "urt_build_table.h"
static volatile int g_build = URT_BUILD_1048522;
void urt_build_set(int build) {
  __atomic_store_n(&g_build, build == URT_BUILD_1090142 ? URT_BUILD_1090142 : URT_BUILD_1048522, __ATOMIC_RELEASE);
}
int urt_build(void) { return __atomic_load_n(&g_build, __ATOMIC_ACQUIRE); }
uint32_t urt_rva(uint32_t rva) {
  if (urt_build() != URT_BUILD_1090142 || !rva) return rva;
  unsigned lo = 0, hi = URT_BUILD_MAP_COUNT;
  while (lo < hi) {
    unsigned mid = (lo + hi) / 2u;
    if (kUrtBuildMap1090142[mid][0] == rva) return kUrtBuildMap1090142[mid][1];
    if (kUrtBuildMap1090142[mid][0] < rva) lo = mid + 1u; else hi = mid;
  }
  return 0;                                  /* not ported: fail closed */
}
uint32_t urt_image_bytes(void) { return urt_build() == URT_BUILD_1090142 ? 0x6400000u : 0x6402000u; }
const char *urt_build_name(void) {
  return urt_build() == URT_BUILD_1090142 ? "game build 1090142 (Steam update of 29 October 2026)" : "Patch 5.01 build 1048522";
}
