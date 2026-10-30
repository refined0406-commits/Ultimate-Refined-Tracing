# RC4 follow-up: tree-shadow popping and the 40 m transition

**Decision: no RC5.** The evidence explains the 40 m artefact but does not yet identify why the popping is URT-specific. RC4 with Tree Shadow Distance = Native / Auto is byte-for-byte RC3 behaviour for these CVars. RC4 and every checkpoint are untouched.

## 1. What the footage demonstrates

### Video 3 (live settings changes, 1036 frames)
* The game is paused, so the background is static. The frame-to-frame difference outside the menu equals encoder noise; GOP refreshes occur every 120 frames.
* The only setting changed is **RT/PT: Tree Shadow Distance**, Native / Auto ↔ 40 m, three round trips. The state was read from the value text per frame.
* Every switch to 40 m brightens specific trees in the upper half of the frame, and every switch back restores them:
  * the region difference moves 1.3 → 2.8 and back on each toggle;
  * the change starts within about 10 frames and settles over about 0.5–1 s, consistent with PT temporal accumulation;
  * independent toggle pairs correlate at r = 0.94 against a noise floor of 0.77.
* **Applies immediately, to existing trees, with no reload and no movement.** This corrects RC4_TREE_SHADOWS.md §7, which assumed a reload was needed.
* **Affected trees:** whole mid-distance spruces (about 20–40 m) plus a dead trunk. Foliage and trunks become lit on the sun side. Nearby and far trees do not change.
* **Branch tips show thin dark/bright fringes**, so the tree's geometry/pose also changes, not only its shading.
* Evidence: `evidence_rc4_followup/live_toggle_diff.png` (40 m minus Native, ×6; white = brighter) and `live_toggle_tree_zoom.jpg` (Native | 40 m | difference).

### Videos 1 and 2 (40 m and Native, moving)
* Same route and soft light.
* Occlusion-masked motion-compensated residuals are dominated by the menu fade, Geralt and ground parallax. No single event is objectively separable, the same limitation found for Timeline_1.
* The live video is therefore the decisive evidence.

## 2. What this establishes
1. The three SpeedTree RT CVars switch a tree between two representations, and the two are **lit visibly differently**: the shared/static copy is darker and more shadowed, the tree's own updated copy is lighter. Within this footage that is a fact, not a hypothesis.
2. So 40 m does not remove a transition. It moves it from 20 m to 40 m, where whole trees are in view: exactly the dark→light change you reported. **The 40 m (and 30 m) option is a regression and should not be used.** It will be withdrawn in the next candidate unless step 3 below finds a use for it.
3. The same switch happens at 20 m under Native. That is the most likely source of the remaining "original" popping.
4. **Not separable from footage:** which of Instancing, Update or WindSimBlend causes it. All three were moved together. The geometry fringes favour the update/wind pose over pure instancing, but this is unproven.

## 3. Why only with URT (open)
URT does not change these CVars at Native / Auto, so URT must be changing **how much the two representations differ**, not where the switch happens. Ranked candidates:
* **H1. PT: Ray-Traced Shadows.**
  * *For:* the change includes direct sun on trunks, which is a sun-shadow component.
  * *Mechanism:* URT On forces mirror value 2 regardless of the game's own value; Off forces the branches off. Either can differ from your no-URT baseline.
  * *Possible effect:* a static shared copy whose pose does not match the visible tree would self-shadow under RT sun shadows.
* **H2. PT: Billboards Off.** The Performance and Balanced Quick Presets set this (RTSTBillboards 0, native 1). That changes which tree representation exists in the RT scene at distance.
* **H3. URT PT shader** (bounce selector, Indirect-Light Compensation, PT RTAO trace). This affects indirect light and contact occlusion against whichever copy is in the TLAS. It is less likely to explain the sunlit-trunk change, and it can't be toggled live (Shader Compatibility needs a restart).

Excluded by your test: CPU: Distant Trees.

## 4. Smallest controlled test (paused, no reload, about 5 minutes)
Use the same spot as video 3: pause menu, transparency on, facing trees 20–40 m away.
1. Note your current PT: Ray-Traced Shadows, PT: Billboards, Geometry Bounces and PT RTAO values, or attach `Documents\The Witcher 3\UltimateRefinedTracing.settings` (read-only; it is not dx12user.settings).
2. **Baseline:** flip Tree Shadow Distance Native ↔ 40 m once. The trees brighten, as in video 3.
3. Set **PT: Ray-Traced Shadows = Native / Auto** (only this change). Flip Tree Shadow again.
4. Restore PT Shadows. Set **PT: Billboards = Native / Auto**. Flip again.
5. Restore everything and set Tree Shadow back to Native / Auto.

Record it like video 3. In whichever step the brightening on the same trees **disappears or clearly shrinks**, that control is the URT amplifier, and the fix targets that interaction. If it survives both steps, the next single test is Shader Compatibility = Native PT shader (restart) with the same flip.

## 5. Update: Native PT shader test (log 2026-10-21 12:15 UTC)
**Log check.**
* The URT PT shader was not active: line 4 reads "Always yield; native PT shader library kept", and line 17 says the shader was yielded.
* Bounce, Indirect and RTAO controls were native that launch.
* Other state: renderer PT, Master On, CPU Distant Trees Off, Tree Shadow Distance native at start.
* Five Native↔40 m round trips are logged between 12:15:54 and 12:16:22.
* No new video was attached; the result is taken from your description plus the log.

**Settings file:** PT Shadows = On (F14=1, forces native shadow mode 2), PT Billboards = Native, Spec screen-space trace On, Spec checkerboard Off.

### Issue A: the 40 m lighting artefact
The artefact survives:
* the native PT shader;
* PT Shadows changes;
* PT Billboards changes;
* CPU Distant Trees Off.

**Conclusion:** the light/dark difference between a tree's shared static copy and its own updated copy is native renderer behaviour. URT's only role is moving the boundary to 40 m, so the 30/40 m option has no remaining justification and is to be withdrawn. Which single CVar produces the difference was not isolated, and it is no longer needed for that decision.

### Issue B: the original popping (Native / Auto, moving)
Every URT shader-driven factor is excluded by the native-shader launch. That leaves:
* **Live controls:**
  * PT Shadows On (forces mode 2);
  * CPU Physics LOD (Physics/LODBudget 25→5; consumers are in the APEX/PhysX code near RVA 0x1E8F8B4, not SpeedTree);
  * Spec SS/checkerboard;
  * CPU Fast Bone Reads (CRenderSkinningData bone copy, byte-identical semantics).
* **Native behaviour:** the same 20 m representation switch as Issue A. This is now the strongest hypothesis, since Issue A shows the native switch alters lighting.

Under Master Off:
* every live control is released to its native value;
* the bone thunk passes straight through to native;
* the shadow mirror carries the native value with branches restored;
* with Shader Compatibility = Native PT Shader, no URT shader is present.

So **Native PT Shader + Master Off is functionally vanilla**, and Master Off vs On isolates the live controls.

### Smallest test
Keep Shader Compatibility = Native PT Shader and Tree Shadow = Native / Auto. Use the same save and route.
* **Run 1:** Master On, then load the save and walk the route.
* **Run 2:** Master Off, then load the save and walk the same route.

| Run 1 | Run 2 | Meaning | Next step |
|---|---|---|---|
| Pops | No pops | A live control causes it | Bisect, PT Shadows first |
| Pops | Pops | Not caused by URT controls or shader | Optional Run 3 with the ASI disabled confirms native |
| No pops | – | The earlier pops need URT's shader | Repeat Run 1 with the URT shader |

## 6. Closure (2026-10-21)
* **Nearby trees:** confirmed in vanilla (URT removed). The near-tree light change is native.
* **Distant trees:** your later observation was that the transitions fade as in-game time and sun angle change. No URT regression was demonstrated, so the investigation is **closed** and no fix was made.
* **Recorded for reference only** (tests/host/evidence_distant_trees.py; not acted on):
  * The game builds tree instances in two passes, mode 0 and mode 1. The mode-1 pass is gated by `UpdateProximityTLAS`; the proximity box is camera ±400/800 m.
  * Billboards are included only if `RTSTBillboards && (RTSTBillboardsProximity || mode == 0)`. CPU: Distant Trees sets `RTSTBillboardsProximity = 0`; PT Billboards Off sets `RTSTBillboards = 0`.
  * Both are existing, documented FPS trade-offs that existed before this investigation. Whether the mode-1 pass feeds the TLAS selected by `ShadowsTlas` was not traced to the GPU.
  * If distant shadows ever need revisiting, the single A/B is CPU: Distant Trees On vs Off with a reload, watching the far forest.
* **Tree Shadow Distance:** removed in V5.0.0-rc5 (see RC5_RELEASE_NOTES.md).
