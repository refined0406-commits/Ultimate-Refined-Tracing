"""Minimal read-only AVM2 (ActionScript 3 bytecode) reader used by the RC5 separator test.
URT's own code; it only parses and disassembles, it never writes a SWF."""
import struct
class R:
    def __init__(s,b,o=0): s.b=b; s.o=o
    def u8(s): v=s.b[s.o]; s.o+=1; return v
    def u16(s): v=struct.unpack_from('<H',s.b,s.o)[0]; s.o+=2; return v
    def s24(s): v=int.from_bytes(s.b[s.o:s.o+3],'little',signed=True); s.o+=3; return v
    def u30(s):
        v=0; sh=0
        while True:
            c=s.b[s.o]; s.o+=1; v|=(c&0x7f)<<sh; sh+=7
            if not c&0x80: break
        return v & 0xffffffff
    def s32(s):
        v=s.u30()
        return v-(1<<32) if v&0x80000000 else v
    def d64(s): v=struct.unpack_from('<d',s.b,s.o)[0]; s.o+=8; return v
class ABC:
    def __init__(s,b):
        r=R(b); r.u16(); r.u16()
        n=r.u30(); s.ints=[0]+[r.s32() for _ in range(max(n-1,0))]
        n=r.u30(); s.uints=[0]+[r.u30() for _ in range(max(n-1,0))]
        n=r.u30(); s.dbls=[0]+[r.d64() for _ in range(max(n-1,0))]
        n=r.u30(); s.strs=['*']
        for _ in range(max(n-1,0)):
            L=r.u30(); s.strs.append(b[r.o:r.o+L].decode('utf8','replace')); r.o+=L
        n=r.u30(); s.ns=[None]
        for _ in range(max(n-1,0)): k=r.u8(); s.ns.append((k,s.strs[r.u30()]))
        n=r.u30(); s.nss=[None]
        for _ in range(max(n-1,0)): c=r.u30(); s.nss.append([r.u30() for _ in range(c)])
        n=r.u30(); s.mn=[None]
        for _ in range(max(n-1,0)):
            k=r.u8()
            if k in (7,13): ns=r.u30(); nm=r.u30(); s.mn.append(('Q',ns,s.strs[nm]))
            elif k in (15,16): s.mn.append(('RTQ',s.strs[r.u30()]))
            elif k in (17,18): s.mn.append(('RTQL',))
            elif k in (9,14): nm=r.u30(); nss=r.u30(); s.mn.append(('M',s.strs[nm]))
            elif k in (27,28): r.u30(); s.mn.append(('ML',))
            elif k==29: t=r.u30(); c=r.u30(); ps=[r.u30() for _ in range(c)]; s.mn.append(('G',t,ps))
            else: raise Exception('mn kind %d'%k)
        n=r.u30(); s.methods=[]
        for _ in range(n):
            pc=r.u30(); r.u30(); [r.u30() for _ in range(pc)]; nm=r.u30(); fl=r.u8()
            if fl&8:
                oc=r.u30()
                for _ in range(oc): r.u30(); r.u8()
            if fl&0x80: [r.u30() for _ in range(pc)]
            s.methods.append(s.strs[nm])
        n=r.u30()
        for _ in range(n):
            r.u30(); c=r.u30()
            for _ in range(c): r.u30(); r.u30()
        n=r.u30(); s.inst=[]
        for _ in range(n):
            nm=r.u30(); sup=r.u30(); fl=r.u8()
            if fl&8: r.u30()
            ic=r.u30(); [r.u30() for _ in range(ic)]
            iinit=r.u30(); tr=s.traits(r)
            s.inst.append(dict(name=s.mname(nm),sup=s.mname(sup),iinit=iinit,traits=tr))
        s.cls=[]
        for _ in range(n):
            cinit=r.u30(); s.cls.append(dict(cinit=cinit,traits=s.traits(r)))
        n=r.u30()
        for _ in range(n): r.u30(); s.traits(r)
        n=r.u30(); s.bodies={}
        for _ in range(n):
            m=r.u30(); ms=r.u30(); lc=r.u30(); isd=r.u30(); msd=r.u30(); L=r.u30()
            code=b[r.o:r.o+L]; r.o+=L
            ec=r.u30()
            for _ in range(ec): r.u30(); r.u30(); r.u30(); r.u30(); r.u30()
            s.traits(r)
            s.bodies[m]=code
    def mname(s,i):
        if i==0: return '*'
        m=s.mn[i]
        if m[0]=='Q': return m[2]
        if m[0] in ('RTQ','M'): return m[1]
        if m[0]=='G': return s.mname(m[1])+'.<'+','.join(s.mname(p) for p in m[2])+'>'
        return str(m)
    def traits(s,r):
        out=[]
        c=r.u30()
        for _ in range(c):
            nm=r.u30(); k=r.u8(); kind=k&15
            if kind in (0,6):
                r.u30(); r.u30(); vi=r.u30()
                if vi: r.u8()
                out.append(('slot',s.mname(nm)))
            elif kind in (1,2,3):
                r.u30(); mi=r.u30(); out.append((['','method','getter','setter'][kind],s.mname(nm),mi))
            elif kind==4: r.u30(); r.u30(); out.append(('class',s.mname(nm)))
            elif kind==5: r.u30(); mi=r.u30(); out.append(('function',s.mname(nm),mi))
            if k&0x40:
                mc=r.u30(); [r.u30() for _ in range(mc)]
        return out
# opcode table: name, operand kinds (u=u30, b=u8, s=s24 branch, S=switch)
OPS={0x01:('bkpt',''),0x02:('nop',''),0x03:('throw',''),0x04:('getsuper','m'),0x05:('setsuper','m'),0x06:('dxns','u'),0x07:('dxnslate',''),
0x08:('kill','u'),0x09:('label',''),0x0c:('ifnlt','s'),0x0d:('ifnle','s'),0x0e:('ifngt','s'),0x0f:('ifnge','s'),0x10:('jump','s'),0x11:('iftrue','s'),
0x12:('iffalse','s'),0x13:('ifeq','s'),0x14:('ifne','s'),0x15:('iflt','s'),0x16:('ifle','s'),0x17:('ifgt','s'),0x18:('ifge','s'),0x19:('ifstricteq','s'),
0x1a:('ifstrictne','s'),0x1b:('lookupswitch','S'),0x1c:('pushwith',''),0x1d:('popscope',''),0x1e:('nextname',''),0x1f:('hasnext',''),0x20:('pushnull',''),
0x21:('pushundefined',''),0x23:('nextvalue',''),0x24:('pushbyte','b'),0x25:('pushshort','u'),0x26:('pushtrue',''),0x27:('pushfalse',''),0x28:('pushnan',''),
0x29:('pop',''),0x2a:('dup',''),0x2b:('swap',''),0x2c:('pushstring','T'),0x2d:('pushint','I'),0x2e:('pushuint','u'),0x2f:('pushdouble','D'),
0x30:('pushscope',''),0x31:('pushnamespace','u'),0x32:('hasnext2','uu'),0x40:('newfunction','u'),0x41:('call','u'),0x42:('construct','u'),
0x43:('callmethod','uu'),0x44:('callstatic','uu'),0x45:('callsuper','mu'),0x46:('callproperty','mu'),0x47:('returnvoid',''),0x48:('returnvalue',''),
0x49:('constructsuper','u'),0x4a:('constructprop','mu'),0x4c:('callproplex','mu'),0x4e:('callsupervoid','mu'),0x4f:('callpropvoid','mu'),0x53:('applytype','u'),
0x55:('newobject','u'),0x56:('newarray','u'),0x57:('newactivation',''),0x58:('newclass','u'),0x59:('getdescendants','m'),0x5a:('newcatch','u'),
0x5d:('findpropstrict','m'),0x5e:('findproperty','m'),0x5f:('finddef','m'),0x60:('getlex','m'),0x61:('setproperty','m'),0x62:('getlocal','u'),0x63:('setlocal','u'),
0x64:('getglobalscope',''),0x65:('getscopeobject','b'),0x66:('getproperty','m'),0x68:('initproperty','m'),0x6a:('deleteproperty','m'),0x6c:('getslot','u'),
0x6d:('setslot','u'),0x6e:('getglobalslot','u'),0x6f:('setglobalslot','u'),0x70:('convert_s',''),0x71:('esc_xelem',''),0x72:('esc_xattr',''),0x73:('convert_i',''),
0x74:('convert_u',''),0x75:('convert_d',''),0x76:('convert_b',''),0x77:('convert_o',''),0x78:('checkfilter',''),0x80:('coerce','m'),0x81:('coerce_b',''),
0x82:('coerce_a',''),0x83:('coerce_i',''),0x84:('coerce_d',''),0x85:('coerce_s',''),0x86:('astype','m'),0x87:('astypelate',''),0x88:('coerce_u',''),0x89:('coerce_o',''),
0x90:('negate',''),0x91:('increment',''),0x92:('inclocal','u'),0x93:('decrement',''),0x94:('declocal','u'),0x95:('typeof',''),0x96:('not',''),0x97:('bitnot',''),
0xa0:('add',''),0xa1:('subtract',''),0xa2:('multiply',''),0xa3:('divide',''),0xa4:('modulo',''),0xa5:('lshift',''),0xa6:('rshift',''),0xa7:('urshift',''),
0xa8:('bitand',''),0xa9:('bitor',''),0xaa:('bitxor',''),0xab:('equals',''),0xac:('strictequals',''),0xad:('lessthan',''),0xae:('lessequals',''),
0xaf:('greaterthan',''),0xb0:('greaterequals',''),0xb1:('instanceof',''),0xb2:('istype','m'),0xb3:('istypelate',''),0xb4:('in',''),0xc0:('increment_i',''),
0xc1:('decrement_i',''),0xc2:('inclocal_i','u'),0xc3:('declocal_i','u'),0xc4:('negate_i',''),0xc5:('add_i',''),0xc6:('subtract_i',''),0xc7:('multiply_i',''),
0xd0:('getlocal_0',''),0xd1:('getlocal_1',''),0xd2:('getlocal_2',''),0xd3:('getlocal_3',''),0xd4:('setlocal_0',''),0xd5:('setlocal_1',''),0xd6:('setlocal_2',''),
0xd7:('setlocal_3',''),0xef:('debug','bubu'),0xf0:('debugline','u'),0xf1:('debugfile','u'),0x35:('li8',''),0x36:('li16',''),0x37:('li32',''),0x38:('lf32',''),0x39:('lf64',''),
0x3a:('si8',''),0x3b:('si16',''),0x3c:('si32',''),0x3d:('sf32',''),0x3e:('sf64',''),0x50:('sxi1',''),0x51:('sxi8',''),0x52:('sxi16','')}
def dis(a,code):
    r=R(code); out=[]
    while r.o<len(code):
        pos=r.o; op=r.u8(); nm,fmt=OPS.get(op,('op%02x'%op,''))
        args=[]
        for f in fmt:
            if f=='u': args.append(r.u30())
            elif f=='b': args.append(r.u8())
            elif f=='s': args.append('->%d'%(r.o+3+r.s24()))
            elif f=='m': args.append(a.mname(r.u30()))
            elif f=='T': args.append(repr(a.strs[r.u30()]))
            elif f=='I': args.append(a.ints[r.u30()])
            elif f=='D': args.append(a.dbls[r.u30()])
            elif f=='S':
                d=r.s24(); c=r.u30(); [r.s24() for _ in range(c+1)]; args.append('switch')
        out.append('%5d %s %s'%(pos,nm,' '.join(map(str,args))))
    return out
