#!/usr/bin/env python3
"""V5.2 selected locale generator, runs AFTER protected V5.1 tools/build_v5_locales.py.
Never touches V5.1 ID assignments except shortening existing tooltips.
11 selected rows (PT Scene Range + 10 URTX rows) and five named separators only.
"""
import sys,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
import rc1_w3codec as C
from v52_selected.locale_records import RECORDS,CORE_TIPS
MAP=json.loads((ROOT/'tools/v52_selected/locale_mapping.json').read_text())
KEY_IDS={**MAP['selected_key_ids'],**MAP['heading_ids']}
def h(s):
 v=0
 for c in s.lower():
  raw=c.encode('utf-16le')
  for i in range(0,len(raw),2):v=(31*v+int.from_bytes(raw[i:i+2],'little'))&0xffffffff
 return v
for lang in sorted(C.MAGIC):
 p=ROOT/'build/loc'/f'{lang}.w3strings'
 d=C.decode(p.read_bytes(),lang)
 current=dict(d.block2)
 for k,txt in CORE_TIPS[lang].items():
  if h(k) in current:
   i=current[h(k)]
   for b in d.blocks:
    if b[0]==i:b[3]=txt;break
 for k,txt in RECORDS[lang].items():
  i=KEY_IDS[k]
  if h(k) in current:raise SystemExit('unexpected existing V5.2 key '+k)
  if any(b[0]==i for b in d.blocks):raise SystemExit('new ID collision '+k)
  d.blocks.append([i,0,0,txt]);d.block2.append([h(k),i]);current[h(k)]=i
 raw=C.encode(d,lang)
 assert C.encode(C.decode(raw,lang),lang)==raw
 p.write_bytes(raw)
print('PASS curated V5.2: 18 locales; %d selected new keys and 5 headings'%len(KEY_IDS))
