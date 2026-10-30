# Ultimate Refined Tracing V5.3.0: build provenance

## Compiled WitcherScript (Windows REDkit WCC, run by the author)
| Item | SHA-256 |
|---|---|
| Kit | `URT_V5.3.0_PRIVATE_WCC_KIT_v530rel-rev13` (the word PRIVATE is the internal tool name) |
| Script source compiled (`RefinedTracingCR01.ws`, staged by the kit) | `0ab1f9570335e22524450c12b85e9eb9cc6788f883cac8ddc2cc0de695556c52` |
| Menu checked against it | `5685cdf2ea2ac82777d8ef36a3daff023330da04d423635b6ea62c70ba21dab3` |
| REDkit `wcc_lite.exe` (the version updated on 29 October 2026) | `1ea22410ccca10c44b3d8a083ae8b862cdd79fa1e5cf4d028120c078078717fe` |
| `precompiled.rsblob` (shipped) | `b2d73f000155b99b0c1f013490a2ab2d5448b95d04455550bb516112048603d7` (28 209 bytes) |
| Results archive returned by the kit | `d445c7a4988b1a6a9a96c7ada6c3f8f473f38450c5ade213f90ce9d4c735310e` |

**How the compile was accepted:**
* `tools/v530/finalize_v530.py` accepted it and recorded it in `game/wcc/V52_VERIFIED_WCC.json`.
* `build.py` packages only when the script source, the menu and the blob match that record.
* The raw run capture (WCC console output, receipt and manifests) stays with the author. It contains local Windows paths and copies of game files.

## Native binary
* `UltimateRefinedTracing.asi`: `aed6ce2573156c892781da674b0dfaf847424aa0d0762a1ef844912d1e51d5b4`.
* **How it is built:** `build.py` builds it twice with clang-cl / lld-link (`/Brepro`) and requires the two builds to be byte-identical.
* **Record:** `BUILD_MANIFEST.json` lists every unit and archive hash.
* The same file is shipped in MAIN and in ASI-ONLY.
* Third-party: `winmm.dll` is the unmodified Ultimate ASI Loader 9.7.4 (`031a3e55…2daf`, MIT).

## Game executables the offline tests ran against
* Patch 5.01 `witcher3.exe`: `2df76539…b101`, build 1048522.
* Steam update `witcher3.exe`: `05948d72…bae0`, build 1090142.
