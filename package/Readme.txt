ULTIMATE REFINED TRACING V5.3.0 - README

Ray Tracing and Path Tracing Performance Overhaul for The Witcher 3: Wild Hunt
(next-gen / Remastered, DX12) - game version 5.01 and the Steam update of
29 October 2026 (build 1090142).

ABOUT
Ultimate Refined Tracing (URT) adds one settings page, Options > Mods >
Ultimate Refined Tracing, with sections for CPU, Path Tracing, Ray Tracing,
RTGI: Quality, RTGI: Performance, RT/PT: Shared, Textures / Rendering,
Memory & Streaming, RT Memory Management and Frame Generation, in 18
languages. Every setting has a description and defaults to Native / Auto
(= the game's own value). Master On/Off returns everything to the game.

WHAT IS NEW IN V5.3.0
Path Tracing shader optimisations (Path Tracing section, below PT: Geometry
Bounces; Native / Auto by default; GAME RESTART REQUIRED - they are built into
URT's Path Tracing shader when the game starts):
- PT: First Bounce Diffuse Coverage (50%): diffuse light of the first bounce
  is traced for half of the pixels per frame in a moving, weighted pattern.
  Less Path Tracing work; indirect light can get a little noisier.
- PT: Bounce Sun Shadow Coverage (50%): sun / moon shadow visibility at bounce
  points is calculated for half of the pixels per frame (weighted). Possible
  shadow noise or flicker.
- PT: Bounce Local Light Shadow Coverage (50%): the same for lamps, torches
  and fires; helps mostly indoors and in towns at night.
Recommended for performance: PT: Geometry Bounces 1 + all three at 50%.
Example (author's RTX 4080, DLSS Quality, Frame Generation off, Geometry
Bounces 1): Kaer Morhen outdoors 58-59 FPS native, 63-64 FPS with Diffuse
50%, about 65 FPS with Diffuse + Sun Shadow 50%; indoors 76-77 FPS with
Diffuse + Sun Shadow 50%, 77-78 FPS with all three. Results vary with the
scene, settings and hardware.
More new controls (Native / Auto by default):
- PT: Moon Light (below Indirect-Light Compensation): moonlight strength at
  night, 0.25 - 3.00 (game: 1.00). Visual only; applies immediately.
- PT: Radiance Cache Density; RT: Async AO, Async GI Shading, Async GI
  Classification; RT: Tree Wind Distance; RT Animated Geometry Trace Mode.
  The Async settings and Trace Mode apply at the next game start.
Game update of 29 October 2026 (build 1090142) supported; Patch 5.01 keeps
working exactly as before.

QUICK PRESETS (Performance / Balanced / Extreme)
Presets stay editable: changing any preset row switches Quick Preset to Off
and keeps every value. Rows marked * apply at the next game start.
- PT: First Bounce Diffuse Coverage*        50% / 50% / Native
- PT: Bounce Sun Shadow Coverage*           50% / 50% / Native
- PT: Bounce Local Light Shadow Coverage*   50% / 50% / Native
- PT: Moon Light                            Native in all three
- PT: Geometry Bounces                      1 / 1 / 4
- PT: Hair Bounces                          1 / 4 / 10
- PT: Scene Range                           150 m / Native / Native
- PT: Ambient Occlusion                     Off / Native / Native
- PT: Ambient Occlusion Quality             0 / 0 / 3
- PT: Radiance Cache Density                8 / 6 / Native
- RT: Async AO*, RT: Async GI Shading*      On / On / Native
- RT: Async GI Classification*              On / On / Native
- RT Reflections: Local Lights              Off / Off / Native
- RT Shadows: Local Light Shadows           Off / 4 / Native
- RT Shadows: Foliage Detail                Very Low / Low / Native
- RT: AO Screen-Space Sampling, HBIL,
  AO Pre-Trace                              Off / Off / Native
- RT: Screen-Space Reflection Assist        On / On / Native
- RT/PT: Fog in Ray-Traced Reflections      Off / Native / Native
- RT: Tree Wind Distance                    10 m / 15 m / Native
- RT: GI Alpha-Test Texture Detail          8 / 6 / 4
- Textures Streamed at Once                 8 / 8 / 16
- Terrain Preload Distance                  10 m / 10 m / Native
- Texture Unloading Speed                   4x / 2x / Native
- Mesh / General Streaming Distance, Memory Cleanup Threshold / Interval,
  RT AS Defragmentation / Build Batch Size  Native in all three
- Frame Generation: Keep Resources in Menus On in all three
The presets also set the CPU options, the remaining Path Tracing rows and the
RTGI Quality / Performance rows, as in V5.2.0.

REQUIREMENTS
- The Witcher 3: Wild Hunt, next-gen/Remastered DX12, game version 5.01
  (build 1048522) or the 29 October 2026 update (build 1090142). Other
  versions: URT runs a reduced, safe feature set and says so in its log.
- SharedUtils 4.1, including its mod and DLC parts (not bundled):
  https://www.nexusmods.com/witcher3/mods/12997
- An Ultimate ASI Loader. The MAIN file includes one (winmm.dll, unmodified
  v9.7.4 by ThirteenAG, MIT licence). Use ASI-ONLY if you already have one.

WHICH FILE?
  MAIN     = URT + Ultimate ASI Loader as bin\x64_dx12\winmm.dll.
             Use this unless you already have an ASI loader.
  ASI-ONLY = URT without a loader, for an existing Ultimate ASI Loader
             (for example a dinput8.dll or winmm.dll installed by another
             mod).
  Install exactly ONE of them.

NEW INSTALLATION
1. Install SharedUtils 4.1.
2. Extract MAIN (or ASI-ONLY) into the game folder (the folder that contains
   bin\ and mods\), merging folders. With a mod manager, install it normally.
3. Start the game: Options (Settings) > Mods > Ultimate Refined Tracing.

UPDATING FROM V5.0.0, V5.1.0 OR V5.2.0
Exit the game. Back up Documents\The Witcher 3\dx12user.settings and keep your
old archive for rollback. Install V5.3.0 over it (same paths). Your settings
are kept; values of removed controls are ignored. If an old V5 preview left
bin\x64_dx12\UltimateRefinedTracing.addon64, delete it.

UPDATING FROM V4.x
1. Exit the game. Keep your V4.3.1 archive and back up
   Documents\The Witcher 3\dx12user.settings.
2. Remove V4 (mod manager: disable/uninstall it), or delete by hand:
     bin\x64_dx12\RefinedTracing.asi
     bin\x64_dx12\dinput8.dll  ONLY if it is URT V4's own file (about 4.5 KB).
                               A dinput8.dll of several hundred KB is another
                               mod's ASI loader: KEEP it.
   If RefinedTracing.asi stays installed, V5 stays inactive and its log says so.
3. Install V5.3.0 as above. Your V4 menu values are kept and your V4
   preferences are copied to Documents\The Witcher 3\
   UltimateRefinedTracing.settings on first start (the old file is never
   changed).

STARTUP SAFEGUARD
Settings that apply at game start (the three coverage options, Async GI
Classification, Trace Mode) work from the first frame after the restart.
Quitting normally is never treated as a crash, however short the session.
If the game ends without a clean exit before a new setting has proved itself
(coverage options: 300 rendered Path Tracing frames, about 5 s; Async GI
Classification / Trace Mode: 1 minute of RT/PT), the next start runs without
it once and the one after retries it; a second failure turns it off until you
choose Native / Auto for one start. Every decision is in the log.
Documents\The Witcher 3\UltimateRefinedTracing.startupguard may always be
deleted.

SETTINGS PROTECTION
URT never writes dx12user.settings itself. Engine values the game saves
itself are applied for the session only, so URT's choices never become your
graphics settings. Native / Auto and Master Off give every value back to the
game. Texture Quality and Foliage Visibility Range are never touched.

COMPATIBILITY
- Orange Hair Fix RT+PT: supported. With the coverage options, its Path
  Tracing hair fix is built into URT's Path Tracing shader.
- Beardmo's Immersive World: supported. Its own ASI loader and URT's
  winmm.dll may be installed together; each mod loads once. URT does not
  touch its grass, foliage, wind or trampling settings.
- Other mods that change the same Path Tracing shader or game code: URT
  verifies every site before changing it and leaves anything already changed
  alone (its log says which feature stays native). Use one mod per feature.
- If another mod sets the same engine settings as URT's Memory & Streaming or
  RT Memory Management rows, leave those URT rows on Native / Auto.

GAME FILES
URT V5.3.0 ships no game files. Earlier versions (V4.3.1 - V5.2.0) installed
a copy of the game's menu file at
mods\modRefinedTracing\content\gameplay\gui_new\swf\mainmenu\panel_ingamemenu.redswf.
Installing V5.3.0 over them leaves that old copy in place (harmless; delete it
to use the game's own file). With the game's own file, a heading's right-hand
line can sometimes be missing after scrolling; nothing else changes.

TROUBLESHOOTING
- Check bin\x64_dx12\UltimateRefinedTracing.log (recreated each launch; save
  it before restarting). "exact Patch 5.01 executable" (or "exact game build
  1090142 executable") + "runtime tier FULL" + "PT shader slots installed" =
  everything active.
- URT menu missing: SharedUtils 4.1 (mod AND DLC parts) is not installed, or
  bin\config\r4game\user_config_matrix\pc\RefinedTracing.xml is missing.
- No log file: the ASI loader is not running. MAIN: check that
  bin\x64_dx12\winmm.dll exists. ASI-ONLY: check your existing loader.
- Log says "dormant - legacy V4 RefinedTracing.asi": delete
  bin\x64_dx12\RefinedTracing.asi.
- Log says "runtime tier LATE" or "ADAPTIVE": the loader started URT too late
  or the game build is not one URT knows. Engine-setting and CPU controls
  still work; the Path Tracing shader controls stay native. Use MAIN's
  winmm.dll loader.
- Immersive World script errors after installing: reinstall Immersive World
  with its required files, then URT; never delete its ASI loader.
- Coverage options not active: the log says why (for example "NOT applied
  this start" from the startup safeguard, or "paused" with a reason).
- Settings problems: URT keeps UltimateRefinedTracing.settings.bak and
  repairs itself; a damaged file is kept as .corrupt for inspection.

ANTIVIRUS
URT is an ASI (a DLL loaded into the game). Some antivirus programs flag new
game-mod DLLs by heuristics. If UltimateRefinedTracing.asi disappears, check
your antivirus quarantine. Compare the archive's SHA-256 with the published
checksums; the full source code is published with every release.

UNINSTALL
Exit the game. Remove (or disable in your mod manager):
  bin\x64_dx12\UltimateRefinedTracing.asi
  bin\x64_dx12\winmm.dll  (MAIN only, and only if no other mod uses it)
  bin\config\r4game\user_config_matrix\pc\RefinedTracing.xml
  mods\modRefinedTracing
Optional: Documents\The Witcher 3\UltimateRefinedTracing.settings, .bak and
.startupguard. Its [URTExtended] lines in dx12user.settings stay but are
ignored by the game.

ROLLBACK
Exit the game and install the complete V5.2.0 (or older V5) MAIN or ASI-ONLY
archive over V5.3.0. The V5.3.0 rows disappear from the menu; all other values
are kept. For V4.3.1: remove V5 as in UNINSTALL, then install the complete
V4.3.1 archive (it still reads RefinedTracing.settings, which V5 never
changes).

SUPPORT
https://www.nexusmods.com/witcher3/mods/13075 - please attach
UltimateRefinedTracing.log.

LICENCES AND CREDITS
- Ultimate ASI Loader by ThirteenAG, MIT licence, shipped unmodified in MAIN:
  LICENSES\Ultimate-ASI-Loader-LICENSE.txt.
- SharedUtils (required, not bundled).
- Orange Hair Fix RT+PT: original Ray Tracing fix by Euricoaw; Path Tracing
  extension by Refined.
