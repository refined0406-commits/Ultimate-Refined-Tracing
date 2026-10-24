#!/usr/bin/env python3
"""Research tool: for each named CVar (category/name), show every verified access with
+-N annotated instructions of context. Usage: readsites.py exe table xrefs.tsv N name1 name2 ..."""
import sys,subprocess,re
exe,tab,xr,N=sys.argv[1],sys.argv[2],sys.argv[3],int(sys.argv[4]);want=sys.argv[5:]
B=0x140000000;slot={};names={}
for l in open(tab):
    p=l.rstrip('\n').split('\t')
    if len(p)>=8: k=p[2]+'/'+p[3];slot[k]=int(p[1],16);names[int(p[1],16)]=k
acc=[l.rstrip('\n').split('\t',4) for l in open(xr)]
for w in want:
    s=slot[w];print('#'*8,w,hex(s))
    for t,fn,ia,a,ins in acc:
        if int(t,16)!=s:continue
        ia=int(ia,16)
        out=subprocess.run(['python3',sys.path[0]+'/annotate.py',exe,tab,'%x'%max(int(fn,16),ia-6*N),'%x'%(ia+5*N)],capture_output=True,text=True).stdout.splitlines()[1:]
        print('  -- func',fn,a,'at %x'%ia)
        for l in out:
            if 'int3' in l: continue
            print('   ',re.sub(r'^\s*14','',l)[:150])
