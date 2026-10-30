#!/usr/bin/env python3
"""R12 fix: rewrite LLVM 18 disassembly (llvm-dis) into the syntax of the game's DXC disassembler (LLVM 3.7 printer):
unnamed block labels "; <label>:N", typed pointers in handle loads, payload allocas/GEPs/loads/stores. Only the
spellings URT's text passes could depend on are rewritten; the result is a format test input, not assembled.
Usage: dxc37_style.py in.ll out.ll"""
import re, sys
t = open(sys.argv[1], encoding='latin-1').read()
t = re.sub(r'(?m)^(\d+):(\s*;.*)?$', lambda m: '; <label>:%s%s' % (m.group(1), m.group(2) or ''), t)
t = re.sub(r'load (%[\w.]+), ptr @', r'load \1, \1* @', t)
t = re.sub(r'getelementptr inbounds (%[\w.]+), ptr ', r'getelementptr inbounds \1, \1* ', t)
t = re.sub(r'load (<4 x i32>|i32|float), ptr ', lambda m: 'load %s, %s* ' % (m.group(1), m.group(1)), t)
t = re.sub(r'store (<4 x i32>|i32|float) ([^,]+), ptr ', lambda m: 'store %s %s, %s* ' % (m.group(1), m.group(2), m.group(1)), t)
t = re.sub(r'(%struct\.\w+), ptr nonnull', r'\1* nonnull', t)
open(sys.argv[2], 'w', encoding='latin-1').write(t)
