#!/usr/bin/env python3
"""Reference implementation of the URT V5 executable code-identity hash.
Usage: exe_codeid.py witcher3.exe  -> prints whole-file SHA-256 and code-identity SHA-256."""
import sys, struct, hashlib
def codeid(d):
    pe=struct.unpack_from('<I',d,0x3c)[0]; assert d[pe:pe+4]==b'PE\0\0'
    nsec,=struct.unpack_from('<H',d,pe+6); optsz,=struct.unpack_from('<H',d,pe+20); opt=pe+24
    h=hashlib.sha256(); h.update(b'URT5CODEID1\0')
    h.update(d[opt+16:opt+20]); h.update(d[opt+56:opt+60]); h.update(d[opt+24:opt+32])
    rngs=[]
    for i in range(nsec):
        o=opt+optsz+i*40; name=d[o:o+8]
        vs,va,rs,ro=struct.unpack_from('<IIII',d,o+8); ch,=struct.unpack_from('<I',d,o+36)
        if name.rstrip(b'\0')==b'.rsrc': continue
        h.update(name+struct.pack('<IIII',va,vs,rs,ch))
        if rs: rngs.append((ro,rs))
    for ro,rs in rngs: h.update(d[ro:ro+rs])
    return h.hexdigest()
if __name__=='__main__':
    d=open(sys.argv[1],'rb').read()
    print('file   ',hashlib.sha256(d).hexdigest()); print('codeid ',codeid(d))
