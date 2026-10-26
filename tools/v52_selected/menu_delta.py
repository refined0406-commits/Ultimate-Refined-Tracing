#!/usr/bin/env python3
"""Menu delta against the menu recorded in the WCC receipt (RC5 mechanism, generalised in RC6).

The WCC receipt (game/wcc/V52_VERIFIED_WCC.json) pins the menu XML that existed when the
script was compiled; that exact file is stored as game/wcc/V52_WCC_MENU_AT_COMPILE.xml.
The shipped menu may differ from it in ONE documented way only, without recompiling:
  * RC6: rows of spec.NATIVE_MENU_HOLD that tools/v52_selected/native_menu_evidence.json marks
    'released' (with evidence) are added. Their tooltip, Master greying and save path are
    already in the compiled script (compiled while held), so the blob stays valid.
  * RC7: rows of spec.RETIRED_AFTER_COMPILE (and spec.RETIRED_HEADINGS) are removed. The compiled
    script keeps inert hover/greying entries for them; the runtime never reads their values.
  * V5.3 DEV2: option displayName swaps listed in spec.OPTION_LABEL_ONLY (label text only).
Everything else must be identical (canonical XML comparison after removing released rows from
the shipped menu and retired rows from the compile-time menu).
This proves the delta mechanically instead of silently re-pinning the receipt."""
import hashlib, json, sys
import xml.etree.ElementTree as ET
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))
import spec as S


def sha(b):
    return hashlib.sha256(b).hexdigest()


def _canon(root):
    return ET.canonicalize(ET.tostring(root, encoding='unicode'), strip_text=True)


def verify(menu_path=None):
    """Return the current menu sha256 if it is the receipt menu (plus documented released rows), else raise."""
    receipt = json.loads((ROOT / 'game/wcc/V52_VERIFIED_WCC.json').read_text())
    old = (ROOT / 'game/wcc/V52_WCC_MENU_AT_COMPILE.xml').read_bytes()
    new = Path(menu_path or ROOT / 'build/menu/RefinedTracing.xml').read_bytes()
    if sha(old) != receipt['menu_sha256']:
        raise AssertionError('stored compile-time menu does not match the WCC receipt')
    if new == old:
        return sha(new)
    ev = json.loads((ROOT / 'tools/v52_selected/native_menu_evidence.json').read_text())
    released = [k for k in S.NATIVE_MENU_HOLD if ev[k]['status'] == 'released']
    if not all(ev[k]['evidence'] for k in released):
        raise AssertionError('a held row was released without evidence')
    retired = tuple(getattr(S, 'RETIRED_AFTER_COMPILE', ())) + tuple(getattr(S, 'RETIRED_HEADINGS', ()))
    def strip(root, ids):
        for g in root.findall('Group'):
            vv = g.find('VisibleVars')
            for v in list(vv.findall('Var')):
                if v.get('id') in ids:
                    vv.remove(v)
        return root
    cur = ET.fromstring(new)
    # V5.3 DEV2: revert the documented option display-name-only swaps (each must be present exactly)
    if S.apply_option_labels(cur, reverse=True) != 2 * len(getattr(S, 'OPTION_LABEL_ONLY', {})):
        raise AssertionError('option display-name corrections incomplete')
    if any(v.get('id') in retired for v in cur.iter('Var')):
        raise AssertionError('a retired row is still in the menu')
    # (a released row may already be in the compile-time menu, as in the V5.3 kit menu: strip it on both sides)
    if _canon(strip(cur, released)) != _canon(strip(strip(ET.fromstring(old), retired), released)):
        raise AssertionError('menu differs from the receipt menu by more than released held rows and retired rows')
    txt = new.decode('utf-8')
    if 'SUBTLE_SEPARATOR' in txt:
        raise AssertionError('a SUBTLE_SEPARATOR row is present')
    src = (ROOT / 'game/wcc/RefinedTracingCR01_RTGI3.ws').read_text('utf-8-sig')
    if not all(("this.onHover('%s'" % k) in src for k in released):
        raise AssertionError('a released row is not described by the compiled script')
    return sha(new)


if __name__ == '__main__':
    print('PASS menu = WCC receipt menu, plus only rows released with evidence, minus only documented retired rows;', verify())
