# Building Ultimate Refined Tracing V5 (V5.3.0)

Requirements (Linux or Windows): Python 3.10+, LLVM `clang-cl` and `lld-link` (tested: Ubuntu clang 18.1.3). No Windows SDK, no MSVC and no CRT: the runtime declares the Win32 APIs it uses, and import libraries are generated from `src/*_gate.def`.

```
python3 build.py                 # -> out/bin/UltimateRefinedTracing.asi, out/*_MAIN.zip, out/*_ASI_ONLY.zip
python3 build.py --legacy-proxy  # also builds the optional legacy V4 dinput8 bootstrap (not shipped)
tests/run_host_tests.sh /path/to/witcher3.exe
```

* Native units: the `UNITS` list in `build.py` plus `src/UNITS_V5.txt`. Flags: `/O2 /Gy /GS- /GR- /Zl /W4 /WX /Brepro`; link `/nodefaultlib /brepro /opt:ref /opt:icf`.
* Reproducibility: every build is performed twice, and the build fails unless the outputs are byte-identical. Archives use sorted entries and fixed timestamps, are also built twice, and their hashes are recorded in `out/build_manifest.json`. The full command log is in `out/build.log`.
* Localization: `tools/build_v5_locales.py` (lossless w3strings extension, REDengine 31-hash). Menu: `tools/build_v5_menu.py`. Inputs `loc/` and `menu/` are the unmodified V4.3.1 files.
* WCC: `game/wcc/precompiled.rsblob` is compiled on Windows by the REDkit WCC with the one-click kit (V5.3.0: `tools/v530/make_kit_v530.py`) and adopted only through `tools/v530/finalize_v530.py`, which checks the receipt. `build.py` refuses to package when the script source, menu or blob differ from `game/wcc/V52_VERIFIED_WCC.json`.
* Menu movie: `game/wcc/panel_ingamemenu.redswf` is the original game asset. The build applies `tools/v52_selected/separator_swf_fix.py` (size-neutral, both movie copies).
* PT shader variant (V5.3.0): built at game start by the ASI with the game's own `dxcompiler.dll` (disassemble, the text passes in `src/urt_bounce501.c`, assemble, validate and sign, re-check). Nothing is compiled at build time; `tests/host/test_bounce501.c` runs the same passes on the PT library extracted from the executable.
* Third-party: `third_party/ultimate_asi_loader_9.7.4/winmm.dll` (unmodified upstream x64 build, MIT licence, SHA-256 checked by the build).
* Byte parity with the shipped V4.3.1 binary needs the original (newer) clang. With clang 18 the V4.3.1 source builds to `361ca9d8…` (git tag-like commit `20c2055`).
