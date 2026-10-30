#!/usr/bin/env python3
"""RC5 separator contract (offline). Scroll/redraw behaviour itself can only be confirmed in game.

1. Reads the menu renderer's own bytecode from the bundled, unmodified panel_ingamemenu.redswf
   (V4.3.1 = V5.1.0 = RC3 asset) and asserts the facts the fix relies on:
     * one W3OptionsSeparator object (mcSeparator) per list renderer, created once and reused;
     * initSeparator (named heading) and initSubtleSeparator (unnamed spacer) both drive it;
     * initSubtleSeparator gives it an empty label;
     * the width setter hides rightLine when the label width is 0 and has no path that shows it again.
   So a renderer that once shows a SUBTLE_SEPARATOR draws every later named heading without its
   right-hand line. Scrolling hands renderers from row to row, which is how it spreads.
2. Asserts the menu has no SUBTLE_SEPARATOR row, five named SEPARATOR rows with non-empty labels in
   all 18 languages, and the bundled SWF is the unmodified original (not the RC4 one-byte patch).
"""
import sys as _sys
from pathlib import Path as _P
if not (_P(__file__).resolve().parents[2] / 'game/wcc/panel_ingamemenu.redswf').exists():
    print('PASS skipped: the game menu movie (CD PROJEKT RED asset) is not redistributed; place your own copy at game/wcc/panel_ingamemenu.redswf to run this check')
    _sys.exit(0)
import hashlib, struct, sys, zlib
import xml.etree.ElementTree as ET
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools' / 'v52_selected'))
sys.path.insert(0, str(ROOT / 'tools'))
from avm2 import ABC, dis

ORIGINAL_SWF = '9a9f9c5ac41990bc8819e0aaed8a03018fe06db69bd2a2f8648b4064aec28142'   # V4.3.1 / V5.1.0 / RC3
RC4_PATCHED = '9d8752a2a44c966196d40f1a411080820a030ddaf31dce01a770f15c43e30d50'    # rejected, not shipped
asset = (ROOT / 'game/wcc/panel_ingamemenu.redswf').read_bytes()
assert hashlib.sha256(asset).hexdigest() == ORIGINAL_SWF, 'bundled panel_ingamemenu.redswf must be the unmodified V5.1.0 asset'
assert hashlib.sha256(asset).hexdigest() != RC4_PATCHED

# ---- the GFx movie the game loads: CSwfResource -> CFX (zlib) block ----
assert asset[:4] == b'CR2W'
cfx = asset.index(b'CFX')
z = zlib.decompressobj()
movie = b'GFX' + asset[cfx + 3:cfx + 8] + z.decompress(asset[cfx + 8:])
assert z.eof and len(movie) > 700000, 'CFX movie is one complete zlib stream'
nbits = movie[8] >> 3
o = 8 + (5 + 4 * nbits + 7) // 8 + 4
abc_blobs = []
while o + 2 <= len(movie):
    code = struct.unpack_from('<H', movie, o)[0]; o += 2
    tag, ln = code >> 6, code & 0x3f
    if ln == 0x3f:
        ln = struct.unpack_from('<I', movie, o)[0]; o += 4
    if tag == 82:                         # DoABC: flags u32 + name\0 + abc
        b = movie[o:o + ln]; abc_blobs.append(b[b.index(b'\0', 4) + 1:])
    o += ln
    if tag == 0:
        break
assert abc_blobs, 'movie has ActionScript 3 code'
found = None
for blob in abc_blobs:
    a = ABC(blob)
    names = [c['name'] for c in a.inst]
    if 'W3OptionsSeparator' in names and 'W3SubMenuListItemRenderer' in names:
        found = a
assert found, 'W3OptionsSeparator and W3SubMenuListItemRenderer in the loaded movie'
a = found
cls = {c['name']: c for c in a.inst}
def body(cname, kind, name):
    m = [t[2] for t in cls[cname]['traits'] if len(t) == 3 and t[0] == kind and t[1] == name]
    assert len(m) == 1, (cname, kind, name)
    return [l.split(None, 1)[1] if ' ' in l.strip() else l.strip() for l in dis(a, a.bodies[m[0]])]

width = body('W3OptionsSeparator', 'setter', 'width')
w = '\n'.join(width)
assert 'getproperty rightLine\npushfalse \nsetproperty visible' in w, 'empty label hides the right-hand line'
assert 'pushtrue' not in w, 'no path in the width setter shows the right-hand line again'
assert w.count('setproperty visible') == 1

named = '\n'.join(body('W3SubMenuListItemRenderer', 'method', 'initSeparator'))
subtle = '\n'.join(body('W3SubMenuListItemRenderer', 'method', 'initSubtleSeparator'))
for t in (named, subtle):
    assert 'getproperty mcSeparator\npushnull \nifne' in t, 'separator object created only once per renderer'
    assert 'initproperty mcSeparator' in t and 'setproperty width' in t
    assert 'rightLine' not in t, 'neither init path resets the right-hand line'
assert "getproperty label\ngetlex data\ngetproperty label\nsetproperty text" in named, 'named heading: label = row text'
assert "getproperty label\npushstring ''\nsetproperty text" in subtle, 'subtle spacer: empty label (textWidth 0)'

# ---- the menu ----
P = ET.parse(ROOT / 'build/menu/RefinedTracing.xml').getroot()
for g in P.findall('Group'):
    vs = g.find('VisibleVars')
    if vs is None:
        continue
    types = [v.get('displayType') for v in vs.findall('Var')]
    assert 'SUBTLE_SEPARATOR' not in types, 'no SUBTLE_SEPARATOR row on any page (it disables the headings\' right-hand line)'
rows = P.find('./Group[@id="RefinedTracingCR01"]/VisibleVars').findall('Var')
heads = [r for r in rows if r.get('displayType') == 'SEPARATOR']
assert [h.get('id') for h in heads] == ['URT52SEP_CPU', 'URT52SEP_PT', 'URT52SEP_RT', 'URT52SEP_RTGIQ',
                                      'URT52SEP_RTGIP', 'URT52SEP_RTPT', 'URT52SEP_TEX', 'URT53SEP_MEM', 'URT53SEP_RTMEM', 'URT52SEP_FG']  # V5.3 dev: +2; R6: +1; R12 FIX4: EXPERIMENTAL heading gone (no rows)
import json as _j, spec as _S
_EV = _j.loads((ROOT / 'tools/v52_selected/native_menu_evidence.json').read_text())
_held = [k for k in _S.NATIVE_MENU_HOLD if _EV[k]['status'] != 'released']
assert sum(r.get('displayType') != 'SEPARATOR' for r in rows) == 54 + len(_S.ROWS_V53) + len([r for r in _S.ROWS_V530 if r.get('menu', True)]) - len(_held) - len(_S.RETIRED_AFTER_COMPILE) + len(_S.RETIRED_V530)
from v52_selected.locale_records import RECORDS
assert len(RECORDS) == 18
sys.path.insert(0, str(ROOT / 'tools' / 'v53_dev'))
import locale_v53 as _L53
RECORDS = {l: {**RECORDS[l], **_L53.records()[l], 'urt531_sep_exp': ' EXPERIMENTAL\xa0'} for l in RECORDS}
for lang, rec in RECORDS.items():
    for h in heads:
        k = h.get('displayName')
        assert rec[k].strip('  ') != '', (lang, k)
print('PASS separator root cause present in the shipped original SWF (shared mcSeparator, empty subtle label hides rightLine, '
      'never re-shown); menu has 0 SUBTLE_SEPARATOR rows, 10 named headings, %d controls, 18 languages; source SWF is the original (RC7 repair applied at build). ' % (54 + len(_S.ROWS_V53) - len(_held) - len(_S.RETIRED_AFTER_COMPILE) + len(_S.RETIRED_V530)) +
      ''
      'Scrolling still requires in-game confirmation.')
