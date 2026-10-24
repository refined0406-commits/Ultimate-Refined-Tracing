#!/usr/bin/env python3
"""Research tool: disassemble an RVA range of witcher3.exe and annotate RIP-relative
targets with CVar names (value slots) and string literals. Usage: annotate.py exe table.tsv start_rva end_rva"""
import sys,subprocess,re,struct
exe,tab,a,b=sys.argv[1],sys.argv[2],int(sys.argv[3],16),int(sys.argv[4],16)
B=0x140000000
names={}
for l in open(tab):
    p=l.rstrip('\n').split('\t')
    if len(p)>=8: names[int(p[1],16)]=p[2]+'/'+p[3]
f=open(exe,'rb').read()
pe=struct.unpack_from('<I',f,0x3c)[0];n=struct.unpack_from('<H',f,pe+6)[0];osz=struct.unpack_from('<H',f,pe+20)[0]
secs=[];o=pe+24+osz
for i in range(n):
    vs,va,rs,ro=struct.unpack_from('<IIII',f,o+8);secs.append((va,vs,ro,rs));o+=40
def s(r):
    for va,vs,ro,rs in secs:
        if va<=r<va+rs:
            q=ro+r-va;e=f.find(b'\0',q,q+120);t=f[q:e]
            if len(t)>=4 and all(32<=c<127 for c in t):return '"'+t.decode()+'"'
            if r>=0x298c000 and r<0x3ff2000:
                return 'f32=%g'%struct.unpack_from('<f',f,q)[0]
    return ''
out=subprocess.run(['llvm-objdump','-d','--no-show-raw-insn','-M','intel','--start-address=%#x'%(B+a),'--stop-address=%#x'%(B+b),exe],capture_output=True,text=True).stdout
for l in out.splitlines():
    m=re.search(r'#\s*0x([0-9a-f]+)',l)
    if m:
        t=int(m.group(1),16)-B
        l=l+'   <'+(names.get(t) or s(t) or hex(t))+'>'
    if ':' in l and l.strip()[:1] in '0123456789abcdef': print(l.rstrip())
