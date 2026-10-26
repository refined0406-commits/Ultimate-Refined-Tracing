# V5.0.0-rc5: cleanup release candidate

**Purpose:** remove the unsuccessful RC4 experiment **RT/PT: Tree Shadow Distance** and return to the RC3 foundation. RC3 was RC2 (gameplay-tested) plus the menu-label fix. There are no other changes: no rendering, performance or shader changes.

## What changed vs RC4
* **Runtime:**
  * `urt_tree_shadow.{c,h}` deleted and removed from the build unit list;
  * every Tree Shadow code path in `cr01_prod_runtime501.c` removed (CVar binding, preference read, refresh, shutdown release, self-check key).
  * The ASI no longer contains the strings `RTTreeShadowDistance`, `RTSTConfigInstancingDistance`, `RTSTConfigUpdateDistance` or `RTSTConfigUpdateWindSimBlendDistance`; `verify_release.py` asserts this.
* **Menu:** the `RTTreeShadowDistance` row is removed. The menu again equals V4.3.1 plus the separator plus PT: Shader Compatibility.
* **Localization:** the 4 RC4 strings and their `option_` twins are removed in all 18 languages. Each language again has exactly V4.3.1 plus 5 keys.
* **Tests:** RC4's tree-shadow unit and evidence tests removed. Added the release-verify residue checks and `evidence_distant_trees.py` (closed investigation, read-only).

## Equivalence with RC3 (measured)
* Both archives contain the same file list as RC3.
* Every file except two is byte-identical to RC3: menu XML, all w3strings, precompiled.rsblob, panel_ingamemenu.redswf, winmm.dll and the licence.
* **IMPORTANT README!.txt** changed (text only).
* **UltimateRefinedTracing.asi:** same size as RC3; exactly 9 bytes differ:
  * 1 byte: the version string `rc3` → `rc5`;
  * 8 bytes: the deterministic build-ID stamp (PE header and debug directory, 4 bytes each), which follows from the version string.
  * The machine code is therefore identical to RC3.

## Saved RC4 values
* RC4 kept the choice only as `RTTreeShadowDistance` in the game's menu settings, and wrote the three distances **only in memory**. Nothing was ever persisted to disk as a distance.
* RC5 never reads that key, so an old saved 30 m or 40 m choice has no effect. From the first launch, the game uses its own 20 / 20 / 15 m.
* The leftover key line is not deleted; URT never writes dx12user.settings. It is harmless: nothing reads it, and the game may drop it on its own.
* UltimateRefinedTracing.settings never contained this setting, and all other preferences are untouched.

## Rollback
* **RC4 or RC3:** install that archive over RC5 (same file names). Preferences are kept.
* **V4.3.1:** see IMPORTANT README!.txt.
* Checkpoints `/home/claude/CHECKPOINTS/URT_V5.0.0-rc1..rc4` and V4.3.1 are untouched.

## Gameplay check (about 3 minutes)
1. Launch the game. The URT menu ends with PT: Shader Compatibility, and there is no Tree Shadow row.
2. The log says `5.0.0-rc5` and contains no `Tree Shadow Distance` line.
3. Your existing URT settings and Quick Preset show the same values as before. A short PT walk looks and performs like RC3/RC4 on Native / Auto.
