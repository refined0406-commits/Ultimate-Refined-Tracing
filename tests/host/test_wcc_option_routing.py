#!/usr/bin/env python3
"""V5.2 RC6 source-only routing and menu regression, independent of REDkit compilation.
V5.3 dev: + 9 rows (PT, RT, Memory & Streaming, RT Memory Management), 2 named headings; every V5.2.0 check kept."""
from pathlib import Path
import json,re,sys,xml.etree.ElementTree as ET
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/v52_selected'))
import spec as S
src=(ROOT/'game/wcc/RefinedTracingCR01_RTGI3.ws').read_text('utf-8-sig')
xml=ET.parse(ROOT/'build/menu/RefinedTracing.xml').getroot()
EV=json.loads((ROOT/'tools/v52_selected/native_menu_evidence.json').read_text())
held=[k for k in S.NATIVE_MENU_HOLD if EV[k]['status']!='released']
retired=list(S.RETIRED_AFTER_COMPILE)
groups=xml.findall('Group')
assert len(groups)==2 and groups[0].get('id')=='RefinedTracingCR01' and groups[1].get('id')=='URTExtended'
assert groups[1].get('visibilityCondition')=='hideAlways'
rows=groups[0].find('VisibleVars').findall('Var')
interactive=[r.get('id') for r in rows if r.get('displayType') not in ('SEPARATOR','SUBTLE_SEPARATOR')]
NV53=len(S.ROWS_V53)
assert len(interactive)==54+NV53-len(held)-len(retired) and len(set(interactive))==len(interactive)
assert interactive[:2]==['MasterEnabled','QuickPresetSelect']
heads=[r.get('displayName') for r in rows if r.get('displayType')=='SEPARATOR']
assert heads==['urt52_sep_cpu','urt52_sep_pt','urt52_sep_rt','urt52_sep_rtgiq','urt52_sep_rtgip','urt52_sep_rtpt','urt52_sep_tex','urt53_sep_mem','urt53_sep_rtmem','urt52_sep_fg']
assert len([r for r in rows if r.get('displayType')=='SUBTLE_SEPARATOR'])==0  # RC5 fix: they hid the headings' right-hand line
ids=[r.get('id') for r in rows]
def sect(h):
    a=ids.index(h);b=next((i for i in range(a+1,len(ids)) if ids[i].startswith(('URT52SEP_','URT53SEP_'))),len(ids));return ids[a+1:b]
assert sect('URT52SEP_CPU')==['CPUDistantTrees','CPUFastBoneReads','CPUPhysicsLOD']
assert sect('URT52SEP_PT')==['GeometryBounces','HairBounces','IndirectLight','PTBillboards','PTShadows','PTSceneRange','AmbientOcclusion',
  'URTX_PTAOQuality','AOStrength','AORadius','AOContrast','SunImpact','SkyImpact','URTX_PTSkinBounceDesat','GrassDay','GrassNight',
  'SpecularScreenSpace','SpecularCheckerboard','URTX_PTNoBlueGlow']
assert 'URT52SEP_PTCAUS' not in ids and not any(r in ids for r in retired) and 'Caustic' not in (ROOT/'build/menu/RefinedTracing.xml').read_text('utf-8')
assert sect('URT52SEP_RT')==['RayTracedOptimizations','RTReflLocalLights','RTShadowLocalLights','RTShadowFoliageDetail',
  'URTX_RTAOScreenSpace','URTX_RTHBIL','URTX_RTAOPretrace','URTX_RTReflScreenSpace','URTX_RTGIAlphaTest']
assert sect('URT52SEP_RTGIQ')==['RTQualityClassificationPrimary','Virtual_RTQualityClassificationSecondary','RTQualityMaxProbes',
  'RTQualityAOQuality','RTQualityGlobalShadingLimit','RTQualityGrid1ShadingLimit']
assert sect('URT52SEP_RTGIP')==['RTPerfClassificationPrimary','Virtual_RTPerfClassificationSecondary','RTPerfMaxProbes',
  'RTPerfAOQuality','RTPerfGlobalShadingLimit','RTPerfGrid1ShadingLimit']
assert sect('URT52SEP_RTPT')==['URTX_RTReflFog']
assert sect('URT52SEP_TEX')==[x for x in ['URTX_TextureStreams','URTX_TerrainPreload'] if x not in held]
assert sect('URT52SEP_FG')==['URTX_FGMenuResources'] and ids[-1]=='URTX_FGMenuResources'
assert sect('URT53SEP_MEM')==['URTX_TexUnloadSpeed','URTX_MeshStreamLead','URTX_GenericStreamLead','URTX_GCDiscardThreshold','URTX_GCIdleFrames']
assert sect('URT53SEP_RTMEM')==['URTX_RTASDefrag','URTX_RTASBatch']
assert ids.index('URT53SEP_MEM')==ids.index('URTX_TerrainPreload')+1 and ids.index('URT52SEP_FG')==ids.index('URTX_RTASBatch')+1
gone=('PTRTAOEnabled','PTRTAORayCount','PTRTAORadius','PTRTAOStrength','RTQualityShadingBudget','RTPerfShadingBudget')
xmltext=(ROOT/'build/menu/RefinedTracing.xml').read_text('utf-8')
assert all(x not in interactive and x not in src and x not in xmltext for x in gone)
assert 'tooltip_cr01_rt_quality_budget' not in src and 'tooltip_cr01_rt_perf_budget' not in src
store=sorted(r.get('id') for r in groups[1].find('VisibleVars').findall('Var'))
assert store==sorted(r['id'] for r in S.ROWS if r['id'] not in held)
assert all(interactive.count(x)==1 for x in store)
assert all(r['id'] in src for r in S.ROWS)
assert all(x not in src and x not in interactive for x in ('URTX_PTTextureDetail','URTX_TextureBudget','URTX_TextureSharpness'))
hovers=re.findall(r"this\.onHover(?:Dynamic)?\('([^']+)'\s*,",src)
assert len(hovers)==54+NV53 and len(set(hovers))==54+NV53
assert sorted(set(hovers)-set(interactive))==sorted(held+retired) and set(interactive)<=set(hovers)  # retired: inert in the compiled script
m=re.search(r"function RT_CR01_IsOwnedOption\(optionName: name\): bool \{\n(.*?)\n\}\n",src,re.S); assert m
owned=re.findall(r"optionName == '([^']+)'",m.group(1))
assert sorted(owned)==sorted(x for x in interactive if not x.startswith('URTX_'))
assert src.count('GetGroupName(')==1
assert src.count('result = wrappedMethod(groupId, optionName, optionValue);')==1
assert "if (this.RT_CR01_IsOwnedOption(optionName))" in src
assert 'if (StrLeft(NameToString(optionName), 5) == "URTX_")' in src
assert src.count('theGame.SaveUserSettings();')==3
# Master greying covers every row except Master itself (Quick Preset and RT Optimizations separately)
dm=re.search(r"function RT_CR01_DisableDependents\(.*?\n\}\n",src,re.S).group(0)
tagged=set(re.findall(r"(?:tags|rtPerfTags|rtQualityTags)\.PushBack\('([^']+)'\)",dm))
assert tagged|{'MasterEnabled','QuickPresetSelect','RayTracedOptimizations'}==set(hovers),set(hovers)^tagged
# Quick Presets: every V5.1.0 / RC5 assignment preserved except exactly the 6 Shading Frame Budget entries
wanted=json.loads((ROOT/'tests/fixtures/v52_presets_rc6.json').read_text())
q=groups[0].find('VisibleVars').find("Var[@id='QuickPresetSelect']")
got={opt.get('id'):{e.get('varId'):e.get('value') for e in opt.iter('Entry')} for opt in q.iter('Option')}
assert got==wanted
assert not any(x.startswith('URTX_') or x=='PTSceneRange' for op in got.values() for x in op)  # new rows are preset by the script
ap=re.search(r"function RT_CR01_ApplyQuickPreset\(selected: string\): bool \{.*?\n\}\n",src,re.S).group(0)
# Expanded V5.2.0: the owner's eight assignments below are preset-controlled; PT: No Blue Glow on Armour is never assigned
pu53=re.search(r"function RT_CR01_IsPresetURTX\(optionName: name\): bool \{.*?\n\}\n",src,re.S).group(0)
assert 'URTX_PTNoBlueGlow' not in ap and 'URTX_PTNoBlueGlow' not in pu53
written=set(re.findall(r"SetVarValue\('RefinedTracingCR01', '([^']+)'",ap))
xml_preset=set(k for op in got.values() for k in op)-{'QuickPresetSelect'}
assert not any('ShadingBudget' in w for w in written)
# ---- V5.2.0 final Quick Presets: the 14 user-specified rows, as STORED values (= XML option ids), audited
# against the menu: (Performance, Balanced, Extreme); every other script assignment identical to RC8.
def table(text):
    t={}
    for mm in re.finditer(r'''  if \(selected == "1"\) \{\n    mInGameConfigWrapper\.SetVarValue\('(\w+)', '(\w+)', "([^"]*)"\);\n  \} else if \(selected == "2"\) \{\n    mInGameConfigWrapper\.SetVarValue\('\1', '\2', "([^"]*)"\);\n  \} else \{\n    mInGameConfigWrapper\.SetVarValue\('\1', '\2', "([^"]*)"\);\n  \}''',text):
        t[mm.group(2)]=[mm.group(1),mm.group(3),mm.group(4),mm.group(5)]
    return t
FINAL={  # row: (group, {display: option}, (Performance, Balanced, Extreme) as displayed by the user)
 'GeometryBounces':('RefinedTracingCR01',('1 Bounce','1 Bounce','4 Bounces')),   # V5.2.0 final: Extreme 4 (was 6)
 'HairBounces':('RefinedTracingCR01',('1 Bounce','4 Bounces','10 Bounces')),      # V5.2.0 final: Extreme 10 (was 6)
 'PTSceneRange':('RefinedTracingCR01',('150 m','Native / Auto','Native / Auto')),
 'AmbientOcclusion':('RefinedTracingCR01',('Off','Native / Auto','Native / Auto')),
 'URTX_PTAOQuality':('URTExtended',('0','0','3')),
 'RTReflLocalLights':('RefinedTracingCR01',('Off','Off','Native / Auto')),
 'RTShadowLocalLights':('RefinedTracingCR01',('Off','4','Native / Auto')),
 'RTShadowFoliageDetail':('RefinedTracingCR01',('Very Low','Low','Native / Auto')),
 'URTX_RTAOScreenSpace':('URTExtended',('Off','Off','Native / Auto')),
 'URTX_RTHBIL':('URTExtended',('Off','Off','Native / Auto')),
 'URTX_RTAOPretrace':('URTExtended',('Off','Off','Native / Auto')),
 'URTX_RTReflScreenSpace':('URTExtended',('On','On','Native / Auto')),
 'URTX_RTReflFog':('URTExtended',('Off','Native / Auto','Native / Auto')),
 'URTX_TextureStreams':('URTExtended',('8','8','16')),
 'URTX_TerrainPreload':('URTExtended',('10 m','10 m','Native / Auto')),
 'URTX_FGMenuResources':('URTExtended',('Keep resources','Keep resources','Keep resources')),
 # expanded V5.2.0 (owner's final table, 2026-10-26; six memory/AS rows Native in all three, final decision 15:37)
 'URTX_RTGIAlphaTest':('URTExtended',('8','6','4')),
 'URTX_TexUnloadSpeed':('URTExtended',('4x','2x','Native / Auto')),
 'URTX_MeshStreamLead':('URTExtended',('Native / Auto','Native / Auto','Native / Auto')),
 'URTX_GenericStreamLead':('URTExtended',('Native / Auto','Native / Auto','Native / Auto')),
 'URTX_GCDiscardThreshold':('URTExtended',('Native / Auto','Native / Auto','Native / Auto')),
 'URTX_GCIdleFrames':('URTExtended',('Native / Auto','Native / Auto','Native / Auto')),
 'URTX_RTASDefrag':('URTExtended',('Native / Auto','Native / Auto','Native / Auto')),
 'URTX_RTASBatch':('URTExtended',('Native / Auto','Native / Auto','Native / Auto')),
}
sys.path.insert(0,str(ROOT/'tools'))
from v52_selected.locale_records import RECORDS
import build_v5_locales as L5
sys.path.insert(0,str(ROOT/'tools'/'v53_dev'))
import locale_v53 as L53, tooltips_dev2 as TD2
EN={**{k:v for k,v in RECORDS['en'].items()},**L53.records()['en'],**TD2.NEW_LABELS}
def label(key):
    if key in EN: return EN[key].strip('\xa0 ')
    if key.startswith('preset_value_refinedtracing_bounce_'):
        n=int(key.rsplit('_',1)[1]); return '%d Bounce%s' % (n, '' if n == 1 else 's')   # V4 menu labels (en)
    return {'preset_value_refinedtracing_native_auto':'Native / Auto','preset_value_cr01_manual_off':'Off','preset_value_cr01_manual_on':'On',
            'preset_value_cr01_master_off':'Off','preset_value_urt_m150':'150 m','preset_value_urt_m300':'300 m','preset_value_urt_n4':'4',
            'preset_value_urt_verylow':'Very Low','preset_value_urt_low':'Low','preset_value_urt_reduced':'Reduced',
            'preset_value_urt_n6':'6','preset_value_urt_n2':'2','preset_value_urt_m250':'250 m','preset_value_urt_m200':'200 m'}[key]
cur=table(src)
for rid,(grp,disp) in FINAL.items():
    v=next(r for r in rows if r.get('id')==rid)
    opt={label(o.get('displayName')).lower():[e.get('value') for e in o.iter('Entry')][0] for o in v.iter('Option')}
    want=[grp]+[opt[d.lower()] for d in disp]
    assert cur[rid]==want,(rid,cur.get(rid),want)
rc8=json.loads((ROOT/'tests/fixtures/v52_script_presets_rc8.json').read_text())
assert {k:v for k,v in cur.items() if k not in FINAL}=={k:v for k,v in rc8.items() if k not in FINAL},'non-specified preset assignments changed'
assert all(k in cur for k in rc8)
assert 'RT_CR01_IsPresetURTX' in src
pu=re.search(r"function RT_CR01_IsPresetURTX\(optionName: name\): bool \{(.*?)\n\}",src,re.S).group(1)
assert sorted(re.findall(r"'(URTX_\w+)'",pu))==sorted(k for k,(g,_) in FINAL.items() if g=='URTExtended'),'manual edit of every preset-controlled hidden row switches Quick Preset to Off'
# every RC6 row defaults to Native / Auto (0) and only sets itself (option index == stored value)
for v in rows:
    vid=v.get('id')
    if vid=='PTSceneRange' or vid in [r['id'] for r in S.ROWS]:
        assert v.get('value')=='0',vid
        opts=list(v.iter('Option')); assert [o.get('id') for o in opts]==[str(i) for i in range(len(opts))],vid
        assert opts[0].get('displayName')=='preset_value_refinedtracing_native_auto',vid
        assert all(e.get('varId')==vid and e.get('value')==o.get('id') for o in opts for e in o.iter('Entry')),vid
print('PASS V5.3 dev menu (V5.2.0 + 9 rows, 10 named headings): %d controls (%d held), 0 subtle; caustics, tree distance and budgets retired; presets = RC5 minus 6 budget entries; final presets for 24 rows exact (14 + 8 expanded + 2 bounce rows), all others identical to RC8; difficulty guard intact' % (len(interactive), len(held)))
