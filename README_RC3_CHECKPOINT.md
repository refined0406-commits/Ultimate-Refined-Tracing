# URT V5.2.0 Curated Private RC3 — source checkpoint

**Not installable. Do not upload to Nexus/GitHub.** This checkpoint supersedes the RC2 private test candidate after the user's in-game feedback.

## Accepted requested changes
- Removed **PT: Scene Range** and **PT: Texture Detail in Reflections and Bounces** (user measured no material FPS gain).
- Removed **Texture Memory Budget** and **Texture Sharpness (Mip Bias)** because they overlap with native Witcher 3 Texture Quality ownership. The user's Ultra+ to Low graphics-menu reset is a real release blocker; its precise cause has not been isolated beyond this credible overlap. **Textures Streamed at Once** is the ONLY texture entry retained; its graphics-menu safety is not yet validated in game.
- Retained five named white native `SEPARATOR` rows with 18-language strings. Restored the R5 leading-space/trailing nonbreaking-space caption padding to give the native layout measured text-width slack on reflow. **The user-observed disappearing right-hand separator line after scrolling is NOT verified fixed; the game's SWF can recycle these elements.** No invasive Flash/SWF patch was authorised or attempted. Game validation is required. Do not claim a confirmed scroll fix from static tests.
- Retained protected V5.1.0 Quick Presets, renderer isolation, compatibility safeguards, PT shader, approved RT controls and the existing Quick Preset choice behavior.

## Build evidence and gate
- Native ASI compiled reproducibly. See `BUILD_EVIDENCE/UltimateRefinedTracing.asi` in this source checkpoint.
- `tests/run_host_tests.sh`: **23 passed, 0 failed** (offline, no Witcher 3 exe).
- The WitcherScript source changed, and its **genuine WCC compilation has NOT occurred**. The new WCC kit is the companion `URT_V5.2.0_CURATED_RC3_WCC_KIT_R2.zip`. Run its check and compilation on the owner's Windows PC. **The old RC2 compiled script must not be reused**, and no installable MAIN/ASI-ONLY packages are generated until the result receipt and compiled blob are independently verified.
- Master Off/user graphics settings integrity and Orange Hair Fix RT+PT/Immersive World compatibility require actual in-game testing. The native game may itself save graphics preferences; a stable filesize or SHA alone does not prove semantic integrity.

## Unresolved release gates
1. Windows WCC compile and exact source/menu/blob hash verification.
2. Windows live native Ultra+ preservation across URT Master On/Off, selected controls, restarts, and game settings edits.
3. Scroll both directions across all five named separators and check both white sides remain present. Do NOT claim fixed until user verifies.
4. Settings persistence, V5.1.0 Quick Presets and Difficulty hotfix.
5. Orange Hair Fix and Immersive World in normal RT and PT.

The sole Texture / Streaming row is `Textures Streamed at Once`. If this still changes the vanilla graphics menu, remove or re-engineer it before a release.
