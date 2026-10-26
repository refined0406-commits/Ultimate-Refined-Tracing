#!/usr/bin/env python3
"""V5.3 DEV2 menu polish / tooltip clarity checks on the BUILT menu and strings (build/menu, build/loc).
 1 every Var and Option displayName resolves in all 18 languages (no unresolved key such as "Urtx n 4");
 2 no identifier leaks into displayed text (key fragments, underscores, '##', "Urtx n");
 3 option IDs, Entry values and Quick Preset entries identical to the WCC compile-time menu (only documented
   display-name swaps differ);
 4 every adjustable control the compiled script describes (onHover) has a DEV2 tooltip in 18 languages, translated;
 5 Native / Auto values stated in tooltips match the engine defaults of the generated table / evidence;
 6 tooltips state a Native / Auto value and when the change applies; no unqualified FPS promise;
 7 tooltip width within the per-language bound (DEV1 rendered maximum, or the English DEV1 maximum, CJK = 2 units);
 8 the DEV2 string pass is idempotent (rebuilding from the built strings changes nothing)."""
import re, struct, sys, unicodedata
import xml.etree.ElementTree as ET
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
for p in ('tools', 'tools/v53_dev', 'tools/v52_selected'):
    sys.path.insert(0, str(ROOT / p))
import rc1_w3codec as C
import tooltips_dev2 as T
import tip_phrases as TP
import spec as S

def h(s):
    v = 0
    for c in s.lower():
        raw = c.encode('utf-16le')
        for i in range(0, len(raw), 2):
            v = (31 * v + int.from_bytes(raw[i:i + 2], 'little')) & 0xffffffff
    return v

fails = []
def check(c, m):
    print(('PASS ' if c else 'FAIL ') + m)
    if not c:
        fails.append(m)

D = {}
for l in T.LANGS:
    d = C.decode((ROOT / 'build/loc' / f'{l}.w3strings').read_bytes(), l)
    D[l] = (dict(d.block2), {b[0]: b[3] for b in d.blocks})
def get(l, k):
    ids, txt = D[l]
    i = ids.get(h(k))
    return None if i is None else txt.get(i)

menu = ET.parse(ROOT / 'build/menu/RefinedTracing.xml').getroot()
keys = {}
for v in menu.iter('Var'):
    keys[v.get('displayName')] = v.get('id')
    for o in v.iter('Option'):
        keys[o.get('displayName')] = v.get('id')
# 1
miss = [(l, k) for k in keys for l in T.LANGS if not (get(l, k) or '').strip()]
check(not miss, '1 all %d menu display names resolve in 18 languages %s' % (len(keys), miss[:5]))
# 2
leak = re.compile(r'(urtx|urt5\d|preset_value|cr01_|refinedtracing_|##|\b[a-z0-9]+_[a-z0-9_]+\b|^urtx n)', re.I)
bad = [(l, k, get(l, k)) for k in keys for l in T.LANGS if get(l, k) and leak.search(get(l, k))]
check(not bad, '2 no identifier leaks in displayed labels %s' % bad[:5])
# 3
can = lambda e: ET.canonicalize(ET.tostring(e, encoding='unicode'), strip_text=True)
def structure(root):
    out = []
    for v in root.iter('Var'):
        out.append((v.get('id'), v.get('displayType'), v.get('value'),
                    tuple((o.get('id'), tuple(can(e) for e in o.iter('Entry'))) for o in v.iter('Option'))))
    return out
old = ET.parse(ROOT / 'game/wcc/V52_WCC_MENU_AT_COMPILE.xml').getroot()
held = set(S.NATIVE_MENU_HOLD); retired = set(S.RETIRED_AFTER_COMPILE) | set(getattr(S, 'RETIRED_HEADINGS', ()))
so = [r for r in structure(old) if r[0] not in retired]
sn = [r for r in structure(menu)]
check([r for r in so if r[0] not in held] == [r for r in sn if r[0] not in held],
      '3 option IDs, Entry values and Quick Preset entries identical to the WCC compile-time menu (%d vars)' % len(sn))
# 4
ws = (ROOT / 'game/wcc/RefinedTracingCR01_RTGI3.ws').read_text('utf-8-sig')
hover = dict(re.findall(r"this\.onHover\('([^']+)',\s*\"\",\s*\"([^\"]+)\"\)", ws))
visible = [v.get('id') for v in menu.find("./Group[@id='RefinedTracingCR01']/VisibleVars").iter('Var')
           if v.get('displayType') != 'SEPARATOR' and v.get('id') not in ('QuickPresetSelect', 'RayTracedOptimizations')]
nospec = [c for c in visible if c not in T.SPEC]
wrong = [c for c in visible if c in T.SPEC and hover.get(c) != T.SPEC[c][0]]
check(not nospec and not wrong, '4a every adjustable visible control (%d) has a DEV2 tooltip under its compiled onHover key %s %s' % (len(visible), nospec, wrong))
untr = [(l, k) for c, s in T.SPEC.items() for k in [s[0]] for l in T.LANGS if l != 'en' and get(l, k) == get('en', k)]
empty = [(l, s[0]) for s in T.SPEC.values() for l in T.LANGS if not (get(l, s[0]) or '').strip()]
check(not untr and not empty, '4b tooltips present and translated in 18 languages %s %s' % (empty[:3], untr[:3]))
# 5
TAB = (ROOT / 'src/urt_gfx_table.c').read_text()
def table_default(cid):
    m = re.search(r'\{"%s", "[^"]+", "[^"]+", 0x([0-9a-f]+)u, 0x([0-9a-f]+)u, 0x[0-9a-f]+u, URT_GX_(FLOAT|INT)' % cid, TAB)
    raw = int(m.group(2), 16)
    return struct.unpack('<f', struct.pack('<I', raw))[0] if m.group(3) == 'FLOAT' else raw
def lit_num(s):
    return float(re.match(r'[\d.]+', s).group(0))
mm = []
for cid, ev in T.EVIDENCE.items():
    td = table_default(cid)
    if abs(td - ev) > 1e-6:
        mm.append((cid, 'table', td, ev))
    nat = T.SPEC[cid][2]
    if nat and nat[0] == 'lit':
        shown = lit_num(nat[1])
        # Texture Unloading Speed is shown as a multiplier, PT No Blue Glow as Off (engine default mip 4 = feature off)
        if cid not in ('URTX_PTNoBlueGlow',) and abs(shown - ev) > 1e-6:
            mm.append((cid, 'shown', shown, ev))
check(not mm, '5 stated Native / Auto values match the registered engine defaults (%d rows) %s' % (len(T.EVIDENCE), mm))
# 6
nat_word = {l: get(l, 'preset_value_refinedtracing_native_auto') for l in T.LANGS}
no_nat = [(l, c) for c, s in T.SPEC.items() if s[2] for l in T.LANGS if nat_word[l] not in get(l, s[0])]
no_time = [(l, c) for c, s in T.SPEC.items() if s[5] for l in T.LANGS if TP.P(s[5], l) not in get(l, s[0])]
promise = re.compile(r'(?<!may )(?<!can )(?<!not )\b(raises|increases|boosts|improves|will raise|will increase|gains?)\b[^.]*\bFPS', re.I)
prom = [c for c, s in T.SPEC.items() if promise.search(get('en', s[0]))]
check(not no_nat and not no_time, '6a every tooltip states Native / Auto and when it applies %s %s' % (no_nat[:3], no_time[:3]))
check(not prom, '6b no unqualified FPS promise (English) %s' % prom)
# 7
W = lambda s: sum(2 if unicodedata.east_asian_width(c) in 'WF' else 1 for c in s)
BOUND = {'en': 386, 'ar': 317, 'br': 408, 'cn': 130 * 2, 'cz': 359, 'de': 418, 'es': 411, 'esmx': 404, 'fr': 440, 'hu': 410,
         'it': 412, 'jp': 386, 'kr': 386, 'pl': 392, 'ru': 401, 'tr': 390, 'ua': 413, 'zh': 386}  # max(DEV1 width, en DEV1)
over = [(l, s[0], W(get(l, s[0]))) for s in T.SPEC.values() for l in T.LANGS if W(get(l, s[0])) > max(BOUND[l], 386)]
check(not over, '7 every tooltip within its tooltip-area bound %s' % over[:5])
# 8
def loc(l, k):
    return get(l, k)
tt, lb = T.tooltips(loc), T.labels(loc)
drift = [(l, k) for l in T.LANGS for k, t in {**tt[l], **lb[l], **T.NEW_LABELS}.items() if get(l, k) != t]
check(not drift, '8 DEV2 string pass idempotent on the built strings %s' % drift[:3])
# 9 V5.2.0 release: PT: Skin Bounce Light Desaturation uses the owner's example (Geralt's skin too orange near firelight)
GERALT = {'en': 'Geralt', 'de': 'Geralt', 'fr': 'Geralt', 'es': 'Geralt', 'esmx': 'Geralt', 'it': 'Geralt', 'pl': 'Geralt', 'ru': 'Геральт',
          'ua': 'Геральт', 'cz': 'Geralt', 'hu': 'Geralt', 'tr': 'Geralt', 'br': 'Geralt', 'jp': 'ゲラルト', 'kr': '게롤트', 'cn': '杰洛特',
          'zh': '傑洛特', 'ar': 'غيرالت'}
FOREST = ('forest', 'Wald', 'forêt', 'bosques', 'boschi', 'lesie', 'лесу', 'лісі', 'lese', 'erdő', 'orman', 'florestas', '森', '숲', '森林', 'الغابات')
skin = {l: get(l, 'tooltip_urtx_ptskinbouncedesat') for l in T.LANGS}
check(all(GERALT[l] in skin[l] for l in T.LANGS) and not any(f in skin[l] for l in T.LANGS for f in FOREST) and 'too orange near firelight' in skin['en'],
      '9 Skin Bounce Light Desaturation: firelight example (Geralt) in 18 languages, forest example gone')
print('PASS test_v53_dev2_strings' if not fails else 'FAIL test_v53_dev2_strings: %d' % len(fails))
sys.exit(1 if fails else 0)
