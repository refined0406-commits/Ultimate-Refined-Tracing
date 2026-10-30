# Ultimate Refined Tracing V5: Nexus page source material (draft for the owner)

Facts below are taken from the V5.1.0 build and README. Wording and layout are the owner's choice.

## Summary
Ray- and path-tracing performance overhaul for The Witcher 3 Remastered DX12 (game version 5.01).
* An in-game menu (Options > Mods > Ultimate Refined Tracing) with live controls and Quick Presets.
* Master On/Off restores the game's own behaviour instantly.

## New in V5.1.0
* **Three Normal RT performance controls** (Normal RT only; each defaults to Native / Auto):
  * RT Reflections: Local Lights (Off removes lamps and torches from RT reflections);
  * RT Shadows: Local Light Shadows (game default 8; 6 / 4 / 2 / Off);
  * RT Shadows: Foliage Detail (Reduced / Low / Very Low).
* **Quick Presets** include them (Performance = Off / Off / Very Low; Balanced = Off / 4 / Low; Extreme = Native / Auto).
* **RTGI Quick Presets updated** for both RTGI groups: Performance Secondary Classification Rays 12; Balanced Secondary Classification Rays 16 and Shading Frame Budget 262144.
* **Removed:** RTGI Shading Rays Per Probe (Performance and Quality) is always the game's own value now.
* Upgrading from V5.0.0: install over it; all settings are kept.

## Fixed in V5.0.0
* No more crash to desktop when changing Game Difficulty or Gwent Difficulty (Options > Gameplay). Thanks to the user who reported it.

## Requirements
* The Witcher 3 next-gen DX12, game version **5.01**.
* **SharedUtils 4.1**, mod **and** DLC parts (Nexus 12997).
* An Ultimate ASI Loader. MAIN includes one (`winmm.dll`, v9.7.4, ThirteenAG, MIT).

## Files
* **MAIN:** URT plus Ultimate ASI Loader (`bin\x64_dx12\winmm.dll`). Use this unless you already have a loader.
* **ASI-ONLY:** URT without a loader, for an existing Ultimate ASI Loader (e.g. Witcher Native Bridge's `dinput8.dll`).
* Install only one of them.
* V5.0.0 (and V4.3.1) stay available as old files for rollback.

## Performance features (all optional, live)
* **Path tracing**
  * Geometry Bounces (example from the author's RTX 4080: 4 bounces 80 FPS → 1 bounce 108 FPS).
  * Hair Bounces 1–10.
  * Indirect-Light Compensation 1.00–1.50.
  * PT RTAO (rays / radius / strength).
  * PT Ray-Traced Shadows and PT Billboards.
  * Sun/Sky impact, AO, grass and specular controls.
* **Normal RT:** RTGI Performance and Quality controls; RT reflection local lights, RT local-light shadows and RT foliage shadow detail.
* **CPU:**
  * Fast Bone Reads;
  * Distant Trees (RT billboard proximity);
  * Physics Level of Detail.
* **General:** Quick Presets with editable values; Master On/Off; renderer isolation (PT, Normal RT and raster each get only their own settings).

## Installing (new)
1. Install SharedUtils 4.1.
2. Install MAIN (or ASI-ONLY) into the game folder, merging `bin` and `mods`.
3. Open the URT menu in game.

## Upgrading from V4.x
1. Back up `Documents\The Witcher 3\dx12user.settings`.
2. Uninstall V4, or delete by hand:
   * `bin\x64_dx12\RefinedTracing.asi`;
   * `bin\x64_dx12\dinput8.dll`, **only** if it is URT V4's file (about 4.5 KB). A several-hundred-KB `dinput8.dll` belongs to another mod's ASI loader (e.g. Witcher Native Bridge): keep it.
3. Install V5.
   * Menu values carry over.
   * V4 private preferences are copied to `UltimateRefinedTracing.settings`; the old file stays for rollback.
4. A forgotten `RefinedTracing.asi` makes V5 stay inactive on purpose; the log says so.
5. Users of RC6–RC8 test builds should delete `UltimateRefinedTracing.addon64`.

## Compatibility
* **Supported:**
  * Beardmo's Immersive World Overhaul, with Witcher Native Bridge and ReShade;
  * Orange Hair Fix RT+PT;
  * DX12 VRAM Relief;
  * camera-light mods (URT never touches camera-light settings).
* **Not compatible:**
  * Beardmo's Black Hair Fix: its PT hair edit cannot apply with URT's PT shader.
  * HourLines' PT Optimization: it edits the same shader and code; use one.

## Troubleshooting
Point users to `bin\x64_dx12\UltimateRefinedTracing.log` and the README's troubleshooting section:
* menu missing;
* no log;
* the dormant message;
* LATE/ADAPTIVE tiers;
* `BFC_` script errors with Immersive World;
* settings repair.

## Uninstall
Delete `bin\x64_dx12\UltimateRefinedTracing.asi`, `bin\config\r4game\user_config_matrix\pc\RefinedTracing.xml` and `mods\modRefinedTracing`. Delete `winmm.dll` only if it came from URT MAIN and no other mod uses it. Optionally delete `Documents\The Witcher 3\UltimateRefinedTracing.settings*`.

## Credits and licences
* Ultimate ASI Loader by ThirteenAG (MIT), shipped unmodified in MAIN.
* SharedUtils (required, not bundled).
