#!/usr/bin/env python3
"""Strict single-page menu generator. Base V5.1 menu from tools/build_v5_menu.py, then
transfer the exact approved row arrangement (RC6 reference). No mutation of source generator.
RC6: rows listed in spec.NATIVE_MENU_HOLD are left out (page + storage) unless
tools/v52_selected/native_menu_evidence.json marks them 'released'."""
from pathlib import Path
import xml.etree.ElementTree as ET
ROOT=Path(__file__).resolve().parents[1]
# V5.3 dev: the V5.2.0 (RC7) arrangement plus only the V5.3 rows and two named separators
# (tools/v53_dev/make_menu_v53.py proves every V5.2.0 row is unchanged).
reference=ET.parse(ROOT/'tools/v53_dev/menu_v53_reference.xml').getroot()
base=ET.parse(ROOT/'build/menu/RefinedTracing.xml').getroot()
rg=reference.find("./Group[@id='RefinedTracingCR01']")
bg=base.find("./Group[@id='RefinedTracingCR01']")
rv=rg.find('VisibleVars');bv=bg.find('VisibleVars')
# Every original V5.1 retained option must be identical except preset entries for four retired RTAO rows.
retired={'PTRTAOEnabled','PTRTAORayCount','PTRTAORadius','PTRTAOStrength','RTQualityShadingBudget','RTPerfShadingBudget'}
import copy
original={x.get('id'):x for x in bv.findall('Var')}
for row in rv.findall('Var'):
 rid=row.get('id')
 if rid in original and rid!='QuickPresetSelect':
  assert ET.canonicalize(ET.tostring(row,encoding='unicode'),strip_text=True)==ET.canonicalize(ET.tostring(original[rid],encoding='unicode'),strip_text=True),rid
assert all(r not in [x.get('id') for x in rv.findall('Var')] for r in retired)
import json,sys
sys.path.insert(0,str(ROOT/'tools/v52_selected'))
import spec as S
EV=json.loads((ROOT/'tools/v52_selected/native_menu_evidence.json').read_text())
held={k for k in S.NATIVE_MENU_HOLD if EV[k]['status']!='released'}
assert all(EV[k]['status'] in ('held','released') for k in S.NATIVE_MENU_HOLD)
assert all(EV[k]['evidence'] for k in S.NATIVE_MENU_HOLD if EV[k]['status']=='released'),'release needs evidence'
for x in list(base.findall('Group')):base.remove(x)
for g in reference.findall('Group'):
 g=copy.deepcopy(g)
 vv=g.find('VisibleVars')
 for v in list(vv.findall('Var')):
  if v.get('id') in held:vv.remove(v)
 base.append(g)
# V5.3 DEV2: documented option display-name-only corrections (page row + hidden storage copy)
assert S.apply_option_labels(base)==2*len(S.OPTION_LABEL_ONLY)
ET.indent(base,space='    ')
s='<?xml version="1.0" encoding="utf-8"?>\n'+ET.tostring(base,encoding='unicode')+'\n'
(ROOT/'build/menu/RefinedTracing.xml').write_text(s,encoding='utf-8')
print('PASS menu recreation from the exact V5.3 reference (RC7 + V5.3 rows); rows held out of the menu (native graphics-menu overlap or no evidence):',sorted(held))
