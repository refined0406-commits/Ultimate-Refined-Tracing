# Ultimate Refined Tracing V5.0.0-rc3: Engineering Report

Private release candidate built from the V4.3.1 protected baseline. Status: **built and verified offline; Windows gameplay acceptance pending** (see section 10). The original V4.3.1 archives are included unchanged in `baseline_v431/` and are the rollback.

## 1. Baseline established

* Effective V4.3.1 native source = `SOURCE/` + `RELEASE_V422/CURRENT/SOURCE` overlay + `RELEASE_V431/CURRENT/SOURCE` overlay. The development README's statement that only two files differ omits the V4.2.2 overlay; `rebuild_native.py` applies all three, and this tree flattens them (`git` baseline commit `20c2055`).
* The shipped `RefinedTracing.asi` (SHA-256 `13313de9…`) cannot be reproduced byte-for-byte here: only clang 18 is available and the shipped binary was built with a newer clang (it materialises some string constants with immediate stores). With clang 18 the same source gives identical imports, exports and function count (`.pdata` size), and two builds are byte-identical. All V5 builds are therefore compared against this clang-18 baseline (`361ca9d8…`), not the shipped hash.
* The supplied `witcher3.exe` (SHA-256 `2df76539…`) is exactly the executable V4.3.1 is pinned to, so all binary analysis below is against the real target.
* Existing V4.3.x tests: the V4.3.1 contract test and three native-evidence verifiers pass. Two V4.1-era menu tests fail because they assert RTGI modes that V4.3.0 later merged into Auto. They are historical and were not carried forward.

## 2. Architecture changes

| Area | V4.3.1 | V5 |
|---|---|---|
| Loader (MAIN) | Own `dinput8.dll` proxy that loads only `RefinedTracing.asi` | Unmodified Ultimate ASI Loader 9.7.4 `winmm.dll` (MIT). The ASI keeps `InitializeASI` and also keeps `CR01EarlyPrepare`, so the legacy proxy still works. |
| Startup dependency | Runtime worker started only if PT shader preparation succeeded; any failure left the whole mod off | Three capability tiers. **FULL**: known code, early start; exactly the V4.3.1 path. **LATE**: known code, attached after renderer start; CVar and CPU features run, no code-site or shader patches. **ADAPTIVE**: unknown executable; only name-resolved, value-validated CVar features run. |
| Executable identity | Whole-file SHA-256 recomputed at every call site (3× on the game's main thread before startup continued, again on the worker, again on every Fast Bone Reads enable) | One cached pass computing two hashes: **EXACT** (whole file) and **CODE-IDENTICAL** (section layout, entry point, image size, and every section except `.rsrc`). A re-signed or resource-only repack keeps full features; any code or data change fails closed. |
| CVar registry resolution | Full-image scan per name (~170 ms per name on the test host) | One-pass index of registry descriptors, exact same acceptance rules (`src/urt_cvar_index.c`) |
| RTGI descriptors | Raw pinned RVAs | Resolved by (name, category, kind) and checked against the registered default. On known code the result must also equal the V4.3.1 pin. Category scoping is required because the RTXGI grids register identical names. |
| Physics LOD | Pinned RVA | Pinned on known code; on unknown code the same descriptor shape (25.0 default beside the value) is required |
| Settings reads | ~45 separate `GetPrivateProfileStringW` calls per menu event (each re-parses the file and can see a different moment of a concurrent save) | One read per event, parsed in memory (`src/urt_ini.c`). At startup every key is cross-checked against the real Windows API, and any difference reverts to the V4.3.1 per-key path for the session. |
| Invalid `dx12user.settings` at launch | Runtime stopped for the whole session | Stays native and retries the same read-only import on the next settings-directory change (no timer reads); recovers as soon as the game saves a valid file |
| Private preferences | `RefinedTracing.settings`; corrupt or unreadable → whole runtime passive | `UltimateRefinedTracing.settings`, migrated read-only from `RefinedTracing.settings` or `RefinedTracingCR01.settings` (originals kept). `ReplaceFileW` keeps a `.bak`. A corrupt file is restored from `.bak`, else defaults are used and the bad file is kept as `.corrupt`. A locked file is never overwritten, and short sharing violations are retried. |
| Instance safety | n/a | Process-scoped mutex plus legacy detection: if a V4 `RefinedTracing.asi` is installed or loaded, V5 stays dormant and logs why, so two runtimes never own the same CVars or sites |
| Diagnostics | `RefinedTracing.log` | `UltimateRefinedTracing.log`: version, identity tier, capability tier, shader state, snapshot self-check result, read-only compatibility scan |

Unchanged on purpose (evidence-protected or compatibility identifiers): PT DXIL transformation and its pinned output identity `f1ad5c37…` (Orange Hair Fix recognises URT by this identity), CameraCB serializer gate, bounce bridge, PT shadow redirection, bone vtable hook, Distant Trees policy, Physics LOD policy, RTGI/C16/C17 logic, menu bridge and codec, all 41 controls, presets, renderer isolation, compiled WCC script (`precompiled.rsblob`) and UI file, the `RefinedTracingCR01` group and section names, and the `modRefinedTracing` folder and `RefinedTracing.xml` file names. Renaming the folder or XML would leave upgraded manual installs with duplicate script classes and menu groups.

## 3. Shader architecture decision

URT's shader system is already a runtime transformation pipeline: the game's own `dxcompiler.dll` disassembles the native PT library, a structurally checked edit is applied (the bounce selector edit is pattern-based, not tied to SSA numbering), and the result is reassembled and validated. Its output is pinned to one validated identity. V5 keeps it **byte-identical**, for two reasons. First, Orange Hair Fix depends on that identity. Second, any new or changed DXIL needs Windows DXC validation and GPU testing that this environment cannot provide (no Wine; building DXC on 2 cores is impractical).

Analysis capability added: LLVM 18 `llvm-dis` reads the game's DXIL bitcode directly, so IR-level analysis is possible offline. This is how the Black Hair Fix interaction below was established.

V5's update-resilience gain sits at the integration level instead. The shader is now an optional component: it can fail, be skipped, or be yielded without disabling anything else. Adaptive mode keeps CVar features working on future builds and logs exactly what was refused.

## 4. Performance comparison (measured offline; no FPS claims)

| Measurement (host: Xeon 2.1 GHz, real Patch 5.01 image) | V4.3.1 | V5 |
|---|---|---|
| CVar resolution, the 21 names resolved at startup (worker thread) | **4267 ms** | **3.5 ms** (index build + lookups) |
| Executable SHA-256 passes over the 90.7 MB exe, main thread before startup continues | 3 | 1 (cached for the process) |
| Executable SHA-256 passes per session, total | 4 + one per Fast Bone Reads enable | 1 |
| `dx12user.settings` opens/parses per URT menu change | ~45 | 1 (+42 once at startup for the self-check) |

The SHA-256 cost on Windows depends on the legacy CryptoAPI provider URT uses. OpenSSL with SHA-NI takes 70 ms per pass here, and CryptoAPI is typically slower. Rendering FPS is intentionally unchanged: V5 adds no new GPU optimisation that has not been gameplay-tested (section 7). The protected FPS baselines (PT Performance, Kaer Morhen A: about 150–152 / 124–126) should reproduce exactly.

## 5. Compatibility report

| Mod | Evidence (files inspected) | Finding | V5 result |
|---|---|---|---|
| Witcher Native Bridge 2.0 (required by Beardmo's IWO) | Its `dinput8.dll` is Ultimate ASI Loader (`IsUltimateASILoader` export, UAL pdb path) | **Direct file collision with URT V4 MAIN's `dinput8.dll`.** Whichever installs last wins: either WNB and IWO stop loading, or URT depends on WNB's loader | Collision removed. URT MAIN uses `winmm.dll`. UAL source (v9.7.4 `FindFiles`) skips any ASI already loaded, so each ASI initialises once. Each instance restores only the IAT pointer it captured, so the hooks are expected to chain. Source-reviewed; gameplay pending. |
| Beardmo's Immersive World Overhaul 2.2.3 | `.asi` (exports `BFC_*`, MinHook) plus ReShade add-on and its own XML group | No shared CVars, code sites or files with URT found statically. Its only conflict was via WNB's `dinput8.dll` (above). | Coexists (static). Gameplay pending. |
| Beardmo's Black Hair Fix 1.1.1 | Disassembled: per DXIL library in a state object (>30 KB, containing "PTRGS"/"PTHair") it computes FNV-1a-64 over the whole library and swaps in its replacement only on an exact match. Its table key for `PTRGSFinal`+`PTRGSWithSERFinal` is the hash of the **native** 231,144-byte library. | **With URT installed, BHF's main PT hair replacement silently never applies**, because URT replaced that library. Its other two replacements (PTHairPrepassRGS, SharcQueryFinal) still apply. BHF does not override URT. | New option **PT: Shader Compatibility**. Default keeps V4.3.1 behaviour. **Auto** keeps the native PT library when BHF is installed, so BHF applies (URT's Geometry/Hair Bounces, ILC and PT RTAO stay native). **Native** always yields. Log explains the active state. A merged mode (URT edits on top of BHF's library) is technically possible but would modify BHF's asset, which its licence forbids without permission. |
| Orange Hair Fix RT+PT | Contract document and URT identity pin | Recognises URT's PT library by identity `f1ad5c37…` | Identity unchanged in V5. OHF also ships UAL `winmm.dll`; identical role, either copy works. |
| PT Optimization 2.2 (`xinput9_1_0.dll`) | Strings show it patches the same PT shader (same CameraCB edit pattern), RT-shadow branch sites and Physics/LODBudget | Mutually exclusive with URT's code-level features | URT already refuses any non-pristine site. In V5's LATE tier, CVar owners use compare-and-swap and release on foreign writes, so they never fight. Log advises using one mod. |
| LessStutter shader cache (`d3d12.dll` proxy) | `d3d12.dll` plus `d3d12_proxy.ini` | URT does not hook D3D12 or DXGI | No overlap; detected and logged |
| DX12 VRAM Relief | Page lists 5 keys | All five (`TextureUnloadSpeed`, `MeshOffset`, `GenericOffset`, `AutoDiscardCount`, `IdleFrameCount`) are live engine CVars in 5.01, verified by the resolver test. URT never writes them. | Compatible; detected and logged |
| DX12 RT Memory Stabilizer | Page lists no keys; the archive was not supplied | Cannot verify its keys without the file. URT writes none of the categories it describes. | Detected and logged; unverified |

## 6. Settings integrity report

* URT never writes `dx12user.settings`: no INI writer is imported (`verify_release.py` asserts it), and every read is read-only. Game-mediated saves still come only from the unchanged compiled WCC script.
* Private file writes: temp file → `FlushFileBuffers` → `ReplaceFileW` (atomic, with `.bak`). On first migration the move refuses to overwrite a canonical file that appears concurrently. The old V4 files are never modified, so V4.3.1 rollback reads them as before.
* Recovery is covered by tests (`tests/host/test_store_v5.c`, ASan/UBSan) across 10 scenarios: fresh install; V4.3.x migration; early-V4 migration; precedence; `.bak` creation; corrupt→`.bak`; corrupt→defaults; locked file left untouched; sharing-violation retry; migration collision refusal. This found and fixed one V5 defect during development: a missing backup had been treated as a successful load.
* Snapshot parser semantics are covered by tests (16 cases), and each user's machine self-checks it at startup against the Windows API.
* Limits: external software can still damage files. V5 removes URT-origin hazards and adds recovery, and does not claim absolute protection.

## 7. Investigated, deliberately not implemented in this RC

* **New GPU, PT or RT optimisations, DLSS/Frame Generation work, and VRAM CVar controls.** Each would change rendering or memory behaviour that cannot be measured without gameplay. Your stated rule is no speculative runtime patches. Best next candidate: optional live controls for the five VRAM Relief CVars, which are already resolvable through the V5 index. That needs one A/B session to confirm the engine re-reads them at runtime.
* **Hard-coded code-site signature scanning for future patches.** The CODE-IDENTICAL tier covers repackaged builds safely. Applying code patches on genuinely changed code without a new pinned build would break the fail-closed rule. Adaptive mode logs what was refused, so porting to a new patch is quick.
* **Renaming internal `cr01_*` source identifiers.** This is churn with no functional gain. New modules use the `urt_*` prefix, and user-facing text and log prefixes are unified as "Ultimate Refined Tracing".

## 8. Build and reproducibility

`python3 build.py` (clang-cl + lld-link, no SDK or CRT) builds twice and requires byte-identical outputs, then builds the locales and menu and packages both archives twice, requiring identical archive hashes. `tests/run_host_tests.sh <witcher3.exe>` runs 25 tests (16 inherited V4 logic tests compiled against V5 sources with ASan/UBSan, 6 V5 tests, 3 native-evidence verifiers against the real exe, plus the release verifier). See `docs/BUILD.md` and `out/build_manifest.json`.

## 9. Installation and rollback

See `package/IMPORTANT README!.txt`, which ships in both archives. When upgrading, remove V4's `RefinedTracing.asi`, and `dinput8.dll` only if it came from URT. Rollback: remove `UltimateRefinedTracing.asi` (and URT's `winmm.dll` if nothing else needs it), then reinstall the V4.3.1 archive.

## 10. Gameplay acceptance (Windows, about 10 minutes)

1. **Upgrade and core run (required).** Remove V4 `RefinedTracing.asi` and URT's V4 `dinput8.dll`, install V5 MAIN, launch in PT. Open `bin\x64_dx12\UltimateRefinedTracing.log` and expect: *exact Patch 5.01 executable*, *runtime tier FULL*, *PT shader slots installed … f1ad5c37*, *settings snapshot self-check passed*, *migrated RefinedTracing.settings*. In the menu all V4 values must be retained. Toggle Master Off → On, set Hair Bounces 10, change Indirect-Light Compensation. Check FPS at Kaer Morhen A (expect 150–152 Shadows Off, 124–126 Shadows On + billboards).
2. **Isolation and persistence (required).** Switch PT → Normal RT → raster → PT; restart; values persist. `Documents\The Witcher 3\UltimateRefinedTracing.settings` exists and `RefinedTracing.settings` is unchanged.
3. **Only if you use them:** WNB + Beardmo's IWO installed: IWO wind/grass works and the URT log lists both loaders. Black Hair Fix: set PT Shader Compatibility = Auto, restart; Geralt's PT hair should show BHF's look and the log should say "yielding".

## RC2 = RC1 + review fixes (branch `dev/rc1-review-fixes`, merged into `release/v5.0.0-rc2`)

1. Portable source: the `tests/runtime501` symlink is removed; inherited V4 tests now include headers via `-I src`.
2. Evidence preservation: a corrupt private settings file is moved to `.corrupt`, then `.corrupt.1` … `.corrupt.99`, and never overwrites an earlier one. If every name is taken, the file stays in place and saving is blocked for that session, so neither it nor the `.bak` is replaced. Store test scenarios 11–12 cover this.
Shader compatibility default unchanged (URT owns the PT shader). No performance changes.

## RC3 = RC2 + UI localization hotfix
See `docs/RC3_RELEASE_NOTES.md`: added the missing `option_cr01_ptshadercompat` label twin and restored `tooltip_cr01_fallback` to its V4.3.1 (empty) value. Native code is byte-identical to RC2. The per-option hover description awaits a Windows WCC compile (branch `dev/wcc-ptshadercompat-hover`).


## RC5 (cleanup)
RC4's RT/PT: Tree Shadow Distance was withdrawn after gameplay showed it moves the game's native near-tree lighting transition from 20 m to 40 m instead of removing it (docs/RC4_FOLLOWUP_TREE_SHADOWS.md). RC5 = RC3 code (ASI machine code identical; 9 bytes differ: version + build ID) with only the version and README changed. See docs/RC5_RELEASE_NOTES.md.

## RC9 (clean URT)
RC5 shader architecture restored on branch dev/rc9-clean; Black Hair Fix layering (RC6-RC8) and the PT Shader Compatibility selector removed; menu/locales = V4.3.1. See docs/RC9_RELEASE_NOTES.md.
