#!/usr/bin/env python3
"""Research tool: direct-call graph (E8 rel32) of witcher3.exe .text using .pdata function bounds
(chained unwind fragments are mapped to their primary function). Saves graph as JSON."""
import struct,bisect,json,sys
exe,out=sys.argv[1],sys.argv[2]
f=open(exe,'rb').read()
pe=struct.unpack_from('<I',f,0x3c)[0];opt=pe+24;osz=struct.unpack_from('<H',f,pe+20)[0];n=struct.unpack_from('<H',f,pe+6)[0]
secs=[];o=opt+osz
for i in range(n):
    vs,va,rs,ro=struct.unpack_from('<IIII',f,o+8);secs.append((va,vs,ro,rs));o+=40
def off(r):
    for va,vs,ro,rs in secs:
        if va<=r<va+rs:return ro+r-va
prva,psz=struct.unpack_from('<II',f,opt+112+3*8);po=off(prva)
ents=sorted({struct.unpack_from('<III',f,po+i) for i in range(0,psz,12)})
starts=[a for a,b,c in ents]
# chained unwind info: flag UNW_FLAG_CHAININFO=4 -> RUNTIME_FUNCTION follows unwind codes
def primary(i):
    seen=0
    while seen<16:
        a,b,u=ents[i]; uo=off(u&~1) if not (u&1) else None
        if uo is None: return a
        vf=f[uo]; flags=vf>>3; cnt=f[uo+2]
        if not flags&4: return a
        q=uo+4+((cnt+1)&~1)*2; pa,pb,pu=struct.unpack_from('<III',f,q)
        j=bisect.bisect_left(starts,pa)
        if j>=len(starts) or starts[j]!=pa: return pa
        i=j; seen+=1
    return ents[i][0]
prim=[primary(i) for i in range(len(ents))]
def fn_of(r):
    i=bisect.bisect_right(starts,r)-1
    return prim[i] if i>=0 and ents[i][0]<=r<ents[i][1] else None
tva,tvs,tro,trs=secs[0];text=f[tro:tro+trs]
g={}
import re
for m in re.finditer(b'\xe8',text):
    p=m.start()
    if p+5>len(text):break
    t=tva+p+5+struct.unpack_from('<i',text,p+1)[0]
    if not(tva<=t<tva+trs):continue
    a=fn_of(tva+p); b=fn_of(t)
    if a is None or b is None: continue
    g.setdefault(a,set()).add(b)
json.dump({'%x'%k:sorted('%x'%x for x in v) for k,v in g.items()},open(out,'w'))
json.dump({'%x'%ents[i][0]:'%x'%prim[i] for i in range(len(ents)) if prim[i]!=ents[i][0]},open(out+'.chain','w'))
print('functions',len(ents),'callers',len(g))
