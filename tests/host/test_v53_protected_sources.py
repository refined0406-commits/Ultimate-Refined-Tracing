#!/usr/bin/env python3
"""V5.3 dev: every protected V5.2.0 source and asset is byte-identical (tests/fixtures/v520_protected_sources.json):
PT shader carrier/identity/slots, PT shadow extension, CPU (bones, distant trees, physics), Normal RT, PT Scene Range,
Frame Generation, settings store/codec, loader gate, menu movie (separator repair input), UAL, V5.2.0 string and
preset generators and fixtures. Only the documented V5.3 edit set may differ."""
import hashlib, json
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
pins = json.loads((ROOT / 'tests/fixtures/v520_protected_sources.json').read_text())['files']
bad = [k for k, v in pins.items() if hashlib.sha256((ROOT / k).read_bytes()).hexdigest() != v]
assert not bad, bad
src = (ROOT / 'src/UNITS_V5.txt').read_text().split()
assert src[-1] == 'urt_compat53' and len(src) == 14, src
print('PASS %d protected V5.2.0 files byte-identical (PT shader identity, CPU/RT/PT modules, store, menu movie, loader, V5.2.0 generators)' % len(pins))
