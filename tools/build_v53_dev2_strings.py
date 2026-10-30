#!/usr/bin/env python3
"""V5.3 DEV2 string pass (menu polish + tooltip clarity). Runs AFTER tools/build_v53_locales.py.
String data only: no option ID, value, row, preset or script change.
 * every adjustable control's tooltip is replaced by the assembled DEV2 text (tools/v53_dev/tooltips_dev2.py):
   what it does, Native / Auto value (or game-controlled), performance class, direction, trade-off, when it applies;
 * existing option-label keys fixed (untranslated On/Off, "Native Auto" without the slash, thousands commas);
 * two keys the V5.3 DEV1 menu referenced but never defined are added (urtx_n_4 = "4", urtx_n_20_m = "20 m"),
   IDs 1194782000 + index (collision-checked)."""
import sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
sys.path.insert(0, str(ROOT / 'tools' / 'v53_dev'))
import rc1_w3codec as C
import tooltips_dev2 as T

BASE_ID = 1194782000


def h(s):
    v = 0
    for c in s.lower():
        raw = c.encode('utf-16le')
        for i in range(0, len(raw), 2):
            v = (31 * v + int.from_bytes(raw[i:i + 2], 'little')) & 0xffffffff
    return v


def main():
    T.check()
    docs = {}
    for lang in T.LANGS:
        docs[lang] = C.decode((ROOT / 'build/loc' / f'{lang}.w3strings').read_bytes(), lang)

    def loc(lang, key):
        d = docs[lang]
        i = dict(d.block2)[h(key)]
        return next(b[3] for b in d.blocks if b[0] == i)

    tips, labels = T.tooltips(loc), T.labels(loc)  # computed from the pre-DEV2 strings, before any write
    new_keys = sorted(T.NEW_LABELS)
    for lang in T.LANGS:
        d = docs[lang]
        ids = dict(d.block2)
        by_id = {b[0]: b for b in d.blocks}
        changes = {**tips[lang], **labels[lang]}
        for k, text in changes.items():
            if h(k) not in ids:
                raise SystemExit('DEV2: expected existing key missing: %s (%s)' % (k, lang))
            by_id[ids[h(k)]][3] = text
        used = set(by_id)
        for n, k in enumerate(new_keys):
            if h(k) in ids:
                if by_id[ids[h(k)]][3] != T.NEW_LABELS[k]:
                    raise SystemExit('DEV2: %s already defined differently' % k)
                continue
            nid = BASE_ID + n
            if nid in used:
                raise SystemExit('DEV2: ID collision ' + k)
            d.blocks.append([nid, 0, 0, T.NEW_LABELS[k]]); d.block2.append([h(k), nid])
        raw = C.encode(d, lang)
        assert C.encode(C.decode(raw, lang), lang) == raw
        (ROOT / 'build/loc' / f'{lang}.w3strings').write_bytes(raw)
    print('PASS V5.3 DEV2 strings: 18 languages; %d tooltips, %d option labels replaced; %d keys added (%s)'
          % (len(tips['en']), len(labels['en']), len(new_keys), ', '.join(new_keys)))


if __name__ == '__main__':
    main()
