# Ultimate Refined Tracing V5.3.0: source

The Witcher 3 Remastered DX12 Ray/Path Tracing performance mod (Nexus 13075), for game version 5.01 and Steam build 1090142. This tree builds the public V5.3.0.

* `src/`: native runtime (`cr01_*` = V4 lineage modules, `urt_*` = V5 modules, `urt_gfx_table.c` generated from `tools/v52_selected/spec.py`)
* `game/wcc/`: WitcherScript source, compiled script and WCC compilation receipt. Original Witcher 3 game assets are not redistributed.
* `tools/`:
  * menu and locale generators (`build_v5*_*.py`);
  * V5.2.0 descriptions (`tools/v53_dev/tooltips_dev2.py` and the `tip_*` files);
  * the separator fix (`tools/v52_selected/separator_swf_fix.py`);
  * the WCC kit and receipt adoption (`tools/v530/`; V5.2.0: `tools/v520_release/`);
  * the PT shader variant passes (`src/urt_bounce501.c`) and the game-build address map (`tools/port/`)
* `tests/`: host test suite (`run_host_tests.sh`)
* `docs/release_v5.3.0/`: Nexus description, GitHub release steps, verification report, build provenance (V5.2.0: `docs/release_v5.2.0/`)
* `docs/v530/`: the V5.3.0 engineering notes (PT optimisation layer, game build 1090142, startup safeguard, reliability audit)
* `docs/BUILD.md`: how to build and reproduce

History: `git log`.
