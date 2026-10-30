# V5.0.0-rc2: First Gameplay Test (Windows, about 20 minutes)

Nothing here resets settings or reproduces the earlier NVIDIA Preset C incident. Leave **NVIDIA Frame Generation on Recommended** and do not change DLSS presets during the test.

## 0. Preparation (copy only, nothing deleted)
1. Under **V4.3.1**, open Settings > Mods > Ultimate Refined Tracing and take screenshots of every page.
2. Exit the game. Copy (do not move) these files from `Documents\The Witcher 3\` to a backup folder: `dx12user.settings`, and `RefinedTracing.settings` if present. Note their sizes; both must be non-zero.
3. Keep the V4.3.1 archive ready for rollback.

## 1. Install
1. Delete `bin\x64_dx12\RefinedTracing.asi`. Delete `bin\x64_dx12\dinput8.dll` **only if it came from URT** (keep it if Witcher Native Bridge installed it).
2. Install **RC2 MAIN** into the game root, merging folders. If you already run an Ultimate ASI Loader `winmm.dll`, use ASI-ONLY instead.

## 2. Startup (pass = game reaches the menu normally)
Launch DX12 and load a save in PT. Open `bin\x64_dx12\UltimateRefinedTracing.log` and expect these lines:
- `Ultimate Refined Tracing 5.0.0-rc2 | session started`
- `exact Patch 5.01 executable`
- `PT shader registration/hook preparation complete`
- `runtime tier FULL`
- `PT shader slots installed … f1ad5c37…`
- `settings snapshot self-check passed`
- `migrated RefinedTracing.settings to UltimateRefinedTracing.settings` (first launch only)
- `runtime ready`

These must **not** appear: `dormant`, `tier LATE`, `tier ADAPTIVE`, `self-check differed`.

## 3. Retained preferences
In the URT menu every value must match your V4.3.1 screenshots. The new last row "PT: Shader Compatibility" should read Ultimate Refined Tracing (default). Hover it once: an explanatory tooltip should show.

## 4. Live controls, presets and shader behaviour (PT)
1. Master Off: visuals return to native. Master On: settings return.
2. Hair Bounces 10, then back to your value. Change Indirect-Light Compensation and watch for a visible change. Geometry Bounces 1 ↔ 4 should show a visible change (proves the URT PT shader is live).
3. Select Quick Preset Balanced, then edit any single row: the preset selector must switch to Off and the other values must stay.
4. Restore your own preferred values. Quit to desktop.

## 5. Persistence and DX12 settings integrity
Relaunch. All values from step 4 must persist. Then check in `Documents\The Witcher 3\`:
- `dx12user.settings` has a non-zero size and still contains `[RefinedTracingCR01]`.
- `UltimateRefinedTracing.settings` exists.
- `RefinedTracing.settings` has an unchanged date and size.
- No `.corrupt` file exists.

## 6. Renderer isolation
In one session switch PT → Normal RT → Raster → PT. The log shows each `renderer transition`. Check for no cave corruption, no Geralt head-shadow artefact, and normal tree shadows. Normal RT presets and RTGI options apply only in Normal RT.

## 7. Performance vs V4.3.1 (same scene, same settings)
Kaer Morhen scene A, PT Performance:
- Shadows Off: expect about 150–152 FPS.
- Shadows On with RT billboards: expect about 124–126 FPS.

V5 adds no GPU optimisation, so it should match V4.3.1. A difference of more than about 2 FPS is worth reporting.

## 8. Compatibility (only the mods you actually use)
- **Orange Hair Fix:** the hair correction behaves as with V4.3.1.
- **Witcher Native Bridge + Beardmo's IWO:** wind and grass work. The URT log lists both loaders.
- **Black Hair Fix:** set PT: Shader Compatibility = Auto and restart. Hair shows BHF's look and the log says `yielding`. Then set it back to the default and restart.

## What to send back
`UltimateRefinedTracing.log` from steps 2 and 5, the FPS figures, and a pass or fail per section.

**Rollback:** delete `UltimateRefinedTracing.asi` (and `winmm.dll` if nothing else needs it), reinstall the V4.3.1 archive, and copy your backed-up settings files back if anything differs.
