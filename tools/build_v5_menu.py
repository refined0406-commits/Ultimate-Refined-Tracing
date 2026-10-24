#!/usr/bin/env python3
"""menu/RefinedTracing.xml (V4.3.1, unchanged) -> build/menu/RefinedTracing.xml (V5.1).

V5.1 appends ONE separator and the three Normal RT performance rows at the END of
the existing group. Every new row defaults to option 0 = Native / Auto. The Quick
Presets set them through the compiled WitcherScript (RT_CR01_ApplyQuickPreset).

V5.1.0-rc3 user-requested changes to the V4.3.1 rows (and ONLY these; asserted):
  * both RTGI "Shading Rays Per Probe" rows are removed (always Native/Auto) and
    their Quick Preset entries are dropped;
  * Quick Preset Performance: Secondary Classification Rays 4 -> 12 (both groups);
  * Quick Preset Balanced: Secondary Classification Rays 12 -> 16 and Shading
    Frame Budget 196608 -> 262144 (both groups).
Every other row, ID, value and Quick Preset entry is byte-identical and in place.
Value semantics live in src/urt_rtperf_core.c; this file only defines indices."""
from pathlib import Path
import xml.etree.ElementTree as ET
ROOT = Path(__file__).resolve().parents[1]
src = (ROOT / 'menu/RefinedTracing.xml').read_text('utf-8')
anchor = '        </VisibleVars>'
assert src.count(anchor) == 1
for old in ('PTShaderCompat', 'URTSpaceCompat', 'RTTreeShadowDistance'):
    assert old not in src, old

# ---- rc3: remove the two Shading Rays Per Probe rows + their preset entries ----
import re
REMOVED_VARS = ('RTQualityShadingRaysPerProbe', 'RTPerfShadingRaysPerProbe')
base = src
for v in REMOVED_VARS:
    pat = re.compile(r'            <Var id="' + v + r'" .*?\n            </Var>\n', re.S)
    assert len(pat.findall(base)) == 1, v
    base = pat.sub('', base, count=1)
    ent = re.compile(r'                        <Entry varId="' + v + r'" value="\d+" />\n')
    assert len(ent.findall(base)) == 3, v
    base = ent.sub('', base)
    assert v not in base, v
qa = base.index('<Var id="QuickPresetSelect"'); qb = base.index('</Var>', qa)
q = base[qa:qb]
def opt(q, oid):
    a = q.index(f'<Option id="{oid}"'); return a, q.index('</Option>', a)
EDITS = {1: [('ClassificationSecondary', '4', '12')],
         2: [('ClassificationSecondary', '12', '16'), ('ShadingBudget', '196608', '262144')]}
for oid, changes in EDITS.items():
    a, b = opt(q, oid); o = q[a:b]
    for f, old, new in changes:
        for g in ('RTQuality', 'RTPerf'):
            e_old = f'<Entry varId="{g}{f}" value="{old}" />'; e_new = f'<Entry varId="{g}{f}" value="{new}" />'
            assert o.count(e_old) == 1, (oid, g, f)
            o = o.replace(e_old, e_new)
    q = q[:a] + o + q[b:]
base = base[:qa] + q + base[qb:]

NATIVE = 'preset_value_refinedtracing_native_auto'
ROWS = [  # (Var id, label key, [option display keys]) - option index == stored value
    # rc3: three controls (rc1 also had Probe Ray Distance, Reflection Texture
    # Detail and RTAO Foliage Detail; rc2 also had Console Probe Grid; removed).
    ('RTReflLocalLights',     'cr01_rt_refl_local_lights',     [NATIVE, 'preset_value_cr01_master_off']),
    ('RTShadowLocalLights',   'cr01_rt_shadow_local_lights',   [NATIVE, 'preset_value_urt_n6', 'preset_value_urt_n4',
                                                                'preset_value_urt_n2', 'preset_value_cr01_master_off']),
    ('RTShadowFoliageDetail', 'cr01_rt_shadow_foliage_detail', [NATIVE, 'preset_value_urt_reduced', 'preset_value_urt_low',
                                                                'preset_value_urt_verylow']),
]

def row(var, label, opts):
    s = f'            <Var id="{var}" displayName="{label}" displayType="OPTIONS" value="0" tags="customNames">\n'
    s += '                <OptionsArray>\n'
    for i, k in enumerate(opts):
        s += f'                    <Option id="{i}" displayName="{k}">\n'
        s += f'                        <Entry varId="{var}" value="{i}" />\n'
        s += '                    </Option>\n'
    s += '                </OptionsArray>\n            </Var>\n'
    return s

add = '            <Var id="URTSpaceRTPerf_01" displayName="cr01_fix2_spacer" displayType="SUBTLE_SEPARATOR" value="" tags="" />\n'
add += ''.join(row(*r) for r in ROWS)
out_text = base.replace(anchor, add + anchor)

old = ET.fromstring(src.encode()); new = ET.fromstring(out_text.encode())
def ser(v):
    v.tail = None; return ET.tostring(v)
ov = [ser(v) for v in old.iter('Var') if v.get('id') not in REMOVED_VARS + ('QuickPresetSelect',)]
nv = [ser(v) for v in new.iter('Var') if v.get('id') != 'QuickPresetSelect']
assert nv[:len(ov)] == ov and len(nv) == len(ov) + 1 + len(ROWS), 'remaining V4.3.1 rows must be byte-identical and in place'
assert [v.get('id') for v in old.iter('Var') if v.get('id') not in REMOVED_VARS] == \
       [v.get('id') for v in new.iter('Var')][:len(list(old.iter('Var'))) - 2], 'row order unchanged'
def presets(root):
    q = next(v for v in root.iter('Var') if v.get('id') == 'QuickPresetSelect')
    return {o.get('id'): [(e.get('varId'), e.get('value')) for e in o.iter('Entry')] for o in q.iter('Option')}
po, pn = presets(old), presets(new)
for oid in po:
    exp = [(k, v) for k, v in po[oid] if k not in REMOVED_VARS]
    for f, a, b in EDITS.get(int(oid), []):
        exp = [(k, b if k.endswith(f) and v == a else v) for k, v in exp]
    assert pn[oid] == exp, f'Quick Preset option {oid}: only the requested rc3 changes'
assert all(k not in REMOVED_VARS for o in pn.values() for k, _ in o)
assert 'RTGIConsoleGrid' not in out_text and 'ShadingRaysPerProbe' not in out_text
ids = [v.get('id') for v in new.iter('Var')]
assert len(ids) == len(set(ids)), 'duplicate Var id'
o = ROOT / 'build/menu'; o.mkdir(parents=True, exist_ok=True)
(o / 'RefinedTracing.xml').write_text(out_text, 'utf-8')
assert (o / 'RefinedTracing.xml').read_text('utf-8') == out_text
print('PASS menu: %d V4.3.1 rows unchanged in place; 2 Shading Rays rows removed; Quick Preset rc3 edits only; '
      '+1 separator +%d Normal RT performance rows (default Native)' % (len(ov), len(ROWS)))
