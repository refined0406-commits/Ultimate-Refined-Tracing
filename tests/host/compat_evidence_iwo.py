#!/usr/bin/env python3
"""Offline static evidence: URT V5 vs Beardmo's Immersive World (IWO) 2.2.3.

Usage: compat_evidence_iwo.py <witcher3.exe> <IWO bin\\x64_dx12 dir>

Read-only. Establishes that IWO and URT share no PT shader library, no URT
code site and no URT-owned engine setting. Runtime/visual behaviour (wind, RT tree shadows, grass, water,
physics) is a gameplay-acceptance item and is NOT claimed here.
"""
import re, struct, sys
from pathlib import Path
exe = Path(sys.argv[1]).read_bytes()
iwo = Path(sys.argv[2])
fails = []
def check(c, m):
    print(('  ok   ' if c else '  FAIL ') + m)
    if not c: fails.append(m)

def containers(d):
    i = 0
    while True:
        i = d.find(b'DXBC', i)
        if i < 0: return
        if i + 32 <= len(d):
            sz, n = struct.unpack_from('<II', d, i + 24)
            if 32 < sz <= len(d) - i and n < 32:
                yield d[i:i + sz]; i += sz; continue
        i += 4
def parts(c):
    n = struct.unpack_from('<I', c, 28)[0]
    out = {}
    for o in struct.unpack_from('<%dI' % n, c, 32):
        out[c[o:o + 4]] = c[o + 8:o + 8 + struct.unpack_from('<I', c, o + 4)[0]]
    return out

asi = (iwo / 'BeardmosImmersiveWorld.asi').read_bytes()
add = (iwo / 'BeardmosImmersiveWorld.addon64').read_bytes()
cs = list(containers(add))
kinds = {}
for c in cs:
    p = parts(c)
    k = struct.unpack_from('<I', p[b'DXIL'], 0)[0] >> 16 if b'DXIL' in p else -1
    kinds[k] = kinds.get(k, 0) + 1
check(len(cs) > 0 and not any(b'RDAT' in parts(c) for c in cs),
      'IWO add-on: %d shaders, none is a DXR library (kinds %s: 0 PS, 1 VS, 4 DS, 5 CS)' % (len(cs), dict(sorted(kinds.items()))))
check(all(b'PTRGSFinal' not in c for c in cs) and b'PTRGSFinal' not in asi,
      'IWO never references the PT Final library URT edits')
check(not list(containers(asi)), 'IWO ASI contains no shaders')
# URT code sites (RVAs) and a 16-byte window of original code at each
SITES = {'CameraCB serializer call': 0x1EF0FCF, 'geometry bounce store': 0x1EF29E1, 'hair bounce store': 0x1EF2F6C,
         'PT Final registration': 0x1EE6DC6, 'PT SER registration': 0x1EE6FF3, 'PT shadow branch': 0x228F866,
         'bone read (vtable target)': 0x1CAC050}
# V5.3.0 R11: on game build 1090142 the same sites at their build-1090142 addresses (src/urt_build_table.h)
import hashlib as _h, re as _re, pathlib as _p
if _h.sha256(exe).hexdigest() == '05948d7238e1c9620a1cb9b3125520f1c143ce04a8e5718a3735120c37d7bae0':
    _m = {int(a, 16): int(b, 16) for a, b in _re.findall(r'\{0x([0-9a-f]+)u,0x([0-9a-f]+)u\}', (_p.Path(__file__).resolve().parents[2] / 'src/urt_build_table.h').read_text())}
    SITES = {k: _m[v] for k, v in SITES.items()}
pe = struct.unpack_from('<I', exe, 0x3c)[0]; ns = struct.unpack_from('<H', exe, pe + 6)[0]; opt = struct.unpack_from('<H', exe, pe + 20)[0]
secs = [struct.unpack_from('<8sIIII', exe, pe + 24 + opt + i * 40) for i in range(ns)]
def off(r):
    for nm, vs, va, rs, ro in secs:
        if va <= r < va + rs: return ro + r - va
for name, r in SITES.items():
    w = exe[off(r):off(r) + 16]
    check(struct.pack('<I', r) not in asi and struct.pack('<I', r) not in add and w not in asi and w not in add,
          'IWO does not address or pattern-match URT site: ' + name)
OWNED = ['RTSTBillboards', 'RTSTBillboardsProximity', 'LODBudget', 'PTSunImpact', 'PTSkyImpact', 'PTAOEnable', 'PTAOStrength',
         'PTAORadius', 'PTAOFinalValuePower', 'PTGrassScale', 'PTGrassNightScale', 'PTLateSpecularScreenSpaceTrace',
         'PTLateSpecularCheckerboard', 'PTIndirectLightMultiplier', 'RTQualityShadingRaysPerProbe', 'NumShadingRaysPerProbe',
         'RTGIPreset', 'RTSTConfigInstancingDistance', 'RTSTConfigUpdateDistance', 'RTSTConfigUpdateWindSimBlendDistance', 'RTSTTrees']
hits = [n for n in OWNED if n.encode() in asi or n.encode() in add or n.encode('utf-16-le') in asi or n.encode('utf-16-le') in add]
check(not hits, 'IWO names none of the engine settings URT owns or that govern RT trees%s' % (': ' + ', '.join(hits) if hits else ''))
check(b'IsUltimateASILoader' not in asi and b'winmm' not in asi.lower(), 'IWO ASI is a plain ASI (no loader of its own)')
print('FAIL %d' % len(fails) if fails else 'PASS IWO static interoperability evidence (gameplay acceptance pending)')
sys.exit(1 if fails else 0)
