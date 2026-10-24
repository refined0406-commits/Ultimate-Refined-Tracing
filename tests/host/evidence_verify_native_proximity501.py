#!/usr/bin/env python3
"""Static source evidence, not a runtime scene-build assertion."""
from pathlib import Path
import hashlib, struct, sys
from os import environ
path=Path(environ.get('WITCHER501_EXE',__import__('sys').argv[1]))
b=path.read_bytes()
assert hashlib.sha256(b).hexdigest()=='2df765398a3d1f165db418271be7200178320a4466817379f2fa0dc0001fb101','Unexpected Patch 5.01 executable'
base=0x140000000
names={'RTSTBillboards':(0x406cbf0,0x406cc10), 'RTSTBillboardsProximity':(0x406cc28,0x406cc48), 'RTSTTrees':(0x406c950,0x406c970)}
for name,(reg,slot) in names.items():
 offset=reg-0x1a00
 ptr=struct.unpack_from('<Q',b,offset+16)[0]
 assert ptr==base+slot,(name,hex(ptr))
 assert struct.unpack_from('<I',b,offset+24)[0]==1,('unexpected native default',name)
 rdata_off=b.find(name.encode()+b'\0',0x298a800,0x3ff0600)
 assert rdata_off>=0
 addr=struct.unpack_from('<Q',b,offset)[0]
 assert addr==base+0x298c000+rdata_off-0x298a800,(name,'registry name mismatch')
# Verify native instructions separately select global billboard and native proximity
# at exact Patch 5.01 code sites. Both conditions gate instance bit 0x4.
for rva, slot in [(0x1ce3d3a,0x406cc10),(0x1ce3d43,0x406cc48),
                  (0x1ce3dc8,0x406cc10),(0x1ce3dd1,0x406cc48),
                  (0x1ce3e4b,0x406cc10),(0x1ce3e54,0x406cc48),
                  (0x1cea53e,0x406cc10),(0x1cea547,0x406cc48)]:
 off=rva-0x1000+0x600
 ins=b[off:off+7]
 assert ins[:3]==b'\x44\x38\x2d' or ins[:2]==b'\x80\x3d',(hex(rva),ins.hex())
 disp_offset=3 if ins[:3]==b'\x44\x38\x2d' else 2
 inslen=7
 dst=rva+inslen+struct.unpack_from('<i',ins,disp_offset)[0]
 assert dst==slot,(hex(rva),hex(dst),hex(slot))
assert b[0x1ce3d50-0x1000+0x600:0x1ce3d52-0x1000+0x600] == b'\xb0\x04'
print('PASS Patch 5.01 exact byte evidence: billboard global vs proximity slots, defaults, native per-instance proximity decision. Dynamic scene rebuilding NOT proven.')
