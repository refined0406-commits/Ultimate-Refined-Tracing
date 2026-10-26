# GitHub release: Ultimate Refined Tracing V5.2.0 (expanded)

## Version and tags
The release is **V5.2.0**. It is built from the commit tagged **`v5.2.0-release`** on branch `release/v5.2.0`.

Some existing tags must stay where they are; do not move them:
* `v5.2.0` marks the earlier V5.2.0 candidate (`9b93fa6`), which was never published.
* `v5.3.0-dev1` and `v5.3.0-dev2` are private development checkpoints. They are kept for traceability; there is no V5.3.0 release.
* Every older tag (`v5.2.0-curated-private-rc*`, `v5.2.0-rc-candidate` and the rest).

`BUILD_EVIDENCE/URT520R_history.bundle` in the DEVELOPMENT_SOURCE zip contains every branch and tag.

## Safe publication (no force-push, no history rewrite)
Run these commands in your existing local clone of the GitHub repository:
```
# 1. bring in the history (new branches land under urt520/*, tags are only added if absent)
git fetch <path>/URT520R_history.bundle 'refs/heads/*:refs/remotes/urt520/*' 'refs/tags/v5.2.0-release:refs/tags/v5.2.0-release'

# 2. check what the public repository already has
git ls-remote --tags origin v5.2.0 v5.2.0-release

# 3. publish the release branch and its tag (fast-forward only; never --force)
git push origin urt520/release/v5.2.0:refs/heads/release/v5.2.0
git push origin refs/tags/v5.2.0-release
```

On GitHub, create the release from tag `v5.2.0-release` and title it **Ultimate Refined Tracing V5.2.0**.

**If step 2 shows a `v5.2.0` tag on GitHub:** leave it alone. It is the earlier candidate.

**If GitHub has no `v5.2.0` tag** and you want the public tag to be named exactly `v5.2.0`, run `git tag v5.2.0-public v5.2.0-release` instead. Do not re-point the existing local `v5.2.0` tag.

Never use `git push --force`, `git tag -f` or `git push --tags`. `--tags` would also publish the private development tags.

## Build
* **Requirements:**
  * Linux (or WSL) with `clang-cl` and `lld-link` (LLVM 17/18);
  * Python 3.10+.
* **Command:** `python3 build.py --out OUT`.

The build does the following:
* builds `UltimateRefinedTracing.asi` twice and refuses to continue unless both builds are byte-identical;
* regenerates the menu and the 18 `.w3strings` files (including the V5.2.0 descriptions);
* applies the separator fix (parts 1 to 3) to the original menu file;
* writes `OUT/Ultimate_Refined_Tracing_V5.2.0_MAIN.zip` and `_ASI_ONLY.zip`.

**Compiled WitcherScript:** `game/wcc/precompiled.rsblob` comes from the Windows REDkit WCC run recorded in `game/wcc/V52_VERIFIED_WCC.json`, which is the kit `v520rel-rev1` receipt. `build.py` refuses to package if the script source, the menu or the blob does not match that record.

## Tests
Run:

```
URT_OUT=OUT URT_V520_PKG=<folder with the earlier V5.2.0 candidate zips> URT_IWO_DIR=<Immersive World bin\x64_dx12> tests/run_host_tests.sh <witcher3.exe 5.01>
```

`baseline_v431/` (the V4.3.1 release zips) is not in the repository. The V4.3.1 comparison is skipped without it.

## Release assets
* `Ultimate_Refined_Tracing_V5.2.0_MAIN.zip`
* `Ultimate_Refined_Tracing_V5.2.0_ASI_ONLY.zip`
* `Ultimate_Refined_Tracing_V5.2.0_DEVELOPMENT_SOURCE.zip`
* `SHA256SUMS.txt`
* `CHANGELOG.md` (5.2.0 section)
