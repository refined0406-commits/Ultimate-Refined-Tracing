# Ultimate Refined Tracing V5.3.0

Ultimate Refined Tracing (URT) is a Ray Tracing and Path Tracing performance overhaul for The Witcher 3: Wild Hunt next-gen (DX12). It supports game version 5.01 and the Steam update of 29 October 2026 (build 1090142).

## What's new in V5.3.0

### Path Tracing shader optimisations
Three new settings in the Path Tracing section, directly below PT: Geometry Bounces. They are Native / Auto by default and **need a game restart**. URT builds them into its Path Tracing shader when the game starts, and the game's own shader compiler validates that shader.
* **PT: First Bounce Diffuse Coverage (50%):** diffuse light of the first bounce is traced for half of the pixels per frame, in a moving, weighted pattern. The result: less Path Tracing work. Indirect light can get a little noisier.
* **PT: Bounce Sun Shadow Coverage (50%):** sun and moon shadow visibility at bounce points is calculated for half of the pixels per frame. Possible shadow noise or flicker.
* **PT: Bounce Local Light Shadow Coverage (50%):** the same for lamps, torches and fires. It helps most indoors and in towns at night.

**Recommended for performance:** PT: Geometry Bounces 1 plus all three at 50%.

**Example results** (author's RTX 4080, DLSS Quality, Frame Generation off, Geometry Bounces 1):

| Scene | Setting | FPS |
|---|---|---|
| Kaer Morhen, outdoors | Native | 58–59 |
| | Diffuse 50% | 63–64 |
| | Diffuse 50% + Sun Shadow 50% | about 65 |
| Indoors | Diffuse 50% + Sun Shadow 50% | 76–77 |
| | All three at 50% | 77–78 |

Results depend on the scene, the settings and your hardware. They are not a guarantee.

### More new controls
All are Native / Auto by default.
* **PT: Moon Light:** moonlight strength at night. Visual only.
* **PT: Radiance Cache Density.**
* **RT: Async AO, RT: Async GI Shading, RT: Async GI Classification:** the game's own async-compute paths. These apply at the next game start.
* **RT: Tree Wind Distance.**
* **RT Animated Geometry Trace Mode:** applies at the next game start.

### Game update support
The Steam update of 29 October 2026 (build 1090142) is supported. Patch 5.01 works exactly as before.

### Safe to experiment
Settings that apply at game start work from the first frame after the restart. Quitting normally is never treated as a crash, however short the session.
* If the game ends without a clean exit before a new setting has proven itself, the next start runs without it once, and the start after that retries it.
* A second failure turns it off until you choose Native / Auto for one start.
* Every decision is written to the log.

## Quick Preset values (Performance / Balanced / Extreme)
| Setting | Performance | Balanced | Extreme |
|---|---|---|---|
| PT: Scene Range | 150 m | Native / Auto | Native / Auto |
| PT: Ambient Occlusion | Off | Native / Auto | Native / Auto |
| PT: Ambient Occlusion Quality | 0 | 0 | 3 |
| RT Reflections: Local Lights | Off | Off | Native / Auto |
| RT Shadows: Local Light Shadows | Off | 4 | Native / Auto |
| RT Shadows: Foliage Detail | Very Low | Low | Native / Auto |
| RT: AO Screen-Space Sampling, HBIL, AO Pre-Trace | Off | Off | Native / Auto |
| RT: Screen-Space Reflection Assist | On | On | Native / Auto |
| RT/PT: Fog in Ray-Traced Reflections | Off | Native / Auto | Native / Auto |
| PT: Geometry Bounces | 1 | 1 | 4 |
| PT: Hair Bounces | 1 | 4 | 10 |
| RT: GI Alpha-Test Texture Detail | 8 | 6 | 4 |
| Textures Streamed at Once | 8 | 8 | 16 |
| Terrain Preload Distance | 10 m | 10 m | Native / Auto |
| Texture Unloading Speed | 4x | 2x | Native / Auto |
| Mesh Streaming Distance | Native / Auto | Native / Auto | Native / Auto |
| General Streaming Distance | Native / Auto | Native / Auto | Native / Auto |
| Memory Cleanup Threshold | Native / Auto | Native / Auto | Native / Auto |
| Memory Cleanup Interval | Native / Auto | Native / Auto | Native / Auto |
| RT Acceleration Structure Defragmentation * | Native / Auto | Native / Auto | Native / Auto |
| RT Acceleration Structure Build Batch Size * | Native / Auto | Native / Auto | Native / Auto |
| Frame Generation: Keep Resources in Menus | On | On | On |
| PT: First Bounce Diffuse Coverage † | 50% | 50% | Native / Auto |
| PT: Bounce Sun Shadow Coverage † | 50% | 50% | Native / Auto |
| PT: Bounce Local Light Shadow Coverage † | 50% | 50% | Native / Auto |
| PT: Moon Light | Native / Auto | Native / Auto | Native / Auto |
| PT: Radiance Cache Density | 8 | 6 | Native / Auto |
| RT: Async AO † | On | On | Native / Auto |
| RT: Async GI Shading † | On | On | Native / Auto |
| RT: Async GI Classification † | On | On | Native / Auto |
| RT: Tree Wind Distance | 10 m | 15 m | Native / Auto |

† Applies at the next game start.

Every other preset value is unchanged from V5.2.0. Presets stay editable: change any preset row and Quick Preset switches to Off, keeping every value.

## Requirements
* The Witcher 3 next-gen DX12, game version **5.01** or the **29 October 2026 Steam update** (build 1090142).
* **SharedUtils 4.1**, both the mod **and** the DLC parts (Nexus 12997).
* An Ultimate ASI Loader. MAIN includes one (`winmm.dll`, v9.7.4, ThirteenAG, MIT).

## Files
Install **one** of these:
* **MAIN:** URT plus Ultimate ASI Loader. Use this unless you already have a loader.
* **ASI-ONLY:** URT without a loader, for an existing Ultimate ASI Loader (for example a `dinput8.dll` or `winmm.dll` installed by another mod).

Both contain the identical `UltimateRefinedTracing.asi`. SHA-256 checksums are published with the release.

## Installation
1. Install SharedUtils 4.1.
2. Extract MAIN (or ASI-ONLY) into the game folder, the folder that contains `bin` and `mods`, and merge folders. Alternatively, install it with your mod manager.
3. In game, open Options > Mods > Ultimate Refined Tracing.

## Updating from V5.0.0, V5.1.0 or V5.2.0
1. Back up `Documents\The Witcher 3\dx12user.settings`.
2. Install over the old version.

Your settings are kept, and values of removed controls are ignored. If you are updating from V4.x, follow `Readme.txt` in the archive.

## Uninstall / rollback
* **Uninstall:** remove `bin\x64_dx12\UltimateRefinedTracing.asi`, `bin\config\r4game\user_config_matrix\pc\RefinedTracing.xml` and `mods\modRefinedTracing`. Remove `bin\x64_dx12\winmm.dll` too (MAIN), unless another mod uses it.
* **Rollback:** install the complete V5.2.0 archive over V5.3.0. The new rows disappear from the menu, and all other values are kept.

## Compatibility
* **Orange Hair Fix RT+PT:** compatible. With the coverage options, its PT hair fix is built into the same shader.
* **Beardmo's Immersive World:** compatible. Its own ASI loader and URT's `winmm.dll` can be installed together, and each mod loads once. URT does not touch its grass, foliage, wind or trampling settings.
* **Other mods that change the same Path Tracing shader or game code:** URT checks every site before changing it. It leaves anything another mod has already changed alone, and its log says which feature stays native. Use one mod per feature.
* **Other mods that set the same engine settings** as URT's Memory & Streaming or RT Memory Management rows: leave those URT rows on Native / Auto.
* **Menu file:** V5.3.0 ships no game files. URT uses the game's own menu file `panel_ingamemenu.redswf`, or the copy another installed mod provides. V5.2.0 shipped a repaired copy that redrew the heading lines after scrolling. Installing V5.3.0 over V5.2.0 leaves that copy in place; it is harmless, and you can delete it to use the game's own file.

## Known limitations
* The coverage options trade a little extra noise in indirect light and shadows for speed. Ray Reconstruction smooths most of it, but how visible it is depends on the scene.
* Coverage options and startup settings only change at a game restart.
* V5.3.0 ships no game files. The game's own menu file is used, so a heading's right-hand line can sometimes be missing after scrolling (V5.2.0 repaired this in a shipped copy of the game file).
* On other game versions, URT runs a reduced, safe feature set and says so in its log.
* Long sessions: some players have reported RT/PT performance dropping after long play and recovering after a restart. URT's own audit and tests found no URT-owned cause. The log records a short memory and state snapshot every 5 minutes; please attach it to any report.

## Antivirus
URT is an ASI, a DLL loaded into the game. Some antivirus programs flag new game-mod DLLs by heuristics (for example Microsoft Defender "Wacatac...!ml"). If that happens:
* compare the archive's SHA-256 with the published checksums;
* the full source code is published with every release.

## Credits and licences
* Ultimate ASI Loader by ThirteenAG (MIT), shipped unmodified in MAIN.
* SharedUtils (required, not bundled).
* Orange Hair Fix RT+PT: the original Ray Tracing fix is by Euricoaw, and the Path Tracing extension is by Refined. Its PT hair patch is built into URT's Path Tracing shader for compatibility.
* Thanks to everyone who reported issues and tested.
