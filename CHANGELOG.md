# Ultimate Refined Tracing changelog

## 5.1.0 (built 2026-10-22)
**New Normal RT performance controls** (Normal RT only; Path Tracing and raster are never affected; each defaults to Native / Auto = V5.0.0 behaviour)
* RT Reflections: Local Lights. Off removes lamps and torches from RT reflections.
* RT Shadows: Local Light Shadows. Fewer small lights cast RT shadows (game default 8; 6 / 4 / 2 / Off).
* RT Shadows: Foliage Detail. Coarser leaf detail for shadow rays (Reduced / Low / Very Low).

**Quick Presets**
* The new controls are included: Performance = Off / Off / Very Low; Balanced = Off / 4 / Low; Extreme = Native / Auto.
* RTGI, both Quality and Performance groups:
  * Performance: Secondary Classification Rays 12 (was 4).
  * Balanced: Secondary Classification Rays 16 (was 12); Shading Frame Budget 262144 (was 196608).
* All other preset values are unchanged. Presets stay editable; changing a row switches Quick Preset to Off and keeps every value.

**Removed**
* RTGI Performance and Quality Shading Rays Per Probe. Always the game's own Native / Auto value now; values saved by earlier versions are ignored.

**Unchanged**
* Everything else from V5.0.0, including the Game/Gwent Difficulty crash fix, PT shader, CPU optimizations, renderer isolation, settings protection, loaders and compatibility with Orange Hair Fix and Immersive World.

## 5.0.0 (release candidate for review, built 2026-10-21)
**Loader and files**
* MAIN uses the standard Ultimate ASI Loader (`winmm.dll`, unmodified v9.7.4, MIT) instead of URT's own `dinput8.dll`. This ends the collision with Witcher Native Bridge. An ASI-ONLY file is provided for existing loaders.
* New names: `UltimateRefinedTracing.asi`, `.log` and `.settings`.
* V4 preferences are migrated automatically. The old file is never changed, so rollback keeps working.
* A V4 `RefinedTracing.asi` left installed is detected; V5 stays inactive and says so in its log.

**Startup and robustness**
* The executable is verified once instead of up to six times, and engine settings are indexed in one pass (about 4.3 s → 3.5 ms measured offline).
* Capability tiers:
  * FULL: known 5.01 build, early start.
  * LATE: started late; engine-setting and CPU controls only.
  * ADAPTIVE: unknown build; name-resolved settings only.
* Unsafe features stay native and the log says which and why, instead of the whole mod switching off.

**Settings safety**
* One coherent `dx12user.settings` read per menu change, with a startup self-check against the Windows INI API.
* Recovery from a missing or empty `dx12user.settings`.
* An atomic, last-known-good backup of URT's own file; a corrupt file is kept as `.corrupt`.
* URT never writes `dx12user.settings`.

**Diagnostics**
* A compatibility report in the log (loaders and known mods).
* Concise production log wording.
* The log is bounded to 64 KB per session, plus 64 KB extra reserved for errors, and is recreated each launch.

**Kept from V4.3.1**
* Every control, preset, editable preset, translation (18 languages), renderer isolation and fix.
* The PT shader (identity `f1ad5c37…`) and its controls: Geometry Bounces, Hair Bounces 1–10, Indirect-Light Compensation and PT RTAO.
* PT Shadows/Billboards, the three CPU optimizations and all Normal RT/RTGI controls.

**Compatibility**
* Beardmo's Immersive World Overhaul (with Witcher Native Bridge and ReShade) is supported.
* Orange Hair Fix RT+PT is supported.
* Beardmo's Black Hair Fix is not supported together with URT, and this is documented.

**Removed during the test phase**
* RT/PT Tree Shadow Distance (RC4).
* PT Shader Compatibility (RC2–RC8).
* The Black Hair Fix shader-layering experiment (RC6–RC8). It is not present: no ReShade add-on and no ReShade dependency.

**Fixed (pre-release hotfix)**
* Crash to desktop when changing Game Difficulty or Gwent Difficulty in Options > Gameplay (reported on Nexus against V4.3.0; present since V4.x). URT's menu script looked up the settings group of every option before the game's own handler; it now does so only for URT's own options. Only `precompiled.rsblob` changed.

**Release hardening vs RC9 (log wording only)**
* Development-era wording was replaced, e.g. "RC1 … ZERO-OPT", "private", "native-passive" and "C16/C17".
* Two purely informational lines were dropped.
* No functional change.

## 4.3.1
Protected baseline (Nexus 13075).
