"""Research tool (read-only): for each TraceRay in a DXIL function: constant-buffer fields of the conditional branches that dominate it.
usage: gates.py file.ll"""
import re,sys,collections
txt=open(sys.argv[1]).read()
funcs=re.findall(r'^define [^\n]*@"([^"]+)"[^\n]*\{\n(.*?)^\}',txt,re.S|re.M)
for fname,body in funcs:
    lines=body.split('\n')
    blocks=collections.OrderedDict(); cur='entry'; blocks[cur]=[]
    for l in lines:
        m=re.match(r'^(\d+):',l) or re.match(r'^([\w.]+):',l)
        if m and not l.startswith(' '): cur=m.group(1); blocks[cur]=[]; continue
        blocks[cur].append(l)
    # value tracking
    cbreg={};field={};handle_of={}
    deps=collections.defaultdict(set)   # value -> set of fields it derives from (shallow propagation)
    for b,ls in blocks.items():
        for l in ls:
            m=re.match(r'\s*%(\S+) = call %dx\.types\.CBufRet\.\w+ @dx\.op\.cbufferLoadLegacy\.\w+\(i32 59, %dx\.types\.Handle %(\S+), i32 (\d+)\)',l)
            if m: cbreg[m.group(1)]=(m.group(2),int(m.group(3))); continue
            m=re.match(r'\s*%(\S+) = extractvalue %dx\.types\.CBufRet\.\w+ %(\S+), (\d+)',l)
            if m and m.group(2) in cbreg:
                h,r=cbreg[m.group(2)]; deps[m.group(1)].add((h,r,int(m.group(3)))); continue
            m=re.match(r'\s*%(\S+) = (?:icmp|fcmp|and|or|xor|trunc|zext|sext|bitcast|lshr|shl|select|fptoui|fptosi|uitofp|sitofp|fadd|fmul|add|sub)\b(.*)',l)
            if m:
                for u in re.findall(r'%([\w.]+)',m.group(2)):
                    if u in deps: deps[m.group(1)]|=deps[u]
    # CFG
    succ={};conds={}
    for b,ls in blocks.items():
        t=[x for x in ls if x.strip()]
        last=t[-1] if t else ''
        m=re.match(r'\s*br i1 %(\S+), label %(\S+), label %(\S+)',last)
        if m: succ[b]=[m.group(2),m.group(3)]; conds[b]=(m.group(1),m.group(2),m.group(3)); continue
        m=re.match(r'\s*br label %(\S+)',last)
        if m: succ[b]=[m.group(1)]; continue
        m=re.match(r'\s*switch .*?label %(\S+) \[',last)
        succ[b]=re.findall(r'label %([\w.]+)',last) if 'switch' in last else []
    names=list(blocks)
    pred=collections.defaultdict(list)
    for b,ss in succ.items():
        for s in ss: pred[s].append(b)
    dom={b:set(names) for b in names}; dom[names[0]]={names[0]}
    changed=True
    while changed:
        changed=False
        for b in names[1:]:
            ps=[dom[p] for p in pred[b] if p in dom]
            nd={b}|(set.intersection(*ps) if ps else set())
            if nd!=dom[b]: dom[b]=nd; changed=True
    # reachability helper to determine which side of a branch leads to b
    def reach(a,target,stop):
        seen=set();st=[a]
        while st:
            x=st.pop()
            if x==target: return True
            if x in seen or x==stop: continue
            seen.add(x); st.extend(succ.get(x,[]))
        return False
    print('==',fname,'blocks',len(blocks))
    n=0
    for b,ls in blocks.items():
        for l in ls:
            if '@dx.op.traceRay' in l:
                n+=1
                m=re.search(r'traceRay\.(\S+)\(i32 157, %dx\.types\.Handle %\S+, i32 ([^,]+), i32 ([^,]+),',l)
                g=[]
                for d in names:
                    if d in dom[b] and d!=b and d in conds:
                        c,t,f=conds[d]
                        if deps.get(c):
                            side='T' if (t in dom[b] or reach(t,b,d)) and not reach(f,b,d) else ('F' if reach(f,b,d) and not reach(t,b,d) else '?')
                            g.append('%s%s'%(side,sorted((r,cmp) for h,r,cmp in deps[c])))
                print(' trace#%d blk %s payload=%s flags=%s mask=%s gates=%s'%(n,b,m.group(1) if m else '?',m.group(2) if m else '?',m.group(3) if m else '?',' '.join(g)))
