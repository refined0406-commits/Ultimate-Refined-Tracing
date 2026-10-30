# V5.0.0-rc4: Tree Shadow Distance (WITHDRAWN in RC5; historical record)

**Status:** offline-verified only. The visual improvement is not yet proven in game; the A/B test below decides it. The default is OFF, which is identical to RC3.

## 1. What the video shows (and what it cannot show)
The 10.6 s clip (2560×1440, 60 fps, AV1) was analysed with three methods:
* frame-to-frame block steps;
* optical-flow motion compensation with forward-backward occlusion masking;
* KLT surface tracks checked for sustained brightness steps.

Most detections were Geralt's own animation, parallax disocclusion at bush silhouettes, or wind-animated foliage. Average luminance of the tree band changes smoothly, so there is no global lighting pop. **A discrete shadow pop could not be isolated objectively from this clip.** Wind animation and compression mask small local changes. This analysis therefore neither confirms nor rules out a specific event. Timestamps of the pops you see would let me target them.

## 2. What the 5.01 executable establishes (tests/host/evidence_rc4_tree_shadow.py)
* `RTSTConfigInstancingDistance` = 20.0 and `RTSTConfigUpdateDistance` = 20.0 are float CVars. `RTSTConfigUpdateWindSimBlendDistance` = 15.0 is a float CVar under `Rendering`. All three are live in 5.01.
* The RT SpeedTree per-object config builder (RVA `0x1CEA6AD`) stores the **squares** of these distances at config `+0x24/+0x28/+0x2C`. Next to them it stores the flags `RTSTTreesUpdateOnlyNear` (1) and `RTSTTreesInstancingOnlyDistant` (1).
* The PT CameraCB writer (RVA `0x1EF268B`) publishes UpdateDistance² and WindSimBlend² at CameraCB `+0x144/+0x148` (row 20 `.y/.z`). The URT PT Final library does not read row 20. The GPU consumer was not identified among the 163 DXIL modules that could be disassembled.
* Also present but **not used** in RC4:
  * `ProximityTLASObjectAngleSize` (0.1) and `ProximityExtendedTLASSize` (800): small-object TLAS inclusion, RT-only per the referenced mod's author, with a sharp object-count cost;
  * `DebugTlas`: on your rejected list;
  * `RTSTConfigSkinnedTimeout`: purpose unverified.

## 3. Suspected root cause (native game, not URT)
Trees within ~20 m of the camera get their own wind-updated RT geometry. Farther trees use shared, instanced, non-updated geometry. When a tree crosses the 20 m line its RT representation switches, and its shadow can jump or appear to darken. That is exactly "close to the camera". This matches the binary evidence and the explanation documented by *Improved Ray Traced Shadows* (Nexus 7685: "trees sharing a model switch to their own, causing jumps"; "distant tree shadows stay still").

URT does not touch these CVars in RC3. CPU Distant Trees owns only `RTSTBillboardsProximity`, which is billboard proximity, a different mechanism, and its PT Billboards priority is unchanged.

## 4. The RC4 change
* **Setting:** one optional menu row, **RT/PT: Tree Shadow Distance**: Native / Auto (default), 30 m, 40 m.
  * 30 m sets 30/30/25 and 40 m sets 40/40/35 (instancing / update / wind blend).
  * The 40 m values equal the referenced mod's tested values. That author observed stutter above about 53 m, and RC4 offers nothing higher.
* **Ownership:** values are owned only in Normal RT and PT with Master On. Raster, Master Off and Off restore the exact native values.
  * Compare-and-swap ownership, the same model as the existing CVar features.
  * Values already changed by another mod or ini (e.g. Improved RT Shadows) are left untouched and logged.
  * A foreign write while URT owns them releases ownership permanently for the session.
* **What it never touches:** shaders, code, presets, `dx12user.settings` and the TLAS settings.
* **When it takes effect:** values are captured when each tree's RT proxy is created, so an area reload (load a save or fast travel) is the clean comparison boundary.

## 5. Expected effect and cost
* **Expected:** the instancing/update switch moves from about 20 m to 30–40 m, so shadow changes happen farther away and are less noticeable. They are still present at the new boundary.
* **Cost:**
  * More trees get their own, per-frame wind-updated RT geometry. CPU and GPU BLAS update cost grows roughly with the square of the distance.
  * The referenced author measured about 1–2 FPS at 40 m on the Remaster. Dense forests cost more.
  * Possible extra memory for non-instanced trees.
  * Not measured on your RTX 4080.

## 6. Focused A/B test (Windows, about 10 minutes, no settings resets)
Keep NVIDIA Frame Generation on Recommended. Use the same save and the same route.
1. **A (native):** set PT, Tree Shadow Distance = Native / Auto, then load the save. Walk the same 20–30 s forest route as in your video, watching tree shadows on the ground 15–30 m ahead. Note the FPS standing still at a fixed spot facing the forest.
2. **B (40 m):** set 40 m, then **load the same save again**. Repeat the route and the FPS spot. The log should say `Tree Shadow Distance 40 m active`.
3. **If B looks better but costs too much:** repeat with 30 m.
4. **Optional:** Normal RT, one pass each for A and B. Raster: the log must say `native`.
5. **Report:** whether pops are reduced, moved farther or unchanged; the FPS for A and B; and any stutter while walking.

## 7. What remains unknown
* Whether the pops in your clip are this transition. Cascade-shadow or self-shadow changes are not excluded.
* The GPU-side use of CameraCB row 20.
* Whether a live change affects trees that already exist. RC4 assumes it does not and recommends a reload.
* The actual cost on your system.
