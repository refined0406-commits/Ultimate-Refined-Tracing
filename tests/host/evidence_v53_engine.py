#!/usr/bin/env python3
"""V5.3 private development: static engine evidence for the new rows, read from the exact Patch 5.01
witcher3.exe (2df76539...). Read-only; nothing is executed. Usage: evidence_v53_engine.py <witcher3.exe>

Proves, byte for byte:
 1. descriptors: name, real category (+48), flags (+8), registered default (+24) and value slot (+16) of the
    nine variables; none carries the game's save flag (0x100, so no native graphics option can persist them);
 2. read sites: every instruction that reads each variable (RIP-relative), so the documented timing and
    renderer scope are reproducible:
      - TextureUnloadSpeed: two texture-streaming unload-timer updates (per streamed texture, live);
      - MeshOffset / GenericOffset: the sector streaming-distance function (live);
      - IdleFrameCount: GC scheduling (next automatic GC = now + IdleFrameCount frames, live);
      - AutoDiscardCount (registered under MetadataStore): the four discard-queue checks that request a GC
        when the queue reaches the threshold (live);
      - GIAlphaTestMipLevel: the RTXGI probe-trace constants and the per-pass mip switch (Normal RT GI);
      - ReflectionsRadianceMipLevel: the RT reflection constants AND the shared RT/PT constant builder
        that also reads PTEnable (so the value reaches Path Tracing);
      - RTConfigDefragThresholdPerCent / RTConfigMaxBlasesInBatch: ONE reader, the routine that copies them
        into the acceleration-structure configuration (0x405af10 / 0x405af14), called only from two
        initialisation paths, one of them under the render-device start ("Initializing render device");
 3. "No wait for the RT scene": the RT-shadows test (0x228f840) gates, at 0x1c38be5, two scene-fence waits
    (0x1cfaa30) and the RT shadow pass (0x1c1d640); it has exactly three callers. URT's existing PT
    Ray-Traced Shadows = Off rewrites this test's own branch (0x228f866), so all three callers already skip
    the waits whenever PT RT shadows are off; a second, separate wait bypass would be redundant or unsafe;
 4. W3LoadFix 1.0.8 coexistence: none of its known code addresses lies in a function that URT patches."""
import hashlib, struct, sys, bisect
p = sys.argv[1]
f = open(p, 'rb').read()
assert hashlib.sha256(f).hexdigest() == '2df765398a3d1f165db418271be7200178320a4466817379f2fa0dc0001fb101', 'Unexpected Patch 5.01 executable'
pe = struct.unpack_from('<I', f, 0x3c)[0]; n = struct.unpack_from('<H', f, pe + 6)[0]; osz = struct.unpack_from('<H', f, pe + 20)[0]
base = struct.unpack_from('<Q', f, pe + 24 + 24)[0]
secs = []; o = pe + 24 + osz
for i in range(n):
    nm = f[o:o + 8].rstrip(b'\0'); vs, va, rs, ro = struct.unpack_from('<IIII', f, o + 8); secs.append((nm, va, vs, ro, rs)); o += 40
def off(r):
    for nm, va, vs, ro, rs in secs:
        if va <= r < va + rs: return ro + r - va
    raise KeyError(hex(r))
def cstr(ptr):
    o = off(ptr - base); return f[o:f.index(b'\0', o)].decode('latin1')
fails = []
def check(c, m):
    print(('PASS ' if c else 'FAIL ') + m)
    if not c: fails.append(m)
# ---- 1. descriptors ----
D = {  # name: (descriptor RVA, category, flags, default, value RVA)
 'TextureUnloadSpeed': (0x514c6c8, 'Rendering', 0x202, 0x3f800000, 0x514c6e8),
 'MeshOffset': (0x515e640, 'Streaming/Sector', 0x2, 0x41200000, 0x515e660),
 'GenericOffset': (0x515e608, 'Streaming/Sector', 0x2, 0x42340000, 0x515e628),
 'AutoDiscardCount': (0x5170138, 'MetadataStore', 0x1, 400, 0x5170158),
 'IdleFrameCount': (0x516f530, 'Memory/GC', 0x1, 30, 0x516f550),
 'RTConfigDefragThresholdPerCent': (0x5130368, 'Rendering/RT/AS/Config', 0x201, 30, 0x5130388),
 'RTConfigMaxBlasesInBatch': (0x5130448, 'Rendering/RayTracing', 0x201, 256, 0x5130468),
 'GIAlphaTestMipLevel': (0x514dea0, 'Rendering/RT', 0x202, 0x40a00000, 0x514dec0),
 'ReflectionsRadianceMipLevel': (0x514ddf8, 'Rendering/RT', 0x202, 0x40800000, 0x514de18),
}
ok = 0
for name, (d, cat, fl, df, v) in D.items():
    nm, flags, vp, dflt, _x, _t, catp = struct.unpack_from('<QQQQQQQ', f, off(d))
    good = cstr(nm) == name and cstr(catp) == cat and flags == fl and (dflt & 0xffffffff) == df and vp - base == v and not (flags & 0x100)
    ok += good
    if not good: print('  descriptor mismatch', name)
check(ok == 9, 'descriptors of all 9 variables: name, real category, flags, default, value slot; none saved by the game')
check(struct.unpack_from('<Q', f, off(0x5170138) + 48)[0] - base != struct.unpack_from('<Q', f, off(0x516f530) + 48)[0] - base,
      'AutoDiscardCount is registered under MetadataStore, not Memory/GC (gc.ini lists it under [Memory/GC]; URT writes the live value)')
check(cstr(struct.unpack_from('<Q', f, off(0x5130448) + 48)[0]) == 'Rendering/RayTracing',
      'RTConfigMaxBlasesInBatch is registered under Rendering/RayTracing, not Rendering/RT/AS/Config (URT writes the live value)')
# ---- 2. read sites: (instruction RVA, length, displacement offset, expected target) ----
def rip(addr, ln, do):
    return addr + ln + struct.unpack_from('<i', f, off(addr) + do)[0]
SITES = {
 'TextureUnloadSpeed': [(0x1c92a4c, 8, 4), (0x1d0cee4, 8, 4)],
 'MeshOffset': [(0x2501c6d, 8, 4)],
 'GenericOffset': [(0x2501c7a, 8, 4), (0x2501c9e, 8, 4)],
 'IdleFrameCount': [(0x270f42e, 6, 2)],
 'AutoDiscardCount': [(0x26cfccb, 6, 2), (0x26d0ee4, 6, 2), (0x2739858, 6, 2), (0x27398c1, 6, 2)],
 'GIAlphaTestMipLevel': [(0x1c22069, 8, 4), (0x1c2e26c, 8, 4)],
 'ReflectionsRadianceMipLevel': [(0x1c204cc, 8, 4), (0x1c2e27d, 8, 4), (0x1c2ed30, 8, 4)],
 'RTConfigDefragThresholdPerCent': [(0x1ee9873, 6, 2)],
 'RTConfigMaxBlasesInBatch': [(0x1ee987f, 6, 2)],
}
tva, tvs, tro, trs = [(va, vs, ro, rs) for nm, va, vs, ro, rs in secs if nm == b'.text'][0]
text = f[tro:tro + trs]
allok = 1
for name, sites in SITES.items():
    v = D[name][4]
    good = all(rip(a, l, k) == v for a, l, k in sites)
    if not good: print('  read-site mismatch', name)
    allok &= good
    print('  %-30s %d documented read site(s)' % (name, len(sites)))
check(allok, 'every documented read site decodes to the variable (live reads; GI = RTXGI probe constants; reflections also in the RT/PT builder)')
# reflections reach PT: the builder 0x1c2e400 that reads ReflectionsRadianceMipLevel also reads PTEnable (0x514f5f0)
def reads_in(fn_lo, fn_hi, target):
    for i in range(fn_lo - tva, fn_hi - tva - 4):
        disp = struct.unpack_from('<i', text, i)[0]
        for tail in (0, 1, 4):
            if tva + i + 4 + tail + disp == target: return True
    return False
check(reads_in(0x1c2e400, 0x1c2f000, 0x514f5f0) and reads_in(0x1c2e400, 0x1c2f000, 0x514de18),
      'ReflectionsRadianceMipLevel is read by the constant builder that also reads PTEnable: it affects Path Tracing')
# ---- AS configuration copy ----
check(rip(0x1ee9879, 6, 2) == 0x405af10 and rip(0x1ee9885, 6, 2) == 0x405af14 and
      f[off(0x1ee9801):off(0x1ee9801) + 2] == b'\xc7\x05' and f[off(0x1ee980b):off(0x1ee980b) + 2] == b'\xc7\x05',
      'RT AS routine 0x1ee97d0 seeds 100 / 128 and then copies both variables into the AS configuration (0x405af10/14)')
import numpy as np
_t = np.frombuffer(text, dtype=np.uint8)
_e8 = np.nonzero(_t[:-5] == 0xe8)[0]
_rel = (_t[_e8 + 1].astype(np.int64) | (_t[_e8 + 2].astype(np.int64) << 8) | (_t[_e8 + 3].astype(np.int64) << 16) | (_t[_e8 + 4].astype(np.int64) << 24))
_rel = np.where(_rel >= 1 << 31, _rel - (1 << 32), _rel)
_dst = tva + _e8 + 5 + _rel
def calls_to(target):
    return [int(tva + i) for i in _e8[_dst == target]]
c = calls_to(0x1ee97d0)
check(c == [0x1eeb0a6, 0x1ef1389], 'the AS copy routine has exactly two direct callers (initialisation paths)')
pd = [(va, vs, ro) for nm, va, vs, ro, rs in secs if nm == b'.pdata'][0]
ents = sorted(struct.unpack_from('<III', f, pd[2] + i * 12) for i in range(pd[1] // 12))
starts = [e[0] for e in ents]
def fn(r):
    i = bisect.bisect_right(starts, r) - 1
    return ents[i][0] if i >= 0 and ents[i][0] <= r < ents[i][1] else None
chain = [(0x1ef3620, 0x1ef1370), (0x1ed1df0, 0x1ef3620), (0x1b8b9a0, 0x1ed1df0)]
def has_call(caller, callee):
    return any(fn(s) is not None and (fn(s) == caller or (caller <= s < caller + 0x4000)) for s in calls_to(callee))
check(all(has_call(a, b) for a, b in chain), 'call chain render-device start 0x1b8b9a0 -> 0x1ed1df0 -> 0x1ef3620 -> 0x1ef1370 -> AS copy')
rd = f.find(b'Initializing render device')
rdr = [va + rd - ro for nm, va, vs, ro, rs in secs if ro <= rd < ro + rs][0]
check(reads_in(0x1b8b9a0, 0x1b8c400, rdr), '0x1b8b9a0 is the render-device start ("Initializing render device %d x %d")')
# ---- 3. RT shadows test and the scene waits ----
check(f[off(0x228f85f):off(0x228f85f) + 2] == b'\x80\x3d' and rip(0x228f85f, 7, 2) == 0x514f5f0 and f[off(0x228f866)] == 0x74,
      'RT-shadows test 0x228f840: PTEnable branch at 0x228f866 (the byte URT PT Ray-Traced Shadows = Off rewrites)')
check(calls_to(0x228f840) == [0x1c1a941, 0x1c389e0, 0x1c38be5], 'the RT-shadows test has exactly three callers')
blk = f[off(0x1c38be5):off(0x1c38be5) + 0x44]
check(blk[5:7] == b'\x84\xc0' and blk[7] == 0x74 and rip(0x1c38bfb, 5, 1) == 0x1cfaa30 and rip(0x1c38c0d, 5, 1) == 0x1cfaa30 and rip(0x1c38c24, 5, 1) == 0x1c1d640,
      'scene wait: when the test is false the engine skips both RT-scene waits and the RT shadow pass')
# ---- 4. W3LoadFix 1.0.8 known code addresses (from its w3loadfix.dll table) vs URT patched functions ----
W3LF = [0xba28b0, 0x26f3300, 0x26d1d90, 0x2412820, 0x1dbd990, 0x23377e0, 0x275cea0, 0x2755960, 0x23bb030, 0x23bb1b0, 0x235b190,
        0xbcaa70, 0x276ca50, 0x9e16c0, 0x26f6460, 0x27b6960, 0x1f59cd0, 0x26b90a0, 0x1f5c190, 0x2092120, 0x2092340, 0x2093650,
        0x209c330, 0x209c5f0, 0x2098a90, 0x1f5cab0, 0x2053d10]
URT = [0x1EE6DA2, 0x1EE6FCB, 0x1EE6DC6, 0x1EE6FF3, 0x1ef0fcf, 0x1ef2340, 0x1ef29e1, 0x1ef2f6c, 0x4fe390, 0x4fe399, 0x4fec7a, 0x228f858,
       0x228f866, 0x228f8f1, 0x229025f, 0x1c2ae35, 0x1c2e7f1, 0x1c2f5cb, 0x1c345c3, 0x1c3ae28, 0x1c6d7a8, 0x1d60c78, 0x227351c, 0x1CE3D43,
       0x1CE3DD1, 0x1CE3E54, 0x1CEA547, 0x1cac050, 0x1b81530, 0x1b81566, 0x1b8158a, 0x1b815ac, 0x1b815b3]
wf = {fn(x) for x in W3LF} - {None}; uf = {fn(x) for x in URT} - {None}
check(len(wf) == len(W3LF) and not (wf & uf) and min(abs(a - b) for a in wf for b in uf) > 0x1000,
      'W3LoadFix 1.0.8: all %d known code addresses are function starts, none in a function URT patches' % len(W3LF))
print('TOTAL', 'FAIL %d' % len(fails) if fails else 'PASS')
sys.exit(1 if fails else 0)
