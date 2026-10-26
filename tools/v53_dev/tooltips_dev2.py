# -*- coding: utf-8 -*-
"""V5.3 DEV2: assembled tooltips for every adjustable control + option-label fixes (18 languages).
Native values are evidence-backed:
 - registered engine defaults read from the Patch 5.01 executable (tools/research cvar table);
 - RTGI tier values from docs/V5.1_RT_RESEARCH.md (RTXGI init 0x1c617e0 tiers);
 - Memory Cleanup Interval 1800 from the game's bin/config/base/gc.ini (owner's install, kit v53dev-rev1 evidence);
 - 'game'/'auto'/'shadowsopt' where the value is chosen or overridden by the game (registered -1, graphics option,
   or an INI whose effective value is not proven)."""
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
import tip_phrases as TP
from tip_core_a import CORE as A
from tip_core_b import CORE as B
from tip_core_c import CORE as C
CORE = {**A, **B, **C}
LANGS = TP.LANGS
# control id -> (tooltip key, core id, native, performance, scope, timing, extras)
#   native: None | ('lit', text) | ('key', loc key) | ('ph', phrase)
S_ = {'PT': 'S_PT', 'RT': 'S_RT', 'RTPT': 'S_RTPT', 'ALL': 'S_ALL', None: None}
ON, OFF = ('key', 'urtx_w_on'), ('key', 'urtx_w_off')
SPEC = {
 'MasterEnabled': ('tooltip_cr01_masterenabled', 'MasterEnabled', None, None, None, None, ()),
 'CPUDistantTrees': ('tooltip_cr01_cpu_distant_trees', 'CPUDistantTrees', ('ph', 'offgame'), 'E_CPU', 'RTPT', 'T_AREA', ()),
 'CPUFastBoneReads': ('tooltip_cr01_cpu_fast_bone_reads', 'CPUFastBoneReads', ('ph', 'offgame'), 'E_CPU', 'ALL', 'T_NOW', ()),
 'CPUPhysicsLOD': ('tooltip_cr01_cpu_physics_lod', 'CPUPhysicsLOD', ('ph', 'offgame'), 'E_CPU', 'ALL', 'T_NOW', ()),
 'GeometryBounces': ('tooltip_cr01_geometrybounces', 'GeometryBounces', ('key', 'preset_value_refinedtracing_bounce_2'), 'E_FPS', 'PT', 'T_NOW', ()),
 'HairBounces': ('tooltip_cr01_hairbounces', 'HairBounces', ('key', 'preset_value_refinedtracing_bounce_4'), 'E_FPS', 'PT', 'T_NOW', ()),
 'IndirectLight': ('tooltip_cr01_indirectlight', 'IndirectLight', ('lit', '1.00'), 'E_VIS', 'PT', 'T_NOW', ()),
 'PTBillboards': ('tooltip_cr01_ptbillboards', 'PTBillboards', ON, 'E_FPS', 'PT', 'T_AREA', ()),
 'PTShadows': ('tooltip_cr01_ptshadows', 'PTShadows', ('ph', 'shadowsopt'), 'E_FPS', 'PT', 'T_NOW', ()),
 'PTSceneRange': ('tooltip_cr01_pt_scene_range', 'PTSceneRange', ('lit', '400 m'), 'E_FPS', 'PT', 'T_NOW', ()),
 'AmbientOcclusion': ('tooltip_cr01_ambientocclusion', 'AmbientOcclusion', ON, 'E_FPS', 'PT', 'T_NOW', ()),
 'URTX_PTAOQuality': ('tooltip_urtx_ptaoquality', 'URTX_PTAOQuality', ('ph', 'auto'), 'E_FPS', 'PT', 'T_NOW', ()),
 'AOStrength': ('tooltip_cr01_aostrength', 'AOStrength', ('ph', 'auto'), 'E_VIS', 'PT', 'T_NOW', ()),
 'AORadius': ('tooltip_cr01_aoradius', 'AORadius', ('ph', 'auto'), 'E_VIS', 'PT', 'T_NOW', ()),
 'AOContrast': ('tooltip_cr01_aocontrast', 'AOContrast', ('ph', 'auto'), 'E_VIS', 'PT', 'T_NOW', ()),
 'SunImpact': ('tooltip_cr01_sunimpact', 'SunImpact', ('lit', '1.00'), 'E_VIS', 'PT', 'T_NOW', ()),
 'SkyImpact': ('tooltip_cr01_skyimpact', 'SkyImpact', ('lit', '1.00'), 'E_VIS', 'PT', 'T_NOW', ()),
 'URTX_PTSkinBounceDesat': ('tooltip_urtx_ptskinbouncedesat', 'URTX_PTSkinBounceDesat', ('lit', '0.50'), 'E_VIS', 'PT', 'T_NOW', ()),
 'GrassDay': ('tooltip_cr01_grassday', 'GrassDay', ('ph', 'auto'), 'E_VIS', 'PT', 'T_NOW', ()),
 'GrassNight': ('tooltip_cr01_grassnight', 'GrassNight', ('ph', 'auto'), 'E_VIS', 'PT', 'T_NOW', ()),
 'SpecularScreenSpace': ('tooltip_cr01_specularscreenspace', 'SpecularScreenSpace', ON, 'E_FPS', 'PT', 'T_NOW', ()),
 'SpecularCheckerboard': ('tooltip_cr01_specularcheckerboard', 'SpecularCheckerboard', ON, 'E_FPS', 'PT', 'T_NOW', ()),
 'URTX_PTNoBlueGlow': ('tooltip_urtx_ptnoblueglow', 'URTX_PTNoBlueGlow', OFF, 'E_FIX', 'PT', 'T_NOW', ()),
 'RTReflLocalLights': ('tooltip_cr01_rt_refl_local_lights', 'RTReflLocalLights', ON, 'E_FPS', 'RT', 'T_NOW', ()),
 'RTShadowLocalLights': ('tooltip_cr01_rt_shadow_local_lights', 'RTShadowLocalLights', ('lit', '8'), 'E_FPS', 'RT', 'T_NOW', ()),
 'RTShadowFoliageDetail': ('tooltip_cr01_rt_shadow_foliage_detail', 'RTShadowFoliageDetail', ('lit', 'mip 2'), 'E_FPS', 'RT', 'T_NOW', ()),
 'URTX_RTAOScreenSpace': ('tooltip_urtx_rtaoscreenspace', 'URTX_RTAOScreenSpace', ON, 'E_MIX', 'RT', 'T_NOW', ()),
 'URTX_RTHBIL': ('tooltip_urtx_rthbil', 'URTX_RTHBIL', OFF, 'E_FPS', 'RT', 'T_NOW', ()),
 'URTX_RTAOPretrace': ('tooltip_urtx_rtaopretrace', 'URTX_RTAOPretrace', ON, 'E_MIX', 'RT', 'T_NOW', ()),
 'URTX_RTReflScreenSpace': ('tooltip_urtx_rtreflscreenspace', 'URTX_RTReflScreenSpace', ON, 'E_FPS', 'RT', 'T_NOW', ()),
 'URTX_RTGIAlphaTest': ('tooltip_urtx_rtgialphatest', 'URTX_RTGIAlphaTest', ('lit', '5'), 'E_FPS', 'RT', 'T_NOW', ()),
 'RTQualityClassificationPrimary': ('tooltip_cr01_rt_quality_primary', 'RTGI_PRIMARY', ('lit', '128'), 'E_FPS', 'RT', 'T_NOW', ('MODE_Q', 'NEED_AUTO')),
 'Virtual_RTQualityClassificationSecondary': ('tooltip_cr01_rt_quality_secondary', 'RTGI_SECONDARY', ('lit', '32'), 'E_FPS', 'RT', 'T_NOW', ('MODE_Q', 'NEED_AUTO')),
 'RTQualityMaxProbes': ('tooltip_cr01_rt_quality_cap', 'RTGI_CAP', ('lit', '65536'), 'E_FPS', 'RT', 'T_NOW', ('MODE_Q', 'NEED_AUTO')),
 'RTQualityAOQuality': ('tooltip_cr01_rt_quality_ao', 'RTGI_AO', ('ph', 'game'), 'E_FPS', 'RT', 'T_NOW', ('MODE_Q', 'NEED_AUTO')),
 'RTQualityGlobalShadingLimit': ('tooltip_cr01_rt_quality_global_shading_limit', 'RTGI_GLOBAL', ('lit', '16384'), 'E_FPS', 'RT', 'T_NOW', ('MODE_Q', 'NEED_AUTO')),
 'RTQualityGrid1ShadingLimit': ('tooltip_cr01_rt_quality_grid1_shading_limit', 'RTGI_GRID1', ('ph', 'game'), 'E_FPS', 'RT', 'T_NOW', ('MODE_Q', 'NEED_AUTO')),
 'RTPerfClassificationPrimary': ('tooltip_cr01_rt_perf_primary', 'RTGI_PRIMARY', ('lit', '128'), 'E_FPS', 'RT', 'T_NOW', ('MODE_P', 'NEED_AUTO')),
 'Virtual_RTPerfClassificationSecondary': ('tooltip_cr01_rt_perf_secondary', 'RTGI_SECONDARY', ('lit', '16'), 'E_FPS', 'RT', 'T_NOW', ('MODE_P', 'NEED_AUTO')),
 'RTPerfMaxProbes': ('tooltip_cr01_rt_perf_cap', 'RTGI_CAP', ('lit', '32768'), 'E_FPS', 'RT', 'T_NOW', ('MODE_P', 'NEED_AUTO')),
 'RTPerfAOQuality': ('tooltip_cr01_rt_perf_ao', 'RTGI_AO', ('ph', 'game'), 'E_FPS', 'RT', 'T_NOW', ('MODE_P', 'NEED_AUTO')),
 'RTPerfGlobalShadingLimit': ('tooltip_cr01_rt_perf_global_shading_limit', 'RTGI_GLOBAL', ('lit', '8192'), 'E_FPS', 'RT', 'T_NOW', ('MODE_P', 'NEED_AUTO')),
 'RTPerfGrid1ShadingLimit': ('tooltip_cr01_rt_perf_grid1_shading_limit', 'RTGI_GRID1', ('ph', 'game'), 'E_FPS', 'RT', 'T_NOW', ('MODE_P', 'NEED_AUTO')),
 'URTX_RTReflFog': ('tooltip_urtx_rtreflfog', 'URTX_RTReflFog', ON, 'E_FPS', 'RTPT', 'T_NOW', ()),
 'URTX_TextureStreams': ('tooltip_urtx_texturestreams', 'URTX_TextureStreams', ('lit', '12'), 'E_LOAD', 'ALL', 'T_NOW', ()),
 'URTX_TerrainPreload': ('tooltip_urtx_terrainpreload', 'URTX_TerrainPreload', ('lit', '20 m'), 'E_LOAD', 'ALL', 'T_AREA', ()),
 'URTX_TexUnloadSpeed': ('tooltip_urtx_texunloadspeed', 'URTX_TexUnloadSpeed', ('lit', '1x'), 'E_MEM', 'ALL', 'T_NOW', ()),
 'URTX_MeshStreamLead': ('tooltip_urtx_meshstreamlead', 'URTX_MeshStreamLead', ('lit', '10 m'), 'E_LOAD', 'ALL', 'T_AREA', ()),
 'URTX_GenericStreamLead': ('tooltip_urtx_genericstreamlead', 'URTX_GenericStreamLead', ('lit', '45 m'), 'E_LOAD', 'ALL', 'T_AREA', ()),
 'URTX_GCDiscardThreshold': ('tooltip_urtx_gcdiscardthreshold', 'URTX_GCDiscardThreshold', ('ph', 'game'), 'E_MEM', 'ALL', 'T_NOW', ()),
 'URTX_GCIdleFrames': ('tooltip_urtx_gcidleframes', 'URTX_GCIdleFrames', ('lit', '1800'), 'E_MEM', 'ALL', 'T_NOW', ()),
 'URTX_RTASDefrag': ('tooltip_urtx_rtasdefrag', 'URTX_RTASDefrag', ('lit', '30%'), 'E_MEM', 'RTPT', 'T_RESTART', ()),
 'URTX_RTASBatch': ('tooltip_urtx_rtasbatch', 'URTX_RTASBatch', ('lit', '256'), 'E_PACE', 'RTPT', 'T_RESTART', ()),
 'URTX_FGMenuResources': ('tooltip_urtx_fgmenuresources', 'URTX_FGMenuResources', ('ph', 'releases'), 'E_FG', None, 'T_NOW', ()),
}
# registered engine defaults these literals must match (checked by tests against src/urt_gfx_table.c / cvar table)
EVIDENCE = {'URTX_TextureStreams': 12, 'URTX_TerrainPreload': 20.0, 'URTX_TexUnloadSpeed': 1.0, 'URTX_MeshStreamLead': 10.0,
            'URTX_GenericStreamLead': 45.0, 'URTX_RTASDefrag': 30, 'URTX_RTASBatch': 256, 'URTX_RTGIAlphaTest': 5.0,
            'URTX_PTSkinBounceDesat': 0.5, 'URTX_PTNoBlueGlow': 4.0}

SCOPE_SHOWN = {'CPUDistantTrees', 'CPUFastBoneReads', 'CPUPhysicsLOD', 'URTX_RTReflFog'}

def tooltips(loc):
    """loc(lang, key) -> existing string. Returns {lang: {tooltip key: text}}."""
    out = {l: {} for l in LANGS}
    for cid, (tk, core, native, perf, scope, timing, extras) in SPEC.items():
        for l in LANGS:
            parts = [CORE[core][l]]
            if native:
                kind, v = native
                val = v if kind == 'lit' else (loc(l, v) if kind == 'key' else TP.P(v, l))
                parts.append(TP.P('native', l).format(N=loc(l, 'preset_value_refinedtracing_native_auto'), v=val))
            if perf: parts.append(TP.P(perf, l))
            # mode/scope already shown by the section heading; only rows whose heading does not imply it say so
            if scope and cid in SCOPE_SHOWN: parts.append(TP.P(S_[scope], l))
            if timing: parts.append(TP.P(timing, l))
            if 'NEED_AUTO' in extras: parts.append(TP.P('NEED_AUTO', l))
            sep = '' if l in ('jp', 'cn', 'zh') else ' '
            out[l][tk] = sep.join(parts)
    return out

# ---- option labels (strings only) ----
NATIVE_VALUE_LABELS = {  # key -> suffix shown after "Native / Auto"
    'preset_value_rtqualityclassificationprimary_0': ' (128)', 'preset_value_rtperfclassificationprimary_0': ' (128)',
    'preset_value_rtqualityclassificationsecondary_0': ' (32)', 'preset_value_rtperfclassificationsecondary_0': ' (16)',
    'preset_value_rtqualitymaxprobes_0': ' (65536)', 'preset_value_rtperfmaxprobes_0': ' (32768)',
    'preset_value_rtqualityaoquality_0': '', 'preset_value_rtperfaoquality_0': '',
}
ONOFF = {'preset_value_cr01_master_on': 'urtx_w_on', 'preset_value_cr01_master_off': 'urtx_w_off',
         'preset_value_cr01_manual_on': 'urtx_w_on', 'preset_value_cr01_manual_off': 'urtx_w_off'}
PLAIN_DIGITS = ['preset_value_rt%smaxprobes_%d' % (m, n) for m in ('quality', 'perf') for n in (8192, 32768, 65536, 131072, 262144)]
NEW_LABELS = {'urtx_n_4': '4', 'urtx_n_20_m': '20 m'}

def labels(loc):
    """{lang: {key: text}} for changed existing label keys; plus NEW_LABELS (added)."""
    out = {l: {} for l in LANGS}
    for l in LANGS:
        nat = loc(l, 'preset_value_refinedtracing_native_auto')
        for k, suf in NATIVE_VALUE_LABELS.items(): out[l][k] = nat + suf
        for k, src in ONOFF.items(): out[l][k] = loc(l, src)
        for k in PLAIN_DIGITS: out[l][k] = k.rsplit('_', 1)[1]
    return out

def check():
    TP.check()
    assert set(SPEC) - {'MasterEnabled'} <= set(SPEC)
    for cid, s in SPEC.items():
        assert set(CORE[s[1]]) == set(LANGS), (cid, set(LANGS) ^ set(CORE[s[1]]))
    return True
