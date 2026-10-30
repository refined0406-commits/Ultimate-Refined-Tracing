# Ultimate Refined Tracing V5.3.0: final verification, compatibility and audit report

**Evidence labels:**
* **OFFLINE:** automated host tests and static checks.
* **OWNER-MEASURED:** the author's gameplay tests.
* Everything else that needs the game is listed under "Not verified".

## 1. Archives (OFFLINE)
`tools/release/finalize_release_v530.py` checks:
1. **ASI-ONLY** contains exactly 22 install files: the ASI, the menu, the compiled script, 18 languages and `Readme.txt`. There is no game asset (the game's menu movie is no longer shipped).
2. **MAIN** contains the same files plus `winmm.dll` (Ultimate ASI Loader, pinned hash) and its licence.
3. The ASI is byte-identical in MAIN, ASI-ONLY, `OUT/bin` and `build_manifest.json` (reproducible double build).
4. The ASI's version string is exactly 5.3.0; no test-build identifier is left.
5. The compiled script is the blob in the WCC receipt (`b2d73f00`, kit v530rel-rev13).
6. The menu file and the 18 `.w3strings` files are the generated ones.
7. `Readme.txt` is the public text, identical in both archives.
8. There are no logs, debug files, scripts, sources, `.json` / `.md` files or backups in the installable archives.
9. The public source has no local paths, user names or credentials, and excludes the raw WCC run captures. It carries the exact compiled script and build script of this release.
10. SHA-256 checksums are written to `SHA256SUMS.txt`.

## 2. Quick Presets (OFFLINE): before → after
The script's preset tables were compared with R12 FIX4. Exactly these five assignments were added (Performance / Balanced / Extreme), and the other **56** script assignments and the menu's RTGI preset entries are unchanged (`test_wcc_option_routing`: exact table plus the RC8 fixture):

| Setting | Before | After |
|---|---|---|
| PT: First Bounce Diffuse Coverage | not in presets | 50% / 50% / Native / Auto |
| PT: Bounce Sun Shadow Coverage | not in presets | 50% / 50% / Native / Auto |
| PT: Bounce Local Light Shadow Coverage | not in presets | 50% / 50% / Native / Auto |
| PT: Moon Light | not in presets | Native / Auto in all three |
| RT: Async GI Classification | not in presets | On / On / Native / Auto |

**Behaviour:**
* A manual change to any of these rows switches Quick Preset to Off and keeps every value (`RT_CR01_IsPresetURTX`).
* The coverage rows and Async GI Classification take effect at the next game start.

**Compile check:** the kit checked the setter counts before compiling (V5.2.0 + 17 rows), and all five names are in the compiled script.

## 3. Test results (OFFLINE)
* **Patch 5.01 (build 1048522):** 54 passed, 0 failed.
* **Build 1090142:** 48 passed, 0 failed.
* **Skipped on 1090142:** 6 Patch 5.01 research-evidence scripts. Every code site URT uses is covered there by `evidence_build_port.py` and the image tests.
* The full list is in `TEST_RESULTS.txt`.

**What the tests cover:**
* **Settings:**
  * Master On/Off restores the game's values byte for byte;
  * renderer isolation (raster / Normal RT / PT);
  * Native / Auto restoration;
  * protected game settings never written;
  * saved preferences;
  * Quick Presets (exact tables);
  * retired keys ignored.
* **Lifecycle:**
  * stress: 30 restarts × 600 random steps (settings events, renderer changes, Master toggles);
  * rapid experimentation: 360 change / start / play / exit cycles; with clean exits, every chosen option was applied at every start;
  * deterministic interruption cases;
  * crash-loop protection.
* **PT shader:**
  * the coverage edits (modules 1–4) on the PT library extracted from both executables;
  * the full variant assembles with `llvm-as`;
  * the edits are identical on the game DXC's text syntax;
  * the weights are unbiased;
  * the coverage level reaches every eligible PT frame (500 000 simulated frames).
* **Normal RT:**
  * RTGI quality / performance rows;
  * RT reflection / local-light / foliage shadow controls;
  * async flags;
  * Trace Mode;
  * legacy RT bindings on both executables.
* **CPU:** Fast Bone Reads, Physics LOD, Distant Trees.
* **Compatibility:**
  * Orange Hair Fix: the C port's output equals OHF's own patch tool;
  * Immersive World: no URT site overlaps it;
  * optional loader-based mods: their settings are left to them;
  * the loader coexistence scan;
  * the game-build address layer (both builds, every pinned address).

## 4. Code audit (static; the game code URT uses)
* **Debug-only code:** none in the linked units (no environment switches, debug output or breakpoints). Build flags `/O2 /W4 /WX /Brepro`.
* **Allocations:** all one-time. Shader backups are freed at commit; the variant library is kept for the process. Nothing is allocated per settings event, save load or renderer change.
* **Threads and hooks:**
  * one worker thread;
  * the serializer gate, shadow and CPU hooks are installed once (CAS-guarded) and never re-installed;
  * every file handle is closed in the function that opens it.
* **Shader:**
  * the PT variant is written once into both read-only `.rdata` slots;
  * it is re-checked read-only at each switch into Path Tracing;
  * the game never re-registers different bytes.
* **Retired features:**
  * PT: Second Bounce Coverage is retired from the menu, script, runtime table, safeguard and gate levels. Its shader text stays at level 0 (native bounce count) as the shared base of the three coverage edits.
  * The R6–R10 EXPERIMENTAL rows and the R12 diffuse key are ignored if saved.
  * The EXPERIMENTAL section is removed.
* **Fixes made during V5.3.0 development:**
  * the settings watcher re-arms itself instead of stopping URT;
  * the startup safeguard: a clean exit is never a crash, and PT coverage qualifies by rendered frames;
  * a misleading "10 minutes" log line was corrected.
* **Changed for the release:** test-build wording removed from two log lines and two script comments. Nothing else.

## 5. Owner-measured results (RTX 4080, DLSS Quality, FG off, Geometry Bounces 1)

| Scene | Configuration | FPS |
|---|---|---|
| Kaer Morhen, outdoors | Native | 58–59 |
| | Diffuse 50% | 63–64 |
| | Diffuse 50% + Sun Shadow 50% | about 65 |
| Indoors | Diffuse 50% + Sun Shadow 50% | 76–77 |
| | All three at 50% | 77–78 |

The author reports longer sessions as satisfactory.

## 6. Not verified (needs the game)
* The final package in game: applying each Quick Preset and restarting.
  * The log should show the three coverage options active, then "PT shader coverage in effect".
* The game's DXC validating the variant on other GPUs and drivers.
* The in-game menu presentation of the moved rows in all 18 languages (string widths are checked offline).
* **Long-session performance:** the earlier reports of FPS dropping after long play have no identified URT-owned cause. The audit and stress tests found none, but that is not proof that it cannot happen. The log's 5-minute snapshots are there for reports.
* Antivirus and Nexus scanning: not checked, and not claimed.
