#!/usr/bin/env python3
"""V5.3.0 R11 (game build 1090142): static evidence that every Windows-only code-site check of URT holds at the
addresses urt_rva() gives for the executable under test (Patch 5.01 build 1048522 or game build 1090142).
The checks repeat, byte for byte, what the Windows modules verify before they write anything:
  register sites + helper entrance (cr01_register_sites.c, cr01_reg_hotpatch_core.c), CameraCB serializer call and
  stores (cr01_gate_core.c), native PT library and both slots (cr01_gate_windows.c, cr01_shader_slots.c), D3D12
  owner globals (cr01_prod_early501.c), PT shadow guard + 11 PT shadow reads (cr01_shadow501.c,
  refined_pt_shadow_ext_501.c), Fast Bone Reads vtable owner (cr01_bone_hook501.c), Frame Generation setter
  (urt_fg501.c), SHaRC diagnostic (urt_gfx501.c). The address table is parsed from src/urt_build_table.h.
Usage: evidence_build_port.py witcher3.exe"""
import hashlib, pathlib, re, struct, sys
ROOT = pathlib.Path(__file__).resolve().parents[2]
SHA_1048522 = '2df765398a3d1f165db418271be7200178320a4466817379f2fa0dc0001fb101'
SHA_1090142 = '05948d7238e1c9620a1cb9b3125520f1c143ce04a8e5718a3735120c37d7bae0'
fails = 0
def check(c, m):
    global fails
    print(('PASS ' if c else 'FAIL ') + m)
    if not c: fails += 1

data = open(sys.argv[1], 'rb').read()
sha = hashlib.sha256(data).hexdigest()
build = {SHA_1048522: 0, SHA_1090142: 1}.get(sha)
check(build is not None, 'executable is a known build (%s)' % ({0: '1048522', 1: '1090142'}.get(build, 'unknown ' + sha[:12])))
if build is None: sys.exit(1)
pe = struct.unpack_from('<I', data, 0x3c)[0]; ns = struct.unpack_from('<H', data, pe + 6)[0]; oh = struct.unpack_from('<H', data, pe + 20)[0]
image_size = struct.unpack_from('<I', data, pe + 24 + 56)[0]
img = bytearray(image_size); img[:0x1000] = data[:0x1000]; text = None
for i in range(ns):
    o = pe + 24 + oh + i * 40; nm = data[o:o + 8].rstrip(b'\0'); vs, va, rs, ra = struct.unpack_from('<IIII', data, o + 8)
    img[va:va + min(vs, rs)] = data[ra:ra + min(vs, rs)]
    if nm == b'.text': text = (va, vs)
check(image_size == (0x6402000, 0x6400000)[build], 'SizeOfImage %#x = urt_image_bytes()' % image_size)

table = {}
for m in re.finditer(r'\{0x([0-9a-f]+)u,0x([0-9a-f]+)u\}', (ROOT / 'src/urt_build_table.h').read_text()):
    table[int(m.group(1), 16)] = int(m.group(2), 16)
def rva(x):
    return x if build == 0 else table.get(x, 0)
u32 = lambda a: struct.unpack_from('<I', img, a)[0]
s32 = lambda a: struct.unpack_from('<i', img, a)[0]
u64 = lambda a: struct.unpack_from('<Q', img, a)[0]
IB = 0x140000000

# --- PT library registration calls and the register-helper entrance
final, ser, helper = rva(0x1EE6DC6), rva(0x1EE6FF3), rva(0x4FE390)
for nm, a in (('final', final), ('SER', ser)):
    check(img[a] == 0xe8 and a + 5 + s32(a + 1) == helper, 'PT %s registration call at %#x -> register helper %#x' % (nm, a, helper))
check((final + 1) & 3 == 3 and (ser + 1) & 3 == 0, 'registration call operands keep the alignment the hook requires')
check(all(img[helper - i] == 0xcc for i in range(1, 9)) and bytes(img[helper:helper + 9]) == bytes([0x48, 0x85, 0xd2, 0x0f, 0x84, 0xe1, 0x08, 0, 0]),
      'register helper: 8 CC padding + TEST RDX,RDX; JE +0x8E1 (kExactEntry)')
check(rva(0x4fe399) == helper + 9 and rva(0x4fec7a) == helper + 0x8ea, 'helper continue/branch RVAs = helper+9 / helper+0x8ea')
# --- CameraCB serializer call (gate)
call, writer, geo, hair = rva(0x1EF0FCF), rva(0x1EF2340), rva(0x1EF29E1), rva(0x1EF2F6C)
check(bytes(img[call:call + 5]) == bytes([0xe8, 0x6c, 0x13, 0, 0]) and (call + 1) & 3 == 0 and call + 5 + 0x136c == writer,
      'serializer call %#x: E8 6C 13 00 00, aligned operand, target = CameraCB writer %#x' % (call, writer))
check(bytes(img[geo:geo + 10]) == bytes([0xc7, 0x83, 0x10, 0x02, 0, 0, 2, 0, 0, 0]) and bytes(img[hair:hair + 10]) == bytes([0xc7, 0x83, 0x80, 0x03, 0, 0, 4, 0, 0, 0]),
      'writer stores native bounce words: [rbx+0x210]=2 and [rbx+0x380]=4')
# --- native PT library + both slots
blob = rva(0x32E7C28)
nat = bytes(img[blob:blob + 231144])
check(hashlib.sha256(nat).hexdigest() == '63828ae55a44ce1ab041065ba4e3169e0b33c277943b19545be7a810ae048121', 'native PT library at %#x: SHA-256 6382…8121 (cr01_gate_verify_native_blob)' % blob)
slots = []; i = img.find(nat)
while i >= 0: slots.append(i); i = img.find(nat, i + 1)
check(len(slots) == 2 and blob in slots, 'exactly two native PT library slots, one at the anchored RVA (%s)' % ', '.join('%#x' % x for x in slots))
check(all(u32(s - 8) == 231144 and img[s + 231144:s + 231144 + 22144].count(0) == 22144 for s in slots), 'each slot: size word 231144 before, 22144 zero bytes after (room for 253288)')
# --- D3D12 owner globals (must be zero in the image: the early start reads them before the renderer exists)
for g in (0x5e514d0, 0x5e514d8):
    a = rva(g); check(a and a % 8 == 0 and u64(a) == 0, 'D3D12 owner global %#x -> %#x: 8-aligned, zero at start' % (g, a))
# --- PT shadow guard (cr01_shadow501.c) + the 11 PT shadow reads (refined_pt_shadow_ext_501.c)
def unique(pat, mask):
    rx = re.compile(b''.join(re.escape(bytes([b])) if m == 'x' else b'.' for b, m in zip(pat, mask)), re.S)
    hits = [text[0] + m.start() for m in rx.finditer(bytes(img[text[0]:text[0] + text[1]]))]
    return hits[0] if len(hits) == 1 else None
gp = [0x84,0xc9,0x74,0x0c,0x84,0xc0,0x75,0x08,0x39,0x3d,0,0,0,0,0x77,0x11,0x40,0x38,0x3d,0,0,0,0,0x74,0x43,0x84,0xc9,0x74,0x3f,0x84,0xc0,0x75,0x3b]
lp = [0x84,0xc9,0x74,0x0d,0x84,0xc0,0x75,0x09,0x83,0x3d,0,0,0,0,0x00,0x77,0x1d,0x80,0x3d,0,0,0,0,0x00,0x0f,0x84,0,0,0,0,0x84,0xc9,0x0f,0x84]
g = unique(gp, 'xxxxxxxxxx????xxxxx????xxxxxxxxxx'); l = unique(lp, 'xxxxxxxxxx????xxxxx????xxx????xxxx')
check(g is not None and l is not None, 'PT shadow guard patterns are unique in .text (%s, %s)' % (g and hex(g), l and hex(l)))
if g and l:
    na, nr = g + 14 + s32(g + 10), l + 15 + s32(l + 10); ptg, ptl = g + 23 + s32(g + 19), l + 24 + s32(l + 19)
    check(na == nr and ptg == ptl and na != ptg, 'guard reads: one native Shadow variable %#x, one PT variable %#x' % (na, ptg))
    p, s = rva(0x228f866), rva(0x228f858)
    check(img[p-7] == 0x80 and img[p-6] == 0x3d and img[p-1] == 0 and img[p] == 0x74 and img[p+1] == 0x0b and img[s-2] == 0x83 and img[s-1] == 0x3d
          and img[s+4] == 0 and img[s+5] == 0x77 and img[s+6] == 0x11 and p + s32(p - 5) == ptg and s + 5 + s32(s) == na,
          'third guard branch %#x and its Shadow read %#x' % (p, s))
    refs = [(0x1c2ae35,0,[0x44,0x39,0x2d],0),(0x1c2e7f1,0,[0x39,0x35],0),(0x1c2f5cb,0,[0x48,0x8b,0x15],0),(0x1d60c78,0,[0x39,0x2d],0),(0x228f8f1,0,[0x4c,0x8b,0x05],0),
            (0x1c345c3,1,[0x83,0x3d],1),(0x1c3ae28,1,[0x83,0x3d],1),(0x1c6d7a8,1,[0x83,0x3d],1),(0x227351c,1,[0x83,0x3d],1),(0x228f858,1,[0x83,0x3d],1),(0x229025f,1,[0x83,0x3d],1)]
    ok = True
    for old, imm, op, trailer in refs:
        a = rva(old)
        good = a and list(img[a - len(op):a]) == op and (not trailer or img[a + 4] == 0) and a + 4 + imm + s32(a) == na
        if not good: print('   site %#x -> %#x does not match' % (old, a)); ok = False
    check(ok, 'all 11 PT shadow reads: opcode, trailer and rip target = the native Shadow variable')
# --- Fast Bone Reads (vtable owner)
nb, vt, head, col = rva(0x1cac050), rva(0x37ae730), rva(0x37ae6e0), rva(0x3ac8e10)
ref_native = (ROOT / 'src/cr01_bone_hook501.c').read_text()
kbytes = bytes(int(x, 16) for x in re.search(r'kNative\[192\] = \{([^}]*)\}', ref_native).group(1).replace(' ', '').split(','))
check(len(kbytes) == 192 and bytes(img[nb:nb + 192]) == kbytes, 'bone native function %#x: 192 bytes identical to kNative' % nb)
check(u64(head - 8) == IB + col and u32(col) == 1 and u32(col + 20) == col and vt % 8 == 0 and u64(vt) == IB + nb,
      'bone class vtable %#x (head %#x, RTTI locator %#x) holds the native function' % (vt, head, col))
# --- Frame Generation setter
fp, fm, fc = rva(0x5ca8140), rva(0x51513e8), rva(0x1b815b3)
ok = fp and fm and fc
for a0, op in ((0x1b81566, [0x48,0x8b,0x05]), (0x1b8158a, [0x4c,0x8d,0x05]), (0x1b815ac, [0x48,0x8b,0x05])):
    a = rva(a0); ok = ok and a and list(img[a:a+3]) == op and a + 7 + s32(a + 3) == fp
check(ok and bytes(img[fc:fc+10]) == bytes([0x48,0x8b,0xd7,0x48,0x8d,0x4c,0x24,0x20,0xff,0xd0]) and u64(fp) == 0,
      'Frame Generation setter: 3 rip loads of the option pointer %#x, call rax at %#x' % (fp, fc))
# --- SHaRC diagnostic (read-only)
st, gt, flag = rva(0x1ef0870), rva(0x1c1e97e), rva(0x5149b40)
check(bytes(img[st:st+7]) == bytes([0x44,0x89,0xaf,0x20,0x02,0,0]) and list(img[gt:gt+3]) == [0x44,0x38,0x0d] and gt + 7 + s32(gt + 3) == flag,
      'SHaRC: entry-count store %#x, PTUseSHARC test %#x -> %#x' % (st, gt, flag))
ri_vt = rva(0x382a950)
check(ri_vt and u64(ri_vt + 0x160) == IB + (0x1eefcd0, 0x1eef5e0)[build],
      'SHaRC render-interface vtable %#x: +0x160 = the SHaRC path (function identical in both builds)' % ri_vt)
# --- CPU Physics Level of Detail: LODBudget value slot (cr01_prod_runtime501.c pins it on known code)
def cstr(va):
    a = va - IB; e = img.index(0, a); return bytes(img[a:e]).decode('latin1')
lb = rva(0x512ee58); d = lb - 0x20
check(lb and cstr(u64(d)) == 'LODBudget' and u64(d + 16) == IB + lb and struct.unpack_from('<f', img, lb - 8)[0] == 25.0,
      'Physics/LODBudget value slot %#x: descriptor name, value pointer, registered default 25.0 (the runtime shape check)' % lb)
print('TOTAL %s %d' % ('FAIL' if fails else 'PASS', fails))
sys.exit(1 if fails else 0)
