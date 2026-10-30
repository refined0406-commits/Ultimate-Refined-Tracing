#!/usr/bin/env python3
"""RC7 separator repair: executable regression test (offline; the game UI itself needs in-game confirmation).

Runs the REAL `W3OptionsSeparator.width` setter bytecode, extracted from both movie copies
(compressed GFx movie and uncompressed SWF) of
  (a) the original panel_ingamemenu.redswf (V4.3.1 / V5.1.0 / RC3-RC6), and
  (b) the RC7 file produced by tools/v52_selected/separator_swf_fix.py (what build.py ships),
in a minimal AVM2 interpreter, on one recycled separator object, through sequences of layouts:
unnamed (empty) label, named headings, repeated relayouts, a heading measured with 0 width first.
 (a) must reproduce the defect (right line stays hidden after one empty layout);
 (b) must show both lines for every named layout, with the original geometry, and keep the
     original single-line look for unnamed separators.
Also checks the patched file differs from the original ONLY inside the two movie bodies."""
import sys as _sys
from pathlib import Path as _P
if not (_P(__file__).resolve().parents[2] / 'game/wcc/panel_ingamemenu.redswf').exists():
    print('PASS skipped: the game menu movie (CD PROJEKT RED asset) is not redistributed; place your own copy at game/wcc/panel_ingamemenu.redswf to run this check')
    _sys.exit(0)
import hashlib, struct, sys, zlib
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools' / 'v52_selected'))
from avm2 import ABC, R
import separator_swf_fix as FIX

orig = (ROOT / 'game/wcc/panel_ingamemenu.redswf').read_bytes()
assert hashlib.sha256(orig).hexdigest() == FIX.ORIGINAL_SHA
fixed = FIX.patch(orig)

def movies(asset):
    c = asset.index(b'CFX'); block = struct.unpack_from('<I', asset, c - 4)[0]
    z = zlib.decompressobj(); gfx = z.decompress(asset[c + 8:c + block]); assert z.eof
    f = asset.index(b'FWS', c + block); fl = struct.unpack_from('<I', asset, f + 4)[0]
    return {'compressed GFx movie': b'GFX' + asset[c + 3:c + 8] + gfx, 'uncompressed SWF copy': asset[f:f + fl]}

def abcs(movie):
    nb = movie[8] >> 3; o = 8 + (5 + 4 * nb + 7) // 8 + 4; out = []
    while o + 2 <= len(movie):
        code = struct.unpack_from('<H', movie, o)[0]; o += 2; tag, ln = code >> 6, code & 0x3f
        if ln == 0x3f: ln = struct.unpack_from('<I', movie, o)[0]; o += 4
        if tag == 82: b = movie[o:o + ln]; out.append(b[b.index(b'\0', 4) + 1:])
        o += ln
        if tag == 0: break
    return out

def width_setter(movie):
    for blob in abcs(movie):
        a = ABC(blob)
        for c in a.inst:
            if c['name'] == 'W3OptionsSeparator':
                mi = [t[2] for t in c['traits'] if len(t) == 3 and t[0] == 'setter' and t[1] == 'width'][0]
                return a, a.bodies[mi]
    raise AssertionError('W3OptionsSeparator not found')

class Obj(dict):
    pass

def run(a, code, this, value):
    """Minimal AVM2 interpreter for exactly the opcodes of this setter."""
    loc = [this, value, None, None, None]; st = []; r = R(code)
    while True:
        op = r.u8()
        if op == 0xd0: st.append(loc[0])
        elif op == 0xd1: st.append(loc[1])
        elif op == 0xd2: st.append(loc[2])
        elif op == 0xd3: st.append(loc[3])
        elif op == 0x62: st.append(loc[r.u30()])
        elif op == 0xd6: loc[2] = st.pop()
        elif op == 0xd7: loc[3] = st.pop()
        elif op == 0x63: loc[r.u30()] = st.pop()
        elif op == 0x30: st.pop()                                   # pushscope
        elif op == 0x24: v = r.u8(); st.append(v - 256 if v > 127 else v)
        elif op == 0x26: st.append(True)
        elif op == 0x27: st.append(False)
        elif op == 0x2a: st.append(st[-1])
        elif op == 0x2b: st[-1], st[-2] = st[-2], st[-1]
        elif op == 0x76: st.append(bool(st.pop()))
        elif op == 0x73: st.append(int(st.pop()))
        elif op in (0xa0, 0xa1, 0xa3):
            b2 = st.pop(); a2 = st.pop(); st.append(a2 + b2 if op == 0xa0 else a2 - b2 if op == 0xa1 else a2 / b2)
        elif op == 0x66: n = a.mname(r.u30()); st.append(st.pop()[n])
        elif op == 0x61:
            n = a.mname(r.u30()); v = st.pop(); o = st.pop()
            o[n] = bool(v) if n == 'visible' else v                 # Boolean-typed property coercion
            if o.get('_is_label') and n == 'width': pass
        elif op in (0x14, 0x11, 0x10):
            d = r.s24(); tgt = r.o + d
            if op == 0x10: r.o = tgt
            elif op == 0x14:
                b2 = st.pop(); a2 = st.pop()
                if a2 != b2: r.o = tgt
            else:
                if bool(st.pop()): r.o = tgt
        elif op == 0x47: return
        else: raise AssertionError('opcode %02x not modelled' % op)

def new_separator():
    return Obj(label=Obj(textWidth=0, width=0, _is_label=True), leftLine=Obj(width=0, x=0, visible=True),
               rightLine=Obj(width=0, x=0, visible=True))

def layout(a, code, sep, text_width, row_width=1167):
    sep['label']['textWidth'] = text_width
    run(a, code, sep, row_width)
    return (sep['leftLine']['width'], sep['leftLine']['x'], sep['label']['x'] if 'x' in sep['label'] else None,
            sep['rightLine']['visible'], sep['rightLine']['width'], sep['rightLine']['x'])

fails = 0
def check(c, m):
    global fails
    print(('PASS ' if c else 'FAIL ') + m); fails += 0 if c else 1

HEADINGS = [52, 140, 160, 141, 152, 205, 156, 229, 214]   # representative text widths (px) of named headings
for asset_name, asset in (('original', orig), ('final', fixed)):
    for mname, movie in movies(asset).items():
        a, code = width_setter(movie)
        sep = new_separator()
        named0 = layout(a, code, sep, 140)
        geom_named = named0
        layout(a, code, sep, 0)                         # recycled row showed an unnamed separator
        after = layout(a, code, sep, 140)               # same row now shows a named heading
        if asset_name == 'original':
            check(named0[3] is True and after[3] is False,
                  '%s / %s: defect reproduced (right line hidden for good after one 0-width layout)' % (asset_name, mname))
            continue
        ok = after == geom_named and after[3] is True
        # long recycled sequence: random-ish order of unnamed and named layouts, repeated relayouts
        seq = [0, 52, 52, 0, 0, 229, 140, 0, 160, 141, 141, 0, 152, 205, 156, 214, 0, 229] * 5
        for tw in seq:
            g = layout(a, code, sep, tw)
            if tw == 0:
                ok &= g[3] is False and g[0] == 1167          # unnamed: original look (one full-width line)
            else:
                gap = int(1167 - tw)
                ok &= g[3] is True and g[4] == gap / 2 - 15 and g[0] == gap / 2 - 5 \
                      and g[5] == g[0] + tw + 15 and g[4] > 0 # named: both lines, original geometry
        check(ok, '%s / %s: every named layout shows both lines with the original geometry over %d recycled layouts; unnamed keep the original look' % (asset_name, mname, len(seq) + 3))
        fresh = new_separator(); fresh['rightLine']['visible'] = False
        g = layout(a, code, fresh, 141)
        check(g[3] is True, '%s / %s: a separator created with a hidden right line shows it at its first named layout' % (asset_name, mname))
# byte-level containment and determinism
c = orig.index(b'CFX'); block = struct.unpack_from('<I', orig, c - 4)[0]
f = orig.index(b'FWS', c + block); fl = struct.unpack_from('<I', orig, f + 4)[0]
diff = [i for i in range(len(orig)) if orig[i] != fixed[i]] if len(orig) == len(fixed) else None
check(diff is not None and all(c + 8 <= i < c + block or f <= i < f + fl for i in diff),
      'patched file: same size; differs only inside the two movie bodies (CR2W header, tables, sizes untouched)')
mo, mf = movies(orig), movies(fixed)
for k in mo:
    d = [i for i in range(len(mo[k])) if mo[k][i] != mf[k][i]]
    ws = mf[k].index(FIX.NEW); wi = mf[k].index(FIX.reordered_init()) + 63
    wd = mf[k].index(FIX.DRAW_HEADER_NEW + FIX.new_draw())   # V5.2.0 release part 3: renderer draw (header max_stack + code)
    check(len(mo[k]) == len(mf[k]) and d and all(ws <= i < ws + 14 or wi <= i < wi + 76
                                                  or wd <= i < wd + len(FIX.DRAW_HEADER) + len(FIX.DRAW_CODE) for i in d),
          '%s: only the 14-byte width-setter window, the 76-byte initSeparator window and the renderer draw body changed' % k)
check(FIX.patch(orig) == fixed, 'fix is deterministic')
shipped = ROOT / 'build/swf/panel_ingamemenu.redswf'
if shipped.exists():
    check(shipped.read_bytes() == fixed, 'build output build/swf/panel_ingamemenu.redswf is exactly the final separator fix of the original')

# ---- initSeparator on a recycled row (final fix, part 2) ----
# Model of the recycled state: the row last showed a normal option, so its separator object is hidden. A title
# measured while the object is hidden reports textWidth 0 (the only remaining way, with part 1, for a layout to
# hide the right line). Runs the REAL initSeparator bytecode (existing-object path) and the REAL width setter.
def init_code(movie):
    for blob in abcs(movie):
        a = ABC(blob)
        for c in a.inst:
            if c['name'] == 'W3SubMenuListItemRenderer':
                mi = [t[2] for t in c['traits'] if len(t) == 3 and t[1] == 'initSeparator'][0]
                return a, a.bodies[mi]
def run_init(a, code, rend, setter):
    st = []; r = R(code); loc = [rend, None]
    while True:
        op = r.u8()
        if op == 0xd0: st.append(loc[0])
        elif op == 0x30: st.pop()
        elif op == 0x20: st.append(None)
        elif op == 0x80: r.u30()
        elif op == 0xd5: loc[1] = st.pop()
        elif op == 0x24: v = r.u8(); st.append(v - 256 if v > 127 else v)
        elif op == 0x25: v = r.u30(); st.append(v - (1 << 32) if v >= 1 << 31 else v)
        elif op == 0x26: st.append(True)
        elif op == 0x60: st.append(rend[a.mname(r.u30())])
        elif op == 0x66: n = a.mname(r.u30()); o = st.pop(); st.append(o['_tw']() if n == 'textWidth' and '_tw' in o else o[n])
        elif op == 0x61:
            n = a.mname(r.u30()); v = st.pop(); o = st.pop()
            if o.get('_is_sep') and n == 'width': setter(o, v)
            else: o[n] = bool(v) if n == 'visible' else v
        elif op == 0x14:
            d = r.s24(); b2 = st.pop(); a2 = st.pop()
            if a2 != b2: r.o += d
        elif op == 0x4f: r.u30(); argc = r.u30(); [st.pop() for _ in range(argc)]; st.pop()
        elif op == 0x47: return
        else: raise AssertionError('init opcode %02x not modelled' % op)
for asset_name, asset in (('original', orig), ('final', fixed)):
    for mname, movie in movies(asset).items():
        ia, icode = init_code(movie); sa, scode = width_setter(movie)
        def mk():
            sep = Obj(_is_sep=True, visible=True, leftLine=Obj(width=0, x=0, visible=True), rightLine=Obj(width=0, x=0, visible=True))
            lab = Obj(text='', width=0, x=0, _is_label=True)
            lab['_tw'] = lambda: (8 * len(lab['text'])) if sep['visible'] else 0     # measured only while shown
            sep['label'] = lab
            return sep
        def setter(o, v):
            o['label']['textWidth'] = o['label']['_tw']()
            run(sa, scode, o, v)
        sep = mk(); rend = Obj(mcSeparator=sep, data=Obj(label=' Ray Tracing\xa0'), width=1167, height=60)
        ok = True
        for k in range(40):
            sep['visible'] = False                       # the row showed a normal option (createSubElementByType)
            rend['data'] = Obj(label=[' CPU\xa0', ' Path Tracing\xa0', ' RTGI: Performance\xa0', ' Textures / Rendering\xa0'][k % 4])
            run_init(ia, icode, rend, setter)
            ok &= sep['rightLine']['visible'] is True and sep['visible'] is True and sep['rightLine']['width'] > 0
        if asset_name == 'original':
            check(not ok, '%s / %s: recycled-hidden model reproduces a missing right line' % (asset_name, mname))
        else:
            check(ok, '%s / %s: 40 recycled rows (hidden before reuse): every heading shows both lines' % (asset_name, mname))
print('TOTAL %s %d' % ('FAIL' if fails else 'PASS', fails)); sys.exit(1 if fails else 0)
