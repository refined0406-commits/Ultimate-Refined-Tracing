# Selected controls for V5.2 (RC3 rows + RC6 additions; RC7: the six PT caustics rows removed; RC8: Tree and Bush Draw Distance removed). Native texture-quality CVars (TextureMemoryBudget,
# TextureMipBias) stay rejected. RC6: every row whose engine variable carries the game's save flag (0x100) is
# detached from the game's settings save while URT owns it (see src/urt_gfx_core.h), so URT values never reach
# dx12user.settings. NATIVE_MENU_HOLD rows are compiled into the script but stay out of the menu until the
# game's own graphics XML proves they are not part of a native graphics-menu option (tools/v52_selected/native_menu_evidence.py).
STORE = 'URTExtended'
CORE_GROUP = 'RefinedTracingCR01'
ROWS = [{'id': 'URTX_PTAOQuality',
  'group': 'URTX_PTAO',
  'label': 'PT: Ambient Occlusion Quality',
  'cvar': 'PTAOQuality',
  'cat': 'Rendering/RT/PathTracer',
  'flags': 513,
  'dflt': 4294967295,
  'rva': 85246544,
  'kind': 'i',
  'vals': [0, 1, 2, 3],
  'scope': 'PT',
  'when': 'live',
  'dir': 'lo_fps',
  'unit': '',
  'notes': (),
  'vaxis': 125,
  'what': 'PTAOQuality'},
 {'id': 'URTX_RTAOScreenSpace',
  'group': 'URTX_RT',
  'label': 'RT: Ambient Occlusion Screen-Space Sampling',
  'cvar': 'EnableAOSSSampling',
  'cat': 'Rendering/RT',
  'flags': 256,
  'dflt': 1,
  'rva': 67536112,
  'kind': 'b',
  'vals': [1, 0],
  'scope': 'RT',
  'when': 'live',
  'dir': 'mixed',
  'unit': 'onoff',
  'notes': (),
  'vaxis': 39,
  'what': 'RTAOScreenSpace'},
 {'id': 'URTX_RTHBIL',
  'group': 'URTX_RT',
  'label': 'RT: Screen-Space Indirect Lighting (HBIL)',
  'cvar': 'UseHBIL',
  'cat': 'Rendering/RT/HBIL',
  'flags': 256,
  'dflt': 0,
  'rva': 67531688,
  'kind': 'b',
  'vals': [1, 0],
  'scope': 'RT',
  'when': 'live',
  'dir': 'off_fps',
  'unit': 'onoff',
  'notes': (),
  'vaxis': 48,
  'what': 'RTHBIL'},
 {'id': 'URTX_RTAOPretrace',
  'group': 'URTX_RT',
  'label': 'RT: Ambient Occlusion Pre-Trace',
  'cvar': 'EnableAOPretrace',
  'cat': 'Rendering/RayTracing',
  'flags': 256,
  'dflt': 1,
  'rva': 85134296,
  'kind': 'b',
  'vals': [1, 0],
  'scope': 'RT',
  'when': 'live',
  'dir': 'mixed',
  'unit': 'onoff',
  'notes': (),
  'vaxis': 38,
  'what': 'RTAOPretrace'},
 {'id': 'URTX_RTReflScreenSpace',
  'group': 'URTX_RTRefl',
  'label': 'RT: Screen-Space Reflection Assist',
  'cvar': 'RtRadianceScreenSpaceTrace',
  'cat': 'Rendering/RT',
  'flags': 256,
  'dflt': 1,
  'rva': 85259928,
  'kind': 'b',
  'vals': [1, 0],
  'scope': 'RT',
  'when': 'live',
  'dir': 'on_fps',
  'unit': 'onoff',
  'notes': (),
  'vaxis': 40,
  'what': 'RTReflScreenSpace'},
 {'id': 'URTX_TextureStreams',
  'group': 'URTX_Tex',
  'label': 'Textures Streamed at Once',
  'cvar': 'TextureInFlightCountBudget',
  'cat': 'Rendering',
  'flags': 257,
  'dflt': 12,
  'rva': 85248672,
  'kind': 'i',
  'vals': [8, 16, 24, 32, 48, 64],
  'scope': 'ALL',
  'when': 'live',
  'dir': 'stream_q',
  'unit': '',
  'notes': (),
  'vaxis': 26,
  'what': 'TextureStreams'},
 {'id': 'URTX_PTSkinBounceDesat',
  'group': 'URTX_PTLight',
  'label': 'PT: Skin Bounce Light Desaturation',
  'cvar': 'PTSkinIndirectDesaturation',
  'cat': 'Rendering/RT/PathTracer',
  'flags': 514,
  'dflt': 1056964608,
  'rva': 85235984,
  'kind': 'f',
  'vals': [0.0, 0.25, 0.75, 1.0],
  'scope': 'PT',
  'when': 'live',
  'dir': 'look',
  'unit': '',
  'vaxis': 97,
  'what': 'PTSkinBounceDesat',
  'notes': ()},
 {'id': 'URTX_RTReflFog',
  'group': 'URTX_RTRefl',
  'label': 'RT/PT: Fog in Ray-Traced Reflections',
  'cvar': 'RtRadianceShadingFog',
  'cat': 'Rendering/RT',
  'flags': 512,
  'dflt': 1,
  'rva': 85253264,
  'kind': 'b',
  'vals': [1, 0],
  'scope': 'RTPT',
  'when': 'live',
  'dir': 'off_fps',
  'unit': 'onoff',
  'vaxis': 82,
  'what': 'RTReflFog',
  'notes': ()},
 {'id': 'URTX_TerrainPreload',
  'group': 'URTX_Geo',
  'label': 'Terrain Preload Distance',
  'cvar': 'TerrainReadAheadDistance',
  'cat': 'Rendering',
  'flags': 258,
  'dflt': 1101004800,
  'rva': 85240072,
  'kind': 'f',
  'vals': [10.0, 40.0, 100.0, 200.0, 400.0, 600.0],
  'scope': 'ALL',
  'when': 'stream',
  'dir': 'stream_q',
  'unit': ' m',
  'vaxis': 6,
  'what': 'TerrainPreload',
  'notes': ()},
 {'id': 'URTX_FGMenuResources',
  'group': 'URTX_FG',
  'label': 'Frame Generation: Keep Resources in Menus',
  'cvar': '',
  'cat': '',
  'flags': 0,
  'dflt': 0,
  'rva': 0,
  'kind': 'b',
  'vals': [1],
  'scope': 'ALL',
  'when': 'live',
  'dir': 'fg',
  'unit': 'onfg',
  'notes': (),
  'vaxis': 0,
  'what': 'FGMenuResources'}]
# V5.3 private development rows (appended; V5.2 indices unchanged). Every row: option 0 = Native / Auto,
# no Quick Preset assignment, flags/default/RVA read from the exact Patch 5.01 executable (2df76539...).
# None of these variables carries the game's save flag (0x100): the game never writes them to dx12user.settings,
# and no native graphics-menu option can persist them (every native graphics-option variable carries 0x100).
# 'stage': 'live' = applied in the renderer scope on every tick; 'startup' = read by the engine once, when ray
# tracing initialises (render-device start), so URT applies it as early as possible and keeps it while Master is On.
ROWS_V53 = [
 {'id': 'URTX_TexUnloadSpeed', 'label': 'Texture Unloading Speed', 'cvar': 'TextureUnloadSpeed', 'cat': 'Rendering',
  'flags': 0x202, 'dflt': 0x3f800000, 'rva': 0x514c6e8, 'kind': 'f', 'vals': [0.5, 2.0, 4.0], 'scope': 'ALL', 'stage': 'live',
  'labels': ['urt53_x0p5', 'urt53_x2', 'urt53_x4']},
 {'id': 'URTX_MeshStreamLead', 'label': 'Mesh Streaming Distance', 'cvar': 'MeshOffset', 'cat': 'Streaming/Sector',
  'flags': 0x2, 'dflt': 0x41200000, 'rva': 0x515e660, 'kind': 'f', 'vals': [5.0, 20.0, 40.0], 'scope': 'ALL', 'stage': 'live',
  'labels': ['urt53_m5', 'urtx_n_20_m', 'urtx_n_40_m']},
 {'id': 'URTX_GenericStreamLead', 'label': 'General Streaming Distance', 'cvar': 'GenericOffset', 'cat': 'Streaming/Sector',
  'flags': 0x2, 'dflt': 0x42340000, 'rva': 0x515e628, 'kind': 'f', 'vals': [30.0, 60.0, 90.0], 'scope': 'ALL', 'stage': 'live',
  'labels': ['urt53_m30', 'urt53_m60', 'urt53_m90']},
 {'id': 'URTX_GCDiscardThreshold', 'label': 'Memory Cleanup Threshold', 'cvar': 'AutoDiscardCount', 'cat': 'MetadataStore',
  'flags': 0x1, 'dflt': 400, 'rva': 0x5170158, 'kind': 'i', 'vals': [200, 800, 1600], 'scope': 'ALL', 'stage': 'live',
  'labels': ['urt53_n200', 'urt53_n800', 'urt53_n1600']},
 {'id': 'URTX_GCIdleFrames', 'label': 'Memory Cleanup Interval', 'cvar': 'IdleFrameCount', 'cat': 'Memory/GC',
  'flags': 0x1, 'dflt': 30, 'rva': 0x516f550, 'kind': 'i', 'vals': [600, 1200, 2400, 3600], 'scope': 'ALL', 'stage': 'live',
  'labels': ['urt53_n600', 'urt53_n1200', 'urt53_n2400', 'urt53_n3600']},
 {'id': 'URTX_RTASDefrag', 'label': 'RT Acceleration Structure Defragmentation', 'cvar': 'RTConfigDefragThresholdPerCent',
  'cat': 'Rendering/RT/AS/Config', 'flags': 0x201, 'dflt': 30, 'rva': 0x5130388, 'kind': 'i', 'vals': [10, 20, 50],
  'scope': 'ALL', 'stage': 'startup', 'labels': ['urt53_p10', 'urt53_p20', 'urt53_p50']},
 {'id': 'URTX_RTASBatch', 'label': 'RT Acceleration Structure Build Batch Size', 'cvar': 'RTConfigMaxBlasesInBatch',
  'cat': 'Rendering/RayTracing', 'flags': 0x201, 'dflt': 256, 'rva': 0x5130468, 'kind': 'i', 'vals': [64, 128, 192],
  'scope': 'ALL', 'stage': 'startup', 'labels': ['urtx_n_64', 'urt53_n128', 'urt53_n192']},
 {'id': 'URTX_RTGIAlphaTest', 'label': 'RT: GI Alpha-Test Texture Detail', 'cvar': 'GIAlphaTestMipLevel', 'cat': 'Rendering/RT',
  'flags': 0x202, 'dflt': 0x40a00000, 'rva': 0x514dec0, 'kind': 'f', 'vals': [4.0, 6.0, 7.0, 8.0], 'scope': 'RT', 'stage': 'live',
  'labels': ['urtx_n_4', 'urt53_n6', 'urt53_n7', 'urtx_n_8']},
 {'id': 'URTX_PTNoBlueGlow', 'label': 'PT: No Blue Glow on Armour', 'cvar': 'ReflectionsRadianceMipLevel', 'cat': 'Rendering/RT',
  'flags': 0x202, 'dflt': 0x40800000, 'rva': 0x514de18, 'kind': 'f', 'vals': [10.0], 'scope': 'PT', 'stage': 'live',
  'labels': ['urtx_w_on']},
]
for _r in ROWS_V53:
    _r.setdefault('notes', ())
ROWS = ROWS + ROWS_V53
NATIVE_MENU_HOLD = ('URTX_TerrainPreload',)
# Removed after the RC6 compile; the compiled RC6 script still names them (inert hover/greying entries for rows
# that no longer exist), the runtime never reads them, so saved values cannot reactivate them.
# RC7: six PT caustics rows. RC8: Tree and Bush Draw Distance (FoliageDistanceScale belongs to the game's Foliage Visibility Range).
RETIRED_AFTER_COMPILE = ('URTX_PTCaustics', 'URTX_PTCausticsInPT', 'URTX_PTCausticsTypes', 'URTX_PTCausticsRes', 'URTX_PTCausticsBlur', 'URTX_PTCausticsArea', 'URTX_TreeDistance')
RETIRED_HEADINGS = ('URT52SEP_PTCAUS',)

# V5.3 DEV2: option display-name-only corrections (no value, option ID, order or row change). The shipped menu may
# differ from the WCC compile-time menu by exactly these displayName swaps (tools/v52_selected/menu_delta.py reverts
# them before the canonical comparison). Option display names are menu-XML data; the compiled script never reads them.
# Skin bounce desaturation showed "0 / 0.25 / 0.75 / 1" because it shared integer keys with PT AO Quality.
OPTION_LABEL_ONLY = {
    ('URTX_PTSkinBounceDesat', '1'): ('urtx_n_0', 'preset_value_refinedtracing_num_0_00'),
    ('URTX_PTSkinBounceDesat', '2'): ('urtx_n_0p25', 'preset_value_refinedtracing_num_0_25'),
    ('URTX_PTSkinBounceDesat', '3'): ('urtx_n_0p75', 'preset_value_refinedtracing_num_0_75'),
    ('URTX_PTSkinBounceDesat', '4'): ('urtx_n_1', 'preset_value_refinedtracing_num_1_00'),
}

def apply_option_labels(root, reverse=False):
    """Swap option displayNames per OPTION_LABEL_ONLY (reverse=True restores the compile-time names). Returns count."""
    n = 0
    for v in root.iter('Var'):
        for o in v.iter('Option'):
            k = (v.get('id'), o.get('id'))
            if k in OPTION_LABEL_ONLY:
                old, new = OPTION_LABEL_ONLY[k]
                frm, to = (new, old) if reverse else (old, new)
                assert o.get('displayName') == frm, (k, o.get('displayName'))
                o.set('displayName', to); n += 1
    return n

# ---------------------------------------------------------------------------------------------------------------
# V5.3.0 PRIVATE (performance programme): four new controls (+1 hidden follower row), every one Native / Auto by
# default and in no Quick Preset. Evidence: tools/research cvar table + verified read sites (docs/v530/FINDINGS.md).
#  stage 'boot'  : written once, before the renderer exists, and never changed later in that session (the engine
#                  reads these flags while recording each frame; changing them mid-frame could split one frame
#                  between two queue layouts). Menu changes, Native / Auto and Master Off apply at the next start.
#  'follows'     : hidden row with no menu entry; its option index is the leader's (one menu control, two variables).
ROWS_V530 = [
 {'id': 'URTX_PTRadianceCacheDensity', 'label': 'PT: Radiance Cache Density', 'cvar': 'PTSharcDownscaleFactor',
  'cat': 'Rendering/RT/PathTracer', 'flags': 0x201, 'dflt': 5, 'rva': 0x5149be8, 'kind': 'i', 'vals': [6, 7, 8],
  'scope': 'PT', 'stage': 'live', 'labels': ['urt53_n6', 'urt53_n7', 'urtx_n_8']},
 {'id': 'URTX_RTAsyncAO', 'label': 'RT: Async AO', 'cvar': 'UseAsyncRTAO',
  'cat': 'Rendering/Async/RT', 'flags': 0x200, 'dflt': 0, 'rva': 0x40671e8, 'kind': 'b', 'vals': [1],
  'scope': 'RT', 'stage': 'boot', 'labels': ['urtx_w_on']},
 {'id': 'URTX_RTAsyncGIShading', 'label': 'RT: Async GI Shading', 'cvar': 'UseAsyncShading',
  'cat': 'Rendering', 'flags': 0x200, 'dflt': 0, 'rva': 0x4067300, 'kind': 'b', 'vals': [1],
  'scope': 'RT', 'stage': 'boot', 'labels': ['urtx_w_on']},
 {'id': 'URTX_RTTreeWindDistance', 'label': 'RT: Tree Wind Distance', 'cvar': 'RTSTConfigUpdateDistance',
  'cat': 'Rendering/RT/SpeedTree', 'flags': 0x202, 'dflt': 0x41a00000, 'rva': 0x514c838, 'kind': 'f', 'vals': [15.0, 10.0],
  'scope': 'RTPT', 'stage': 'live', 'labels': ['urt530_m15', 'urtx_n_10_m']},
 {'id': 'URTX_RTTreeWindBlend', 'label': '(follows RT: Tree Wind Distance)', 'cvar': 'RTSTConfigUpdateWindSimBlendDistance',
  'cat': 'Rendering/RT', 'flags': 0x202, 'dflt': 0x41700000, 'rva': 0x514c870, 'kind': 'f', 'vals': [11.25, 7.5],
  'scope': 'RTPT', 'stage': 'live', 'labels': [], 'follows': 'URTX_RTTreeWindDistance', 'menu': False},

 # V5.3.0 private rev2: three more engine-provided paths, Native / Auto by default, no Quick Preset.
 # ClassifyProbesAsync: read in the GI shading pass (0x1c21d30, sync classification) and the async-compute
 #   recording (0x1c21f10, async classification); 0x1c60220 keeps a separate resource state for each queue.
 # UseAsyncCompact: selects the queue of the 'CompactBlases' pass (0x1c22250, 0x1c268af) like UseAsyncRTAO does
 #   for RTAO. Both are session-fixed (boot stage), exactly like the two approved async rows.
 # rev4: the build flags are recomputed from that byte on every BLAS build AND refit, and the copy routine has a
 #   second caller (0x1ef1370), so a mid-session change can refit a BLAS with flags different from its build: BOOT stage.
 # RTConfigFastTraceDynamic: copied into the AS manager configuration (0x1ee9897 -> 0x405af23) when ray tracing
 #   initialises (the same point as RT AS Defrag / Batch); BLAS build flags (0x5106f0): dynamic geometry gets
 #   PREFER_FAST_TRACE instead of PREFER_FAST_BUILD. Startup stage, exactly like the two RT memory rows.
 {'id': 'URTX_RTAsyncGIClassify', 'label': 'RT: Async GI Classification', 'cvar': 'ClassifyProbesAsync',
  'cat': 'RTXGI/GlobalGrid', 'flags': 0x200, 'dflt': 0, 'rva': 0x4069358, 'kind': 'b', 'vals': [1],
  'scope': 'RT', 'stage': 'boot', 'labels': ['urtx_w_on']},
 # R5: URTX_RTASAsyncCompact (UseAsyncCompact) REMOVED after the owner's isolated test: On crashed the game after loading
 #   (Windows: witcher3.exe 0xc0000005 at 0x1cfa664, CRenderTaskBatchProcessor, command-context handle 0). Its saved value
 #   is no longer read and the engine variable is never bound or written. Retired after compile (menu row removed).
 {'id': 'URTX_RTASFastTrace', 'label': 'RT Animated Geometry Trace Mode', 'cvar': 'RTConfigFastTraceDynamic',
  'cat': 'Rendering/RayTracing', 'flags': 0x200, 'dflt': 0, 'rva': 0x5130580, 'kind': 'b', 'vals': [1],
  'scope': 'ALL', 'stage': 'boot', 'labels': ['urt531_fasttrace']},   # rev4: session-fixed, applied only before RT initialises
 # PTMoonImpact (owner's request, visual): per-frame PT constants (0x1ef2a0e..0x1ef2a2e) blend the celestial light as
 #   Moon + (Sun - Moon) x day factor (clamped 0..1) into the PT constant at +0x21c; the moon term is the night end.
 #   Independent of PT: Sun Impact (PTSunImpact, the day end). Live, PT only.
 {'id': 'URTX_PTMoonLight', 'label': 'PT: Moon Light', 'cvar': 'PTMoonImpact',
  'cat': 'Rendering/RT/PathTracer', 'flags': 0x202, 'dflt': 0x3f800000, 'rva': 0x5149750, 'kind': 'f',
  'vals': [0.25, 0.5, 0.75, 1.5, 2.0, 3.0], 'scope': 'PT', 'stage': 'live',
  'labels': ['preset_value_refinedtracing_num_0_25', 'preset_value_refinedtracing_num_0_50', 'preset_value_refinedtracing_num_0_75',
             'preset_value_refinedtracing_num_1_50', 'preset_value_refinedtracing_num_2_00', 'preset_value_refinedtracing_num_3_00']},
 # ---- EXPERIMENTAL section (last, beneath Frame Generation; Native / Auto by default; no Quick Preset) ----
 # R11 retired every R6-R10 EXPERIMENTAL row after the owner's tests showed no meaningful gain (history: tags
 #   v5.3.0-private-r7 .. r10): RT/PT: Ray-Traced Grass Density, RT: HBIL Samples, RT: HBIL Directions, PT: Light Limit,
 #   PT: Small Object Culling (urt_ptcode501.c no longer linked), PT: Radiance Cache (SHaRC) (R9/R10; the game's own
 #   SHaRC setting is left alone and the R10 serializer protection stays). R8 and R9 retired five more earlier.
 # R11 PT Bounce Optimisation Layer, module 1 (docs/v530/V5.3.0_R11_BOUNCE_LAYER.md): PT: Second Bounce Coverage.
 #   No engine variable. At startup (value not Native) URT builds a variant of its own PT library in which unselected
 #   pixels stop after the first bounce (rotating 1/2 or 1/4 pattern; hair excluded) and carries Orange Hair Fix's PT
 #   hair patch (its documented Path A). The level travels in bits 16-17 of URT's CameraCB bounce word. Session-fixed,
 #   startup-guarded. Values = level 1 (50 % of pixels per frame) / 2 (25 %).
   # R12 FIX3 (owner): PT: Second Bounce Coverage retired from the active mod (row removed; key in RETIRED_R12F2, never read).
   #   Its shader text (module 1) stays in the variant library as the shared base of modules 2 and 3, at level 0 (native
   #   bounce count): the shader the owner measured is kept byte for byte. History: tags v5.3.0-private-r11 .. r12-fix2.
 # R12 PT Bounce Optimisation Layer, module 2 (docs/v530/V5.3.0_R12_FIRST_BOUNCE.md): PT: First Bounce Diffuse Coverage.
 #   Same variant library (module 1 + this edit + Orange Hair Fix Path A). On the first bounce, a pixel whose sample took
 #   the diffuse lobe and falls outside this frame's pattern traces nothing (the game's own empty-sample exit); the kept
 #   pixels' diffuse result is scaled by 4/3 or 2. Specular unchanged. Level in bits 18-19 of the bounce word.
 #   Values = level 1 (75 % of diffuse samples per frame) / 2 (50 %). Session-fixed, startup-guarded.
 {'id': 'URTX_PTDiffuseCoverage', 'label': 'PT: First Bounce Diffuse Coverage', 'cvar': '', 'cat': '', 'flags': 0, 'dflt': 0, 'rva': 0,
  'kind': 'i', 'vals': [1], 'scope': 'PT', 'stage': 'boot', 'section': 'PT', 'labels': ['urt11_p50']},
  # R12 FIX2 (owner): directly beneath PT: Geometry Bounces; Native / Auto and 50 % only. New key: the game saves an OPTIONS
  # row as its option index, so saved 1 now means 50 % (shader level 2, the R12 FIX1 edit unchanged). The R12 key
  # URTX_ExpPTDiffuseCoverage (where 1 = the retired 75 %) is retired: never read, so no saved value can activate a shader.
 # R12 FIX2 PT Bounce Optimisation Layer, module 3 (docs/v530/V5.3.0_R12_FIX2.md): PT: Bounce Sun Shadow Coverage.
 #   Same variant library. At bounce hits, the three sun / moon shadow rays of each PT export run for half of the pixels per
 #   frame (the others: instance mask 0, nothing traversed, visibility 0); traced visibility x2. Level in bits 20-21.
 #   EXPERIMENTAL (not yet tested in game). Values = level 1 (50 %). Session-fixed, startup-guarded.
 {'id': 'URTX_ExpPTSunShadowCoverage', 'label': 'PT: Bounce Sun Shadow Coverage', 'cvar': '', 'cat': '', 'flags': 0, 'dflt': 0, 'rva': 0,
  'kind': 'i', 'vals': [1], 'scope': 'PT', 'stage': 'boot', 'section': 'PT', 'labels': ['urt11_p50']},   # R12 FIX3: PT section, beneath First Bounce Diffuse Coverage (key kept: saved 1 = 50 % stays valid)
 # R12 FIX3 PT Bounce Optimisation Layer, module 4 (docs/v530/V5.3.0_R12_FIX3.md): PT: Bounce Local Light Shadow Coverage.
 #   Same variant library, module 4 built only when set. At bounce hits the local-light shadow ray of each shading path runs
 #   for half of the pixels per frame (mask 0 = no traversal; at the branch site the light evaluation is skipped too); that
 #   light is weighted x2 for the traced half. Level in bits 22-23. EXPERIMENTAL (not tested in game). Session-fixed, guarded.
 {'id': 'URTX_ExpPTLocalShadowCoverage', 'label': 'PT: Bounce Local Light Shadow Coverage', 'cvar': '', 'cat': '', 'flags': 0, 'dflt': 0, 'rva': 0,
  'kind': 'i', 'vals': [1], 'scope': 'PT', 'stage': 'boot', 'section': 'PT', 'labels': ['urt11_p50']},   # R12 FIX4 (owner): PT section, beneath Bounce Sun Shadow Coverage (key kept)
]
for _r in ROWS_V530:
    _r.setdefault('notes', ())
ROWS = ROWS + ROWS_V530
RETIRED_V530 = ('URTX_RTASAsyncCompact',)
RETIRED_R12F2 = ('URTX_ExpPTDiffuseCoverage', 'URTX_ExpPTSecondBounce')   # R12 FIX2: R12 diffuse key replaced; R12 FIX3: Second Bounce Coverage retired (not in the new script)
RETIRED_R8 = ('URTX_ExpRTGrassWind', 'URTX_ExpRTStaticTrees', 'URTX_ExpRTBillboardUpdates')   # removed before the R8 compile (not in its script)   # R5: compiled into the rev3 script (inert), no longer a defined row
RETIRED_R9 = ('URTX_ExpRTSolidFoliage',)   # R9: removed before the R9 compile (not in its script)
RETIRED_R11 = ('URTX_ExpRTGrassDensity', 'URTX_ExpHBILSamples', 'URTX_ExpHBILDirections', 'URTX_ExpPTLightLimit',
               'URTX_ExpPTObjectCulling', 'URTX_ExpPTRadianceCache')   # R11: removed before the R11 compile (not in its script)
RETIRED_AFTER_COMPILE = RETIRED_AFTER_COMPILE + RETIRED_V530
MENU_ROWS = [r for r in ROWS if r.get('menu', True)]
