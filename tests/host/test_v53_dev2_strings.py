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
# 1b V5.3.0 rev2: the game resolves a Var title as "option_" + displayName (rev1 missed this and showed raw ids)
vt = [v.get('displayName') for v in menu.find("./Group[@id='RefinedTracingCR01']/VisibleVars").iter('Var') if v.get('displayType') != 'SEPARATOR']
miss_t = [(l, k) for k in vt for l in T.LANGS if not (get(l, 'option_' + k) or '').strip()]
bad_t = [(l, k) for k in vt for l in T.LANGS if (get(l, 'option_' + k) or '') and leak.search(get(l, 'option_' + k))] if 'leak' in dir() else []
check(not miss_t, '1b every row title resolves as option_<displayName> in 18 languages (%d rows) %s' % (len(vt), miss_t[:5]))
NAMES = {'urtx_ptradiancecachedensity': 'PT: Radiance Cache Density', 'urtx_rtasyncao': 'RT: Async AO',
         'urtx_rtasyncgishading': 'RT: Async GI Shading', 'urtx_rttreewinddistance': 'RT: Tree Wind Distance', 'urtx_ptmoonlight': 'PT: Moon Light',
         'urtx_ptdiffusecoverage': 'PT: First Bounce Diffuse Coverage', 'urtx_expptsunshadowcoverage': 'PT: Bounce Sun Shadow Coverage', 'urtx_expptlocalshadowcoverage': 'PT: Bounce Local Light Shadow Coverage'}   # R12 FIX3: Second Bounce Coverage retired
check(all(get(l, 'option_' + k) == n for k, n in NAMES.items() for l in T.LANGS), "1c the owner's V5.3.0 titles exactly (four renamed rows + PT: Moon Light), all 18 languages, via the game's option_ lookup")
_pt = [v.get('id') for v in menu.find("./Group[@id='RefinedTracingCR01']/VisibleVars")]
_mv = menu.find(".//Group[@id='RefinedTracingCR01']/VisibleVars/Var[@id='URTX_PTMoonLight']")
check(get('en','option_urt531_sep_exp') == ' EXPERIMENTAL\xa0' and all(get(l,'option_urt531_sep_exp') == ' EXPERIMENTAL\xa0' for l in T.LANGS), '1e EXPERIMENTAL heading resolves through the option_ lookup in 18 languages')
_vals = lambda vid: [get('en', o.get('displayName')) for o in menu.find(".//Group[@id='RefinedTracingCR01']/VisibleVars/Var[@id='%s']" % vid).iter('Option')]
_nat = get('en', 'preset_value_refinedtracing_native_auto')
check(_vals('URTX_PTDiffuseCoverage') == [_nat, '50%'] and _vals('URTX_ExpPTSunShadowCoverage') == [_nat, '50%'] and _pt.index('URTX_ExpPTSunShadowCoverage') == _pt.index('URTX_PTDiffuseCoverage') + 1 == _pt.index('GeometryBounces') + 2 == _pt.index('URTX_ExpPTLocalShadowCoverage') - 1 and _vals('URTX_ExpPTLocalShadowCoverage') == [_nat, '50%'],
      '1f R12 FIX3 PT coverage rows: Geometry Bounces, First Bounce Diffuse Coverage, Bounce Sun Shadow Coverage; Native / Auto first, then 50%')
check(_pt.index('URTX_PTMoonLight') == _pt.index('IndirectLight') + 1 and _mv.get('displayName') == 'urtx_ptmoonlight'
      and _mv.get('displayType') == 'OPTIONS' and _mv.get('tags') == 'customNames' and _mv.get('overrideGroup') == 'URTExtended'
      and [get('en', o.get('displayName')) for o in _mv.iter('Option')] == [get('en', 'preset_value_refinedtracing_native_auto'), '0.25', '0.50', '0.75', '1.50', '2.00', '3.00'],
      '1d PT: Moon Light directly beneath PT: Indirect-Light Compensation; same row type, tags and value labels as the other PT option rows')
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
    m = re.search(r'\{"%s", "[^"]+", "[^"]+", 0x([0-9a-f]+)u, 0x([0-9a-f]+)u, 0x[0-9a-f]+u, URT_GX_(FLOAT|INT|BOOL)' % cid, TAB)
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
# 10 V5.3.0 rev2: no "Experimental" wording in the V5.3.0 descriptions; async rows keep the hardware caveat and the
#    next-start timing; the Master tooltip carries the restart sentence and no unqualified "at once"
EXP = ('Experiment', 'xpérim', 'Sperim', 'Eksperym', 'Экспер', 'Експер', 'Kísérl', 'Deneysel', '実験', '실험', '实验', '實驗', 'تجريبي')
V530 = ['tooltip_urtx_ptradiancecachedensity', 'tooltip_urtx_rtasyncao', 'tooltip_urtx_rtasyncgishading', 'tooltip_urtx_rttreewinddistance',
        'tooltip_urtx_rtasyncgiclassify', 'tooltip_urtx_rtasfasttrace', 'tooltip_urtx_ptmoonlight']
check(not [(l, k) for k in V530 for l in T.LANGS if any(e in get(l, k) for e in EXP)], '10a no Experimental wording in the V5.3.0 descriptions (18 languages)')
check(all('faster or slower' in get('en', k) and 'GPU' in get('en', k) and TP.P('T_NEXTSTART', 'en') in get('en', k)
          for k in ('tooltip_urtx_rtasyncao', 'tooltip_urtx_rtasyncgishading', 'tooltip_urtx_rtasyncgiclassify')),
      '10b async descriptions: can be faster or slower depending on the GPU; applies at the next game start')
mt = get('en', 'tooltip_cr01_masterenabled')
check("Some settings require a game restart to take effect. Check each setting's description for details." in mt and 'at once for most settings' in mt
      and all(get(l, 'tooltip_cr01_masterenabled') != mt for l in T.LANGS if l != 'en'), '10c Master tooltip: restart sentence, qualified immediacy, translated')
# 11 R5: async AS compaction removed from the menu and from every language (title, description)
ck = [(l, k) for l in T.LANGS for k in ('option_urtx_rtasasynccompact', 'urtx_rtasasynccompact', 'tooltip_urtx_rtasasynccompact') if get(l, k) is not None]
check(not ck and 'URTX_RTASAsyncCompact' not in ET.tostring(menu, encoding='unicode'), '11 async compaction gone from the menu and all 18 languages %s' % ck[:3])
print('PASS test_v53_dev2_strings' if not fails else 'FAIL test_v53_dev2_strings: %d' % len(fails))
sys.exit(1 if fails else 0)
