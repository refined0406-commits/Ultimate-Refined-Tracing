#!/usr/bin/env python3
"""Separator lifecycle repeatability test (V5.2.0 release, part 3), on the REAL compiled menu code.

Executes, in a small AVM2 interpreter, the actual bytecode of
  W3SubMenuListItemRenderer.initSeparator, W3SubMenuListItemRenderer.draw and W3OptionsSeparator.width (setter)
taken from BOTH movie copies (compressed GFx movie, uncompressed SWF) of
  (a) the previous V5.2.0 menu file (parts 1+2), and (b) the release file built by separator_swf_fix.py (parts 1-3),
inside a model of the list lifecycle found in the same movie: CLIK invalidation (invalidate schedules a validation on
the next frame / RENDER; validateNow runs draw then clears the invalidation hash), ScrollingList.populateData
(setData then validateNow per row), drawLayout (renderer.width = list width - margins - scroll bar), recycled rows,
and closing/reopening (new rows). Titles can be unmeasurable at setData time (0 px) for a few frames.

Scenarios (each must leave EVERY visible heading with both lines at the row's current width, the original geometry,
and no listener left behind once settled):
  1 row resized by the list after setData (first opening / page or menu switch);
  2 title not measurable when the row is filled, measurable a few frames later;
  3 5,000 randomised frames: scrolling, recycling, reopening, list width changes, quick-preset style refreshes,
    Master On/Off greying (alpha), unmeasurable windows, subtle (unnamed) separators of other pages.
The previous V5.2.0 file must FAIL 1 and 2 (proving the scenarios reproduce the remaining defect classes).
This is offline evidence; final acceptance is in game."""
import hashlib, random, struct, sys, zlib
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools' / 'v52_selected'))
from avm2 import ABC, R
import separator_swf_fix as FIX

fails = 0
def check(c, m):
    global fails
    print(('PASS ' if c else 'FAIL ') + m); fails += 0 if c else 1

orig = (ROOT / 'game/wcc/panel_ingamemenu.redswf').read_bytes()
assert hashlib.sha256(orig).hexdigest() == FIX.ORIGINAL_SHA
release = FIX.patch(orig)

def movies(asset):
    c = asset.index(b'CFX'); block = struct.unpack_from('<I', asset, c - 4)[0]
    z = zlib.decompressobj(); gfx = z.decompress(asset[c + 8:c + block]); assert z.eof
    f = asset.index(b'FWS', c + block); fl = struct.unpack_from('<I', asset, f + 4)[0]
    return {'GFx movie': b'GFX' + asset[c + 3:c + 8] + gfx, 'SWF copy': asset[f:f + fl]}

def abc_of(movie):
    nb = movie[8] >> 3; o = 8 + (5 + 4 * nb + 7) // 8 + 4
    while o + 2 <= len(movie):
        code = struct.unpack_from('<H', movie, o)[0]; o += 2; tag, ln = code >> 6, code & 0x3f
        if ln == 0x3f: ln = struct.unpack_from('<I', movie, o)[0]; o += 4
        if tag == 82:
            b = movie[o:o + ln]; return ABC(b[b.index(b'\0', 4) + 1:])
        o += ln
    raise AssertionError('no ABC')

def v520_movie(movie):
    """previous V5.2.0 file = parts 1+2 only (the release file minus part 3)"""
    k = movie.index(FIX.DRAW_HEADER_NEW + FIX.new_draw())
    return movie[:k] + FIX.DRAW_HEADER + FIX.DRAW_CODE + movie[k + len(FIX.DRAW_HEADER) + len(FIX.DRAW_CODE):]

def code_of(a, cls, name):
    c = [c for c in a.inst if c['name'] == cls][0]
    return a.bodies[[t[2] for t in c['traits'] if len(t) == 3 and t[1] == name][0]]

class Obj(dict):
    pass

SEP, SUBTLE, OPTION = 'SEP', 'SUBTLE', 'OPT'
FRAME = {'n': 0}

class World:
    def __init__(self, a):
        self.a = a
        self.draw = code_of(a, 'W3SubMenuListItemRenderer', 'draw')
        self.init = code_of(a, 'W3SubMenuListItemRenderer', 'initSeparator')
        self.setter = code_of(a, 'W3OptionsSeparator', 'width')
        self.enter_frame = {}          # renderer id -> (listener name, weak)
        self.pending = set()           # renderers with a scheduled validation (ENTER_FRAME / RENDER)
        self.unmeasurable = set()      # separators whose title measures 0 px right now

    # ---- textWidth model: 9 px per character when measurable, else 0 ----
    def text_width(self, label):
        return 0 if label['_sep_id'] in self.unmeasurable or not label['text'] else 9 * len(label['text'])

    # ---- CLIK UIComponent semantics (from the movie: invalidate / validateNow) ----
    def invalidate(self, r, *types):
        r['_hash'].update(types or ('all',))
        if not r['_invalid']:
            r['_invalid'] = True; self.pending.add(id(r)); r['_world_ref'] = r
    def validate_now(self, r):
        self.pending.discard(id(r))
        if not r['_invalid']: return
        self.run(self.draw, r, None)
        r['_hash'] = set(); r['_invalid'] = False

    def frame(self, rows):
        """one frame: ENTER_FRAME listeners (retry), then scheduled validations (RENDER / ENTER_FRAME)."""
        FRAME['n'] += 1
        for r in rows:
            if id(r) in self.enter_frame:
                self.invalidate(r, '[object Event]')   # invalidate(event): the event becomes a hash key
        for r in rows:
            if id(r) in self.pending: self.validate_now(r)

    # ---- minimal AVM2 interpreter for exactly these three methods ----
    def run(self, code, this, arg):
        a = self.a; loc = [this, arg, None, None, None, None]; st = []; r = R(code)
        while True:
            op = r.u8()
            if op == 0xd0: st.append(loc[0])
            elif op == 0xd1: st.append(loc[1])
            elif op == 0xd2: st.append(loc[2])
            elif op == 0xd3: st.append(loc[3])
            elif op == 0x62: st.append(loc[r.u30()])
            elif op == 0xd5: loc[1] = st.pop()
            elif op == 0xd6: loc[2] = st.pop()
            elif op == 0xd7: loc[3] = st.pop()
            elif op == 0x63: loc[r.u30()] = st.pop()
            elif op == 0x30: st.pop()
            elif op == 0x02: pass
            elif op == 0x20: st.append(None)
            elif op == 0x80: r.u30()
            elif op == 0x24: v = r.u8(); st.append(v - 256 if v > 127 else v)
            elif op == 0x25: v = r.u30(); st.append(v - (1 << 32) if v >= 1 << 31 else v)
            elif op == 0x26: st.append(True)
            elif op == 0x27: st.append(False)
            elif op == 0x2a: st.append(st[-1])
            elif op == 0x2b: st[-1], st[-2] = st[-2], st[-1]
            elif op == 0x29: st.pop()
            elif op == 0x76: st.append(bool(st.pop()))
            elif op == 0x73: st.append(int(st.pop()))
            elif op in (0xa0, 0xa1, 0xa3):
                b2 = st.pop(); a2 = st.pop(); st.append(a2 + b2 if op == 0xa0 else a2 - b2 if op == 0xa1 else a2 / b2)
            elif op in (0x5d, 0x60):
                n = a.mname(r.u30())
                if op == 0x60 and n in this: st.append(this[n]); continue
                st.append(this if op == 0x5d else {'IngameMenu': Obj(IGMActionType_Separator=SEP, IGMActionType_SubtleSeparator=SUBTLE),
                                                   'InvalidationType': Obj(DATA='data', SIZE='size'), 'Event': Obj(ENTER_FRAME='enterFrame'), 'TextFieldAutoSize': Obj(NONE='none')}[n])
            elif op == 0x66:
                n = a.mname(r.u30()); o = st.pop()
                if n == 'textWidth': st.append(self.text_width(o))
                elif n == 'invalidate' and o is this: st.append('invalidate')
                else: st.append(o[n])
            elif op == 0x61:
                n = a.mname(r.u30()); v = st.pop(); o = st.pop()
                if o.get('_is_sep') and n == 'width': self.run(self.setter, o, v)
                else: o[n] = bool(v) if n == 'visible' else v
            elif op == 0x46:
                n = a.mname(r.u30()); argc = r.u30(); args = [st.pop() for _ in range(argc)][::-1]; o = st.pop()
                assert n == 'isInvalid', n
                st.append('all' in o['_hash'] or args[0] in o['_hash'])
            elif op == 0x4f:
                n = a.mname(r.u30()); argc = r.u30(); args = [st.pop() for _ in range(argc)][::-1]; o = st.pop()
                if n == 'invalidate': self.invalidate(o)
                elif n == 'validateNow': self.validate_now(o)
                elif n == 'updateText': pass
                elif n == 'addEventListener':
                    assert args[0] == 'enterFrame' and args[1] == 'invalidate'
                    self.enter_frame[id(o)] = ('invalidate', args[4] if len(args) > 4 else False)
                elif n == 'removeEventListener':
                    assert args[0] == 'enterFrame' and args[1] == 'invalidate'
                    self.enter_frame.pop(id(o), None)
                else: raise AssertionError('call ' + n)
            elif op in (0x10, 0x11, 0x12, 0x13, 0x14):
                d = r.s24(); tgt = r.o + d
                if op == 0x10: r.o = tgt
                elif op == 0x11:
                    if bool(st.pop()): r.o = tgt
                elif op == 0x12:
                    if not bool(st.pop()): r.o = tgt
                else:
                    b2 = st.pop(); a2 = st.pop()
                    if (a2 != b2) == (op == 0x14): r.o = tgt
            elif op == 0x47: return
            else: raise AssertionError('opcode %02x not modelled' % op)

    # ---- renderer model (setData / createSubElementByType / initSubtleSeparator per the movie) ----
    def new_row(self, width):
        sid = object()
        sep = Obj(_is_sep=True, visible=False, alpha=1, x=0, y=0, height=0,
                  leftLine=Obj(width=10, x=0, visible=True), rightLine=Obj(width=10, x=0, visible=True))
        sep['label'] = Obj(text='', width=0, x=0, _sep_id=sid, height=0); sep['_id'] = sid
        return Obj(mcSeparator=sep, _type=OPTION, width=width, height=60, data=None, _hash=set(), _invalid=False, alpha=1,
                   autoSize='none', preventAutosizing=True, constraintsDisabled=True)   # BaseListItem constructor
    def set_data(self, row, item):
        row['data'] = Obj(label=item[1]); row['_type'] = item[0]
        row['mcSeparator']['visible'] = False                         # createSubElementByType
        if item[0] == SEP:
            self.run(self.init, row, None)
        elif item[0] == SUBTLE:                                        # initSubtleSeparator (original game code path)
            sep = row['mcSeparator']; sep['label']['text'] = ''; sep['x'] = -150 + 300; sep['y'] = 34
            self.run(self.setter, sep, row['width'] - 600); sep['visible'] = True; sep['alpha'] = 0.05
            row['_subtle_w'] = row['width'] - 600                     # game behaviour: laid out once, untouched by part 3
        self.invalidate(row); self.validate_now(row)                   # end of setData
    def set_width(self, row, w):                                       # ScrollingList.drawLayout -> setSize
        if row['width'] != w:
            row['width'] = w; self.invalidate(row, 'size')

def expected(row, w_text):
    gap = int(row['width'] - w_text)
    return (gap / 2 - 5, 0, gap / 2 - 15, gap / 2 - 5 + w_text + 15)

def heading_ok(world, row):
    sep = row['mcSeparator']; lab = sep['label']
    if not (sep['visible'] and sep['alpha'] == 1): return False
    tw = 9 * len(lab['text'])
    lw, lx, rw, rx = expected(row, tw)
    return (sep['rightLine']['visible'] is True and sep['leftLine']['visible'] is True and sep['leftLine']['width'] == lw
            and sep['leftLine']['x'] == lx and sep['rightLine']['width'] == rw and sep['rightLine']['x'] == rx and rw > 0)

HEAD = [' CPU\xa0', ' Path Tracing\xa0', ' Ray Tracing\xa0', ' RTGI: Quality\xa0', ' RTGI: Performance\xa0', ' RT/PT: Shared\xa0',
        ' Textures / Rendering\xa0', ' Memory & Streaming\xa0', ' RT Memory Management\xa0', ' Frame Generation\xa0']

def settle(world, rows, n=6):
    for _ in range(n): world.frame(rows)

def scenario_resize(world):
    rows = [world.new_row(400) for _ in range(3)]                  # rows created at the symbol width
    for r, t in zip(rows, HEAD): world.set_data(r, (SEP, t))
    for r in rows: world.set_width(r, 1167)                         # the list lays the rows out afterwards
    settle(world, rows)
    return all(heading_ok(world, r) for r in rows)

def scenario_unmeasurable(world):
    rows = [world.new_row(1167) for _ in range(3)]
    for r in rows: world.unmeasurable.add(r['mcSeparator']['_id'])
    for r, t in zip(rows, HEAD[3:]): world.set_data(r, (SEP, t))
    settle(world, rows, 3)
    world.unmeasurable.clear()                                      # font / list ready a few frames later
    settle(world, rows)
    return all(heading_ok(world, r) for r in rows), not world.enter_frame

def scenario_random(world, seed=520):
    rnd = random.Random(seed); bad = 0; leaks = 0; checks = 0
    page = [(OPTION, 'opt%d' % i) for i in range(60)]
    for k, t in enumerate(HEAD): page.insert(k * 6, (SEP, t))
    other_page = [(SUBTLE, '')] + [(OPTION, 'x%d' % i) for i in range(12)] + [(SUBTLE, '')]
    rows = [world.new_row(1167) for _ in range(12)]; pos = 0; data = page; width = 1167
    for f in range(5000):
        ev = rnd.random()
        if ev < 0.35:                                               # scroll one or more steps
            pos = max(0, min(len(data) - len(rows), pos + rnd.choice((-3, -1, 1, 1, 3))))
            for i, r in enumerate(rows): world.set_data(r, data[pos + i] if pos + i < len(data) else (OPTION, ''))
        elif ev < 0.40:                                             # close and reopen the menu: new rows
            width = rnd.choice((1167, 1167, 1100, 1210)); rows = [world.new_row(rnd.choice((400, 1167))) for _ in range(12)]
            data = rnd.choice((page, page, other_page)); pos = 0
            if rnd.random() < 0.5:
                for r in rows: world.unmeasurable.add(r['mcSeparator']['_id'])
            for i, r in enumerate(rows): world.set_data(r, data[i] if i < len(data) else (OPTION, ''))
            for r in rows: world.set_width(r, width)
        elif ev < 0.45:                                             # list width change (scroll bar / menu context)
            width = rnd.choice((1167, 1100, 1210))
            for r in rows: world.set_width(r, width)
        elif ev < 0.55:                                             # quick preset / master: values refreshed, rows redrawn
            for r in rows: world.invalidate(r)
        elif ev < 0.60:
            world.unmeasurable.clear()
        world.frame(rows)
        if f % 10 == 9:                                             # settled check
            world.unmeasurable.clear(); settle(world, rows)
            for r in rows:
                if r['_type'] == SEP:
                    checks += 1; bad += 0 if heading_ok(world, r) else 1
                elif r['_type'] == SUBTLE:
                    sep = r['mcSeparator']
                    bad += 0 if (sep['rightLine']['visible'] is False and sep['leftLine']['width'] == r['_subtle_w'] and sep['alpha'] == 0.05) else 1
            leaks += sum(1 for r in rows if id(r) in world.enter_frame)   # discarded rows: weak listener, collected
    return bad, leaks, checks

for asset_name, asset in (('release', release),):
    for mname, movie in movies(asset).items():
        new = abc_of(movie); old = abc_of(v520_movie(movie))
        check(not scenario_resize(World(old)), '%s: previous V5.2.0 code reproduces lines laid out for the wrong row width' % mname)
        check(scenario_resize(World(new)), '%s: rows resized by the list after setData: both lines at the new width' % mname)
        ok_old, _ = scenario_unmeasurable(World(old))
        check(not ok_old, '%s: previous V5.2.0 code reproduces a heading left with one line (title unmeasurable when filled)' % mname)
        ok_new, no_leak = scenario_unmeasurable(World(new))
        check(ok_new and no_leak, '%s: unmeasurable title: retried each frame, both lines once measurable, retry listener removed' % mname)
        w = World(new); w.run(w.draw, w.new_row(1167), None)
        bad, leaks, checks = scenario_random(World(new))
        check(bad == 0 and leaks == 0 and checks > 500, '%s: 5,000 random frames (scroll, recycle, reopen, width changes, refreshes, subtle separators): %d settled heading checks, %d wrong, %d listeners left' % (mname, checks, bad, leaks))
        bad2, _, _ = scenario_random(World(old), 7)
        check(bad2 > 0, '%s: same randomised lifecycle breaks lines with the previous V5.2.0 code (%d wrong)' % (mname, bad2))
        # weak retry listener
        w = World(new); r = w.new_row(1167); w.unmeasurable.add(r['mcSeparator']['_id']); w.set_data(r, (SEP, HEAD[0]))
        check(w.enter_frame.get(id(r)) == ('invalidate', True), '%s: retry uses a weak ENTER_FRAME listener on the row itself' % mname)
# static verification of the new draw body (AVM2 verifier rules that matter here): consistent stack depth at every
# branch target, never negative, maximum == the declared max_stack, ends in returnvoid, same length as the original
EFF = {0xd0: (0, 1), 0x30: (1, 0), 0x5d: (0, 1), 0x60: (0, 1), 0x66: (1, 1), 0x61: (2, 0), 0x12: (1, 0), 0x11: (1, 0), 0x14: (2, 0),
       0x27: (0, 1), 0x26: (0, 1), 0x24: (0, 1), 0x02: (0, 0), 0x47: (0, 0)}
def verify_stack(code, max_stack):
    from avm2 import R as RR
    depth = {0: 0}; work = [0]; peak = 0
    while work:
        pc = work.pop(); h = depth[pc]; r = RR(code, pc); op = r.u8(); nxt = []
        if op in (0x46, 0x4f):
            r.u30(); argc = r.u30(); h -= argc + 1; h += 1 if op == 0x46 else 0
        else:
            pop, push = EFF[op]
            if op in (0x5d, 0x60, 0x66, 0x61): r.u30()
            if op == 0x24: r.u8()
            if op in (0x11, 0x12, 0x14): d = r.s24(); nxt.append(r.o + d)
            assert h - pop >= 0, ('underflow', pc); h = h - pop + push
        peak = max(peak, h)
        if op != 0x47: nxt.append(r.o)
        for t in nxt:
            if t in depth: assert depth[t] == h, ('inconsistent stack at', t)
            else: depth[t] = h; work.append(t)
    return peak
peak = verify_stack(FIX.new_draw(), FIX.DRAW_HEADER_NEW[2])
check(peak == FIX.DRAW_HEADER_NEW[2] == 6 and len(FIX.new_draw()) == len(FIX.DRAW_CODE) and FIX.new_draw()[-1] == 0x47,
      'new draw body: consistent stack depth at every branch target, peak %d = declared max_stack, same length, ends in returnvoid' % peak)
print('TOTAL %s %d' % ('FAIL' if fails else 'PASS', fails)); sys.exit(1 if fails else 0)
