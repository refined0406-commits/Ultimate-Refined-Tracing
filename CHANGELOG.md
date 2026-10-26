# Ultimate Refined Tracing changelog

## 5.2.0
The complete V5.2.0. It includes everything from the earlier (never published) V5.2.0 candidate and from the private development checkpoints that followed it (tagged `v5.3.0-dev1` and `v5.3.0-dev2` in git; never released under that number).

**Menu**
* One page with named white headings: CPU, Path Tracing, Ray Tracing, RTGI: Quality, RTGI: Performance, RT/PT: Shared, Textures / Rendering, Memory & Streaming, RT Memory Management, Frame Generation (always last). 18 languages.
* Every adjustable setting has a description in all 18 languages:
  * what it does, which way to adjust it, and the trade-off;
  * its Native / Auto value, or "chosen by the game" where the value cannot be verified;
  * what it affects: FPS, loading and stutter, memory, or only visuals;
  * when it applies: immediately, in newly loaded areas, or after a restart.
* Option labels are fully localised (On/Off, "Native / Auto (value)", plain numbers).
* Separator lines (heading line | title | line):
  * the game's separator layout no longer hides a heading's right-hand line for good after laying out an empty or unmeasured title (setter fix);
  * the separator is shown before its title is measured;
  * it is laid out again whenever its row is resized or redrawn;
  * a title that cannot be measured yet is retried on the next frame.
  * Unnamed separators of other menus look as before.

**New controls** (Native / Auto by default)
* **Path Tracing:** Scene Range, Ambient Occlusion Quality, Skin Bounce Light Desaturation, No Blue Glow on Armour.
* **Ray Tracing:** Ambient Occlusion Screen-Space Sampling, Screen-Space Indirect Lighting (HBIL), Ambient Occlusion Pre-Trace, Screen-Space Reflection Assist, GI Alpha-Test Texture Detail.
* **RT/PT:** Fog in Ray-Traced Reflections.
* **Textures / Rendering:** Textures Streamed at Once, Terrain Preload Distance.
* **Memory & Streaming:** Texture Unloading Speed, Mesh Streaming Distance, General Streaming Distance, Memory Cleanup Threshold, Memory Cleanup Interval.
* **RT Memory Management:** RT Acceleration Structure Defragmentation and Build Batch Size. The game reads both once, when ray tracing starts. URT applies the chosen value before that point, so a change takes effect after a game restart.
* **Frame Generation:** Keep Resources in Menus.
* PT: Ray-Traced Shadows = Off also skips the CPU wait for the ray-traced scene (URT's form of "No Wait for RT Scene").

**Quick Presets** (Performance / Balanced / Extreme)
* PT: Scene Range 150 m / Native / Native. PT: Ambient Occlusion Off / Native / Native. PT: Ambient Occlusion Quality 0 / 0 / 3.
* RT Reflections: Local Lights Off / Off / Native. RT Shadows: Local Light Shadows Off / 4 / Native. RT Shadows: Foliage Detail Very Low / Low / Native.
* RT: AO Screen-Space Sampling, HBIL and AO Pre-Trace Off / Off / Native. RT: Screen-Space Reflection Assist On / On / Native.
* RT/PT: Fog in Ray-Traced Reflections Off / Native / Native.
* Textures Streamed at Once 8 / 8 / 16. Terrain Preload Distance 10 m / 10 m / Native. Frame Generation: Keep Resources in Menus on in all three.
* PT: Geometry Bounces 1 / 1 / 4 and PT: Hair Bounces 1 / 4 / 10 (Extreme was 6 and 6).
* RT: GI Alpha-Test Texture Detail 8 / 6 / 4.
* Texture Unloading Speed 4x / 2x / Native.
* Mesh Streaming Distance, General Streaming Distance, Memory Cleanup Threshold, Memory Cleanup Interval, RT Acceleration Structure Defragmentation and Build Batch Size: Native / Auto in all three presets (selecting a preset releases any manual value; they stay individually adjustable).
* PT: No Blue Glow on Armour is never set by a preset.
* Every other preset value is unchanged from V5.1.0, except that the Shading Frame Budget was removed.
* Editing any preset-controlled row switches Quick Preset to Off and keeps every value.

**Removed** (always the game's own value; saved values ignored)
* PT: Ray-Traced Ambient Occlusion, RTAO Ray Count, RTAO Radius, RTAO Strength.
* RTGI: Quality and Performance Shading Frame Budget.

**Settings protection**
* URT never writes dx12user.settings.
* Engine values that the game saves are applied for the session only: the game's save skips them while URT owns them, so URT's choices never become your graphics settings.
* If the game or another mod changes one of these values, URT steps back until the game has saved it.
* Texture Quality and Foliage Visibility Range are never touched.

**Compatibility**
* **Optional W3LoadFix (Nexus 13881):** detected, never required, never bundled. Variables set in its [Override] section are left to it.
* **DX12 VRAM Relief and RT Memory Stabilizer:** no longer needed. If you keep them, leave the matching URT rows on Native / Auto.

**Unchanged**
* V5.1.0 optimisations and CPU features.
* PT Hair Bounces up to 10, and Indirect-Light Compensation with its persistence fix.
* Renderer isolation and the Game/Gwent Difficulty crash fix.
* The protected PT shader (Orange Hair Fix RT+PT).
* Immersive World compatibility and ASI loader coexistence.

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
