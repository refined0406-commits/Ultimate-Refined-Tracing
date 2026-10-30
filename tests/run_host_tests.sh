#!/usr/bin/env bash
# Ultimate Refined Tracing V5 host test runner (Linux; gcc ASan/UBSan + clang).
# Usage: tests/run_host_tests.sh /path/to/witcher3.exe
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; S="$ROOT/src"; T="$ROOT/tests"; O="$ROOT/out_host"
EXE="${1:-}"; mkdir -p "$O"
SAN="-O1 -g -fsanitize=address,undefined -fno-sanitize-recover=all -std=gnu11 -w -I$S -D__declspec(x)= -D__stdcall="
POOL_EXTRA="$T/support/cr01_pt_carrier501.c"
POOL="cr01_contract cr01_controls cr01_validation cr01_native_execution cr01_rtao_config cr01_menu_bridge cr01_settings_codec cr01_settings_store cr01_indirect_live cr01_gate_core cr01_gate_live_adapter cr01_shadow_policy cr01_distant_trees501 cr01_physics_lod501"
pass=0; fail=0; results=()
run(){ local name="$1"; shift; if "$@" >"$O/$name.log" 2>&1; then pass=$((pass+1)); results+=("PASS $name"); else fail=$((fail+1)); results+=("FAIL $name (see out_host/$name.log)"); fi; }
build_run(){ local name="$1"; shift; local srcs=(); for u in "$@"; do srcs+=("$u"); done
  if gcc $SAN -o "$O/$name" "${srcs[@]}" >"$O/$name.build.log" 2>&1; then run "$name" "$O/$name"; else fail=$((fail+1)); results+=("FAIL $name (build, see out_host/$name.build.log)"); fi; }
pool(){ for u in $POOL; do echo "$S/$u.c"; done; echo $POOL_EXTRA; }

# --- inherited V4.x pure-logic suite, now compiled against the V5 sources ---
for t in test_native_execution test_menu_bridge test_settings_codec test_settings_store test_pt_carrier501 \
         test_indirect_live test_fix3_indirect test_lifecycle01 test_lifecycle_final_all15 test_four_control_integration \
         test_native_sentinel_fix test_r15_geometry_indirect_grid test_dt39_owner test_dt39_guardrails \
         test_hair10_native test_hair10_settings; do
  build_run "legacy_$t" "$T/legacy/$t.c" $(pool)
done
# --- V5 tests ---
build_run v5_ini "$T/host/test_ini.c" "$S/urt_ini.c"
build_run v5_store "$T/host/test_store_v5.c" "$S/cr01_settings_store_windows.c" $(pool)
# --- V5.2 selected graphics and PT controls ---
build_run v52_curated_core "$T/host/test_v52_curated_core.c" "$S/urt_gfx_core.c" "$S/urt_gfx_table.c"
build_run v52_ptperf_core "$T/host/test_ptperf_core.c" "$S/urt_ptperf_core.c"
# --- V5.1 Normal RT performance controls ---
build_run v51_rtperf_core "$T/host/test_rtperf_core.c" "$S/urt_rtperf_core.c"
# --- V5.3 private development rows + W3LoadFix coexistence ---
build_run v53_core "$T/host/test_v53_core.c" "$S/urt_gfx_core.c" "$S/urt_gfx_table.c" "$S/urt_compat53.c"
build_run v520_presets_core "$T/host/test_v520_presets_core.c" "$S/urt_gfx_core.c" "$S/urt_gfx_table.c"
build_run v53f3_coverage_lifecycle "$T/host/test_coverage_lifecycle.c" "$S/cr01_gate_core.c"   # R12 FIX3: coverage level persistence through the gate
if gcc $SAN -o "$O/v531_startup_guard" "$T/host/test_startup_guard.c" "$S/urt_startup_guard.c" >"$O/v531_startup_guard.build.log" 2>&1; then run v531_startup_guard "$O/v531_startup_guard"; else fail=$((fail+1)); results+=("FAIL v531_startup_guard (build)"); fi
if [ -n "$EXE" ]; then
  # V5.3.0 R11: the known game build under test (Patch 5.01 build 1048522 or the 29 October 2026 build 1090142)
  case "$(sha256sum "$EXE" | cut -d' ' -f1)" in
    05948d7238e1c9620a1cb9b3125520f1c143ce04a8e5718a3735120c37d7bae0) export URT_TEST_BUILD=1; PT_RVA=32de6a8;;
    *) export URT_TEST_BUILD=0; PT_RVA=32e7c28;;
  esac
  rm -rf "$O/dxil_all" "$O/bounce"
  if clang -O2 -std=c11 -w -o "$O/v5_cvar_index" "$T/host/test_cvar_index.c" "$S/urt_cvar_index.c"; then run v5_cvar_index "$O/v5_cvar_index" "$EXE"; else fail=$((fail+1)); results+=("FAIL v5_cvar_index (build)"); fi
  if clang -O2 -std=c11 -w -o "$O/v5_scoped" "$T/host/test_scoped_bindings.c" "$S/urt_cvar_index.c" "$S/urt_build.c"; then run v5_scoped_rtgi "$O/v5_scoped" "$EXE"; else fail=$((fail+1)); results+=("FAIL v5_scoped_rtgi (build)"); fi
  if gcc $SAN -o "$O/v51_legacy_shading_native" "$T/host/test_legacy_shading_native.c" "$S/cr01_rt_legacy501.c" "$S/cr01_rt_legacy_core.c" "$S/cr01_rt_c17_core.c" "$S/urt_build.c" >"$O/v51_legacy_shading_native.build.log" 2>&1; then run v51_legacy_shading_native "$O/v51_legacy_shading_native" "$EXE"; else fail=$((fail+1)); results+=("FAIL v51_legacy_shading_native (build)"); fi
  if gcc $SAN -o "$O/v51_rtperf_image" "$T/host/test_rtperf_image.c" "$S/urt_rtperf501.c" "$S/urt_rtperf_core.c" "$S/urt_cvar_index.c" "$S/urt_build.c" >"$O/v51_rtperf_image.build.log" 2>&1; then run v51_rtperf_image "$O/v51_rtperf_image" "$EXE"; else fail=$((fail+1)); results+=("FAIL v51_rtperf_image (build)"); fi
  # R8: solid distant foliage - real AnyHitTrees DXIL from the exe, disassembled with LLVM 18 (research tools)
  python3 -I "$T/../tools/research/dxil_extract.py" "$EXE" "$O/dxil_all" >/dev/null 2>&1
  if [ "$URT_TEST_BUILD" = 1 ]; then results+=("SKIP v538_foliage (retired R8 unit, not linked since R9: Patch 5.01 evidence only)")
  elif FOLLL=$(ls "$O"/dxil_all/*_339a628.ll 2>/dev/null) && [ -s "$FOLLL" ] && gcc $SAN -o "$O/v538_foliage" "$T/host/test_foliage501.c" "$S/urt_foliage501.c" >"$O/v538_foliage.build.log" 2>&1; then run v538_foliage "$O/v538_foliage" "$EXE" "$FOLLL"; else fail=$((fail+1)); results+=("FAIL v538_foliage (build/extract)"); fi
  if gcc $SAN -o "$O/v5310_sharc" "$T/host/test_sharc_r10.c" "$S/cr01_gate_core.c" "$S/urt_build.c" >"$O/v5310_sharc.build.log" 2>&1; then run v5310_sharc "$O/v5310_sharc" "$EXE"; else fail=$((fail+1)); results+=("FAIL v5310_sharc (build)"); fi
  if gcc $SAN -o "$O/v5311_bounce" "$T/host/test_bounce501.c" "$S/urt_bounce501.c" >"$O/v5311_bounce.build.log" 2>&1 && PTLL=$(ls "$O"/dxil_all/*_${PT_RVA}.ll 2>/dev/null) && [ -s "$PTLL" ] && mkdir -p "$O/bounce"; then python3 -I "$T/host/dxc37_style.py" "$PTLL" "$O/bounce/native37.ll"; run v5311_bounce "$O/v5311_bounce" "$PTLL" "$O/bounce" "$O/bounce/native37.ll" "$O/bounce/r12_llvm18.ll" "$T/host/dxc37_style.py"; else fail=$((fail+1)); results+=("FAIL v5311_bounce (build/extract)"); fi
  # R11: the Orange Hair Fix port must produce exactly Orange Hair Fix's own patch_pt.py output (when its source is available)
  if [ -n "${URT_OHF_PATCH:-}" ] && [ -f "$URT_OHF_PATCH" ]; then
    if gcc -O1 -std=gnu11 -w -I"$S" -o "$O/ohf_drv" "$T/host/ohf_port_driver.c" "$S/urt_bounce501.c" >"$O/ohf_drv.build.log" 2>&1 && "$O/ohf_drv" "$O/bounce/r11_cov.ll" "$O/bounce/c_hair.ll" && python3 -I "$URT_OHF_PATCH" "$O/bounce/r11_cov.ll" "$O/bounce/py_hair.ll" >/dev/null && python3 -I "$T/host/ohf_compare.py" "$O/bounce/c_hair.ll" "$O/bounce/py_hair.ll" >"$O/v5311_ohf_equivalence.log" 2>&1; then pass=$((pass+1)); results+=("PASS v5311_ohf_equivalence"); else fail=$((fail+1)); results+=("FAIL v5311_ohf_equivalence (see out_host/v5311_ohf_equivalence.log)"); fi
  fi
  if gcc $SAN -o "$O/v52_ptperf_image" "$T/host/test_ptperf_image.c" "$S/urt_ptperf501.c" "$S/urt_ptperf_core.c" "$S/urt_cvar_index.c" "$S/urt_build.c" -lm >"$O/v52_ptperf_image.build.log" 2>&1; then run v52_ptperf_image "$O/v52_ptperf_image" "$EXE"; else fail=$((fail+1)); results+=("FAIL v52_ptperf_image (build)"); fi
  if gcc $SAN -I"$S" -o "$O/v52_gfx_image_rc6" "$T/host/test_gfx_image_rc6.c" "$S/urt_gfx501.c" "$S/urt_build.c" "$S/urt_fg501.c" "$S/urt_gfx_core.c" "$S/urt_gfx_table.c" "$S/urt_cvar_index.c" "$S/urt_compat53.c" "$S/urt_startup_guard.c" >"$O/v52_gfx_image_rc6.build.log" 2>&1; then run v52_gfx_image_rc6 "$O/v52_gfx_image_rc6" "$EXE"; else fail=$((fail+1)); results+=("FAIL v52_gfx_image_rc6 (build)"); fi
  if gcc $SAN -I"$S" -o "$O/v53_image" "$T/host/test_v53_image.c" "$S/urt_gfx501.c" "$S/urt_build.c" "$S/urt_fg501.c" "$S/urt_gfx_core.c" "$S/urt_gfx_table.c" "$S/urt_cvar_index.c" "$S/urt_compat53.c" "$S/urt_startup_guard.c" >"$O/v53_image.build.log" 2>&1; then run v53_image "$O/v53_image" "$EXE"; else fail=$((fail+1)); results+=("FAIL v53_image (build)"); fi
  if gcc $SAN -I"$S" -o "$O/v530_image" "$T/host/test_v530_image.c" "$S/urt_gfx501.c" "$S/urt_build.c" "$S/urt_fg501.c" "$S/urt_gfx_core.c" "$S/urt_gfx_table.c" "$S/urt_cvar_index.c" "$S/urt_compat53.c" "$S/urt_startup_guard.c" >"$O/v530_image.build.log" 2>&1; then run v530_image "$O/v530_image" "$EXE"; else fail=$((fail+1)); results+=("FAIL v530_image (build)"); fi
  if clang -O2 -std=c11 -w -o "$O/v5_exe_identity" "$T/host/test_exe_identity.c" "$S/urt_exe_identity_plan.c" "$S/urt_build.c"; then run v5_exe_identity "$O/v5_exe_identity" "$EXE"; else fail=$((fail+1)); results+=("FAIL v5_exe_identity (build)"); fi
  for e in "$T"/host/evidence_*.py; do
    # Patch 5.01 research evidence (hard-pinned to build 1048522); on build 1090142 every code site URT uses is
    # covered by evidence_build_port.py and the image tests above
    if [ "$URT_TEST_BUILD" = 1 ] && [ "$(basename "$e")" != evidence_build_port.py ]; then results+=("SKIP $(basename "$e" .py) (Patch 5.01 research evidence)"); continue; fi
    run "$(basename "$e" .py)" python3 -I "$e" "$EXE"; done
  [ -f "$T/host/test_signatures.py" ] && run v5_signatures python3 -I "$T/host/test_signatures.py" "$EXE"
  # IWO static interoperability evidence against the user's own installed IWO files (read-only)
  if [ -n "${URT_IWO_DIR:-}" ]; then run v9_compat_evidence_iwo python3 -I "$T/host/compat_evidence_iwo.py" "$EXE" "$URT_IWO_DIR"
  else results+=("SKIP v9_compat_evidence_iwo (set URT_IWO_DIR)"); fi
fi
OUTDIR="${URT_OUT:-$ROOT/out}"
if ls "$OUTDIR"/*_MAIN.zip >/dev/null 2>&1; then
  if [ -f "$ROOT/baseline_v431/Ultimate_Refined_Tracing_V4.3.1_MAIN.zip" ]; then run v5_release_verify python3 -I "$T/host/verify_release_v52.py" "$OUTDIR" "$ROOT/baseline_v431/Ultimate_Refined_Tracing_V4.3.1_MAIN.zip" "$ROOT/baseline_v431/Ultimate_Refined_Tracing_V4.3.1_ASI_ONLY.zip"
  else results+=("SKIP v5_release_verify (V4.3.1 archives not in baseline_v431/)"); fi
  # V5.3 dev: the built private archives against the protected V5.2.0 release archives
  if [ -n "${URT_V520_PKG:-}" ]; then run v53_release_verify python3 -I "$T/host/verify_release_v53.py" "$OUTDIR" "$URT_V520_PKG"
  else results+=("SKIP v53_release_verify (set URT_V520_PKG to the folder with the V5.2.0 zips)"); fi
fi
for f in "$T"/host/test_*.py; do [ -e "$f" ] || continue; [ "$(basename "$f")" = test_signatures.py ] && continue; run "py_$(basename "$f" .py)" python3 -I "$f"; done
printf '%s\n' "${results[@]}"
echo "TOTAL: $pass passed, $fail failed"
[ "$fail" -eq 0 ]
