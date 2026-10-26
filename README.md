# Ultimate Refined Tracing V5.2.0: development source

The Witcher 3 Remastered DX12 Patch 5.01 Ray/Path Tracing performance mod (Nexus 13075). This tree builds the public V5.2.0, the expanded release. It includes the private development checkpoints tagged `v5.3.0-dev1` / `v5.3.0-dev2`; there is no V5.3.0 release. Earlier checkpoints are git tags, and `v5.2.0` marks the earlier, unpublished V5.2.0 candidate.

* `src/`: native runtime (`cr01_*` = V4 lineage modules, `urt_*` = V5 modules, `urt_gfx_table.c` generated from `tools/v52_selected/spec.py`)
* `game/wcc/`: the WitcherScript source, the compiled script and its WCC receipt (`V52_VERIFIED_WCC.json`), and the original menu movie
* `tools/`:
  * menu and locale generators (`build_v5*_*.py`);
  * V5.2.0 descriptions (`tools/v53_dev/tooltips_dev2.py` and the `tip_*` files);
  * the separator fix (`tools/v52_selected/separator_swf_fix.py`);
  * the WCC kit and receipt adoption (`tools/v520_release/`)
* `tests/`: host test suite (`run_host_tests.sh`)
* `docs/release_v5.2.0/`: Nexus description, GitHub release and publication steps, verification report, in-game acceptance checklist
* `docs/BUILD.md`: how to build and reproduce

History: `git log`.
