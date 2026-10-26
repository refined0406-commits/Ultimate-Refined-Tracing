# V5.0.0-rc9: clean URT (Black Hair Fix integration removed, RC5 shader restored)

**Direction (project owner, 2026-10-21):**
* Black Hair Fix compatibility is cancelled.
* Targets: URT at full performance, Immersive World Overhaul (IWO), and Orange Hair Fix RT+PT.

## How RC9 was built
* Branch `dev/rc9-clean` from the protected **RC5** tag, the last gameplay-tested shader architecture.
* The only RC7 change ported is the removal of the PT Shader Compatibility selector (menu/locales).
* None of the RC6–RC8 layering code exists on this branch:
  * no `urt_pt_layer.c`;
  * no `addon/`;
  * no `cr01_build_layered_dxil`;
  * no ReShade calls or exports.
* `verify_release.py` asserts this in the shipped ASI and archives.

## Every difference vs RC5 (`git diff v5.0.0-rc5`)
| Area | Change | Why |
|---|---|---|
| `cr01_asi_entry.c` | The yield branch is removed: on tier FULL URT always installs its PT shader (the RC5 default path). The shadow redirect is installed when the shader was prepared (V4.3.1 rule). | No selector; no Black Hair Fix handling. |
| `urt_environment.c/.h` | `urt_pt_shader_should_yield` is removed. The log reports, without affecting anything, an old saved `PTShaderCompat` value as ignored, and says honestly that Black Hair Fix is not supported. | Saved values cannot steer anything; no pretended compatibility. |
| `cr01_prod_runtime501.c` | `PTShaderCompat` dropped from the self-check keys and the yield log line. | Dead code. |
| `urt_runtime.h` | `URT_SHADER_YIELDED` removed. | Dead state. |
| Menu / 18 w3strings | **Byte-identical to V4.3.1** (the RC2–RC5 separator, option and 5 strings removed). The compiled script `precompiled.rsblob` is unchanged since V4.3.1 and never referenced the option, so no WCC compile was needed. | Selector removed (owner decision, RC7). |
| README / version | RC9 text and upgrade note (delete the RC6–RC8 add-on). | — |
| Tests | `verify_release.py` checks: menu/locales = V4.3.1; ASI exports exactly InitializeASI + CR01EarlyPrepare; no add-on and no layering/ReShade code. New `compat_evidence_iwo.py`. | Evidence. |

**Unchanged vs RC5, byte for byte in source:**
* the PT shader transformation (`cr01_pt_shared_dxil.c`);
* shader slots and identity pin (`f1ad5c37`);
* the CameraCB carrier (`cr01_gate_windows.c`, `cr01_gate_core.c`);
* every CPU, RT/RTGI and PT control;
* presets, settings store and migration;
* loader and tiers.

**Archives vs RC5:** the same file list, and still no ReShade add-on. Changed: the ASI, the menu XML and the 18 w3strings (both back to V4.3.1), and the README.

## Why RC8 lost URT's FPS gains, and why RC9 restores them
* With Black Hair Fix installed, RC8 deliberately left URT's shader out of every PT pipeline Black Hair Fix did not modify (the RC7 crash fix). That was most of them.
* So Geometry Bounces 1, Hair Bounces, Indirect-Light Compensation and PT RTAO mostly did not apply.
* RC9 always installs URT's PT shader before the renderer starts, exactly as RC5 did. Without a Black Hair Fix-modified pipeline, no ReShade rebuild is involved.
* The expected result is RC5's gameplay-tested FPS. This is not claimed until measured.

## Orange Hair Fix RT+PT
* URT's PT library identity is unchanged: the same transformation source and the same runtime SHA-256 pin `f1ad5c374ae1188508568d621e8be64ede3f2167e4ab06dcfca5a6781746e6ac`. It is still installed into the executable's PT slots before the renderer starts, as in V4.3.1–RC5.
* The log still reports `HairLightFixPT.asi` when present.
* **Not verifiable here:** Orange Hair Fix's own binary or source is not in this workspace. Confirming its matching rule, load order and interaction with Hair Bounces needs the current `HairLightFixPT.asi`, or its source plus its handoff contract.

## Immersive World Overhaul (2.2.3)
`compat_evidence_iwo.py`, run read-only on your installed IWO files, passes:
* 81 raster/compute shaders, with no DXR library and no PT Final reference;
* no URT code site addressed or pattern-matched;
* none of URT's owned settings, and none of the RT-tree settings, named.

No genuine conflict was found, so RC9 makes **no IWO-specific change**:
* URT does not touch IWO's grass/terrain pipelines or its RT tree-wind compute shaders.
* Witcher Native Bridge's `dinput8.dll` loader and URT's `winmm.dll` loader coexist (each ASI loads once), as confirmed by your RC7 log.
* Runtime and visual compatibility remains a gameplay item.

## Gameplay acceptance checklist (RC9)
Use the same save, NVIDIA Frame Generation on Recommended, and IWO + Witcher Native Bridge + Orange Hair Fix installed. Black Hair Fix is not needed; if it is still installed, the log says it is unsupported.
1. **Log.** `PT shader slots installed … f1ad5c37`, `runtime tier FULL`, no "layering" lines. Delete `UltimateRefinedTracing.addon64` if it is left over from RC6–RC8.
2. **FPS at Kaer Morhen A in PT.** It should match RC5 (about 150–152 with Shadows Off, 124–126 with Shadows On + billboards). Geometry Bounces 1 vs default shows the gain.
3. **Shader controls.** Hair Bounces 1/10, ILC 1.00/1.50 and the PT RTAO controls change the image live.
4. **Other controls.** PT Shadows, Billboards, CPU ×3, Quick Presets, Master Off/On, and PT→RT→raster→PT all behave as in RC5.
5. **Orange Hair Fix.** Geralt's PT and RT hair still corrected; test Hair Bounces 1 and 10.
6. **IWO.**
   * Wind on grass and trees, and RT tree shadows moving with the trees.
   * Trampling and trails, Aard/bomb sway, water waves, HairWorks wind.
   * With the Performance preset (Billboards Off, Distant Trees On) and with Extreme.
7. **Stability.** 15+ minutes of play, fast travel, save/load, settings persist.

## Rollback
* Install RC5 (or any checkpoint) over RC9; preferences are kept.
* V4.3.1 and RC1–RC8 checkpoints are untouched.
