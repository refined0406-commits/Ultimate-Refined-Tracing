# Ultimate Refined Tracing V5.0.0-rc2: Release Notes and Evidence

Private gameplay-test candidate. **Offline-verified only. Not gameplay-accepted.**

## Changes relative to protected RC1 (tag `v5.0.0-rc1`, untouched)
1. **Portable development source.** The `tests/runtime501` symlink is removed, and the inherited V4 tests include headers through `-I src`. This is test code only, nothing in the shipped files.
2. **Diagnostic evidence preserved.** A corrupt `UltimateRefinedTracing.settings` is moved to `.corrupt`, then `.corrupt.1` … `.corrupt.99`, never overwriting an existing file. If the evidence cannot be preserved (all names taken, or any other move failure), the invalid file stays where it is and saving URT's private file is blocked for that session, so neither it nor the `.bak` is replaced. Menu values remain live from `dx12user.settings` regardless.
3. **Metadata.** Version `5.0.0-rc2` in the log banner, README title and archive names. The test runner accepts `URT_OUT=<dir>`.

No other source, menu, locale, WCC, loader or shader change. No performance change.

## Proof that the change set is restricted
* `git diff --stat v5.0.0-rc1 v5.0.0-rc2`: one native file (`src/cr01_settings_store_windows.c`), tests, docs and version metadata.
* All 45 native units compiled from both tags with an identical version define: **44/45 objects are byte-identical**. Only `cr01_settings_store_windows.obj` differs.
* Archive members: identical names and order. Only `UltimateRefinedTracing.asi` and the README title line differ, in both MAIN and ASI-ONLY.
* ASI: identical import and export tables, identical function count (`.pdata`). `.text` is +384 bytes (the quarantine loop).

## Artifacts (SHA-256)
| File | SHA-256 |
|---|---|
| Ultimate_Refined_Tracing_V5.0.0-rc2_MAIN.zip | `7b48c98bb7a1c3f484aaaee68f784c1f55bc0f028124a65b6afb0c7bedecd483` |
| Ultimate_Refined_Tracing_V5.0.0-rc2_ASI_ONLY.zip | `ef60f2f00900ad9ebfa5c2f936422529d70e9254fe80cb30a19571791272ee44` |
| bin/x64_dx12/UltimateRefinedTracing.asi | `5c38da88901a6150cf11e48e81d641f51932d0ccd4698e28258fd99cb00402a3` |
| bin/x64_dx12/winmm.dll (UAL 9.7.4, unmodified) | `031a3e5576d91dce1e438d36b9a3d462c7334ab4791990a8ff1e3ddc0e132daf` |
| RC1 MAIN / ASI-ONLY (protected, unchanged) | `85087bdf…7eb` / `9bbab8ed…85c` |
| V4.3.1 MAIN / ASI-ONLY rollback (unchanged) | `61c30b3b…a912` / `7c623ced…5e` |

The source archive hash is listed in the checkpoint's `SHA256SUMS.txt`, because the source cannot contain its own hash.

## Build and test evidence
* Toolchain: Ubuntu clang 18.1.3 (clang-cl) + lld-link. The native build ran twice and was byte-identical. Both archives were built twice and were identical (`build_manifest.json`, `build.log`).
* Tests: **25/25 passed** (`TEST_RESULTS_rc2.txt`). That is 16 inherited V4 logic tests under ASan/UBSan, the V5 INI and store tests (store now 12 scenarios), the CVar index, scoped RTGI and exe identity tests against the real `witcher3.exe`, 3 native-evidence verifiers against the real exe, and the release verifier of RC2 against V4.3.1. The verifier covers the unchanged WCC/UI, all 50 menu rows and preset entries, 18 lossless locales, and the import allow-list with no INI writer.

## Known limitations (unchanged from RC1)
No Windows execution or gameplay testing. FPS unchanged by design. Coexistence with WNB, Beardmo's IWO and Black Hair Fix is established by static analysis only. DX12 RT Memory Stabilizer keys are unverified. Windows CryptoAPI hashing time is not measured. Byte parity with the shipped V4.3.1 binary is not possible with clang 18.
