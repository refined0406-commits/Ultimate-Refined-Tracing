#!/usr/bin/env python3
"""Research tool: R7 disposition of every RT/PT/RTXGI/NRD/Async/DLSS engine variable of Patch 5.01 (the 513-row scan).
Input: inv2.tsv (category, name, type, default-int, default-float, scan status, real reader functions, value RVA) produced
from the verified xref scan (getter stubs that nothing references are excluded from 'readers').
Output: TSV with one disposition + evidence per variable, and a summary. Dispositions:
  OWNED        URT already controls this variable (V5.2.0 / V5.3.0 rows or modules)
  IMPLEMENTED  new in R6/R7 EXPERIMENTAL
  COVERED      the workload it would change is already reachable through a URT control (same pass, same effect)
  DUPLICATE    a second registration/alias of a variable listed elsewhere, or a per-grid copy of a covered one
  VISUAL       read only into lighting/shading/denoise constants or colour factors: no ray, dispatch or geometry change
  NOT_APPLIC   no effect in the owner's configuration or an inactive feature (NRD under DLSS RR, caustics off,
               debug/diagnostic/timelapse, editor/test switches, statistics the game writes)
  UNSAFE       resource sizes/allocation fixed at creation, AS-build state, PSO/state-object topology, crash history,
               native graphics settings the owner protects, or Orange Hair Fix / Immersive World conflicts
  TESTED_NO    the owner already measured it (earlier URT release) with no material gain, or it costs more work
  CANDIDATE    real workload lever not implemented this round; reason given
  NOREAD       no code in Patch 5.01 reads the variable (registration only)
usage: r7_disposition.py inv2.tsv out.tsv"""
import sys, collections
rows = [l.rstrip('\n').split('\t') for l in open(sys.argv[1]) if l.strip()]
NEW = {'GrassDensityLodScale', 'RTSTGrassUpdates', 'RTSTTreesUpdateOnlyOnce', 'RTSTBillboardsUpdates', 'HBILSamplesCount',
       'HBILDirectionsCount', 'ProximityTLASObjectAngleSize'}
V530 = {'UseAsyncRTAO', 'ClassifyProbesAsync', 'RTConfigFastTraceDynamic', 'PTMoonImpact', 'PTSharcDownscaleFactor',
        'RTSTConfigUpdateDistance', 'RTSTConfigUpdateWindSimBlendDistance'}
NRD_FN = '1c2fcd0'
def D(name, cat, rd):
    r = set(rd.split())
    # ---- explicit decisions (evidence in docs/v530/V5.3.0_R7_INVESTIGATION.md) ----
    X = {
     'UseAsyncCompact': ('UNSAFE', 'R5: crashed the game in the owner test (0xc0000005 at 0x1cfa664); removed, never written'),
     'RTConfigRebuildEveryFrame': ('TESTED_NO', 'debug switch: rebuilds every BLAS every frame (more work, never less)'),
     'RTConfigForbidRefit': ('TESTED_NO', 'forces rebuilds instead of refits (more work)'),
     'RTConfigRegenerateDynamic': ('TESTED_NO', 'regenerates dynamic BLASes (more work)'),
     'RTConfigClearAllAllocations': ('NOT_APPLIC', 'debug: frees AS allocations'),
     'RTConfigCompactResult': ('UNSAFE', 'AS memory compaction state read at RT init (0x1ee97d0); memory only; the async compaction crash path'),
     'RTConfigDefragWithCompact': ('UNSAFE', 'AS-manager configuration at RT init (0x1ee97d0); memory only'),
     'RTConfigEnableDefrag': ('COVERED', 'URT RT memory: AS Defrag threshold (startup row)'),
     'RTConfigKeepDescriptorRange': ('UNSAFE', 'descriptor-heap layout at RT init'),
     'RTConfigMaxShaderScratchSizeMB': ('UNSAFE', 'scratch allocation size at RT init (memory, not ray work)'),
     'RTConfigFastBuildDynamic': ('DUPLICATE', 'pair of RTConfigFastTraceDynamic (URT RT Animated Geometry Trace Mode)'),
     'RightPrimaryHitGroup': ('NOT_APPLIC', 'debug hit-group override (0 = native)'), 'LeftPrimaryHitGroup': ('NOT_APPLIC', 'debug hit-group override'),
     'InlineThreadGroup64': ('UNSAFE', 'inline-RT PSO selection (0x1ef0970): pipeline topology'),
     'InlineRTAO': ('CANDIDATE', 'Normal RT AO via the inline "DXR Inline Occlusion PSO" (0x1ef0adf); needs proof the PSO exists when the game starts with it off, then a restart-guarded test'),
     'EnableSER': ('NOT_APPLIC', 'already on (shader execution reordering, 0x1eec960)'),
     'RtRadianceEnableLowQ': ('NOT_APPLIC', 'already on (low-quality reflection shading path)'),
     'UseShadeCulling': ('NOT_APPLIC', 'already on'), 'RtRadiaceShadingOpts': ('NOT_APPLIC', 'already on'),
     'ProcessProbesGridCheckerboard': ('NOT_APPLIC', 'already on (RTXGI checkerboard probe processing)'),
     'RTAOEnableCheckerboarding': ('NOT_APPLIC', 'already on (RTAO checkerboard)'),
     'HBILInterleaved': ('NOT_APPLIC', 'already on (interleaved HBIL)'),
     'UseAsyncDataUpdates': ('NOT_APPLIC', 'already on'), 'UseAsyncTLASUpdates': ('NOT_APPLIC', 'already on'),
     'EarlyAsyncTLASUpdates': ('NOT_APPLIC', 'already on'), 'EarlyAsyncGIUpdates': ('NOT_APPLIC', 'already on'),
     'EarlyAsyncGIPrepareToShade': ('NOT_APPLIC', 'already on'), 'UseHighPriorityAsyncRT': ('NOT_APPLIC', 'already on'),
     'UseAsyncComputeGlobal': ('NOT_APPLIC', 'already on'),
     'UseUniversalOcclusionAHS': ('CANDIDATE', 'switches to the AnyHitUni* any-hit shaders (state-object topology, 0x1d2552e/0x1d257e0); restart-guarded A/B test needed; direction of the effect unknown'),
     'UseUniversalClassificationAHS': ('CANDIDATE', 'as UseUniversalOcclusionAHS'), 'UseUniversalGBufferAHS': ('CANDIDATE', 'as UseUniversalOcclusionAHS'),
     'UseUniversalBaryModeCHS': ('CANDIDATE', 'ClosestHitUniBary path; as UseUniversalOcclusionAHS'),
     'ShadowsTlas': ('CANDIDATE', 'selects one of two TLAS descriptors for RT shadows (0x1c1d5b2 -> pipeline flag); which TLAS is smaller is not provable statically: needs a GPU capture'),
     'RadianceTlas': ('CANDIDATE', 'as ShadowsTlas, for RT reflections (0x1c20ba5)'),
     'RTAOTlas': ('CANDIDATE', 'as ShadowsTlas, for RTAO (0x1c1e0e3)'), 'DebugTlas': ('NOT_APPLIC', 'debug TLAS view'),
     'RayGenType': ('CANDIDATE', 'ray-generation variant selector read by 7 passes (0x1c1af12..0x1c38900); semantics of the alternatives not established'),
     'RTDataUseDefaultMaterial': ('UNSAFE', 'replaces every RT material at data creation (0x1d0da97): whole-scene visual change, not live'),
     'RTDataUseSharedVertexData': ('UNSAFE', 'RT vertex-data layout at creation'), 'RTDataAlignVerticesBy64': ('UNSAFE', 'RT vertex-data layout'),
     'RTStateMeshMask': ('UNSAFE', 'instance ray masks used by every RT/PT pass; changing excludes geometry classes from all passes (and the hair masks Orange Hair Fix relies on)'),
     'RTStateApexMask': ('UNSAFE', 'as RTStateMeshMask'), 'RTStateTerrainMask': ('UNSAFE', 'as RTStateMeshMask'),
     'RTSTTrees': ('UNSAFE', 'removes trees from the RT scene; URT design rule (cr01_distant_trees501.c): never modify RTSTTrees (owned distant-tree/billboard logic); also removes every PT tree shadow'),
     'RTSTTreesProximity': ('UNSAFE', 'as RTSTTrees (near trees)'), 'RTSTGrassProximity': ('CANDIDATE', 'removes near grass from the RT scene entirely; covered in effect by RT/PT: Ray-Traced Grass Density 0.25 (R6), kept native'),
     'RTSTTreesInstancing': ('UNSAFE', 'tree BLAS instancing (memory/build layout at component creation)'),
     'RTSTTreesInstancingOnlyDistant': ('UNSAFE', 'as RTSTTreesInstancing'), 'RTSTBillboardsInstancing': ('UNSAFE', 'as RTSTTreesInstancing'),
     'RTSTTreesForceNonInstancedShader': ('UNSAFE', 'hit-shader selection for trees (state object)'),
     'RTSTTreesUpdates': ('COVERED', 'RT/PT: Static Ray-Traced Trees (R6) stops the same tree refits'),
     'RTSTTreesUpdateOnlyNear': ('COVERED', 'RT: Tree Wind Distance (V5.3.0) sets the near range'),
     'RTSTGrassUpdateOnlyOnce': ('COVERED', 'RT/PT: Ray-Traced Grass Wind Off (R6) is the stronger form'),
     'RTSTConfigUpdates': ('COVERED', 'master of the vegetation updates; the R6 rows control each class'),
     'RTSTConfigCloneRTMeshComponents': ('UNSAFE', 'component cloning at creation'), 'RTSTConfigInstancingDistance': ('UNSAFE', 'instancing layout'),
     'RTSTConfigSkinnedTimeout': ('VISUAL', 'timeout of skinned tree updates'), 'RTSTTreesBillboardShadowsTowardViewer': ('VISUAL', 'billboard orientation'),
     'FoliageShadowDistanceScale': ('TESTED_NO', 'V5.1 RT foliage-shadow detail row, removed (no material gain)'),
     'ReflectionsAlphaTestMipLevel': ('TESTED_NO', 'any-hit alpha mip for PT and RT reflections; it is the V5.2 RC3 row "PT: Texture Detail in Reflections and Bounces", removed after the owner measured no material FPS gain'),
     'AOAlphaTestMipLevel': ('TESTED_NO', 'RTAO any-hit alpha mip; V5.1 "RTAO: Foliage Detail", removed (no material gain)'),
     'GIRadianceMipLevel': ('TESTED_NO', 'listed with the V5.2 texture-detail rows (no material gain)'),
     'PTHairAlphaTestMipLevel': ('UNSAFE', 'PT hair any-hit alpha mip: Orange Hair Fix territory'),
     'ProximityTLASBigObject': ('IMPLEMENTED', 'Normal RT filter size; R7 PT: Small Object Culling makes PT use it too (kept at the native 15 m)'),
     'ProximityTLASSize': ('OWNED', 'PT: Scene Range'),
     'GrassDistanceScale': ('UNSAFE', 'native graphics setting (owner protection)'), 'MeshRenderingDistanceScale': ('UNSAFE', 'native graphics setting (owner protection)'),
     'SSAOEnabled': ('UNSAFE', 'native graphics setting'), 'SSREnabled': ('UNSAFE', 'native graphics setting'), 'Enable': ('UNSAFE', 'native RT on/off'),
     'Shadows': ('UNSAFE', 'native graphics setting'), 'RTAOEnabled': ('UNSAFE', 'native graphics setting'), 'BalanceModeRT': ('UNSAFE', 'native graphics setting'),
     'XessFrameGeneration': ('UNSAFE', 'native graphics setting'), 'DLSSRRPreset': ('UNSAFE', 'native DLSS setting'), 'UnlockGraphicsSettings': ('NOT_APPLIC', 'menu unlock'),
     'CharacterLodGameplayDownscale': ('UNSAFE', 'native setting'), 'AllowChromaticAberration': ('VISUAL', 'post effect'), 'FSRSharpnessHigh': ('VISUAL', 'upscaler sharpening'),
     'ForcedRendererOverlayResolution': ('NOT_APPLIC', 'overlay resolution'), 'InventoryBgColorScaleR': ('VISUAL', 'UI'),
     'LastGameVersion': ('NOT_APPLIC', 'startup bookkeeping'), 'ForceInstantAdaptation': ('VISUAL', 'exposure'), 'BloomDirtTextureScale': ('VISUAL', 'post effect'),
     'PTEnableGlobalOcean': ('CANDIDATE', 'ocean surface in the PT frame (0x1c2f2a9); helps only at sea; effect not established'),
     'PTUseSHARC': ('NOT_APPLIC', 'radiance cache switch (code default off); R7 logs its effective state once in PT (read-only)'),
     'PTSharcResetCache': ('NOT_APPLIC', 'one-shot reset'), 'PTSharcEntriesNumLog2': ('UNSAFE', 'SHaRC hash-table size, allocated at init'),
     'PTSharcAccumulationFrameNum': ('VISUAL', 'temporal accumulation length'), 'PTSharcStaleFrameNum': ('VISUAL', 'cache eviction age'),
     'PTSharcSceneScale': ('VISUAL', 'cache voxel scale (quality/collisions; density is the URT row)'), 'PTSharcRoughnessMin': ('VISUAL', 'cache use threshold'),
     'PTSharcRadianceScale': ('VISUAL', 'encoding scale'), 'PTSharcDebugMode': ('NOT_APPLIC', 'debug'),
     'PTLocalLightMaskEnable': ('UNSAFE', 'interior/exterior light masking inside the PT light loop; Off makes lights leak through walls'),
     'PTGameLightsIndirect': ('UNSAFE', 'PT indirect from game lights; gates traces but it is the light that Orange Hair Fix / IW tune (off natively)'),
     'PTHairGameLightIndirect': ('UNSAFE', 'hair light path (Orange Hair Fix)'), 'PTHairGameplayCameraFill': ('UNSAFE', 'hair camera fill (Orange Hair Fix)'),
     'LocalShadowsHair': ('UNSAFE', 'hair in local shadows (Orange Hair Fix)'), 'PTHairSpecularTintScale': ('UNSAFE', 'hair shading (Orange Hair Fix)'),
     'HairStochasticGBuffer': ('UNSAFE', 'hair G-buffer (Orange Hair Fix)'), 'GTAOOvHairPower': ('UNSAFE', 'hair AO (Orange Hair Fix)'),
     'PTDimmerEnable': ('VISUAL', 'interior dimmer factor in PT constants'), 'ShadowTMin': ('VISUAL', 'shadow-ray start offset (self-shadow acne vs. leaks)'),
     'ProximityTLASObjectAngleSize': ('IMPLEMENTED', 'R7 PT: Small Object Culling (owned in PT while chosen)'),
     'HBILDrawOnly': ('NOT_APPLIC', 'debug'), 'HBILOnlyReproj': ('VISUAL', 'reprojection-only mode (stale lighting)'),
     'HBILUseReprojection': ('VISUAL', 'temporal reuse'), 'HBILSpatialDenoise': ('VISUAL', 'small denoise pass'),
     'HBILInterleavedGroupDiv': ('UNSAFE', 'must match the interleave pattern of the HBIL shader'),
     'HBILRadius': ('VISUAL', 'search radius (same sample count)'), 'HBILImpact': ('VISUAL', 'strength'), 'HBILDistanceMin': ('VISUAL', 'fade'),
     'HBILDistanceMax': ('VISUAL', 'fade'), 'HBILAlbedoUseMode': ('VISUAL', 'albedo mode'), 'HBILIntencityHack': ('VISUAL', 'intensity'),
     'HBILAfterFog': ('VISUAL', 'composite order'), 'AllowHBIL': ('COVERED', 'URT RT: HBIL row (UseHBIL)'), 'NrdAmbientOcclusionHBILPower': ('VISUAL', 'AO power'),
     'HiResEntityCustomShadowmapSize': ('UNSAFE', 'shadow-map allocation size'),
     'RTShadowDayPoint': ('VISUAL', 'time-of-day point for RT shadow switching (0x1c2277d)'),
     'GridIdForDebugOutput': ('NOT_APPLIC', 'debug'), 'UseTiledDeferred': ('UNSAFE', 'deferred lighting path (pipeline)'),
     'MergedShadowMeshTwoSided': ('VISUAL', 'two-sided particle shadow mesh'),
     'ProbeSamplerMipBias': ('VISUAL', 'sampler LOD bias'), 'GbufferSamplerMipBias': ('VISUAL', 'sampler LOD bias'), 'OcclusionSamplerMipBias': ('VISUAL', 'sampler LOD bias'),
     'GbufferSamplerMinMagLinear': ('VISUAL', 'sampler filter'), 'GbufferSamplerMipLinear': ('VISUAL', 'sampler filter'),
     'ForbidSSSR': ('UNSAFE', 'native screen-space reflection path'), 'EnableLightingOnlyMode': ('NOT_APPLIC', 'debug view'),
     'AmbientLightingOnlyMode': ('NOT_APPLIC', 'debug view'), 'DynamicResolutionDance': ('NOT_APPLIC', 'debug'),
     'RtScreenTraceDebug': ('NOT_APPLIC', 'debug'), 'UseMultiThreadRenderDebug': ('NOT_APPLIC', 'debug'),
     'ForcedDebugPreviewIndex': ('NOT_APPLIC', 'debug'), 'RemapDrawPoints': ('NOT_APPLIC', 'debug'),
     'TerrainNormalSheenGrassSlider': ('VISUAL', 'terrain sheen'), 'SpeedTreeAlwaysUseLowQualityPCF': ('VISUAL', 'raster shadow filter (already low)'),
     'EnableDeferredLightsStencil': ('NOT_APPLIC', 'already on'), 'EnableVarianceTracking': ('VISUAL', 'probe variance'),
     'CausticsEnable': ('NOT_APPLIC', 'caustics off natively (V5.2 caustics rows retired)'),
     'CausticsOutputToPT': ('NOT_APPLIC', 'caustics off natively'),
     'RtRadianceShadingEnvGrid': ('VISUAL', 'reflection shading term'), 'RtRadianceShadingGlobalLight': ('VISUAL', 'reflection shading term'),
     'EnableRtRadianceShading': ('UNSAFE', 'switches reflection shading off entirely (broken reflections)'),
     'ProbeRaysBaseAngleForGradient': ('VISUAL', 'ray-cone texture LOD for probes'), 'RadianceRaysBaseAngleForGradient': ('VISUAL', 'ray-cone texture LOD'),
     'OcclusionRaysBaseAngleForGradient': ('VISUAL', 'ray-cone texture LOD'),
     'PTBaseAngleForGradient': ('CANDIDATE', 'PT ray-cone texture LOD (0x1c1e328 into the PT frame); a bandwidth lever like the radiance mip that PT: No Blue Glow already raises to 10; not separately measurable'),
     'ForceNotScreenBasedGradCalc': ('VISUAL', 'gradient method'), 'RayTracingGradCalcTanFactor': ('VISUAL', 'gradient'),
     'TurnOffInvalidation': ('UNSAFE', 'RTXGI block invalidation (stale GI)'), 'ClearBlockList': ('NOT_APPLIC', 'maintenance'),
     'FramesPerRegularClearBlockListCycle': ('VISUAL', 'maintenance period'), 'ForceBlockReclassification': ('NOT_APPLIC', 'debug'),
     'VisualizeImpactZone': ('NOT_APPLIC', 'debug'), 'ActiveSubBlocksRatio': ('NOT_APPLIC', 'statistic the game writes'),
     'FreeProbesSpaceLeftPercent': ('NOT_APPLIC', 'statistic'), 'MinFreeProbesSpaceLeft': ('NOT_APPLIC', 'statistic'),
     'ActiveProbesNum': ('NOT_APPLIC', 'statistic'), 'UpperBoundOfActiveProbesNum': ('UNSAFE', 'probe pool size (allocation)'),
     'RelocationCoef': ('VISUAL', 'probe relocation'), 'UndergroundProbeDotProd': ('VISUAL', 'probe test'), 'UndergroundProbeRayDist': ('VISUAL', 'probe test'),
     'CalculateCastadeShadowFactor': ('VISUAL', 'probe shading term'), 'CalculateProbeSkyShadowFactor': ('VISUAL', 'probe shading term'),
     'ShadeVisibleBlocksRatio': ('COVERED', 'RTXGI shading budget (URT RTGI presets: probes shaded per frame)'),
     'MaxProbesClassificatedPerFrameAfterFirstPass': ('COVERED', 'URT RTGI presets (probes classified per frame)'),
     'ClassificationFrameInd': ('NOT_APPLIC', 'frame index the game writes'), 'ShadingFrameInd': ('NOT_APPLIC', 'frame index'), 'StagingFrameInd': ('NOT_APPLIC', 'frame index'),
     'RTGIPreset': ('OWNED', 'URT RTGI presets'), 'SkyImpact': ('OWNED', 'URT'),
     'DLSSRRPreDenoiseShadows': ('VISUAL', 'RR shadow pre-denoise'), 'NrdEnableDiffuse': ('UNSAFE', 'native denoiser path switch'), 'NrdEnableAo': ('UNSAFE', 'native denoiser path switch'),
     'UseCharactersLightingBoost': ('VISUAL', 'character lighting'), 'RtRadianceScreenTraceZThreshold': ('VISUAL', 'screen-trace threshold'),
     'HBILUseAONRD': ('VISUAL', 'HBIL AO denoise'),
     'PTAODenoisePasses': ('CANDIDATE', 'PT AO denoise pass count (0x1d19e15); compute-only, small; -1 = engine choice; kept native this round'),
     'PTAOQuality': ('OWNED', 'URT PT AO Quality'),
     'RT_Cull_More_Than_DiagSize': ('VISUAL', 'size filter of one RT mesh class (0x1d2a3bf): culls only objects LARGER than 4000 m'),
     'RT_Cull_Less_Than_DiagSize': ('CANDIDATE', 'small-size filter of the same class (0 = off); class identity not established'),
     'RT_Tapu16_DiagSize': ('UNSAFE', 'instance mask bits 0x40/0xc0 by size (0x1d2a3e5): pass visibility masks'), 'RT_Tapu4_DiagSize': ('UNSAFE', 'as RT_Tapu16_DiagSize'),
     'HBILSamplesCount': ('IMPLEMENTED', 'R6 RT: HBIL Samples'), 'HBILDirectionsCount': ('IMPLEMENTED', 'R7 RT: HBIL Directions'),
     'RCASAmount': ('VISUAL', 'RCAS sharpening amount (post-process)'), 'GrassNightScale': ('VISUAL', 'grass night brightness'),
     'PTFinalShaders': ('NOREAD', 'registration only'), 'PTNumBounces': ('NOREAD', 'not read; bounces come from URT Geometry/Hair Bounces'),
     'PTSamplesPerPixel': ('NOREAD', 'not read by any code: the PT always traces one path per pixel; no sample-count lever exists'),
     'PTHairNumBounces': ('NOREAD', 'not read'),
    }
    if name in X: return X[name]
    if name in NEW: return ('IMPLEMENTED', 'R6 EXPERIMENTAL')
    if name in V530: return ('OWNED', 'V5.3.0 row')
    if not r: return ('NOREAD', 'no code reads it (registration only)')
    if cat.startswith('Rendering/NRD') or name.startswith('Nrd') or r <= {NRD_FN, '1c32154', '1c31550', '1c32d80'}:
        return ('NOT_APPLIC', 'NRD denoiser constants (0x1c2fcd0): replaced by DLSS Ray Reconstruction in the owner configuration; in PT the NRD dispatch is skipped while RR is active (0x1c1eac9); denoise quality, not ray work')
    if name.startswith('PTHair') or name.startswith('PTAOHair'):
        return ('UNSAFE', 'PT hair shading/shadow constant (0x1ef2340): Orange Hair Fix territory')
    if name.startswith('Caustics'): return ('NOT_APPLIC', 'caustics off natively; V5.2 caustics rows retired')
    if name.startswith('Remap') or cat.endswith('RoughnessRemap'): return ('VISUAL', 'roughness remap curve (0x1c34a10)')
    if r <= {'1d1997a'} or name.startswith('PTAO'): return ('VISUAL', 'PT AO radius/power constant (0x1d1997a); radius/strength/power are URT rows')
    if r <= {'1be4c00'}: return ('VISUAL', 'AO/GI application factor (0x1be4c00)')
    if r <= {'1c61f50', '1c2be90', '1c6ff10', '1c60f20'}: return ('VISUAL', 'RTXGI lighting multiplier (0x1c61f50)')
    if cat.startswith('RTXGI'):
        if name.startswith(('ProbeGridCounts', 'ProbeGridSpacing', 'GridCoverage', 'ProbeNum')):
            return ('UNSAFE', 'RTXGI grid dimensions / probe texel counts: texture and buffer sizes fixed when the grid is created')
        if name.startswith(('FramesNumPer', 'NumShadingDirection')): return ('COVERED', 'RTXGI cycle budget; URT RTGI presets set rays/probes per frame')
        return ('VISUAL', 'RTXGI probe update constant (hysteresis/bias/thresholds): GI quality, same rays')
    if r & {'1ef2340'} and (name.startswith('RayTracing') or name.startswith('PT')):
        return ('VISUAL', 'PT/RT frame constant (0x1ef2340): bias, sequence, impact or material factor; no TraceRay is gated by it (PTRGS gate analysis)')
    if name.startswith(('RTAOOv', 'RayTracingAmbientOcclusion')): return ('VISUAL', 'RTAO radius/power override')
    if name.startswith(('LocalShadow', 'CameraLight', 'ShadowsClamp', 'SunAngular')): return ('VISUAL', 'RT shadow softness/range constant')
    if r <= {'22bd820', '22bd990', '22bcb00', '22bd8f0', '1c7182f', '1c1e3f0'}: return ('VISUAL', 'light intensity scale')
    if name in ('PTAnalyticSkyTurbidity', 'PTAnalyticSkyExposure', 'PTDirectLightImpact', 'PTSkinIndirectDesaturation'): return ('VISUAL', 'PT sky/light factor')
    return ('VISUAL', 'shading constant')
out = []
for c, n, t, di, df, st, rd, rva in [r + [''] * (8 - len(r)) for r in rows]:
    disp, why = ('OWNED', 'URT control (V5.2.0 / V5.3.0 module)') if st == 'URT' else D(n, c, rd)
    if st == 'URT' and n in ('SkyImpact', 'RTGIPreset', 'PTAOQuality'): disp, why = 'OWNED', 'URT control'
    out.append((c, n, t, df, rva, rd or '-', disp, why))
with open(sys.argv[2], 'w') as f:
    f.write('category\tvariable\ttype\tdefault\tvalue_rva\treaders (real code)\tdisposition\tevidence / reason\n')
    for o in out: f.write('\t'.join(o) + '\n')
cnt = collections.Counter(o[6] for o in out)
print(len(out), dict(sorted(cnt.items(), key=lambda x: -x[1])))
