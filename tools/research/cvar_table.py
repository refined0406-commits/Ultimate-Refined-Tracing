#!/usr/bin/env python3
"""Research tool: full CVar registry table of a witcher3.exe (static, no execution).
Descriptor (56 B): +0 name*, +8 flags, +16 value*, +24 default(8B), +32 ?, +40 type vtable*, +48 category*.
Usage: cvar_table.py witcher3.exe cvars.tsv(from cvar_dump) > table.tsv"""
import struct,sys
f=open(sys.argv[1],'rb').read()
pe=struct.unpack_from('<I',f,0x3c)[0]; n=struct.unpack_from('<H',f,pe+6)[0]; osz=struct.unpack_from('<H',f,pe+20)[0]
base=struct.unpack_from('<Q',f,pe+24+24)[0]; secs=[]; o=pe+24+osz
for i in range(n):
    vs,va,rs,ro=struct.unpack_from('<IIII',f,o+8); secs.append((va,vs,ro,rs)); o+=40
def off(r):
    for va,vs,ro,rs in secs:
        if va<=r<va+rs: return ro+r-va
def cstr(va):
    if not va or not(base<=va<base+0x7000000): return ''
    o=off(va-base)
    if o is None: return ''
    e=f.find(b'\0',o,o+256); s=f[o:e]
    return s.decode('latin1') if all(32<=c<127 for c in s) else ''
for line in open(sys.argv[2],encoding='latin1'):
    p=line.split('\t'); d=int(p[0],16); v=int(p[1],16); o=off(d)
    nm,fl,vp,df,x32,vt,cat=struct.unpack_from('<QQQQQQQ',f,o)
    fv=struct.unpack('<f',struct.pack('<I',df&0xffffffff))[0]
    print('\t'.join([hex(d),hex(v),cstr(cat),cstr(nm),hex(fl),hex(vt-base if vt else 0),str(df&0xffffffff),'%.6g'%fv,hex(x32)]))
