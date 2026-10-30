#!/usr/bin/env python3
"""V5.3.0 R11 (game build 1090142): every Patch 5.01 address the ASI uses has a build-1090142 entry.
 * src/urt_build_table.h is exactly what tools/port/gen_build_table.py makes from tools/port/port_map_1090142.tsv;
 * every urt_rva(0x...) literal in the linked sources is in the table;
 * every address table that is translated where it is used (PT shadow reads, Frame Generation sites, graphics rows,
   PT/RT performance rows, legacy RTGI descriptors) is fully in the table;
 * no linked source uses a raw Patch 5.01 code/data address outside comments except through urt_rva()."""
import re, subprocess, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
fails = 0
def check(c, m):
    global fails
    print(('PASS ' if c else 'FAIL ') + m)
    if not c: fails += 1
r = subprocess.run([sys.executable, '-I', str(ROOT / 'tools/port/gen_build_table.py'), '--check'], capture_output=True, text=True)
check(r.returncode == 0, 'src/urt_build_table.h is generated from tools/port/port_map_1090142.tsv (' + r.stdout.strip() + ')')
table = {int(a, 16) for a, b in re.findall(r'\{0x([0-9a-f]+)u,0x([0-9a-f]+)u\}', (ROOT / 'src/urt_build_table.h').read_text())}
build = (ROOT / 'build.py').read_text()
units = re.search(r"UNITS = '''(.*?)'''", build, re.S).group(1).split() + (ROOT / 'src/UNITS_V5.txt').read_text().split()
check('urt_build' in units, 'urt_build is linked into the ASI')
def code(text):
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.S); text = re.sub(r'//[^\n]*', '', text)
    return re.sub(r'"(?:\\.|[^"\\])*"', '""', text)
files = set()
for u in units:
    files.add(ROOT / 'src' / (u + '.c'))
    for h in re.findall(r'#include "([^"]+\.h)"', (ROOT / 'src' / (u + '.c')).read_text(encoding='latin1')):
        if (ROOT / 'src' / h).exists(): files.add(ROOT / 'src' / h)
files.discard(ROOT / 'src/urt_build_table.h')
lit, raw = set(), []
TABLE_USERS = {'refined_pt_shadow_ext_501.c', 'urt_fg501.c', 'urt_gfx_table.c', 'urt_ptperf501.c', 'urt_rtperf501.c', 'cr01_rt_legacy501.c'}
translated = set()
for f in sorted(files):
    t = code(f.read_text(encoding='latin1'))
    for m in re.finditer(r'urt_rva\(0x([0-9a-fA-F]+)u?\)', t): lit.add(int(m.group(1), 16))
    for m in re.finditer(r'(?<![\w(])0x([0-9a-fA-F]{6,8})u?(?![0-9a-fA-F])', t):
        v = int(m.group(1), 16)
        if not (0x400000 <= v < 0x6400000) or v in (0xffffff, 0x1000000, 0x1d00000): continue   # constants; 0x1d00000 = allocation centre only
        if f.name in TABLE_USERS: translated.add(v)
        else: raw.append('%s:%#x' % (f.name, v))
check(lit and lit <= table, '%d urt_rva() literals, all in the table%s' % (len(lit), '' if lit <= table else ': missing ' + ', '.join('%#x' % x for x in sorted(lit - table))))
check(translated <= table, '%d addresses of tables translated at their use site, all in the table%s' % (len(translated), '' if translated <= table else ': missing ' + ', '.join('%#x' % x for x in sorted(translated - table))))
check(not raw, 'no raw Patch 5.01 address outside urt_rva() in the linked sources%s' % ('' if not raw else ': ' + ', '.join(raw)))
print('TOTAL %s %d' % ('FAIL' if fails else 'PASS', fails))
sys.exit(1 if fails else 0)
