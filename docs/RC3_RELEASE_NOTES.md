# Ultimate Refined Tracing V5.0.0-rc3: UI Hotfix Release Notes

Private UI hotfix candidate built on the gameplay-tested RC2. **Offline-verified only; RC3 itself is not yet gameplay-tested.**

## Defects reported in RC2 and their root causes
1. **The label showed "Cr01 ptshadercompat".** The options menu draws a row label from the key `option_` + displayName. Every V4.3.1 row has both `cr01_x` and `option_cr01_x` strings, but RC2 added only `cr01_ptshadercompat`. With the key missing, the game prettified the raw key name. An audit of all 52 menu rows showed this was the only row missing its twin.
2. **The description stayed visible in the bottom-left banner while browsing.** RC2 placed the option's description in `tooltip_cr01_fallback`. SharedUtils' menu-descriptor source (`mod_sharedutils_menudescriptors/internal.ws`) shows the fallback message on menu entry and whenever the hovered row has no description of its own, so it was effectively always on. The string was empty in V4.3.1, which is why the effect had never been visible.

## Fixes (localization only)
* Added `option_cr01_ptshadercompat`. The label is now exactly **PT: Shader Compatibility** in all 18 languages, matching URT's English "PT: …" label convention (for example "PT: Billboards").
* `tooltip_cr01_fallback` is restored to its exact V4.3.1 value (empty) in all 18 languages. Every other option's hover description is untouched.
* **Known limitation:** PT: Shader Compatibility now shows no description on hover. A per-option hover description requires adding `this.onHover('PTShaderCompat', "", "tooltip_cr01_ptshadercompat");` to the compiled WitcherScript descriptor. That needs a real WCC (REDkit `wcc_lite`) compile on Windows, which this build environment cannot run. Patching the compiled bytecode or shipping uncompiled script was ruled out. The change is prepared and isolated on branch `dev/wcc-ptshadercompat-hover`. The README notes that the option applies at the next game start.

## Proof the change set is restricted (RC3 vs protected RC2)
* Native: all 45 units compiled from both tags with the same version define are **byte-identical (45/45)**. The ASI differs only by its version string. `git diff v5.0.0-rc2 -- src/ menu/ game/ third_party/` is empty.
* Archives: same file names and order. Changed files are the 18 `.w3strings`, the ASI (version string only) and the README.
* Strings, identical in all 18 languages: changed `cr01_ptshadercompat` (label text) and `tooltip_cr01_fallback` (restored to V4.3.1); added `option_cr01_ptshadercompat`; nothing removed. Compared to V4.3.1: zero existing strings changed, 5 new keys.
* Unchanged: menu XML, `precompiled.rsblob`, UI file, loader, shader, presets, persistence, compatibility logic.

## Artifacts (SHA-256)
| File | SHA-256 |
|---|---|
| Ultimate_Refined_Tracing_V5.0.0-rc3_MAIN.zip | `7eb7bc793f455da14b6faea757d819e5cb3aff6bbe8b7ee38448e21d6a61dcd0` |
| Ultimate_Refined_Tracing_V5.0.0-rc3_ASI_ONLY.zip | `c431eed1dd9b891ec03c0148c709f0d1ef6c56b8fb52de70b9316a6f2fd47a76` |
| bin/x64_dx12/UltimateRefinedTracing.asi | `e0b9dd05d859bcc96d73354a6f076f7cb6433d3c85c90afb110f370a77fabf74` |

The source archive hash is listed in the RC3 checkpoint `SHA256SUMS.txt`.

## Build and tests
* Toolchain: Ubuntu clang 18.1.3 (clang-cl) + lld-link. Native build and both archives were each built twice, byte-identical.
* **25/25 tests passed.** The release verifier now also requires every menu row's `option_` twin and an unchanged fallback, and it fails RC2 in all 18 languages, so both defects are now caught automatically.

## Quick in-game check (about 3 minutes, no settings changes needed)
1. Replace only `mods\modRefinedTracing\content\*.w3strings` and `bin\x64_dx12\UltimateRefinedTracing.asi`, or install the RC3 archive over RC2.
2. Open Settings > Mods > Ultimate Refined Tracing. The last row reads **PT: Shader Compatibility**.
3. Browse several rows. The bottom-left banner shows only the hovered row's own description and disappears for rows without one. Spot-check three existing rows (for example Master, Hair Bounces and RTGI) to confirm their descriptions still appear.
4. Optionally switch the game language once and confirm the label still reads PT: Shader Compatibility.

Rollback: RC2, RC1 and V4.3.1 checkpoints are untouched.
