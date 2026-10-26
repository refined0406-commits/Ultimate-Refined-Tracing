# Ultimate Refined Tracing V5.2.0 Curated — Private RC3

**PRIVATE GAMEPLAY TEST CANDIDATE. NOT A NEXUS/GITHUB RELEASE.**

## Provenance and build

Protected final V5.1.0 is the baseline (development source SHA256
`564c2094b07d33a652d73a473560a2f5b8bf1bafdd8b38d64c18db6600427c41`).
RC3 preserves the accepted renderer isolation, menu/preset behaviours,
18 languages, original PT shader carrier and loader arrangement.

The native ASI is built twice with clang-cl/lld-link and compared for byte identity.
The WitcherScript was compiled on the owner's Windows REDkit with the RC3 kit,
then adopted only after receipt, source, menu and blob SHA256 verification.
Offline tests are a separate check; gameplay compatibility remains unverified.

## The seven selected V5.2 controls

* PT: Ambient Occlusion Quality.
* RT: Ambient Occlusion Screen-Space Sampling.
* RT: Screen-Space Indirect Lighting (HBIL).
* RT: Ambient Occlusion Pre-Trace.
* RT: Screen-Space Reflection Assist.
* Textures Streamed at Once (sole Textures / Streaming row).
* Frame Generation: Keep Resources in Menus (opt-in, experimental behaviour).

The author rejected PT: Scene Range and PT: Texture Detail in Reflections and
Bounces for not improving FPS in observed tests. Texture Memory Budget and
Texture Sharpness (Mip Bias) are not present because changing the texture
variables risks disturbing the game's native Texture Quality setting.

Four older PT RTAO controls have also been retired: PTRTAOEnabled,
PTRTAORayCount, PTRTAORadius, PTRTAOStrength. Other original PT AO controls,
Indirect-Light Compensation and V5.1 RT controls remain.

## Menu, saves, compatibility and safety

There are 45 visible interactive controls in one menu, five named separators
and two subtle RT separators. The existing Quick Preset mappings are unchanged,
with the new seven entries Native/Auto and not yet mapped to presets.

The owner observed RC2 reverting native Texture Quality Ultra+ to Low. RC3
removes both candidate texture-quality-owned controls; **there is not yet
Windows-gameplay proof** that Texture Quality stays Ultra+ with the remaining
streaming control. Test native graphics values, not only file hashes/lengths.

The owner also observed the right separator line vanishing after scrolling.
RC3 restores earlier caption padding for the game's native separators.
**The runtime scrolling issue is NOT confirmed fixed.** Full in-game menu
scroll and reopening are required for acceptance.

Orange Hair Fix RT+PT, Immersive World and renderer switching must be tested
in-game. No proprietary Witcher 3 game binaries are distributed.

## Compiling and testing

Use `python3 build.py --out <output_dir>` for a reproducible native build and
MAIN/ASI-ONLY packaging. Packaging refuses to run without source/menu/blob-
matched `game/wcc/V52_VERIFIED_WCC.json`. For host tests, use
`tests/run_host_tests.sh`. For the next essential gameplay checks, see
`docs/V5.2.0_PRIVATE_RC3_TEST_CHECKLIST.md`.

**Never publish or upload RC3 as a public release without in-game acceptance.**
