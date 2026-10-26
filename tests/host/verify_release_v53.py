#!/usr/bin/env python3
"""V5.3 dev: static verification of the built PRIVATE archives against the protected V5.2.0 release archives.
Usage: verify_release_v53.py <out dir with *_MAIN.zip / *_ASI_ONLY.zip> <dir with the V5.2.0 MAIN and ASI_ONLY zips>
Checks: CRC; MAIN = ASI-ONLY + loader + licence; same entry set as V5.2.0; loader, licence and menu movie (separator
repair) byte-identical to V5.2.0; no W3LoadFix or reference-mod file shipped; blob = receipt-pinned; menu = every
V5.2.0 row and Quick Preset unchanged + only the 9 V5.3 rows and 2 named separators; every V5.2.0 string (ID, key,
text) preserved in all 18 languages except the DEV2 tooltip/label keys (each equal to its generated DEV2 text), plus
exactly the 52 V5.3 keys and the 2 DEV2 label keys; the menu may also differ by the documented option display-name
swaps (spec.OPTION_LABEL_ONLY); ASI version 5.2.0 (native sources unchanged since DEV2); exports unchanged and imports = V5.2.0 +
GetPrivateProfileSectionW only."""
import hashlib, io, struct, sys, zipfile
from pathlib import Path
import xml.etree.ElementTree as ET
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools')); sys.path.insert(0, str(ROOT / 'tools' / 'v53_dev')); sys.path.insert(0, str(ROOT / 'tools' / 'v52_selected'))
import rc1_w3codec as C
import locale_v53 as L
import spec as S
import tooltips_dev2 as T
DEV1_ASI = '38745faa04b0c35926bbc5a957d29d54cb15b10967b544cd729401a72e98bfe3'
out, old = Path(sys.argv[1]), Path(sys.argv[2])
sha = lambda b: hashlib.sha256(b).hexdigest()
fails = []
def check(c, m):
    print(('  ok   ' if c else '  FAIL ') + m)
    if not c: fails.append(m)
main = zipfile.ZipFile(next(out.glob('*_MAIN.zip'))); asi = zipfile.ZipFile(next(out.glob('*_ASI_ONLY.zip')))
omain = zipfile.ZipFile(next(old.glob('*V5.2.0_MAIN.zip'))); oasi = zipfile.ZipFile(next(old.glob('*V5.2.0_ASI_ONLY.zip')))
check(main.testzip() is None and asi.testzip() is None, 'archives pass CRC test')
mn, an = set(main.namelist()), set(asi.namelist())
check(mn - an == {'bin/x64_dx12/winmm.dll', 'LICENSES/Ultimate-ASI-Loader-LICENSE.txt'} and all(main.read(n) == asi.read(n) for n in an), 'MAIN = ASI-ONLY + loader + licence; shared files identical')
check(mn == set(omain.namelist()) and an == set(oasi.namelist()), 'same file set as V5.2.0 MAIN / ASI-ONLY (no new or removed files)')
low = ' '.join(mn).lower()
check(not any(x in low for x in ('dsound', 'w3loadfix', 'xinput', 'd3d12', 'vram_relief', 'memory_stabilizer', 'rtx_fix', 'dinput8', 'minhook', 'libdeflate')), 'no W3LoadFix, reference-mod or second-loader file shipped')
for n in ('bin/x64_dx12/winmm.dll', 'LICENSES/Ultimate-ASI-Loader-LICENSE.txt'):
    check(main.read(n) == omain.read(n), 'byte-identical to V5.2.0: ' + n.split('/')[-1])
import separator_swf_fix as FIX
SWF = 'mods/modRefinedTracing/content/gameplay/gui_new/swf/mainmenu/panel_ingamemenu.redswf'
check(main.read(SWF) == FIX.patch((ROOT / 'game/wcc/panel_ingamemenu.redswf').read_bytes()) and len(main.read(SWF)) == len(omain.read(SWF)),
      'menu movie = original game asset + separator fix parts 1-3 (same size as the earlier V5.2.0 file)')
blob = main.read('mods/modRefinedTracing/content/precompiled.rsblob')
check(sha(blob) == (ROOT / 'game/wcc/precompiled.rsblob.sha256').read_text().split()[0] and blob != omain.read('mods/modRefinedTracing/content/precompiled.rsblob'),
      'precompiled.rsblob = receipt-pinned V5.3 blob (not the V5.2.0 one)')
# ---- menu ----
can = lambda e: ET.canonicalize(ET.tostring(e, encoding='unicode'), strip_text=True)
nx = ET.fromstring(main.read('bin/config/r4game/user_config_matrix/pc/RefinedTracing.xml')); nswap = S.apply_option_labels(nx, reverse=True); ox = ET.fromstring(omain.read('bin/config/r4game/user_config_matrix/pc/RefinedTracing.xml'))
v53 = {r['id'] for r in S.ROWS_V53}; seps = {'URT53SEP_MEM', 'URT53SEP_RTMEM'}
ok = True
for gid in ('RefinedTracingCR01', 'URTExtended'):
    o = [can(v) for v in ox.find("./Group[@id='%s']/VisibleVars" % gid)]
    nv = list(nx.find("./Group[@id='%s']/VisibleVars" % gid))
    ok &= o == [can(v) for v in nv if v.get('id') not in v53 | seps]
    ok &= {v.get('id') for v in nv} - {v.get('id') for v in ox.find("./Group[@id='%s']/VisibleVars" % gid)} == (v53 | seps if gid == 'RefinedTracingCR01' else v53)
check(ok and nswap == 2 * len(S.OPTION_LABEL_ONLY), 'menu: apart from %d documented option display-name swaps, every V5.2.0 row,' % nswap + ' option and Quick Preset entry unchanged and in order; only the 9 V5.3 rows (+2 named separators) added')
# ---- strings ----
def h(s):
    v = 0
    for c in s.lower():
        raw = c.encode('utf-16le')
        for i in range(0, len(raw), 2): v = (31 * v + int.from_bytes(raw[i:i + 2], 'little')) & 0xffffffff
    return v
REC = L.records(); NEWH = {h(k) for k in L.new_keys()} | {h(k) for k in T.NEW_LABELS}
OLD = {}
for lang in L.LANGS:
    o = C.decode(omain.read('mods/modRefinedTracing/content/%s.w3strings' % lang), lang)
    OLD[lang] = (dict(o.block2), {b[0]: b[3] for b in o.blocks})
def loc(l, k):  # pre-DEV2 text: V5.2.0 strings + V5.3 DEV1 records
    return REC[l][k] if k in REC[l] else OLD[l][1][OLD[l][0][h(k)]]
TIPS, LABS = T.tooltips(loc), T.labels(loc)
langs_ok = 0; nchg = 0
for lang in L.LANGS:
    n = C.decode(main.read('mods/modRefinedTracing/content/%s.w3strings' % lang), lang)
    o = C.decode(omain.read('mods/modRefinedTracing/content/%s.w3strings' % lang), lang)
    nt = {b[0]: b[3] for b in n.blocks}; ot = {b[0]: b[3] for b in o.blocks}
    nk = dict(n.block2); okk = dict(o.block2)
    exp = {**TIPS[lang], **LABS[lang], **T.NEW_LABELS}
    chg = {nk[h(k)]: t for k, t in exp.items()}
    same = all(nk.get(k) == i for k, i in okk.items()) and all(nt.get(i) == t for i, t in ot.items() if i not in chg)
    dev2 = all(nt[i] == t for i, t in chg.items())
    added = set(nk) - set(okk) == NEWH and all(nt[nk[h(k)]] == REC[lang][k] for k in L.new_keys() if nk[h(k)] not in chg)
    nchg = len(chg)
    if same and dev2 and added: langs_ok += 1
    else: print('   ', lang, same, dev2, added)
check(langs_ok == 18, '18 languages: every V5.2.0 ID/key/text preserved except the %d DEV2 tooltip/label texts (each = generated DEV2 text); exactly the 52 V5.3 + 2 DEV2 keys added' % nchg)
# ---- ASI imports / exports ----
def pe_dirs(b):
    pe = struct.unpack_from('<I', b, 0x3c)[0]; n = struct.unpack_from('<H', b, pe + 6)[0]; osz = struct.unpack_from('<H', b, pe + 20)[0]
    secs = []; o = pe + 24 + osz
    for i in range(n):
        vs, va, rs, ro = struct.unpack_from('<IIII', b, o + 8); secs.append((va, max(vs, rs), ro)); o += 40
    def off(r):
        for va, sz, ro in secs:
            if va <= r < va + sz: return ro + r - va
    def cstr(r):
        o = off(r); return b[o:b.index(b'\0', o)].decode()
    exp_rva = struct.unpack_from('<I', b, pe + 24 + 112)[0]; imp_rva = struct.unpack_from('<I', b, pe + 24 + 120)[0]
    exps = set()
    if exp_rva:
        e = off(exp_rva); nn, fn_names = struct.unpack_from('<I', b, e + 24)[0], struct.unpack_from('<I', b, e + 32)[0]
        exps = {cstr(struct.unpack_from('<I', b, off(fn_names) + 4 * i)[0]) for i in range(nn)}
    imps = set(); d = off(imp_rva)
    while True:
        ilt, _, _, name, iat = struct.unpack_from('<IIIII', b, d)
        if not name: break
        dll = cstr(name).lower(); t = off(ilt or iat)
        while True:
            q = struct.unpack_from('<Q', b, t)[0]
            if not q: break
            imps.add(dll + '!' + (cstr((q & 0x7fffffff) + 2) if not q >> 63 else '#%d' % (q & 0xffff))); t += 8
        d += 20
    return exps, imps
_asi = main.read('bin/x64_dx12/UltimateRefinedTracing.asi')
check('Ultimate Refined Tracing 5.2.0 '.encode() in _asi and b'5.3.0' not in _asi and b'DEV' not in _asi.upper().replace(b'DEVICE', b''),
      'ASI identifies itself as 5.2.0 (no 5.3.0 / DEV identifier); native sources = DEV2 (only the version define differs)')
ne, ni = pe_dirs(main.read('bin/x64_dx12/UltimateRefinedTracing.asi')); oe, oi = pe_dirs(omain.read('bin/x64_dx12/UltimateRefinedTracing.asi'))
check(ne == oe, 'ASI exports unchanged (%d)' % len(ne))
check(ni - oi == {'kernel32.dll!GetPrivateProfileSectionW'} and not (oi - ni), 'ASI imports = V5.2.0 + GetPrivateProfileSectionW (read-only w3loadfix.ini [Override]) only')
check(not any(x in ni for x in ('kernel32.dll!WritePrivateProfileSectionW',)), 'no new INI writer')
print('PASS verify_release_v53' if not fails else 'FAIL verify_release_v53: %d' % len(fails))
sys.exit(1 if fails else 0)
