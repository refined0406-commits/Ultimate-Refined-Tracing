# Ultimate Refined Tracing changelog

## 5.3.0 (release; changes since V5.2.0)
**Path Tracing shader optimisations** (Path Tracing section, below PT: Geometry Bounces; Native / Auto by default; game restart required). They are built at game start into a variant of URT's PT shader, which the game's own shader compiler validates and signs. Orange Hair Fix's PT hair patch is built into the same shader.
* **PT: First Bounce Diffuse Coverage** (50 %): first-bounce diffuse sampling for half of the pixels per frame, in a moving, weighted pattern.
* **PT: Bounce Sun Shadow Coverage** (50 %): sun / moon shadow visibility at bounce hits for half of the pixels per frame (weighted).
* **PT: Bounce Local Light Shadow Coverage** (50 %): local-light shadow visibility at bounce hits for half of the pixels per frame. Where the light is not traced, its evaluation is skipped too.

**More new controls** (Native / Auto by default):
* PT: Moon Light;
* PT: Radiance Cache Density;
* RT: Async AO, RT: Async GI Shading, RT: Async GI Classification;
* RT: Tree Wind Distance;
* RT Animated Geometry Trace Mode.

**Quick Presets** (Performance / Balanced / Extreme), new assignments:
* the three coverage options: 50 % / 50 % / Native;
* PT: Moon Light: Native in all three;
* RT: Async GI Classification: On / On / Native;
* PT: Radiance Cache Density: 8 / 6 / Native;
* RT: Async AO, RT: Async GI Shading: On / On / Native;
* RT: Tree Wind Distance: 10 m / 15 m / Native.

Every V5.2.0 assignment is unchanged.

**Game update** of 29 October 2026 (build 1090142) supported. Patch 5.01 is unchanged.

**Startup safeguard** for the settings that apply at game start:
* they are active from the first frame;
* a normal quit is never treated as a crash;
* a suspected start-up failure runs native once and is retried, and a second failure turns the option off until Native / Auto is chosen;
* every decision is logged.

**Diagnostics:** the log records:
* whether the PT coverage is in effect or paused (and why);
* a re-check of the PT shader slots at each renderer change;
* a short session snapshot every 5 minutes (game GPU memory against its budget, process memory, URT state).

**Reliability:** the settings-change watcher now re-arms itself instead of stopping URT for the session.

**Game files:** URT no longer ships the game's menu file (`panel_ingamemenu.redswf`, a CD PROJEKT RED asset; V4.3.1 to V5.2.0 installed a copy). The game's own file is used. V5.2.0's heading-line repair in that copy is therefore not part of V5.3.0.

The development history (private test builds R1 to R12 FIX4) follows below.

## 5.3.0 PRIVATE R12 FIX4 (test build; not published)
* **Menu:** PT: Bounce Local Light Shadow Coverage moves to the Path Tracing section, beneath Bounce Sun Shadow Coverage. A saved 50 % stays selected. New tooltip in 18 languages. The EXPERIMENTAL section is removed (no rows left).
* **Startup safeguard:** PT coverage options now become safe after 300 Path Tracing frames rendered with them (about 5 s), not 1 minute; they still work from the first frame. RT restart rows keep 1 minute, because their recorded failures happened during scene loads. A session interrupted after an option qualified no longer counts against it. The misleading "10 minutes" log line is corrected.
* **Tests:** 360 rapid change / restart / exit cycles, plus deterministic interruption cases.
* **Research only:** specular coverage, adaptive shadow sampling, hit-shader cost, CPU threading. The game already schedules threads by CPU topology (P/E cores), and Path Tracing here is GPU-limited. Details: docs/v530/V5.3.0_R12_FIX4.md.
* **WCC:** script unchanged; kit v530priv-rev12 checks the new menu (the compile is expected to give the R12 FIX3 blob again).

## 5.3.0 PRIVATE R12 FIX3 (test build; not published)
* **Menu:** PT: Bounce Sun Shadow Coverage moves out of EXPERIMENTAL to sit directly beneath First Bounce Diffuse Coverage. A saved 50 % stays selected. The new tooltip in 18 languages gives the recommended set (Geometry Bounces 1, Diffuse 50 %, Sun Shadow 50 %).
* **Removed: PT: Second Bounce Coverage.** Saved values are ignored; no other setting changes. Its shader base stays at a neutral level, so the measured R12 FIX2 shader is unchanged.
* **Reliability:** audited every way a coverage option or a URT value could stop working or build up during a session. No URT-owned cause was found.
  * New log lines show when the coverage is in effect or paused (and why), re-check the shader slots, and take a session snapshot every 5 minutes with GPU and process memory.
  * The settings watcher now re-arms itself instead of stopping.
* **New EXPERIMENTAL: PT: Bounce Local Light Shadow Coverage** (50 %; not tested in game).
* **WCC:** new compile needed (kit v530priv-rev11). Details: docs/v530/V5.3.0_R12_FIX3.md.

## 5.3.0 PRIVATE R12 FIX2 (test build; not published)
* **PT: First Bounce Diffuse Coverage** now sits directly beneath PT: Geometry Bounces and offers Native / Auto and 50 % only (the 50 % shader edit is the R12 FIX1 one, unchanged). New tooltip in 18 languages.
  * New saved key `URTX_PTDiffuseCoverage`. The R12 key (where 1 meant the retired 75 %) is ignored, so no old value can enable a shader. **Choose 50 % once more.**
* **Startup guard redesigned:** a clean game exit is never treated as a crash, so a quit and immediate restart always applies the setting.
  * A session that ends without a clean exit (suspected crash) runs native once, then retries automatically; a second interruption turns the option off until Native / Auto is chosen for one start.
  * Proof after 1 minute of RT/PT (was 10 minutes). Every decision is logged with its reason.
* **Delayed FPS rise investigated:** no URT-owned delay found; nothing changed on speculation. A new log line shows when the optimisation reaches rendered PT frames.
* **New EXPERIMENTAL: PT: Bounce Sun Shadow Coverage** (50 %; Native by default; no Quick Preset; next start; not yet tested in game). The sun / moon shadow rays at bounce hits run for half of the pixels per frame, weighted ×2 (unbiased).
* **WCC:** new compile needed (kit v530priv-rev10). Details: docs/v530/V5.3.0_R12_FIX2.md.

## 5.3.0 PRIVATE R12 FIX1 (test build; not published)
* **Fixed: PT: First Bounce Diffuse Coverage was never built in game.** The shader edit looked for code labels in the spelling of the offline test tools, but the game's shader compiler spells them differently. Both spellings are now handled, and the offline tests now also run in the game compiler's syntax.
* **Better log:** a variant that cannot be built now names the exact stage and reason (compiler, text pass and anchor, assembly or validation with the compiler's own message, slot size, identity). Success confirms that the signed shader contains both edits in both PT exports.
* No menu or WitcherScript change (same R12 compile). Details: docs/v530/V5.3.0_R12_FIRST_BOUNCE.md, section 7.

## 5.3.0 PRIVATE R12 (test build; not published)
* **New: PT: First Bounce Diffuse Coverage** (EXPERIMENTAL; 75 % / 50 %; Native by default; in no Quick Preset; applies at the next start). Second module of the PT Bounce Optimisation Layer, aimed below the one-bounce cost.
  * On the first bounce, the diffuse sample of a pixel outside this frame's pattern traces nothing: no bounce ray, no hit shading, no shadow rays. The kept pixels are weighted up (4/3 or 2), so the average stays the same. The pattern moves every frame for Ray Reconstruction.
  * Reflections (specular samples) and hair are unchanged. Combines with PT: Second Bounce Coverage (same variant library; Orange Hair Fix's hair patch built in). Startup-guarded.
* **Investigation:** docs/v530/V5.3.0_R12_FIRST_BOUNCE.md. Primary visibility is raster, so at one bounce the PT cost is the bounce ray, its hit shading and about two shadow rays.
* **WCC:** new compile (one row added), kit v530priv-rev9 (blob 8c48a00b). REDkit's compiler was updated by Steam on 29 October 2026; the new compiler (wcc_lite 1ea22410…) is accepted by its exact hash.

## 5.3.0 PRIVATE R11 (test build; not published)
* **New: PT: Second Bounce Coverage** (EXPERIMENTAL; 50 % / 25 %; Native by default; in no Quick Preset; applies at the next start). First module of the PT Bounce Optimisation Layer.
  * The second bounce is traced for that share of pixels each frame, in a pattern that moves every frame; the other pixels stop after the first bounce (exactly what Geometry Bounces 1 does for them). Hair keeps its own bounce count.
  * Built at startup as a variant of URT's own PT library with the game's shader compiler (validated, signed, both edits checked), only when the option is set; Native keeps the unchanged library.
  * Orange Hair Fix: the variant carries Orange Hair Fix's own PT hair patch (its documented "Path A"; C port, output identical to its patch tool).
  * Covered by the startup guard; log proof lines.
* **Retired** (no meaningful gain in the owner's tests): RT/PT: Ray-Traced Grass Density, RT: HBIL Samples, RT: HBIL Directions, PT: Light Limit, PT: Small Object Culling, PT: Radiance Cache (SHaRC). Saved values are ignored. The R10 SHaRC crash protection stays for a game-enabled SHaRC.
* PT: Radiance Cache Density is unchanged (approved); it has no effect while the game's SHaRC is off.
* **Investigation:** docs/v530/V5.3.0_R11_BOUNCE_LAYER.md. **WCC:** new compile, kit v530priv-rev8.
* **Game build 1090142 (Steam update of 29 October 2026) supported.** All shaders are unchanged, but code and data moved. URT now recognises both executables and uses each build's own verified addresses. Without this, the update would have put every URT version into its safe ADAPTIVE mode (no PT shader or code-site features). Patch 5.01 is unchanged. Details: docs/v530/V5.3.0_R11_GAME_BUILD_1090142.md. The WCC kit stays v530priv-rev8.

## 5.3.0 PRIVATE R10 (test build; not published)
* **Fixed: the game crashed with PT: Radiance Cache (SHaRC) On** (R9).
  * With SHaRC on, the game chooses its SHaRC shaders from two bounce values in the frame data that URT's PT shader controls overwrite. With URT's values it chose two shaders that Patch 5.01 never loads, and the GPU faulted.
  * While SHaRC is on, URT now leaves those values native, so the game uses its registered SHaRC shaders.
  * A render-thread safety net switches SHaRC off for the session if the frame data would still select the unloaded shaders.
  * New log proof line: "frame data checked; the game selects its registered SHaRC shaders".
* No menu, string, Quick Preset or WCC change (same compiled script as R9).
* **Investigation:** docs/v530/V5.3.0_R10_SHARC_REPAIR.md; new test `test_sharc_r10` on the real executable.

## 5.3.0 PRIVATE R9 (test build; not published)
* **New: PT: Radiance Cache (SHaRC)** (EXPERIMENTAL; On; Native by default; in no Quick Preset; applies at the next start). First module of the PT Optimisation Layer.
  * Turns on the game's own Path Tracing radiance cache (`PTUseSHARC`), which the game ships switched off. The game allocates the cache on first use and falls back to normal Path Tracing if it cannot.
  * Covered by the startup guard. The log proves use: "PT radiance cache (SHaRC) active: the game allocated it (N entries)", read from the game's own renderer.
  * While it is on, URT's Geometry Bounces, Hair Bounces and Indirect-Light Compensation do not apply (the game uses its own SHaRC shader, which Orange Hair Fix already patches). PT: Radiance Cache Density takes effect.
* **Retired:** RT/PT: Solid Distant Foliage (the owner's test with the modified shader confirmed active showed no change).
* **Investigation:** docs/v530/V5.3.0_R9_PT_LAYER.md (bottleneck measurements, SHaRC engine path, next modules).
* **WCC:** new compile, kit v530priv-rev7 (blob 5f5adb2b).

## 5.3.0 PRIVATE R8 (test build; not published)
* **New: RT/PT: Solid Distant Foliage** (EXPERIMENTAL; 30 / 20 / 10 / 5 m; Native by default; in no Quick Preset; applies at the next start).
  * Leaves farther than the chosen distance from a ray's start count as solid, so shadow and bounce rays stop at the first distant leaf instead of testing leaf after leaf through the canopy.
  * It affects every Path Tracing ray and Normal RT reflections, RTGI and local shadows.
  * Built at startup from the game's own tree any-hit shader with the game's own shader compiler (one comparison changed); covered by the startup guard; leaves the Path Tracing shader and Orange Hair Fix untouched.
* **Retired** (no measurable change in the owner's forest test; they only reduced acceleration-structure update work): RT/PT: Ray-Traced Grass Wind, Static Ray-Traced Trees, Ray-Traced Billboard Updates.
* **Investigation:** docs/v530/V5.3.0_R8_FOLIAGE_INVESTIGATION.md (why R7 did not change forest FPS; all R7 candidates).
* **WCC:** new compile, kit v530priv-rev6 (blob 4ba66ba1).

## 5.3.0 PRIVATE R7 (test build; not published)
* **New EXPERIMENTAL section** (last, beneath Frame Generation; Native / Auto by default; in no Quick Preset; every row applies while playing):
  * PT: Light Limit (128 / 64 / 32 / 16; game: 256): fewer lights checked at every PT bounce. Lights past the limit stop lighting PT.
  * PT: Small Object Culling (1 % / 2 % / 5 % / 10 %): PT leaves small, distant objects out of its scene, as Normal RT does.
  * RT/PT: Ray-Traced Grass Density, Ray-Traced Grass Wind, Static Ray-Traced Trees, Ray-Traced Billboard Updates.
  * RT: HBIL Samples (8 / 4), RT: HBIL Directions (2).
* **PT code limits:** the light limit and the culling switch change a few fingerprinted bytes of game code. They are restored on Native / Auto, leaving PT, Master Off and exit, and never overwrite another mod's change.
* **Log:** one read-only line in PT says whether the game's PT radiance cache (SHaRC) is on.
* **Investigation:** all 513 RT/PT engine variables classified with evidence (docs/v530/R7_VARIABLE_DISPOSITION.tsv).
* **WCC:** new compile, kit v530priv-rev5 (blob ea4edb1c).
* Every Quick Preset, every R5 control and Orange Hair Fix / Immersive World handling are unchanged.

## 5.3.0 PRIVATE R5 (test build; not published)
* **RT Acceleration Structure Async Compaction removed.** In your isolated test, On crashed the game after loading. Windows reported `witcher3.exe` 0xc0000005 at 0x1cfa664, in the render task processor (a GPU command context that was never created).
  * The row is gone from the menu, from every language and from the runtime table.
  * A saved On value is ignored, and `UseAsyncCompact` is never written.
  * An R4 startup-guard record for it is dropped.
* **Kept as they are:**
  * RT: Async GI Classification (stable in your test);
  * RT Animated Geometry Trace Mode, applied only at startup (stable in your test, Fast Trace with Async GI Classification);
  * the startup guard for those two;
  * every Quick Preset, PT: Moon Light and all other features.
* **No new compile:** the compiled rev3 script still names the removed row inertly, as with the earlier retired rows.

## 5.3.0 PRIVATE R4 (test build; not published)
* **RT Animated Geometry Trace Mode is now session-fixed.** It is applied once at game start, only before ray tracing initialises, and never switched mid-game. In rev3 a live change could leave animated BLASes refitted with flags different from their build, which D3D12 forbids.
* **Startup guard for the three restart settings still under test:** a value whose session ended before 10 minutes of RT/PT play is not applied again (logged). Native / Auto for one start retries it.
* **Logging:** each applied startup override is logged by name.
* **Unchanged:** the WitcherScript and menu (rev3 compile).

## 5.3.0 PRIVATE (test build; not published)
**In the Quick Presets** (Performance / Balanced / Extreme; your final values):
* PT: Radiance Cache Density 8 / 6 / Native.
* RT: Async AO On / On / Native.
* RT: Async GI Shading On / On / Native.
* RT: Tree Wind Distance 10 m / 15 m / Native.
* Every V5.2.0 assignment is unchanged.

**Not in any Quick Preset** (Native / Auto by default):
* RT: Async GI Classification (next game start).
* RT Acceleration Structure Async Compaction (next game start).
* RT Animated Geometry Trace Mode (after a restart).
* PT: Moon Light (visual, immediate, Path Tracing only), 0.25–3.00.

**Fixes**
* The V5.3.0 row titles showed internal identifiers. The menu title key (`option_`) was missing.
* "Experimental" removed from the descriptions.
* The Master tooltip now notes that some settings need a game restart.
* The "RT memory settings applied before ray tracing initialised" log line can now appear (log accuracy only).

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
