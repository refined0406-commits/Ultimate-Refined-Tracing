#!/usr/bin/env python3
"""One-shot, asserted transformation of the approved RC5 menu reference into the RC6 reference.

RC6 changes (user request, 2026-10-25), nothing else:
  * remove RTQualityShadingBudget / RTPerfShadingBudget rows and their 6 Quick Preset entries;
  * PT: Scene Range (core key PTSceneRange, values unchanged from the private research builds)
    directly under PT: Ray-Traced Shadows;
  * PT: Skin Bounce Light Desaturation directly under PT: Sky Impact;
  * new named heading "PT: Caustics" with the six caustics rows, after the main PT rows;
  * named headings "RTGI: Quality" / "RTGI: Performance" above the RTGI groups;
  * named heading "RT/PT: Shared" with RT/PT: Fog in Ray-Traced Reflections after RTGI;
  * Texture heading renamed (string only) to "Textures / Rendering"; Terrain Preload Distance and
    Tree and Bush Draw Distance under Textures Streamed at Once;
  * hidden storage group URTExtended gains the new URTX_ rows.
No SUBTLE_SEPARATOR is (re)introduced. Every other row element is moved byte-identical.
Usage: rc6_menu_edit.py <RC5 reference xml> <out xml>"""
import re, sys
import xml.etree.ElementTree as ET
from pathlib import Path
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import spec as S

IND = '            '
src = Path(sys.argv[1]).read_text('utf-8')
rows = {r['id']: r for r in S.ROWS}

def block(text, vid):
    m = re.search(r'^' + IND + r'<Var (?:overrideGroup="URTExtended" )?id="%s"[^\n]*\n' % re.escape(vid), text, re.M)
    assert m, vid
    if m.group(0).rstrip().endswith('/>'):
        return m.start(), m.end()
    end = text.index('\n' + IND + '</Var>\n', m.start()) + len('\n' + IND + '</Var>\n')
    return m.start(), end

def remove(text, vid):
    a, b = block(text, vid)
    return text[:a] + text[b:]

def insert_after(text, vid, new):
    a, b = block(text, vid)
    return text[:b] + new + text[b:]

def insert_before(text, vid, new):
    a, b = block(text, vid)
    return text[:a] + new + text[a:]

def sep(vid, key):
    return IND + '<Var id="%s" displayName="%s" displayType="SEPARATOR" tags="" />\n' % (vid, key)

OPTS = {   # option label keys per new URTX row (option 0 = Native / Auto); same keys as the research builds
    'URTX_PTCaustics': ['urtx_w_on', 'urtx_w_off'],
    'URTX_PTCausticsInPT': ['urtx_w_on', 'urtx_w_off'],
    'URTX_PTCausticsTypes': ['urtx_w_cau_water', 'urtx_w_cau_surface', 'urtx_w_cau_reflect'],
    'URTX_PTCausticsRes': ['urtx_n_512', 'urtx_n_1024', 'urtx_n_4096'],
    'URTX_PTCausticsBlur': ['urtx_n_0', 'urtx_n_1', 'urtx_n_4', 'urtx_n_8'],
    'URTX_PTCausticsArea': ['urtx_n_20_m', 'urtx_n_40_m', 'urtx_n_100_m', 'urtx_n_150_m', 'urtx_n_200_m'],
    'URTX_PTSkinBounceDesat': ['urtx_n_0', 'urtx_n_0p25', 'urtx_n_0p75', 'urtx_n_1'],
    'URTX_RTReflFog': ['urtx_w_on', 'urtx_w_off'],
    'URTX_TerrainPreload': ['urtx_n_10_m', 'urtx_n_40_m', 'urtx_n_100_m', 'urtx_n_200_m', 'urtx_n_400_m', 'urtx_n_600_m'],
    'URTX_TreeDistance': ['urtx_n_0p75', 'urtx_n_1', 'urtx_n_1p5', 'urtx_n_2', 'urtx_n_2p6', 'urtx_n_3', 'urtx_n_4', 'urtx_n_5'],
}
NATIVE = 'preset_value_refinedtracing_native_auto'
for k, v in OPTS.items():
    assert len(v) == len(rows[k]['vals']), k

def urtx(vid, page):
    og = ' overrideGroup="URTExtended"' if page else ''
    s = IND + '<Var%s id="%s" displayName="%s" displayType="OPTIONS" value="0" tags="customNames">\n' % (og, vid, vid.lower())
    s += IND + '    <OptionsArray>\n'
    for i, k in enumerate([NATIVE] + OPTS[vid]):
        s += IND + '        <Option id="%d" displayName="%s">\n' % (i, k)
        s += IND + '            <Entry overrideGroup="URTExtended" varId="%s" value="%d" />\n' % (vid, i)
        s += IND + '        </Option>\n'
    return s + IND + '    </OptionsArray>\n' + IND + '</Var>\n'

def core(vid, label, keys):
    s = IND + '<Var id="%s" displayName="%s" displayType="OPTIONS" value="0" tags="customNames">\n' % (vid, label)
    s += IND + '    <OptionsArray>\n'
    for i, k in enumerate(keys):
        s += IND + '        <Option id="%d" displayName="%s">\n' % (i, k)
        s += IND + '            <Entry varId="%s" value="%d" />\n' % (vid, i)
        s += IND + '        </Option>\n'
    return s + IND + '    </OptionsArray>\n' + IND + '</Var>\n'

t = src
# 1. retire both Shading Frame Budget rows and exactly their 6 Quick Preset entries
for vid in ('RTQualityShadingBudget', 'RTPerfShadingBudget'):
    t = remove(t, vid)
    pat = re.compile(r'^\s*<Entry varId="%s" value="\d+" />\n' % vid, re.M)
    assert len(pat.findall(t)) == 3, vid
    t = pat.sub('', t)
assert 'ShadingBudget' not in t
# 2. Path Tracing additions
t = insert_after(t, 'PTShadows', core('PTSceneRange', 'cr01_pt_scene_range',
                 [NATIVE, 'preset_value_urt_m300', 'preset_value_urt_m250', 'preset_value_urt_m200', 'preset_value_urt_m150']))
t = insert_after(t, 'SkyImpact', urtx('URTX_PTSkinBounceDesat', True))
caus = ['URTX_PTCaustics', 'URTX_PTCausticsInPT', 'URTX_PTCausticsTypes', 'URTX_PTCausticsRes', 'URTX_PTCausticsBlur', 'URTX_PTCausticsArea']
t = insert_after(t, 'SpecularCheckerboard', sep('URT52SEP_PTCAUS', 'urt52_sep_ptcaus') + ''.join(urtx(c, True) for c in caus))
# 3. RTGI headings and the shared RT/PT section
t = insert_before(t, 'RTQualityClassificationPrimary', sep('URT52SEP_RTGIQ', 'urt52_sep_rtgiq'))
t = insert_before(t, 'RTPerfClassificationPrimary', sep('URT52SEP_RTGIP', 'urt52_sep_rtgip'))
t = insert_after(t, 'RTPerfGrid1ShadingLimit', sep('URT52SEP_RTPT', 'urt52_sep_rtpt') + urtx('URTX_RTReflFog', True))
# 4. Textures / Rendering rows (the heading text is a string-only rename of urt52_sep_tex)
t = insert_after(t, 'URTX_TextureStreams', urtx('URTX_TerrainPreload', True) + urtx('URTX_TreeDistance', True))
# 5. hidden storage group: every new URTX row (storage order = spec order)
store_a = t.index('<Group id="URTExtended"')
store = t[store_a:]
for vid in [r['id'] for r in S.ROWS]:
    if ('id="%s"' % vid) in store:
        continue
    prev = [r['id'] for r in S.ROWS][[r['id'] for r in S.ROWS].index(vid) - 1]
    store = insert_after(store, prev, urtx(vid, False))
t = t[:store_a] + store

# ---- checks ----
root = ET.fromstring(t.encode('utf-8'))
g = root.findall('Group')
assert [x.get('id') for x in g] == ['RefinedTracingCR01', 'URTExtended']
page = list(g[0].find('VisibleVars').findall('Var'))
ids = [v.get('id') for v in page]
assert not any(v.get('displayType') == 'SUBTLE_SEPARATOR' for v in page)
heads = [v.get('id') for v in page if v.get('displayType') == 'SEPARATOR']
assert heads == ['URT52SEP_CPU', 'URT52SEP_PT', 'URT52SEP_PTCAUS', 'URT52SEP_RT', 'URT52SEP_RTGIQ', 'URT52SEP_RTGIP',
                 'URT52SEP_RTPT', 'URT52SEP_TEX', 'URT52SEP_FG'], heads
assert ids[ids.index('PTShadows') + 1] == 'PTSceneRange'
assert ids[ids.index('SkyImpact') + 1] == 'URTX_PTSkinBounceDesat'
assert ids[ids.index('URT52SEP_PTCAUS') + 1:ids.index('URT52SEP_RT')] == caus
assert ids[ids.index('URT52SEP_RTPT') + 1:ids.index('URT52SEP_TEX')] == ['URTX_RTReflFog']
assert ids[ids.index('URT52SEP_TEX') + 1:ids.index('URT52SEP_FG')] == ['URTX_TextureStreams', 'URTX_TerrainPreload', 'URTX_TreeDistance']
assert len(ids) == len(set(ids))
assert sorted(v.get('id') for v in g[1].find('VisibleVars').findall('Var')) == sorted(rows)
q = next(v for v in page if v.get('id') == 'QuickPresetSelect')
assert not any(e.get('varId') in ('PTSceneRange',) or e.get('varId', '').startswith(('URTX_', 'URT52SEP')) for e in q.iter('Entry'))
# every untouched element of the RC5 reference is still present byte-identical
old_root = ET.fromstring(src.encode('utf-8'))
for v in old_root.iter('Var'):
    vid = v.get('id')
    if vid in ('RTQualityShadingBudget', 'RTPerfShadingBudget', 'QuickPresetSelect'):
        continue
    a, b = block(src[src.index('<Group id="URTExtended"'):] if False else src, vid) if True else (0, 0)
    # first occurrence (page) must exist verbatim in the new text
    assert src[a:b] in t, vid
Path(sys.argv[2]).write_text(t, 'utf-8')
print('PASS RC6 menu reference: %d page entries (%d controls, %d named headings), 0 subtle separators; '
      'budget rows and their 6 preset entries removed' % (len(ids), len(ids) - len(heads), len(heads)))
