#!/usr/bin/env python3
"""Executable evidence for the distant-tree shadow investigation (Patch 5.01).
Usage: evidence_distant_trees.py witcher3.exe   (read-only)
Establishes, from code bytes:
 1. Native defaults of the RT SpeedTree / proximity-TLAS CVars.
 2. Billboard inclusion gate, at 7 sites:
      include = RTSTBillboards && (RTSTBillboardsProximity || mode == 0)
    CPU: Distant Trees writes RTSTBillboardsProximity = 0, so billboards are
    dropped from every mode != 0 build. PT Billboards Off writes
    RTSTBillboards = 0, so they are dropped from all builds.
 3. The same gate for 3D trees uses RTSTTrees / RTSTTreesProximity
    (URT never writes these).
 4. The two build paths construct jobs with mode 0 and mode 1; the mode-1
    path is gated by UpdateProximityTLAS.
 5. The proximity box is camera +/- ProximityTLASSize (400) or
    ProximityExtendedTLASSize (800).
 6. Shadow and radiance rays select TLAS index ShadowsTlas/RadianceTlas
    (default 1); RTAO uses RTAOTlas (default 0).
"""
import sys, struct
exe = open(sys.argv[1], 'rb').read()
pe = struct.unpack_from('<I', exe, 0x3c)[0]; n = struct.unpack_from('<H', exe, pe + 6)[0]; opt = struct.unpack_from('<H', exe, pe + 20)[0]
secs = [struct.unpack_from('<8sIIII', exe, pe + 24 + opt + i * 40) for i in range(n)]
IB = 0x140000000
def off(rva):
    for nm, vs, va, rs, ro in secs:
        if va <= rva < va + rs: return ro + rva - va
def u32(rva): return struct.unpack_from('<I', exe, off(rva))[0]
def f32(rva): return struct.unpack_from('<f', exe, off(rva))[0]
def rip_target(rva_insn, disp_at, length):
    return rva_insn + length + struct.unpack_from('<i', exe, off(rva_insn + disp_at))[0]
fails = []
def check(c, m):
    print(('  ok   ' if c else '  FAIL ') + m)
    if not c: fails.append(m)
# 1. descriptors: name ptr, kind, value ptr, default
D = {'RTSTTrees': (0x406c950, 0x200, 1), 'RTSTTreesProximity': (0x406c988, 0x200, 1),
     'RTSTBillboards': (0x406cbf0, 0x200, 1), 'RTSTBillboardsProximity': (0x406cc28, 0x200, 1),
     'UpdateProximityTLAS': (0x514c888, 0x200, 1), 'ShadowsTlas': (0x514e728, 0x201, 1),
     'RadianceTlas': (0x514e530, 0x201, 1), 'RTAOTlas': (0x514e840, 0x201, 0),
     'ProximityTLASSize': (0x514c7e0, 0x202, 400.0), 'ProximityExtendedTLASSize': (0x514c7a8, 0x202, 800.0)}
VAL = {}
for name, (desc, kind, dflt) in D.items():
    o = off(desc); nptr = struct.unpack_from('<Q', exe, o)[0] - IB
    nm = exe[off(nptr):off(nptr) + 64].split(b'\0')[0].decode()
    k = struct.unpack_from('<I', exe, o + 8)[0]; vp = struct.unpack_from('<Q', exe, o + 16)[0] - IB
    d = struct.unpack_from('<f', exe, o + 24)[0] if kind == 0x202 else (struct.unpack_from('<i', exe, o + 24)[0] if kind == 0x201 else exe[o + 24])
    VAL[name] = vp
    check(nm == name and k == kind and d == dflt, f'{name}: kind {k:#x}, native default {d}')
# 2/3. inclusion gates: cmp [Global] ; je ; cmp [Proximity] ; jne ; test reg,reg ; jne
def gate_sites(glob, prox):
    hits = []
    for nm, vs, va, rs, ro in secs:
        if not nm.startswith(b'.text'): continue
        for i in range(ro, ro + rs - 32):
            r = va + i - ro
            # prox compare forms: 44 38 2d disp (7) | 80 3d disp 00 (7)
            if exe[i:i + 3] == b'\x44\x38\x2d' or (exe[i:i + 2] == b'\x80\x3d' and exe[i + 6] == 0):
                L = 7; dpos = 3 if exe[i] == 0x44 else 2
                if rip_target(r, dpos, L) != prox: continue
                j = i + L
                if exe[j] != 0x75: continue
                k = j + 2
                tl = 3 if exe[k] in (0x45, 0x41) else 2
                if exe[k + (1 if tl == 3 else 0)] != 0x85 or exe[k + tl] != 0x75: continue
                # preceding global compare (7 bytes) + je (2 bytes)
                p = i - 9
                if exe[p + 7] != 0x74: continue
                dp = 3 if exe[p] == 0x44 else 2
                if rip_target(r - 9, dp, 7) != glob: continue
                hits.append(r)
    return hits
bb = gate_sites(VAL['RTSTBillboards'], VAL['RTSTBillboardsProximity'])
check(len(bb) == 7, f'billboard gate "Billboards && (BillboardsProximity || mode==0)" at {len(bb)} sites: ' + ', '.join(hex(x) for x in bb))
tr = gate_sites(VAL['RTSTTrees'], VAL['RTSTTreesProximity'])
check(len(tr) >= 1, f'tree-geometry gate "Trees && (TreesProximity || mode==0)" at {len(tr)} site(s)')
# 4. mode argument: job ctor 0x1cdbe80 stores 5th arg at +0x40; run 0x1cdbdb0 passes [+0x40] as r9 to 0x1ce3a90
check(exe[off(0x1cdbf4d):off(0x1cdbf4d) + 3] == b'\x89\x47\x40', 'job constructor stores the build mode at +0x40')
check(exe[off(0x1cdbe02):off(0x1cdbe02) + 4] == b'\x44\x8b\x4b\x40' and rip_target(0x1cdbe13, 1, 5) == 0x1ce3a90, 'job run passes the mode to the instance builder (r9)')
check(exe[off(0x1ce384a):off(0x1ce384a) + 6] == b'\x41\xbc\x01\x00\x00\x00', 'one build path uses mode 1')
check(exe[off(0x1ce4ece):off(0x1ce4ece) + 2] == b'\x33\xdb' and exe[off(0x1ce4f5e):off(0x1ce4f5e) + 4] == b'\x89\x5c\x24\x20', 'the other build path uses mode 0')
check(rip_target(0x1ce2255, 3, 7) == VAL['UpdateProximityTLAS'], 'proximity rebuild gated by UpdateProximityTLAS')
# 5. proximity box
check(rip_target(0x1cf2b00, 4, 8) == VAL['ProximityTLASSize'] and rip_target(0x1cf2b0a, 4, 8) == VAL['ProximityExtendedTLASSize'], 'proximity box = camera +/- 400 m (extended 800 m)')
# 6. TLAS selection for shadows
check(rip_target(0x1c1d5b2, 3, 7) == VAL['ShadowsTlas'] and rip_target(0x1c1f9a6, 2, 6) == VAL['ShadowsTlas'], 'shadow pass selects its TLAS from ShadowsTlas != 0')
check(rip_target(0x1c20ba5, 2, 6) == VAL['RadianceTlas'], 'radiance pass selects its TLAS from RadianceTlas != 0')
print('FAIL %d' % len(fails) if fails else 'PASS distant-tree evidence')
sys.exit(1 if fails else 0)
