#!/usr/bin/env python3
"""V5.3 private development: derive tools/v53_dev/menu_v53_reference.xml from the approved V5.2.0 (RC7) menu
reference by inserting ONLY the V5.3 rows and two named separators. Every V5.2.0 row, option, Quick Preset
entry and separator stays byte-for-byte identical in content and order (verified below).
 * PT section end (after SpecularCheckerboard): PT: No Blue Glow on Armour
 * RT section end (after RT: Screen-Space Reflection Assist): RT: GI Alpha-Test Texture Detail
 * after Textures / Rendering: [Memory & Streaming] 5 rows, [RT Memory Management] 2 rows, then Frame Generation
Hidden storage (URTExtended, hideAlways) receives the same nine rows. Usage: make_menu_v53.py [--check]"""
import copy, sys
import xml.etree.ElementTree as ET
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools' / 'v52_selected'))
import spec as S
NATIVE = 'preset_value_refinedtracing_native_auto'
SRC = ROOT / 'tools/v52_selected/menu_rc7_reference.xml'
DST = ROOT / 'tools/v53_dev/menu_v53_reference.xml'
V53 = {r['id']: r for r in S.ROWS_V53}
PLACE = [('SpecularCheckerboard', ['URTX_PTNoBlueGlow']),
         ('URTX_RTReflScreenSpace', ['URTX_RTGIAlphaTest']),
         ('URTX_TerrainPreload', ['URT53SEP_MEM', 'URTX_TexUnloadSpeed', 'URTX_MeshStreamLead', 'URTX_GenericStreamLead',
                                  'URTX_GCDiscardThreshold', 'URTX_GCIdleFrames',
                                  'URT53SEP_RTMEM', 'URTX_RTASDefrag', 'URTX_RTASBatch'])]
SEPS = {'URT53SEP_MEM': 'urt53_sep_mem', 'URT53SEP_RTMEM': 'urt53_sep_rtmem'}

def option_var(r, visible):
    v = ET.Element('Var')
    if visible:
        v.set('overrideGroup', 'URTExtended')
    v.set('id', r['id']); v.set('displayName', r['id'].lower()); v.set('displayType', 'OPTIONS'); v.set('value', '0'); v.set('tags', 'customNames')
    oa = ET.SubElement(v, 'OptionsArray')
    for i, lab in enumerate([NATIVE] + r['labels']):
        o = ET.SubElement(oa, 'Option', {'id': str(i), 'displayName': lab})
        ET.SubElement(o, 'Entry', {'overrideGroup': 'URTExtended', 'varId': r['id'], 'value': str(i)})
    return v

def build():
    root = ET.parse(SRC).getroot()
    vis = root.find("./Group[@id='RefinedTracingCR01']/VisibleVars")
    hid = root.find("./Group[@id='URTExtended']/VisibleVars")
    for anchor, ids in PLACE:
        kids = list(vis)
        at = [k.get('id') for k in kids].index(anchor) + 1
        for j, rid in enumerate(ids):
            if rid in SEPS:
                el = ET.Element('Var', {'id': rid, 'displayName': SEPS[rid], 'displayType': 'SEPARATOR', 'tags': ''})
            else:
                el = option_var(V53[rid], True)
            vis.insert(at + j, el)
    for r in S.ROWS_V53:
        hid.append(option_var(r, False))
    ET.indent(root, space='    ')
    return '<?xml version="1.0" encoding="utf-8"?>\n' + ET.tostring(root, encoding='unicode') + '\n'

def verify(text):
    old = ET.parse(SRC).getroot(); new = ET.fromstring(text)
    can = lambda e: ET.canonicalize(ET.tostring(e, encoding='unicode'), strip_text=True)
    for gid in ('RefinedTracingCR01', 'URTExtended'):
        o = [v for v in old.find("./Group[@id='%s']/VisibleVars" % gid)]
        n = [v for v in new.find("./Group[@id='%s']/VisibleVars" % gid) if v.get('id') not in V53 and v.get('id') not in SEPS]
        assert [can(x) for x in o] == [can(x) for x in n], gid + ': a V5.2.0 row changed'
    ids = [v.get('id') for v in new.find("./Group[@id='RefinedTracingCR01']/VisibleVars")]
    assert ids.index('URT53SEP_MEM') == ids.index('URTX_TerrainPreload') + 1 and ids.index('URT52SEP_FG') == ids.index('URTX_RTASBatch') + 1
    return True

if __name__ == '__main__':
    t = build(); verify(t)
    if '--check' in sys.argv:
        ok = DST.read_text('utf-8') == t
        print('PASS V5.3 menu reference reproducible' if ok else 'FAIL V5.3 menu reference differs'); sys.exit(0 if ok else 1)
    DST.write_text(t, encoding='utf-8')
    print('wrote', DST.name, '- every V5.2.0 row identical; 9 rows and 2 named separators added')
