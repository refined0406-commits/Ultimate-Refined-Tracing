#!/usr/bin/env python3
"""R11: URT's C port of Orange Hair Fix's PT hair patch == Orange Hair Fix's own patch_pt.py output (text from
'target datalayout' on, which is where patch_pt.py starts its output)."""
import sys
a = open(sys.argv[1]).read(); b = open(sys.argv[2]).read()
a = a[a.index('target datalayout'):]
ok = a.rstrip('\n') == b.rstrip('\n')
print(('PASS' if ok else 'FAIL') + ' URT Orange Hair Fix port output identical to patch_pt.py (%d lines)' % b.count('\n'))
sys.exit(0 if ok else 1)
