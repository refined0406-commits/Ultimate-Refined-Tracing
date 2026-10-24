# Ultimate Refined Tracing V5.1.0 - source for Nexus security review

Source snapshot of the published V5.1.0 native mod and WitcherScript modifications, prepared from the author's authoritative **V5.1.0 DEVELOPMENT_SOURCE** archive for Nexus Mods security review of mod #13075.

This repository is a **review-oriented current-version source snapshot** and does **not** include historical Git objects, previously rejected private experiments, game binaries, or compiled distribution packages. It is not an official download for the mod; use Nexus Mods for public releases.

## Build

See [BUILD_FOR_NEXUS.md](BUILD_FOR_NEXUS.md). The native `UltimateRefinedTracing.asi` can be built using Python 3.10+, LLVM clang-cl 18 and lld-link 18, without Visual Studio or a Windows SDK. To compile the native ASI only:

```bash
python3 build.py --no-package
```

Build output: `out/bin/UltimateRefinedTracing.asi`.

The native ASI SHA256 for final V5.1.0 is:

`942cddbd31cb8e4f72ec94fb80413ddb2da8425175d7569dcb2e46bf8a93cd27`

The exact pinned toolchain used in the original release build is Ubuntu clang 18.1.3 (1ubuntu1). Other toolchain versions may compile successfully but produce a different binary hash.

## Repository contents

- `src/`: native C source, headers, import definitions and module list.
- `build.py`: reproducible two-pass native build and optional release packager.
- `game/wcc/RefinedTracingCR01_RTGI3.ws`: final WitcherScript source, compiled separately using Witcher 3 REDkit.
- `menu/` and `loc/`: configuration and localization inputs.
- `tools/` and `tests/`: generators, source checks, and host-side tests.
- `third_party/.../LICENSE...`: credits/license for the official Ultimate ASI Loader.

## Intentionally not hosted here

The development ZIP also includes historical Git metadata, old release archives, a prebuilt loader DLL, game-derived precompiled menu/UI assets, localized binary output and generated build products. Those binary resources are excluded from this public source review snapshot. The exact release assets are already submitted to Nexus as the V5.1.0 MAIN and ASI-ONLY files. See [BUILD_FOR_NEXUS.md](BUILD_FOR_NEXUS.md) for the distinction between compiling the ASI and reproducing the complete distributable ZIP.

This review snapshot is provided to aid provenance/security assessment. It does not grant permission to reuse proprietary game assets or third-party binaries.
