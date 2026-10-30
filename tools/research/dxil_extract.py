#!/usr/bin/env python3
"""Research tool (read-only): extract every DXBC container's DXIL bitcode from witcher3.exe.
usage: dxil_extract.py exe outdir  -> outdir/NNN_<rva>.bc + index.tsv (rva, size, exported functions after llvm-dis)"""
import sys,struct,os,re,subprocess
f=open(sys.argv[1],'rb').read(); out=sys.argv[2]; os.makedirs(out,exist_ok=True)
idx=open(os.path.join(out,'index.tsv'),'w'); n=0
for m in re.finditer(b'DXBC',f):
    o=m.start()
    try: size,parts=struct.unpack_from('<II',f,o+24)
    except struct.error: continue
    if size<64 or size>64<<20 or parts>64: continue
    for p in range(parts):
        po=o+struct.unpack_from('<I',f,o+32+4*p)[0]
        fourcc=f[po:po+4]
        if fourcc!=b'DXIL': continue
        d=po+8
        bo,bs=struct.unpack_from('<II',f,d+16)
        bc=f[d+8+bo:d+8+bo+bs]
        if bc[:4]!=b'BC\xc0\xde': continue
        rva=o-0x298a800+0x298c000
        name=os.path.join(out,'%03d_%x.bc'%(n,rva)); open(name,'wb').write(bc)
        ll=subprocess.run(['llvm-dis','-o','-',name],capture_output=True,text=True).stdout
        if not ll:   # older DXIL datalayout (i8:32 ...) rejected by LLVM 18: disassemble a dlfix copy (analysis only)
            fx=name[:-3]+'.fix.bc'
            subprocess.run([sys.executable,'-I',os.path.join(os.path.dirname(os.path.abspath(__file__)),'dxil_dlfix.py'),name,fx],capture_output=True)
            ll=subprocess.run(['llvm-dis','-o','-',fx],capture_output=True,text=True).stdout
        fns=re.findall(r'^define [^@]*@"?([^"(]+)"?\(',ll,re.M)
        open(name[:-3]+'.ll','w').write(ll)
        idx.write('%x\t%d\t%s\n'%(rva,size,','.join(fns))); n+=1
print(n,'DXIL modules')
