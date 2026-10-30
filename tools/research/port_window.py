#!/usr/bin/env python3
"""Second pass of port_build.py for sites whose function changed size or has no .pdata entry (research tool, R12).
For each old site: the normalised instruction window [site-BEFORE insns, site+AFTER insns] of the old build is
searched in the new build's disassembly around site+delta (delta from the nearest resolved site), rip
displacements and out-of-window branch targets masked. Reports the best match, its length on each side and
whether it is unique.
Usage: port_window.py old.asm new.asm resolved.tsv sites(hex...) """
import bisect, re, sys
sys.path.insert(0, __import__('os').path.dirname(__file__))
from port_build import Asm, B

def norm1(t, lo, hi, base):
    t2 = re.sub(r'\s*#.*$', '', t)
    t2 = re.sub(r'\[rip[+-]0x[0-9a-f]+\]', '[rip+R]', t2)
    m = re.match(r'(call|j[a-z]+)\s+0x([0-9a-f]+)$', t2)
    if m:
        tgt = int(m.group(2), 16)
        t2 = m.group(1) + (' W%+x' % (tgt - base) if lo <= tgt < hi else ' X')
    return re.sub(r'\s+', ' ', t2)

def window(A, va, span):
    ins = A.lines(va - span, va + span)
    return ins

def main():
    oasm, nasm, resolved = sys.argv[1:4]
    sites = [int(x, 16) for x in sys.argv[4:]]
    OA, NA = Asm(oasm), Asm(nasm)
    known = []
    for l in open(resolved):
        p = l.split('\t')
        if p[1] != '-' and p[2] == 'code': known.append((int(p[0], 16), int(p[1], 16)))
    known.sort(); kk = [a for a, b in known]
    for r in sites:
        i = bisect.bisect_left(kk, r)
        cands = [known[j] for j in (i - 1, i) if 0 <= j < len(known)]
        delta = min(cands, key=lambda ab: abs(ab[0] - r)); delta = delta[1] - delta[0]
        va = B + r
        oi = window(OA, va, 0x100)
        idx = next((k for k, (a, t) in enumerate(oi) if a == va), None)
        if idx is None:
            idx = next((k for k, (a, t) in enumerate(oi) if a > va), 0) - 1
            inside = va - oi[idx][0]
        else: inside = 0
        on = [norm1(t, va - 0x100, va + 0x100, oi[idx][0]) for a, t in oi]
        ni = NA.lines(va + delta - 0x30000, va + delta + 0x30000)
        best = []
        # anchor on the site instruction text, then extend both ways
        for k, (a, t) in enumerate(ni):
            if norm1(t, 0, 0, 0).split(' ')[0] != on[idx].split(' ')[0]: continue
            nb = a
            nn = lambda kk: norm1(ni[kk][1], nb - 0x100, nb + 0x100, nb)
            if nn(k) != on[idx]: continue
            f = 0
            while idx + f + 1 < len(on) and k + f + 1 < len(ni) and nn(k + f + 1) == on[idx + f + 1]: f += 1
            b = 0
            while idx - b - 1 >= 0 and k - b - 1 >= 0 and nn(k - b - 1) == on[idx - b - 1]: b += 1
            best.append((b + f, b, f, a - B + inside))
        best.sort(reverse=True)
        top = best[:3]
        uniq = len(best) > 0 and (len(best) == 1 or best[0][0] > best[1][0] + 4)
        print('%x\tdelta %x\t%s\t%s' % (r, delta & 0xffffffff, 'UNIQUE' if uniq else 'AMBIG',
              ' | '.join('new %x len %d (-%d/+%d)' % (n, s, b, f) for s, b, f, n in top)))

if __name__ == '__main__':
    main()
