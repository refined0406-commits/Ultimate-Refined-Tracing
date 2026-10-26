# Ultimate Refined Tracing V5.2.0: final verification report

**Status:** the release build is ready but unpublished. It waits for your in-game acceptance; the separators are the release blocker.

**Labels used below:**
- **[automated]** means a check run here.
- **[in game]** means it can only be confirmed in the game.

## Build identity
| Item | Value |
|---|---|
| `Ultimate_Refined_Tracing_V5.2.0_MAIN.zip` | `98fc537fcf65fc3798df95e32bb592bad67c53c1be00a8f78326892cf34ce8d2` |
| `Ultimate_Refined_Tracing_V5.2.0_ASI_ONLY.zip` | `400ee4a07700bf682bcbc0819e44eb8b5d0ba532f1e6b5eec8916120ebd43fc4` |
| `UltimateRefinedTracing.asi` | `cdef02d6…`<ul><li>log line `Ultimate Refined Tracing 5.2.0`</li><li>native sources identical to the private DEV2 checkpoint; only the version define changed</li></ul> |
| Compiled script | `f13fdbb6…` (26,217 bytes), from WCC kit `v520rel-rev3` run 2026-10-26 15:45.<ul><li>Source `89c09a56…` = V5.2.0 final script + 8 hash-pinned changes (final presets: six memory/AS rows Native in every preset; Extreme Geometry Bounces 4, Hair Bounces 10).</li><li>Compiler `9f448e0c…` (same as every earlier build).</li><li>Workspaces restored; nothing installed.</li></ul> |
| Menu movie | Original game asset + separator fix parts 1–3; same size as before. |
| Build | Two-pass byte-identical (`BUILD_MANIFEST.json`). |

## The 18 requested checks
| # | Check | Result |
|---|---|---|
| 1 | Final preset assignments match your tables | **Pass** [automated]. 24 rows audited by displayed label against the menu (`test_wcc_option_routing`), using the stored option IDs: GI alpha 8/6/4, Texture Unloading 4x/2x/Native, six memory/AS rows Native in all three, Extreme Geometry Bounces 4 / Hair Bounces 10. |
| 2 | Existing preset mappings identical | **Pass** [automated]. Menu Quick Preset entries match the receipt menu; the 14 earlier script rows and all other assignments are unchanged. |
| 3 | Manual edit → Quick Preset Off | **Pass** [automated]. All eight rows are in `RT_CR01_IsPresetURTX`; other values are kept. |
| 4 | Master On/Off | **Pass** [automated]. In the real ownership core (`test_v520_presets_core`), Off restores every engine byte and On reapplies the kept preferences. |
| 5 | Native / Auto releases overrides | **Pass** [automated]. Every preset leaves the six memory/AS rows byte-identical to the game's values; Extreme also releases Texture Unloading; GI alpha = 4 (Normal RT only). |
| 6 | RT memory rows restart-dependent | **Pass** [automated]. No preset sets them. A manual value is applied at boot, before ray tracing initialises; the log reports whether the engine took it. Descriptions say "Applies after a game restart". |
| 7 | Separator repeatability evidence | **Pass offline** [automated]; **[in game] pending**. See below. |
| 8 | 18 languages complete | **Pass** [automated]. All 234 menu labels resolve. |
| 9 | Option labels localised | **Pass** [automated]. No key or identifier leaks. |
| 10 | Improved descriptions preserved | **Pass** [automated]. All 54 descriptions, idempotent, within the tooltip-area bound. |
| 11 | Skin Bounce example | **Pass** [automated]. "Geralt's skin too orange near firelight" in 18 languages; forest example gone. |
| 12 | Protected shader identity | **Pass** [automated]. 106 protected files byte-identical. The only re-pin is `separator_swf_fix.py`, which you authorised. |
| 13 | Compatibility safeguards | **Pass** [automated]:<ul><li>Immersive World static evidence;</li><li>W3LoadFix [Override] coexistence;</li><li>no bundled third-party mod files;</li><li>loader unchanged.</li></ul> |
| 14 | Graphics-settings protections | **Pass** [automated]:<ul><li>no `dx12user.settings` writer;</li><li>session-only ownership of saved engine values;</li><li>Texture Quality and Foliage Visibility Range untouched.</li></ul> |
| 15 | Genuine, source-matched WCC compile | **Pass**. All 11 receipt checks in `finalize_v520` (kit rev3). The blob is none of the earlier V5.2.0, DEV or rev1 blobs. |
| 16 | Archives, source and checksums match | **Pass** [automated]. `verify_release_v53` against the earlier V5.2.0 archives; `SHA256SUMS.txt`. |
| 17 | Version strings consistent | **Pass** [automated]. ASI, package names, README, changelog and docs say 5.2.0. No `5.3.0`/DEV identifier in any shipped file or string. |
| 18 | No protected tag overwritten | **Pass**. `v5.2.0`, `v5.3.0-dev1` and `v5.3.0-dev2` are unchanged. The release commit is tagged `v5.2.0-release`; the pre-compile checkpoints are tagged `v5.2.0-release-precompile`, `-rev2-precompile`, `-rev3-precompile`. |

**Regression:** 46 passed, 0 failed (`TEST_RESULTS.txt`). The V4.3.1 comparison is skipped because those archives were not supplied.

## Separator correction (part 3)
**Root causes found in the loaded menu movie's code:**
- **Layout runs only once.** `W3SubMenuListItemRenderer.initSeparator` lays the heading out once, when the row is filled, from the row width at that moment and the title width measured at that moment.
- **The row width can change afterwards.** The list sizes rows independently: `ScrollingList.drawLayout` sets `renderer.width` = list width − margins − scroll bar. A heading laid out at one width therefore keeps that geometry after the row is resized.
- **The title can be measured before it is shown.** `W3ScrollingList.populateData` fills every row and only then shows the list (`ShowRenderers(true)`). A title that measures 0 px at that instant leaves the heading with one line until the row is refilled.
- **Earlier fixes could not cover this.** Parts 1 and 2 made each layout correct, but nothing ever laid the heading out again.

**Fix.** It uses dead code space in the renderer's `draw()`. Its SIZE branch never runs for these rows: `BaseListItem` sets `preventAutosizing`/`constraintsDisabled`, and its autoSize `invalidateSize()` is discarded by `validateNow`.
- **On every validation of a named-heading row:** the heading is laid out again at the current row width.
- **If the title still measures 0 px:** the row retries on the next frame through a weak `ENTER_FRAME` listener on itself.
- **The listener is removed** as soon as the title is measurable, and for every other row.

**What stays the same:**
- unnamed (subtle) separators of other menus;
- the appearance and geometry;
- the menu architecture;
- the file size. Both movie copies are patched; the code length is unchanged and max_stack goes from 3 to 6.

**Evidence** (`test_separator_lifecycle_v520`, real bytecode of `initSeparator`, `draw` and the width setter from both movie copies, inside a model of CLIK invalidation, `populateData`, `drawLayout`, recycling and reopening):
- **Row resized after it is filled:** both lines at the new width. The previous V5.2.0 code fails this.
- **Title unmeasurable for 3 frames:** both lines once measurable, and the retry listener is removed. The previous code fails this.
- **5,000 randomised frames** (scroll, recycle, reopen, width changes, preset refreshes, subtle separators):
  - new code: 618 settled heading checks, 0 wrong, no listener left behind;
  - previous code: 193 wrong.
- **Static check of the new `draw` body:** consistent stack depth at every branch target; peak = declared max_stack.

**Not proven offline:** that these are the only failure modes in the game's renderer. Final acceptance is yours, in game.

## Git publication
See `GITHUB_RELEASE.md`. Use tag `v5.2.0-release` on branch `release/v5.2.0`. Do not force-push, do not push `--tags`, and do not move `v5.2.0`.
