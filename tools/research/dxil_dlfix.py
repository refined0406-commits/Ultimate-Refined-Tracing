#!/usr/bin/env python3
"""Research tool (read-only on the game): copy a DXIL .bc and rewrite old DXIL datalayout alignments
'i1:32'/'i8:32'/'i16:32' (rejected by LLVM 18) to 'i1:08'/'i8:08'/'i16:16', same bit length, in
fixed-8 or VBR6 record encoding, so llvm-dis can print the module. Analysis only; never shipped.
usage: dxil_dlfix.py in.bc out.bc"""
import sys,numpy as np
b=np.frombuffer(open(sys.argv[1],'rb').read(),dtype=np.uint8)
bits=np.unpackbits(b,bitorder='little')
def enc(s,kind):
    out=[]
    for c in s.encode():
        if kind==8: out+=[(c>>i)&1 for i in range(8)]
        else:
            v=c
            while True:
                ch=v&31; v>>=5
                ch=ch|32 if v else ch
                out+=[(ch>>i)&1 for i in range(6)]
                if not v: break
    return np.array(out,dtype=np.uint8)
done=0
for old,new in (('i8:32','i8:08'),('i1:32','i1:08'),('i16:32','i16:16')):
    for k in (8,6):
        o=enc(old,k); nw=enc(new,k)
        if len(o)!=len(nw): continue
        w=len(o)
        win=np.lib.stride_tricks.sliding_window_view(bits,w)
        hit=np.nonzero((win==o).all(axis=1))[0]
        for h in hit[:4]: bits[h:h+w]=nw; done+=1
open(sys.argv[2],'wb').write(np.packbits(bits,bitorder='little').tobytes())
print('rewrites',done)
