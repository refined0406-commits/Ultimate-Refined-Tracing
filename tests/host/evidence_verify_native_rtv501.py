#!/usr/bin/env python3
"""Exact Patch 5.01 registration and instruction crossrefs; NOT gameplay proof."""
from pathlib import Path
import os,struct,hashlib
p=Path(os.environ.get('WITCHER501_EXE',__import__('sys').argv[1]))
b=p.read_bytes();assert hashlib.sha256(b).hexdigest()=='2df765398a3d1f165db418271be7200178320a4466817379f2fa0dc0001fb101'
base=0x140000000
native={'RTSTTreesUpdates':(0x406c9c0,0x406c9e0,1),'RTSTTreesUpdateOnlyOnce':(0x406ca30,0x406ca50,0),'RTSTTreesUpdateOnlyNear':(0x406ca68,0x406ca88,1),'RTSTConfigUpdateDistance':(0x514c818,0x514c838,20.0),'RTSTConfigUpdateWindSimBlendDistance':(0x514c850,0x514c870,15.0)}
for name,(reg,slot,default) in native.items():
 # Native data section RVA 0x3FF2000 maps to raw offset 0x3FF0600.
 off=reg-0x1a00
 assert struct.unpack_from('<Q',b,off+16)[0]==base+slot,name
 if isinstance(default,int): assert struct.unpack_from('<I',b,off+24)[0]==default,(name,'default mismatch')
 else:assert struct.unpack_from('<f',b,off+24)[0]==default,(name,'default mismatch')
 name_off=b.find(name.encode()+b'\0');assert name_off>=0
 ptr=base+0x298c000+(name_off-0x298a800)
 assert struct.unpack_from('<Q',b,off)[0]==ptr,(name,'registered name mismatch')
# Native helper maps tree update flags from CVars into per-instance bitfield.
for rva,slot in [(0x1cea4ca,0x406c9e0),(0x1cea502,0x406ca50),(0x1cea520,0x406ca88)]:
 off=rva-0x1000+0x600
 # Instructions differ: cmp byte ptr RIP-relative vs movzx eax RIP-relative.
 inst=b[off:off+7]
 if rva in (0x1cea4ca,0x1cea502):
  assert inst[:2]==b'\x80\x3d'; target=rva+7+struct.unpack_from('<i',inst,2)[0]
 else:
  assert inst[:3]==b'\x0f\xb6\x05';target=rva+7+struct.unpack_from('<i',inst,3)[0]
 assert target==slot,(hex(rva),hex(target),hex(slot))
assert b[0x1ef268b-0x1000+0x600:0x1ef2693-0x1000+0x600].startswith(b'\xc5\xfa\x10\x0d')
assert b[0x1ef2693-0x1000+0x600:0x1ef2697-0x1000+0x600]==b'\xc5\xf2\x59\xd1' # multiply distance by itself
print('PASS native registration defaults, selected instance flag reads and squared distance conversion; CPU update rate, safe distance cutoff and scene refresh still NOT proven')
