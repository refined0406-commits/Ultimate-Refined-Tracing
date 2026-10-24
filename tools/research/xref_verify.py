#!/usr/bin/env python3
"""Research tool: verify brute-force xrefs by disassembling the containing function (.pdata bounds)
with llvm-objdump and keeping only instructions whose RIP-relative target equals the CVar value RVA.
Output TSV: target_rva func_start instr_rva access(R/W/RW/A) instruction"""
import struct,sys,subprocess,bisect,re,collections,concurrent.futures as cf
exe,raw,out=sys.argv[1:4]
f=open(exe,'rb').read()
pe=struct.unpack_from('<I',f,0x3c)[0]; n=struct.unpack_from('<H',f,pe+6)[0]; osz=struct.unpack_from('<H',f,pe+20)[0]
opt=pe+24; base=struct.unpack_from('<Q',f,opt+24)[0]
secs=[];o=opt+osz
for i in range(n):
    vs,va,rs,ro=struct.unpack_from('<IIII',f,o+8);secs.append((va,vs,ro,rs));o+=40
def off(r):
    for va,vs,ro,rs in secs:
        if va<=r<va+rs:return ro+r-va
prva,psz=struct.unpack_from('<II',f,opt+112+3*8)
po=off(prva); funcs=sorted({struct.unpack_from('<II',f,po+i)[0:2] for i in range(0,psz,12)})
starts=[a for a,b in funcs]
refs=collections.defaultdict(set)
tsec=secs[0]; text=f[tsec[2]:tsec[2]+tsec[3]]
def leaf_bounds(p):
    # Leaf functions carry no .pdata entry: bound them by int3 (0xCC) padding runs.
    q=p-tsec[0]; a=q
    while a>0 and q-a<0x4000 and not (text[a-1]==0xCC and text[a-2]==0xCC): a-=1
    b=q
    while b<len(text)-1 and b-q<0x4000 and not (text[b]==0xCC and text[b+1]==0xCC): b+=1
    return (tsec[0]+a,tsec[0]+b)
leaf_count=0
for line in open(raw):
    t,p,k=line.split();t=int(t,16);p=int(p,16)
    i=bisect.bisect_right(starts,p)-1
    if i>=0 and funcs[i][0]<=p<funcs[i][1]: refs[funcs[i]].add(t)
    else: refs[leaf_bounds(p)].add(t); leaf_count+=1
def dis(fn):
    a,b=fn; tg=refs[fn]
    r=subprocess.run(['llvm-objdump','-d','--no-show-raw-insn','-M','intel',f'--start-address={base+a:#x}',f'--stop-address={base+b:#x}',exe],capture_output=True,text=True).stdout
    res=[]
    for l in r.splitlines():
        m=re.search(r'#\s*0x([0-9a-f]+)',l)
        if not m:continue
        t=int(m.group(1),16)-base
        if t in tg:
            ins=l.split(':',1)[1].strip(); ins=re.sub(r'\s+',' ',ins.split('#')[0]).strip()
            ia=int(l.split(':')[0].strip(),16)-base
            mn=ins.split(' ')[0]; dst=ins[len(mn):].split(',')[0]
            acc='W' if ('ptr [rip' in dst and mn.startswith(('vmov','mov','movss','movsd','movd','movq','movups','movaps','and','or','xor','add','sub','inc','dec','lock','xchg','cmpxchg','setb'))) else ('A' if mn=='lea' else 'R')
            res.append((t,a,ia,acc,ins))
    return res
with cf.ThreadPoolExecutor(16) as ex, open(out,'w') as w:
    for res in ex.map(dis,list(refs)):
        for t,a,ia,acc,ins in res: w.write(f'{t:07x}\t{a:07x}\t{ia:07x}\t{acc}\t{ins}\n')
print('functions',len(refs),'refs outside .pdata (leaf):',leaf_count)
