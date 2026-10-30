#!/usr/bin/env python3
"""V5.3 private development locale step. Runs AFTER tools/build_v5_locales.py and tools/build_v52_locales.py.
Adds only new keys (IDs 1194781000 + index in sorted key order: deterministic, collision-checked) and
appends one sentence to the existing PT Ray-Traced Shadows tooltip. Every other V5.2.0 string is untouched."""
import sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
import rc1_w3codec as C
sys.path.insert(0, str(ROOT / 'tools' / 'v53_dev'))
import locale_v53 as L
BASE_ID = 1194781000
L.check()
REC = L.records(); KEYS = L.new_keys()
IDS = {k: BASE_ID + i for i, k in enumerate(KEYS)}
def h(s):
    v = 0
    for c in s.lower():
        raw = c.encode('utf-16le')
        for i in range(0, len(raw), 2): v = (31 * v + int.from_bytes(raw[i:i + 2], 'little')) & 0xffffffff
    return v
assert sorted(C.MAGIC) == sorted(L.LANGS)
for lang in sorted(C.MAGIC):
    p = ROOT / 'build/loc' / f'{lang}.w3strings'
    d = C.decode(p.read_bytes(), lang)
    cur = dict(d.block2)
    used = {b[0] for b in d.blocks}
    # one sentence appended to the existing PT Ray-Traced Shadows tooltip (idempotent)
    i = cur[h('tooltip_cr01_ptshadows')]
    for b in d.blocks:
        if b[0] == i and not b[3].endswith(L.PTSHADOWS_ADD[lang]):
            b[3] = b[3] + L.PTSHADOWS_ADD[lang]
    for k in KEYS:
        if h(k) in cur: raise SystemExit('unexpected existing V5.3 key ' + k)
        if IDS[k] in used: raise SystemExit('ID collision ' + k)
        d.blocks.append([IDS[k], 0, 0, REC[lang][k]]); d.block2.append([h(k), IDS[k]]); cur[h(k)] = IDS[k]
    raw = C.encode(d, lang)
    assert C.encode(C.decode(raw, lang), lang) == raw
    p.write_bytes(raw)
print('PASS V5.3 dev locales: 18 languages; %d new keys (IDs %d..%d); PT shadows tooltip extended' % (len(KEYS), BASE_ID, BASE_ID + len(KEYS) - 1))
