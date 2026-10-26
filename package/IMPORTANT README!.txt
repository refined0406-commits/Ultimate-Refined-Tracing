IMPORTANT README! - ULTIMATE REFINED TRACING V5.2.0

Ray and Path Tracing Performance Overhaul - Witcher 3 Remastered DX12 Patch 5.01

REQUIREMENTS
- The Witcher 3: Wild Hunt, next-gen/Remastered DX12, game version 5.01
  (other versions: URT runs a reduced, safe feature set and says so in its log)
- SharedUtils 4.1, including its mod and DLC parts (not bundled):
  https://www.nexusmods.com/witcher3/mods/12997
- An Ultimate ASI Loader. The MAIN file includes one (winmm.dll, unmodified
  v9.7.4 by ThirteenAG, MIT licence). Use ASI-ONLY if you already have one.

WHICH FILE?
  MAIN     = URT + Ultimate ASI Loader as bin\x64_dx12\winmm.dll.
             Use this unless you already have an ASI loader.
  ASI-ONLY = URT without a loader. Use with an existing Ultimate ASI Loader,
             e.g. the dinput8.dll of Witcher Native Bridge, or the winmm.dll
             of another mod.
  Install exactly ONE of them.

NEW INSTALLATION
1. Install SharedUtils 4.1.
2. Extract MAIN (or ASI-ONLY) into the game folder (the folder that contains
   bin\ and mods\), merging folders. With a mod manager, install it normally.
3. Start the game: Options (Settings) > Mods > Ultimate Refined Tracing.

UPDATING FROM V4.x (READ CAREFULLY)
1. Exit the game. Keep your V4.3.1 archive for rollback. Back up
   Documents\The Witcher 3\dx12user.settings.
2. Remove V4 (mod manager: disable/uninstall it), or delete by hand:
     bin\x64_dx12\RefinedTracing.asi
     bin\x64_dx12\dinput8.dll  ONLY if it is URT V4's own file. URT V4's
                               dinput8.dll is tiny (about 4.5 KB). If yours
                               is several hundred KB it belongs to an ASI
                               loader from another mod (e.g. Witcher Native
                               Bridge): KEEP it.
   If RefinedTracing.asi stays installed, V5 deliberately stays inactive (two
   copies must never run) and its log tells you so.
3. Install V5 as above. bin\config\...\RefinedTracing.xml and
   mods\modRefinedTracing are replaced by the V5 versions (same names).
4. Your V4 menu values are kept, and your V4 private preferences are copied to
   Documents\The Witcher 3\UltimateRefinedTracing.settings on first start.
   The old RefinedTracing.settings is never changed (rollback keeps working).
   Optional leftovers you may delete: bin\x64_dx12\RefinedTracing.log.

UPDATING FROM V5.0.0 OR V5.1.0 (or a private test build)
Exit the game. Back up Documents\The Witcher 3\dx12user.settings and keep your
old archive for rollback. Install V5.2.0 over it (same paths). Your settings are
kept; values of removed controls are ignored.

UPDATING FROM A V5 TEST BUILD
Install over it. If bin\x64_dx12\UltimateRefinedTracing.addon64 exists, delete it
(a leftover from old test builds; it stays idle).

WHAT IS NEW IN V5.2.0
One page, Options > Mods > Ultimate Refined Tracing, with named white headings:
CPU, Path Tracing, Ray Tracing, RTGI: Quality, RTGI: Performance, RT/PT: Shared,
Textures / Rendering, Memory & Streaming, RT Memory Management, Frame Generation.
Every new control defaults to Native / Auto (= the game's own value).
- Path Tracing: Scene Range, Ambient Occlusion Quality, Skin Bounce Light
  Desaturation, No Blue Glow on Armour.
- Ray Tracing: Ambient Occlusion Screen-Space Sampling, Screen-Space Indirect
  Lighting (HBIL), Ambient Occlusion Pre-Trace, Screen-Space Reflection Assist,
  GI Alpha-Test Texture Detail.
- RT/PT: Fog in Ray-Traced Reflections.
- Textures / Rendering: Textures Streamed at Once, Terrain Preload Distance.
- Memory & Streaming: Texture Unloading Speed, Mesh Streaming Distance, General
  Streaming Distance, Memory Cleanup Threshold, Memory Cleanup Interval.
- RT Memory Management: RT Acceleration Structure Defragmentation and Build
  Batch Size. The game reads these once, when ray tracing starts: a change
  takes effect after a game restart.
- Frame Generation: Keep Resources in Menus.
- Every setting has a description: what it does, its Native / Auto value, what
  it affects (FPS, loading/stutter, memory or only visuals), which way to
  adjust it, the trade-off and when it applies.
- PT: Ray-Traced Shadows = Off also skips the CPU wait for the ray-traced scene.
Quick Presets (Performance / Balanced / Extreme) also set the new rows:
  PT: Scene Range 150 m / Native / Native; PT: Ambient Occlusion Off / Native /
  Native; PT: AO Quality 0 / 0 / 3; RT Reflections: Local Lights Off / Off /
  Native; RT Shadows: Local Light Shadows Off / 4 / Native; RT Shadows: Foliage
  Detail Very Low / Low / Native; RT AO Screen-Space Sampling, HBIL and AO
  Pre-Trace Off / Off / Native; RT Screen-Space Reflection Assist On / On /
  Native; RT/PT Fog Off / Native / Native; Textures Streamed at Once 8 / 8 / 16;
  Terrain Preload Distance 10 m / 10 m / Native; Keep Resources in Menus on.
  PT: Geometry Bounces 1 / 1 / 4; PT: Hair Bounces 1 / 4 / 10.
  RT: GI Alpha-Test Texture Detail 8 / 6 / 4; Texture Unloading Speed 4x / 2x /
  Native. Mesh / General Streaming Distance, Memory Cleanup Threshold /
  Interval and RT AS Defragmentation / Build Batch Size: Native in all three
  (selecting a preset releases a manual value; each stays adjustable).
  PT: No Blue Glow on Armour is never set by a preset. Changing any preset row
  switches Quick Preset to Off and keeps every value.
Removed: PT Ray-Traced Ambient Occlusion and its Ray Count / Radius / Strength;
RTGI Quality and Performance Shading Frame Budget (always the game's value now).
Settings protection: URT never writes dx12user.settings. Engine values the game
saves itself are applied for the session only, so URT's choices never become
your graphics settings. Native / Auto and Master Off give every value back to
the game. Texture Quality and Foliage Visibility Range are never touched.
Menu: the white lines of the named headings are laid out again whenever a row
is resized or redrawn, and a title that cannot be measured yet is retried on the
next frame (fix in URT's copy of the game's menu file).
Optional W3LoadFix (Nexus 13881): not needed and not included. If you use it,
URT leaves every variable set in w3loadfix.ini [Override] to W3LoadFix.
  With Orange Hair Fix RT+PT: OHF's dsound.dll is OHF's own loader. Do NOT copy
  W3LoadFix's dsound.dll. Copy only w3loadfix.dll (renamed to w3loadfix.asi),
  w3loadfix.ini and its menu XML; URT's ASI loader (winmm.dll) then loads it.
DX12 VRAM Relief / RT Memory Stabilizer: no longer needed (URT's Memory &
Streaming and RT Memory Management rows cover them). If you keep them, leave
the matching URT rows on Native / Auto.

WHAT IS NEW IN V5.1
Three new Normal RT performance controls at the end of the URT menu (Normal RT
only; Path Tracing and raster are never affected):
- RT Reflections: Local Lights - Off removes lamps/torches from RT reflections.
- RT Shadows: Local Light Shadows - fewer small lights cast RT shadows
  (game default 8; options 6 / 4 / 2 / Off).
- RT Shadows: Foliage Detail - coarser leaf detail for shadow rays
  (Reduced / Low / Very Low).
Each defaults to Native / Auto (= V5.0.0 behaviour). Gains depend on the scene
(lights, foliage) and your GPU.
Quick Presets now include them: Performance = Off / Off / Very Low;
Balanced = Off / 4 / Low; Extreme = all Native / Auto.
Updated RTGI Quick Presets (RTGI Quality and RTGI Performance alike):
- Performance: Secondary Classification Rays 12 (was 4).
- Balanced: Secondary Classification Rays 16 (was 12), Shading Frame Budget
  262144 (was 196608).
RTGI Shading Rays Per Probe (Performance and Quality) has been removed from
the menu and is always the game's own Native / Auto value; any value saved by
an earlier version is ignored.
Presets stay editable: changing any row switches Quick Preset to Off and keeps
every value.

WHAT IS NEW IN V5
- Standard loader: MAIN uses Ultimate ASI Loader (winmm.dll) instead of URT's
  own dinput8.dll, so URT no longer collides with other mods' dinput8.dll
  (e.g. Witcher Native Bridge, required by Beardmo's Immersive World).
- New file names: UltimateRefinedTracing.asi / .log / .settings, with
  automatic migration of V4 preferences.
- Faster start: the game executable is verified once, and engine settings are
  found in one pass.
- More robust: if the game is updated, the mod is loaded late, or a feature
  cannot be applied safely, URT keeps every feature that is still safe and
  logs exactly which ones stay native and why.
- Settings safety: one consistent read per menu change, recovery from a
  missing/empty dx12user.settings at launch, and a last-known-good backup of
  URT's own settings file. URT native ASI does not directly write dx12user.settings; game-owned SaveUserSettings may rewrite it on menu edits.
- Compatibility report in the log (loaders and known mods).
- All V4.3.1 controls, presets, translations, renderer isolation and fixes
  are kept, including Geometry Bounces, Hair Bounces, Indirect-Light
  Compensation, PT Shadows/Billboards, the CPU optimizations and the
  Normal RT/RTGI controls.

COMPATIBILITY
- Beardmo's Immersive World Overhaul (with Witcher Native Bridge and ReShade):
  supported. Both ASI loaders (URT's winmm.dll and Witcher Native Bridge's
  dinput8.dll) may be installed together; each mod loads once.
- Orange Hair Fix RT+PT: supported (it recognises URT's PT shader).
- Beardmo's Black Hair Fix: NOT supported together with URT. URT replaces the
  original PT shader that Black Hair Fix looks for, so its PT hair edit does
  not apply. URT never touches its files. Use one or the other.
- HourLines' PT Optimization (xinput9_1_0.dll): edits the same shader and code
  as URT. Use one of the two; URT refuses anything already changed.
- DX12 VRAM Relief / RT Memory Stabilizer: they set the same engine variables
  as URT's Memory & Streaming / RT Memory Management rows. Not needed with URT;
  if you keep them, leave those URT rows on Native / Auto.
- LessStutter shader cache: no overlap; URT does not hook D3D12.
- W3LoadFix (optional): see "What is new in V5.2.0".
- Camera-light mods (NAPL): URT never changes camera-light settings.

TROUBLESHOOTING
- First check bin\x64_dx12\UltimateRefinedTracing.log (recreated each launch;
  save it before restarting the game).
  "exact Patch 5.01 executable" + "runtime tier FULL" + "PT shader slots
  installed" = everything active.
- URT menu missing: SharedUtils 4.1 (mod AND DLC parts) is not installed, or
  bin\config\r4game\user_config_matrix\pc\RefinedTracing.xml is missing.
- No log file at all: the ASI loader is not running. MAIN: check that
  bin\x64_dx12\winmm.dll exists. ASI-ONLY: check your other mod's loader.
- Log says "dormant - legacy V4 RefinedTracing.asi": delete
  bin\x64_dx12\RefinedTracing.asi.
- Log says "runtime tier LATE" or "ADAPTIVE": the loader started URT too late,
  or the game version is not 5.01. The engine-setting and CPU controls still
  work; the PT shader controls (Geometry/Hair Bounces, Indirect-Light
  Compensation) and PT Ray-Traced Shadows stay native. Use the MAIN
  file's winmm.dll loader.
- "Cannot find global function 'BFC_...'" (Immersive World): reinstall Witcher
  Native Bridge, Immersive World, then URT; never delete Witcher Native
  Bridge's dinput8.dll.
- UltimateRefinedTracing.asi disappeared, or URT stopped working after it
  worked before: check your antivirus quarantine/protection history. Some
  antivirus programs flag new game-mod DLLs by heuristics (for example
  Microsoft Defender "Wacatac...!ml"). Restore the file and allow it, then
  check the log starts again.
- Settings problems: URT keeps UltimateRefinedTracing.settings.bak and repairs
  itself; a damaged file is kept as .corrupt for inspection.

ROLLBACK TO PROTECTED V5.1.0
Exit the game and reinstall your preserved COMPLETE V5.1.0 MAIN or ASI-ONLY
archive. V5.2-only menu settings no longer display or apply. Do not delete
SharedUtils or another mod's ASI loader. Preserve dx12user.settings backups.

ROLLBACK TO V5.0.0
Install the complete V5.0.0 archive over V5.2.0. The three V5.1 rows simply
disappear from the menu; all other values are kept.

ROLLBACK TO V4.3.1
Delete bin\x64_dx12\UltimateRefinedTracing.asi (and winmm.dll if it came from
this MAIN file and no other mod needs it), then install the complete V4.3.1
archive. V4.3.1 still reads RefinedTracing.settings, which V5 never modifies.
Note: V4's own dinput8.dll collides with Witcher Native Bridge's.

SUPPORT
https://www.nexusmods.com/witcher3/mods/13075 - please attach
UltimateRefinedTracing.log.
Third-party licence: LICENSES\Ultimate-ASI-Loader-LICENSE.txt (MAIN only).
