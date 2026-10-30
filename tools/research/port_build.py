#!/usr/bin/env python3
"""Port URT's pinned Patch 5.01 addresses from one witcher3.exe build to another (research tool, R12).

For every site RVA (code or data) of the old build:
  * code: the old function containing it (.pdata) is disassembled (objdump text of the whole exe, read by address),
    normalised (rip-relative displacements and branch targets made symbolic: rip targets compared through the
    mapping being built, in-function branches as function offsets), and matched against the new build's functions
    near the same .pdata index; an exact normalised match gives the new site = new function + same offset;
  * data: functions of the old build that reference it (rip-relative) are matched the same way; the matched
    instruction's rip target in the new build is the new address (all referencing matches must agree).
Usage: port_build.py old.exe old.asm new.exe new.asm sites.txt out.tsv
sites.txt: '<hex rva> <files...>' per line. out.tsv: old, new, kind, how, function sizes, note."""
import bisect, re, struct, subprocess, sys

def pe(path):
    d = open(path, 'rb').read()
    p = struct.unpack_from('<I', d, 0x3c)[0]; ns = struct.unpack_from('<H', d, p + 6)[0]; oh = struct.unpack_from('<H', d, p + 20)[0]
    secs = []
    for i in range(ns):
        o = p + 24 + oh + i * 40
        nm = d[o:o + 8].rstrip(b'\0').decode(); vs, va, rs, ra = struct.unpack_from('<IIII', d, o + 8); secs.append((nm, va, vs, rs, ra))
    pd = next(s for s in secs if s[0] == '.pdata')
    funcs = []
    for k in range(pd[3] // 12):
        b, e, u = struct.unpack_from('<III', d, pd[4] + 12 * k)
        if b and e > b: funcs.append((b, e))
    funcs.sort()
    return d, secs, funcs

class Asm:
    def __init__(self, path):
        self.f = open(path, 'rb'); self.f.seek(0, 2); self.size = self.f.tell()
    def _addr_at(self, off):
        self.f.seek(off); self.f.readline()
        while True:
            pos = self.f.tell(); l = self.f.readline()
            if not l: return None, pos
            m = re.match(rb'\s+([0-9a-f]+):', l)
            if m: return int(m.group(1), 16), pos
    def lines(self, start, end):                          # VAs
        lo, hi = 0, self.size
        while hi - lo > 4096:
            mid = (lo + hi) // 2
            a, _ = self._addr_at(mid)
            if a is None or a >= start: hi = mid
            else: lo = mid
        self.f.seek(lo); self.f.readline(); out = []
        for l in self.f:
            m = re.match(rb'\s+([0-9a-f]+):\t(.*)', l)
            if not m: continue
            a = int(m.group(1), 16)
            if a < start: continue
            if a >= end: break
            out.append((a, m.group(2).decode('latin-1').rstrip()))
        return out

B = 0x140000000
def norm(ins, fs, fe):
    """[(va, normalised text, rip target or None, branch target or None)]"""
    out = []
    for a, t in ins:
        rip = None
        m = re.search(r'#\s*0x([0-9a-f]+)', t)
        if 'rip' in t and m: rip = int(m.group(1), 16)
        t2 = re.sub(r'\s*#.*$', '', t)
        t2 = re.sub(r'\[rip\+0x[0-9a-f]+\]', '[rip+R]', t2); t2 = re.sub(r'\[rip-0x[0-9a-f]+\]', '[rip+R]', t2)
        br = None
        m2 = re.match(r'(call|j[a-z]+)\s+0x([0-9a-f]+)$', t2)
        if m2:
            tgt = int(m2.group(2), 16); br = tgt
            t2 = m2.group(1) + (' F+%x' % (tgt - fs) if fs <= tgt < fe else ' X')
        t2 = re.sub(r'\s+', ' ', t2)
        out.append((a, t2, rip, br))
    return out

def main():
    oexe, oasm, nexe, nasm, sites, outp = sys.argv[1:7]
    od, osecs, of = pe(oexe); nd, nsecs, nf = pe(nexe)
    OA, NA = Asm(oasm), Asm(nasm)
    obeg = [b for b, e in of]; nbeg = [b for b, e in nf]
    tsec = next(s for s in osecs if s[0] == '.text')
    cache = {}
    def fn_old(r):
        i = bisect.bisect_right(obeg, r) - 1
        return (i, of[i]) if i >= 0 and of[i][0] <= r < of[i][1] else (None, None)
    def nins(which, f):
        k = (which, f)
        if k not in cache:
            A = OA if which == 'o' else NA
            cache[k] = norm(A.lines(B + f[0], B + f[1]), B + f[0], B + f[1])
        return cache[k]
    def match_fn(i, f):
        """exact normalised match of old function f among new functions near index i"""
        oi = nins('o', f); size = f[1] - f[0]; hits = []
        lo = max(0, i - 2500); hi = min(len(nf), i + 2500)
        for j in range(lo, hi):
            g = nf[j]
            if g[1] - g[0] != size: continue
            ni = nins('n', g)
            if len(ni) == len(oi) and all(x[1] == y[1] for x, y in zip(oi, ni)): hits.append(g)
        return hits, oi
    res = []
    datamap = {}
    def learn(oi, ni):
        for x, y in zip(oi, ni):
            if x[2] is not None and y[2] is not None: datamap.setdefault(x[2] - B, set()).add(y[2] - B)
            if x[3] is not None and y[3] is not None and x[1].endswith(' X'): datamap.setdefault(x[3] - B, set()).add(y[3] - B)
    for line in open(sites):
        if not line.strip(): continue
        r = int(line.split()[0], 16); files = ' '.join(line.split()[1:])
        if tsec[1] <= r < tsec[1] + tsec[2]:
            i, f = fn_old(r)
            if f is None: res.append((r, None, 'code', 'no .pdata function', files)); continue
            hits, oi = match_fn(i, f)
            if len(hits) == 1:
                g = hits[0]; learn(oi, nins('n', g))
                off = r - f[0]
                # the site must be an instruction boundary in both (or a byte inside one, same offset)
                res.append((r, g[0] + off, 'code', 'fn %x+%x exact (%d insns)' % (f[0], off, len(oi)), files))
            else:
                res.append((r, None, 'code', 'fn %x size %x: %d exact matches' % (f[0], f[1] - f[0], len(hits)), files))
        else:
            res.append((r, None, 'data', 'pending', files))
    # data: from referencing functions
    pending = [x for x in res if x[2] == 'data']
    for k, (r, _, kind, how, files) in enumerate(res):
        if kind != 'data': continue
        if r in datamap and len(datamap[r]) == 1:
            res[k] = (r, next(iter(datamap[r])), 'data', 'via site functions', files); continue
        out = subprocess.run(['grep', '-m', '12', '-F', '# 0x%x' % (B + r), oasm], capture_output=True, text=True).stdout.splitlines()
        got = set(); used = 0
        for l in out:
            m = re.match(r'\s+([0-9a-f]+):', l)
            if not m: continue
            a = int(m.group(1), 16) - B
            i, f = fn_old(a)
            if f is None: continue
            hits, oi = match_fn(i, f)
            if len(hits) != 1: continue
            ni = nins('n', hits[0]); learn(oi, ni); used += 1
            for x, y in zip(oi, ni):
                if x[0] - B == a and x[2] is not None and y[2] is not None: got.add(y[2] - B - (x[2] - B - r))
            if used >= 4: break
        if len(got) == 1: res[k] = (r, got.pop(), 'data', 'via %d referencing functions' % used, files)
        else: res[k] = (r, None, 'data', 'unresolved (%d refs matched, %d candidates)' % (used, len(got)), files)
    with open(outp, 'w') as o:
        for r, n, kind, how, files in res:
            o.write('%x\t%s\t%s\t%s\t%s\n' % (r, '%x' % n if n is not None else '-', kind, how, files))
    print('resolved %d / %d' % (sum(1 for x in res if x[1] is not None), len(res)))

if __name__ == '__main__':
    main()
