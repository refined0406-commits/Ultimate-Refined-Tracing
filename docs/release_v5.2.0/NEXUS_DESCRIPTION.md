# Ultimate Refined Tracing V5.2.0

Ultimate Refined Tracing (URT) is a Ray Tracing and Path Tracing performance overhaul for The Witcher 3: Wild Hunt next-gen (DX12, game version 5.01).

## What's new in V5.2.0

**One clean settings page.** The page has named headings: CPU, Path Tracing, Ray Tracing, RTGI: Quality, RTGI: Performance, RT/PT: Shared, Textures / Rendering, Memory & Streaming, RT Memory Management and Frame Generation. It is available in 18 languages.

**A clear description for every setting.** Each one tells you:
* what the setting does;
* its Native / Auto value;
* whether it affects FPS, loading and stutter, memory, or only visuals;
* which way to adjust it;
* the trade-off;
* when it applies.

**New controls.** All of them are Native / Auto by default.
* **Path Tracing:** Scene Range, Ambient Occlusion Quality, Skin Bounce Light Desaturation, No Blue Glow on Armour.
* **Ray Tracing:** Ambient Occlusion Screen-Space Sampling, Screen-Space Indirect Lighting (HBIL), Ambient Occlusion Pre-Trace, Screen-Space Reflection Assist, GI Alpha-Test Texture Detail.
* **RT/PT:** Fog in Ray-Traced Reflections.
* **Textures / Rendering:** Textures Streamed at Once, Terrain Preload Distance.
* **Memory & Streaming:** Texture Unloading Speed, Mesh Streaming Distance, General Streaming Distance, Memory Cleanup Threshold, Memory Cleanup Interval.
* **RT Memory Management:** RT Acceleration Structure Defragmentation and Build Batch Size. The game reads these when ray tracing starts, so restart the game after changing them.
* **Frame Generation:** Keep Resources in Menus.

**Quick Presets cover the new controls.** Performance, Balanced and Extreme also set the new rows. PT: No Blue Glow on Armour is the exception and is never set by a preset. Presets stay editable: change any preset row and Quick Preset switches to Off.

**Removed.** These now always use the game's own value:
* PT Ray-Traced Ambient Occlusion and its Ray Count, Radius and Strength;
* RTGI Quality/Performance Shading Frame Budget.

**Your graphics settings are protected.**
* URT never writes `dx12user.settings`.
* Engine values that the game saves itself are applied for the session only.
* Texture Quality and Foliage Visibility Range are never touched.

**Menu fix.** URT now maintains the white lines of the section headings through scrolling, resizing and reopening the menu.

The full list is in the changelog.

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

\* Selecting a preset resets these to Native / Auto. A value you choose yourself takes effect after a game restart.

Extreme now uses Geometry Bounces 4 and Hair Bounces 10 (V5.1.0: 6 and 6). The other CPU, PT and RTGI preset values are unchanged from V5.1.0.

## Requirements
* The Witcher 3 next-gen DX12, game version **5.01**.
* **SharedUtils 4.1**, both the mod **and** the DLC parts (Nexus 12997).
* An Ultimate ASI Loader. MAIN includes one (`winmm.dll`, v9.7.4, ThirteenAG, MIT).

## Files
Install **one** of these:
* **MAIN:** URT plus Ultimate ASI Loader. Use this unless you already have a loader.
* **ASI-ONLY:** URT without a loader, for an existing Ultimate ASI Loader (for example Witcher Native Bridge's `dinput8.dll`).

## Installation
1. Install SharedUtils 4.1.
2. Extract MAIN (or ASI-ONLY) into the game folder, the folder that contains `bin` and `mods`, and merge folders. Alternatively, install it with your mod manager.
3. In game, open Options > Mods > Ultimate Refined Tracing.

## Updating from V5.0.0 or V5.1.0
1. Back up `Documents\The Witcher 3\dx12user.settings`.
2. Install over the old version.

Your settings are kept, and values of removed controls are ignored. If you are updating from V4.x, follow the README in the archive.

## Compatibility
* **Orange Hair Fix RT+PT:** compatible, because the URT Path Tracing shader identity is unchanged.
* **Beardmo's Immersive World Overhaul:** compatible. URT does not touch its grass, foliage, wind or trampling settings.
* **W3LoadFix (optional):** not needed and not included. If you use it, URT leaves every variable in its `[Override]` section to W3LoadFix. With Orange Hair Fix, do **not** copy W3LoadFix's `dsound.dll`, because OHF's `dsound.dll` is OHF's own loader. Instead, rename `w3loadfix.dll` to `w3loadfix.asi` and copy it with `w3loadfix.ini` and its menu XML.
* **DX12 VRAM Relief / RT Memory Stabilizer:** no longer needed. If you keep them, leave the matching URT rows on Native / Auto.
* **Mods that replace `panel_ingamemenu.redswf`:** URT ships this menu file. If another mod also ships it, your mod manager decides which copy is used.
* **Other ASI loaders:** use ASI-ONLY.
* **Not compatible:**
  * Beardmo's Black Hair Fix (it needs the original PT shader);
  * HourLines' PT Optimization (it edits the same shader and code).

## Credits and licences
* Ultimate ASI Loader by ThirteenAG (MIT), shipped unmodified in MAIN.
* SharedUtils (required, not bundled).
* Thanks to the authors of DX12 VRAM Relief, RT Memory Stabilizer and W3LoadFix for documenting the engine variables these settings use.
* Thanks to everyone who reported issues and tested.
