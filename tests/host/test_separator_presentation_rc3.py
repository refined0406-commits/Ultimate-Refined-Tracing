#!/usr/bin/env python3
"""Offline UI string/structure contract. Scroll redraw is Windows/in-game only."""
from pathlib import Path
import xml.etree.ElementTree as ET
import sys
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools'))
from v52_selected.locale_records import RECORDS
sys.path.insert(0,str(ROOT/'tools'/'v53_dev'))
import locale_v53 as _L53
for _l,_r in _L53.records().items(): RECORDS[_l]={**RECORDS[_l],**_r,'urt531_sep_exp':' EXPERIMENTAL\xa0','option_urt531_sep_exp':' EXPERIMENTAL\xa0'}   # V5.3 dev headings; R6 EXPERIMENTAL (English in every language)
import rc1_w3codec as C
P=ET.parse(ROOT/'build/menu/RefinedTracing.xml').getroot()
g=P.find('./Group[@id="RefinedTracingCR01"]/VisibleVars')
rows=g.findall('Var')
headings=[r for r in rows if r.get('displayType')=='SEPARATOR']
expected={'urt52_sep_cpu','urt52_sep_pt','urt52_sep_rt','urt52_sep_rtgiq','urt52_sep_rtgip','urt52_sep_rtpt','urt52_sep_tex','urt53_sep_mem','urt53_sep_rtmem','urt52_sep_fg'}   # R12 FIX4: no EXPERIMENTAL rows remain, so no EXPERIMENTAL heading
assert len(headings)==10 and {x.get('displayName') for x in headings}==expected
assert all(x.get('tags')=='' for x in headings), 'Do not bypass 18-language localisation'
assert len(RECORDS)==18
for lang, rec in RECORDS.items():
    loc=C.decode((ROOT/'build/loc'/f'{lang}.w3strings').read_bytes(),lang)
    byid={v[0]:v[3] for v in loc.blocks}
    bm=dict(loc.block2)
    def h(s):
        v=0
        for ch in s.lower():
            raw=ch.encode('utf-16le')
            for i in range(0,len(raw),2):v=(31*v+int.from_bytes(raw[i:i+2],'little'))&0xffffffff
        return v
    for k in expected:
        label=rec[k]
        assert label.startswith(' ') and label.endswith('\xa0'),(lang,k,repr(label))
        assert rec['option_'+k]==label
        for x in [k,'option_'+k]:
            assert byid[bm[h(x)]]==label,(lang,x)
ids=[r.get('id') for r in rows]
assert len(ids)==len(set(ids))
assert ids[0:2]==['MasterEnabled','QuickPresetSelect']
assert 'URTX_TextureStreams' in ids
assert ids.index('URT52SEP_TEX')+1==ids.index('URTX_TextureStreams')
EN=RECORDS['en']
assert [EN[h.get('displayName')] for h in headings]==[' CPU\xa0',' Path Tracing\xa0',' Ray Tracing\xa0',' RTGI: Quality\xa0',
  ' RTGI: Performance\xa0',' RT/PT: Shared\xa0',' Textures / Rendering\xa0',' Memory & Streaming\xa0',' RT Memory Management\xa0',' Frame Generation\xa0'],'approved English heading names'   # R12 FIX4: EXPERIMENTAL heading gone
for lang,rec in RECORDS.items():
    for h in headings:
        t=rec[h.get('displayName')]
        assert '(' not in t and '\uff08' not in t,(lang,t)     # no "(Experimental)" left in any heading
for forbidden in ('URTX_PTTextureDetail','URTX_TextureBudget','URTX_TextureSharpness','RTQualityShadingBudget','RTPerfShadingBudget'):
    assert forbidden not in ids
import json as _j
sys.path.insert(0,str(ROOT/'tools/v52_selected'))
import spec as _S
_EV=_j.loads((ROOT/'tools/v52_selected/native_menu_evidence.json').read_text())
_held=[k for k in _S.NATIVE_MENU_HOLD if _EV[k]['status']!='released']
assert sum(r.get('displayType') not in ('SEPARATOR','SUBTLE_SEPARATOR') for r in rows)==54+len(_S.ROWS_V53)+len([r for r in _S.ROWS_V530 if r.get('menu',True)])-len(_held)-len(_S.RETIRED_AFTER_COMPILE)+len(_S.RETIRED_V530)
assert not any(r.get('displayType')=='SUBTLE_SEPARATOR' for r in rows)
print('PASS 11 named separators padded in all 18 languages (approved names, no parentheses); %d controls; no subtle separator. Scroll behaviour still requires game validation.'%(54+len(_S.ROWS_V53)-len(_held)-len(_S.RETIRED_AFTER_COMPILE)+len(_S.RETIRED_V530)))
