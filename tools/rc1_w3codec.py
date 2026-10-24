"""Lossless v164 RTSW reader/writer restricted to existing Refined Tracing files.
Uses established Witcher RTSW block layout/bit6 and per-language rotate-XOR.
Never alters string IDs, hashes, ordering, other text or file version.
"""
import struct
from pathlib import Path
from dataclasses import dataclass

MAGIC = {'ar':0,'br':0,'cn':0,'cz':0x21793217,'de':0x42791159,'en':0x79321793,'es':0x42387566,'esmx':0,'fr':0x75921975,'hu':0x67823218,'it':0x12375973,'jp':0x59825646,'kr':0,'pl':0x73946816,'ru':0x42386347,'tr':0,'ua':0,'zh':0x16875467}
KEY = {'ar':0,'br':0,'cn':0,'cz':0x24987354,'de':0x75886138,'en':0x43975139,'es':0x18796651,'esmx':0,'fr':0x23863176,'hu':0x42378932,'it':0x45931894,'jp':0x54834893,'kr':0,'pl':0x83496237,'ru':0x63481486,'tr':0,'ua':0,'zh':0x18632176}

def read_bit6(b,p):
    out=0; shift=0; i=1
    while True:
        byt=b[p];p+=1
        if byt>127: mask=127;bits=7
        elif byt>63: mask=63 if i==1 else 255;bits=6
        else: mask=255;bits=6
        out |= (byt&mask)<<shift; shift+=bits
        if byt<64 or (i>=3 and byt<128): break
        i+=1
    return out,p

def write_bit6(c):
    if c==0:return b'\x80'
    seq=[]; left=c; i=0
    while left>0:
        if i==0:seq.append(left&63);left>>=6
        else:seq.append(left&255);left>>=7
        i+=1
    # Two-group lengths with a high final byte need an explicit terminal
    # group, otherwise the reader treats that high byte as continuation.
    if len(seq)==2 and seq[1]>=64:seq.append(0)
    buf=[]
    for i,val in enumerate(seq):
        if i!=len(seq)-1:
            if len(seq)-1-i>=1 and i>=1:val|=128
            elif val<64:val|=64
            else:val|=128
        # 0x80 is a legal continuation when the middle group is zero.
        buf.append(val)
    return bytes(buf)

@dataclass
class W3:
    version:int
    key:int
    blocks:list
    block2:list
    buffer_count:int
    extra:bytes


def transform(arr,magic,encode):
    init=(magic>>8)&0xffff
    key=init
    length=len(arr)
    out=[]
    for unit in arr:
        out.append((unit ^ (((length+1)*key)&0xff)) & 0xff)
        key=((key<<1)|(key>>15)) & 0xffff
    return out

def decode(blob, lang):
    assert blob[:4]==b'RTSW'
    v=struct.unpack_from('<I',blob,4)[0]
    assert v==164,(lang,v)
    key1=struct.unpack_from('<H',blob,8)[0];key2=struct.unpack_from('<H',blob,len(blob)-2)[0]
    key=(key1<<16)|key2
    assert key==KEY[lang],(lang,hex(key),hex(KEY[lang]))
    magic=MAGIC[lang]
    p=10
    n,p=read_bit6(blob,p);blocks=[]
    for _ in range(n):
        raw_id,offset,strlen=struct.unpack_from('<III',blob,p);p+=12
        blocks.append([raw_id^magic, offset,strlen,None])
    n2,p=read_bit6(blob,p);block2=[]
    for _ in range(n2):
        h,raw_id=struct.unpack_from('<II',blob,p);p+=8
        block2.append([h,raw_id^magic])
    chars,p=read_bit6(blob,p)
    end=p+chars
    assert end+2<=len(blob),(lang,chars,len(blob),p)
    extra=blob[end:-2]
    for row in blocks:
        ident,off,length,_=row
        l=list(blob[p+off:p+off+length])
        decoded=transform(l,magic,False)
        row[3]=bytes(decoded).decode('utf-8')
        assert blob[p+off+length:p+off+length+1]==b'\x00',('missing null',lang,ident)
    return W3(v,key,blocks,block2,chars,extra)

def encode(obj,lang):
    mag=MAGIC[lang]
    out=bytearray(b'RTSW'+struct.pack('<I',obj.version)+struct.pack('<H',obj.key>>16))
    blocks=sorted(obj.blocks,key=lambda a:a[0]^mag)
    buffer=bytearray();rec=[]
    for ident,old_off,old_len,s in blocks:
        data=s.encode('utf-8');units=list(data)
        cipher=transform(units,mag,True)
        rec.append((ident^mag,len(buffer),len(units)))
        buffer+=bytes(x&255 for x in cipher)
        buffer+=b'\x00'
    out+=write_bit6(len(blocks))
    for a,b,c in rec:out+=struct.pack('<III',a,b,c)
    out+=write_bit6(len(obj.block2))
    for h,ident in sorted(obj.block2,key=lambda a:a[0]):out+=struct.pack('<II',h,ident^mag)
    out+=write_bit6(len(buffer))
    out+=buffer
    out+=obj.extra
    out+=struct.pack('<H',obj.key&0xffff)
    return bytes(out)

if __name__=='__main__':
    folder=Path('/mnt/data/RC1_TOOLTIP_FINAL_WORK/main/mods/modRefinedTracing/content')
    for f in folder.glob('*.w3strings'):
        b=f.read_bytes();obj=decode(b,f.stem);out=encode(obj,f.stem)
        print(f.stem, len(b), len(obj.blocks), len(obj.block2),len(obj.extra), 'roundtrip',b==out,
              'GEOM', [(hex(x[0]),x[3][:45]) for x in obj.blocks if x[3].startswith('PT only. Controls indirect-light bounces for geometry')])
        if b!=out:
            q=next((i for i,(x,y) in enumerate(zip(b,out)) if x!=y),-1)
            print('mismatch byte',q, b[q-6:q+9].hex(),out[q-6:q+9].hex());raise SystemExit(1)
