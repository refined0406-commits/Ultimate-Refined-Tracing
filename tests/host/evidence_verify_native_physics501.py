#!/usr/bin/env python3
"""Static Patch5.01 executable provenance for Physics/LODBudget.
Does not establish the visuals/gameplay behavior of changing the physics budget.
"""
from pathlib import Path
import hashlib,struct,os,zipfile
exe=Path(os.environ.get('WITCHER501_EXE',__import__('sys').argv[1]))
b=exe.read_bytes()
assert hashlib.sha256(b).hexdigest()=='2df765398a3d1f165db418271be7200178320a4466817379f2fa0dc0001fb101'
base=0x140000000
reg=0x512ee38
fileoff=reg-0x1a00
# Native registration block: name ptr, type marker, writable value ptr,
# registered default float 25.0, runtime value is initialized by engine.
assert struct.unpack_from('<Q',b,fileoff)[0]==base+0x3201f28
assert struct.unpack_from('<Q',b,fileoff+16)[0]==base+0x512ee58
assert struct.unpack_from('<f',b,fileoff+24)[0]==25.0
assert b[0x3201f28-0x1800:0x3201f28-0x1800+10]==b'LODBudget\0'
assert b[0x3201f38-0x1800:0x3201f38-0x1800+8]==b'Physics\0'
h=0x811c9dc5
for k in b'Physics/LODBudget':h=((h^k)*0x01000193)&0xffffffff
assert h==0xbab96f8b
print('PASS exact native Patch 5.01 registry Physics/LODBudget: default 25.0, current RVA 0x512EE58, competitor FNV1a descriptor 0xBAB96F8B')
