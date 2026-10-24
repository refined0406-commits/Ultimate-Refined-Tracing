# Building Ultimate Refined Tracing V5.1.0 for Nexus review

## Native ASI: reproducible source build

Confirmed build metadata from final V5.1.0 `out_v510/build_manifest.json`:

- Target: The Witcher 3 Remastered DX12, Patch 5.01, x64 Windows.
- Toolchain: Ubuntu clang 18.1.3 (1ubuntu1), `clang-cl`, `lld-link`.
- Python 3.10+.
- No CRT, Windows SDK or Visual Studio required for the native ASI.
- Main entry script: `build.py`.
- Runtime source modules: `build.py` `UNITS` plus `src/UNITS_V5.txt`.
- Compile flags include `/O2 /Gy /GS- /GR- /Zl /W4 /WX /Brepro`.
- Link flags include `/nodefaultlib /brepro /opt:ref /opt:icf`.

From the repository root, on a system with those tools accessible on PATH:

```bash
python3 build.py --no-package
```

The script builds the native ASI twice and refuses output if the two hashes differ.

Output:

```
out/bin/UltimateRefinedTracing.asi
out/build_manifest.json
out/build.log
```

Check the SHA256 hash of `out/bin/UltimateRefinedTracing.asi` with `sha256sum` (Linux) or `Get-FileHash` (PowerShell).

**Expected native V5.1.0 ASI SHA256:**

`942cddbd31cb8e4f72ec94fb80413ddb2da8425175d7569dcb2e46bf8a93cd27`

A different LLVM version or build environment may yield different output even if compilation succeeds. The supplied release manifest reports a matching two-pass reproducible build with clang 18.1.3; this GitHub source presentation was not itself recompiled by the packager.

## WitcherScript (`precompiled.rsblob`)

The V5.1.0 script source is `game/wcc/RefinedTracingCR01_RTGI3.ws`. The final release includes a compiled WitcherScript blob produced using the official Witcher 3 REDkit WCC compiler. This is a separate Windows build stage, **not** produced by the native Python ASI build step.

The original controlled WCC build used the Witcher 3 REDkit compiler (`wcc_lite.exe`) and prepared baseline/patch workspaces with game-owned script inputs. Reproducing the final blob requires a matching Windows REDkit environment and the specific V5.1 rc3 input workspace/compile workflow, which is not part of this GitHub source snapshot. An older WCC kit found in the private development archive is intentionally excluded, as it corresponds to an earlier release candidate and should not be misrepresented as the exact final compilation procedure. Nexus reviewers can inspect the final script source and compare the final compiled blob supplied in the quarantined release to the recorded hash. If exact final WCC reproduction is essential, the author should supply the corresponding final WCC kit and prerequisites privately.

**Expected compiled V5.1.0 WitcherScript blob SHA256:**

`71c07eff388b3fca5da46fb1afcccf1d325f5f8719e9cf03f10f8822c8909899`

The original release archive includes that compiled blob; the proprietary game tools/assets are not included in this repository.

## Other packaged files

The final MAIN release also includes the unmodified **Ultimate ASI Loader v9.7.4**, packaged as `winmm.dll`, sourced from its official project and distributed under MIT license:

`031a3e5576d91dce1e438d36b9a3d462c7334ab4791990a8ff1e3ddc0e132daf`

UI and localization package assets include a precompiled `.redswf` and generated `.w3strings` binaries. Source/generator inputs are included here when available; built and game-derived assets are intentionally not reposted.

`build.py` normally produces release ZIPs with its packaging stage, but that full packaging step **requires those external binary assets to be available locally**. The `--no-package` command above is intentionally the standalone native ASI reproduction path.

## Final submitted release hash checks

- MAIN zip: `db042b0c9bc158aecd058408722f923a6606de69df278ae7aabc3dfbe69bb8f9`
- ASI-ONLY zip: `ee275bd015ec3428bb7f70858a94c7392a616ae2b64f0490a9f7a221b920772c`
- Native ASI: `942cddbd31cb8e4f72ec94fb80413ddb2da8425175d7569dcb2e46bf8a93cd27`
- `winmm.dll`: `031a3e5576d91dce1e438d36b9a3d462c7334ab4791990a8ff1e3ddc0e132daf`
- Compiled WitcherScript blob: `71c07eff388b3fca5da46fb1afcccf1d325f5f8719e9cf03f10f8822c8909899`

The existing quarantined MAIN and ASI-ONLY archives can be compared to these hashes by Nexus reviewers.

## Safety & scope

The `.asi` is a native in-process Witcher 3 rendering mod. Inspect `src/` for its executable identity checks, version-specific runtime hooks, settings storage and patch logic. The support package is not a request to bypass antivirus or quarantine controls. The author requests normal security review.
