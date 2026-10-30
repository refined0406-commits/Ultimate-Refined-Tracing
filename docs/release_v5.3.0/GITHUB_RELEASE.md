# GitHub release: Ultimate Refined Tracing V5.3.0

## What to publish
The public source is `Ultimate_Refined_Tracing_V5.3.0_DEVELOPMENT_SOURCE.zip`. It contains the source tree of the release commit (local tag **`v5.3.0-release`**, branch `release/v5.3.0`), with three things left out:
* `game/wcc/panel_ingamemenu.redswf`: the game's menu movie (a CD PROJEKT RED asset). Its redistribution is not authorised, and V5.3.0 no longer ships it.
* `docs/evidence*`: the raw WCC run captures. These contain local Windows paths (including the user folder name) and copies of game files (`graphics.xml`, `gc.ini`, script backups). Their hashes are summarised in `docs/release_v5.3.0/BUILD_PROVENANCE.md`.
* The private development history (`v5.3.0-private-*` test builds).

**Safe publication workflow.** It uses a separate worktree, so your main checkout is never touched. It never deletes outside that worktree, never force-pushes, never rewrites history, and publishes no private tags.

1. **Create an isolated worktree** from your published V5.2.0 release:
   ```
   cd <your GitHub clone>
   git fetch origin
   git worktree add ../urt-v5.3.0 -b release/v5.3.0 <your published V5.2.0 branch or tag>
   ```
2. **Unpack the release source next to it:**
   ```
   mkdir ../urt-v5.3.0-src && cd ../urt-v5.3.0-src
   unzip -q <path>/Ultimate_Refined_Tracing_V5.3.0_DEVELOPMENT_SOURCE.zip
   ```
   Check the archive against SHA256SUMS.txt first.
3. **Preview, then copy.** Copy the release source into the worktree, removing files that V5.3.0 no longer has.
   * Run the preview first (`--dry-run`) and read the list. Nothing outside `../urt-v5.3.0` is affected, and `.git` is excluded:
     ```
     rsync -a --delete --exclude .git --dry-run --itemize-changes URT5/ ../urt-v5.3.0/
     ```
   * When the list is right, run the same command without `--dry-run`.
4. **Review the result:**
   ```
   cd ../urt-v5.3.0
   git status
   git diff --stat
   ```
   In particular, check that no `docs/evidence*` folder, no `*.redswf` and no `*.zip` remains.
5. **Commit and tag locally:**
   ```
   git add -A
   git commit -m "Ultimate Refined Tracing V5.3.0"
   git tag v5.3.0
   ```
   If `git ls-remote --tags origin v5.3.0` shows that the tag already exists on GitHub, choose another name. Never move an existing tag.
6. **Publish only this branch and this one tag:**
   ```
   git push origin release/v5.3.0
   git push origin refs/tags/v5.3.0
   ```
   Then create the GitHub release from tag `v5.3.0`. Attach the three archives and SHA256SUMS.txt.
7. **Clean up** (optional): `git worktree remove ../urt-v5.3.0`, or keep it.

**Never use** `--force`, `git tag -f`, `git push --tags` or `git push --mirror`. Also never push the local private repository, which holds the `v5.3.0-private-*` tags and the raw evidence.

**Excluded from the public source:**
* `docs/evidence*`: raw WCC run captures, with local paths and copies of game files;
* `game/wcc/panel_ingamemenu.redswf`: CD PROJEKT RED's menu movie, not redistributed;
* the private history.

The tests that need the menu movie skip themselves when it is absent.

## Build
* **Requirements:**
  * Linux (or WSL) with `clang-cl` and `lld-link` (LLVM 17/18);
  * Python 3.10+.
* **Command:** `python3 build.py --out OUT`.

The build does the following:
* builds `UltimateRefinedTracing.asi` twice and stops unless both builds are byte-identical;
* regenerates the menu and the 18 `.w3strings` files;
* writes `OUT/Ultimate_Refined_Tracing_V5.3.0_MAIN.zip` and `_ASI_ONLY.zip`.

**Compiled WitcherScript:** `game/wcc/precompiled.rsblob` comes from the Windows REDkit WCC run recorded in `game/wcc/V52_VERIFIED_WCC.json`, which is the kit `v530rel-rev13` receipt. `build.py` refuses to package if the script source, the menu or the blob does not match that record.

## Tests
```
URT_IWO_DIR=<Immersive World bin\x64_dx12> URT_OHF_PATCH=<Orange Hair Fix RT+PT pt_plugin/tools/patch_pt.py> tests/run_host_tests.sh <witcher3.exe>
```
Run it with the Patch 5.01 executable and with the build 1090142 executable. The Immersive World and Orange Hair Fix comparisons need those mods' files and are skipped without them.

## Release assets
* `Ultimate_Refined_Tracing_V5.3.0_MAIN.zip`
* `Ultimate_Refined_Tracing_V5.3.0_ASI_ONLY.zip`
* `Ultimate_Refined_Tracing_V5.3.0_DEVELOPMENT_SOURCE.zip`
* `SHA256SUMS.txt`
* Release text: the 5.3.0 section of `CHANGELOG.md` and `docs/release_v5.3.0/NEXUS_DESCRIPTION.md`.

## Licence
* Third-party: Ultimate ASI Loader (MIT), licence in `LICENSES/` (MAIN) and `third_party/ultimate_asi_loader_9.7.4/`.
* **URT itself:** the repository has no licence file. Unless you add one, GitHub treats the code as "all rights reserved". Choose a licence before publishing if you want others to be able to reuse it.
