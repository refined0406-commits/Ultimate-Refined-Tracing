#!/usr/bin/env python3
"""Build the V5.1 localization set (loc/ -> build/loc/).

Lossless extension, the same method as the accepted V4.x and V5 RC2-RC4 tooling:
* every existing string ID, key hash and text of the protected V4.3.1 files is
  preserved byte-exactly in meaning (no existing record changes; fallback untouched);
* the 15 new V5.1 keys (rc3: three controls) (tools/v51_strings.py) use the REDengine 31-hash (validated
  below against known pairs) and a fresh ID range that collides with nothing,
  above every ID V4.3.x and the withdrawn RC2-RC6 keys ever used;
* every output file round-trips through the codec.
"""
import sys, json, hashlib
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
import rc1_w3codec as codec
import v51_strings as S

def h(s):
    v = 0
    for ch in s.lower():
        units = ch.encode('utf-16le')
        for i in range(0, len(units), 2):
            v = (31 * v + int.from_bytes(units[i:i+2], 'little')) & 0xffffffff
    return v
assert h('cr01_masterenabled') == 0xad285f50
assert h('tooltip_cr01_rt_opt_title') == 0xdee73402
assert h('preset_value_raytracedoptimizations_1') == 0x072c13bd

LANGS = sorted(codec.MAGIC)
NEW_FIRST_ID = 1194770000          # V4.3.x max 1194765003; withdrawn RC2-RC6 keys used 1194766000+
REMOVED_KEYS = ['cr01_ptshadercompat', 'option_cr01_ptshadercompat', 'cr01_rttreeshadowdistance',
                'cr01_rtgi_probe_ray_distance', 'cr01_rt_refl_texture_detail', 'cr01_rt_ao_foliage_detail']
FALLBACK_KEY = 'tooltip_cr01_fallback'
# V5.1.0-rc3: the two RTGI Shading Rays Per Probe rows were removed from the menu
# (always Native/Auto), so their V4.3.1 label/twin/tooltip records and the value
# names only those two rows used are deleted. These are the ONLY V4.3.1 records
# that change; everything else is preserved exactly (asserted).
DELETED_V431_KEYS = ['cr01_rt_quality_shading_rays', 'option_cr01_rt_quality_shading_rays',
                     'tooltip_cr01_rt_quality_shading_rays',
                     'cr01_rt_perf_shading_rays', 'option_cr01_rt_perf_shading_rays',
                     'tooltip_cr01_rt_perf_shading_rays',
                     'cr01_rt_value_16', 'option_cr01_rt_value_16', 'cr01_rt_value_24',
                     'option_cr01_rt_value_24', 'cr01_rt_value_32', 'option_cr01_rt_value_32']

def main():
    src = ROOT / 'loc'; dst = ROOT / 'build' / 'loc'; dst.mkdir(parents=True, exist_ok=True)
    keys = list(S.new_strings('en'))
    ids = {k: NEW_FIRST_ID + i for i, k in enumerate(keys)}
    report = {'languages': {}, 'new_keys': {k: {'id': i, 'hash': hex(h(k))} for k, i in ids.items()},
              'changed_existing': None}
    assert len(keys) == 15 and len(set(h(k) for k in keys)) == 15, 'new key hashes must be unique'
    for lang in LANGS:
        strings = S.new_strings(lang)
        assert list(strings) == keys, lang
        assert all(isinstance(t, str) and t.strip() for t in strings.values()), (lang, 'empty text')
        original = (src / f'{lang}.w3strings').read_bytes()
        loc = codec.decode(original, lang)
        assert codec.encode(loc, lang) == original, (lang, 'parent roundtrip')
        before_ids = {b[0]: b[3] for b in loc.blocks}; before_keys = dict(loc.block2)
        assert not set(ids.values()) & set(before_ids), (lang, 'ID collision')
        assert not any(h(k) in before_keys for k in REMOVED_KEYS), (lang, 'withdrawn key present')
        fb_id = before_keys[h(FALLBACK_KEY)]
        del_ids = set()
        for k in DELETED_V431_KEYS:
            assert h(k) in before_keys, (lang, 'expected V4.3.1 key missing', k)
            del_ids.add(before_keys[h(k)])
        assert len(del_ids) == len(DELETED_V431_KEYS) and fb_id not in del_ids
        assert sum(1 for _, i in loc.block2 if i in del_ids) == len(del_ids), (lang, 'deleted ID shared by another key')
        loc.blocks = [b for b in loc.blocks if b[0] not in del_ids]
        loc.block2 = [e for e in loc.block2 if e[1] not in del_ids]
        kept_keys = {k: i for k, i in before_keys.items() if i not in del_ids}
        for k, i in ids.items():
            assert h(k) not in before_keys, (lang, 'key collision', k)
            loc.blocks.append([i, 0, 0, strings[k]])
            loc.block2.append([h(k), i])
        raw = codec.encode(loc, lang); back = codec.decode(raw, lang)
        assert codec.encode(back, lang) == raw, (lang, 'new roundtrip')
        after = {b[0]: b[3] for b in back.blocks}
        assert set(after) - set(before_ids) == set(ids.values())
        assert set(before_ids) - set(after) == del_ids, (lang, 'only the rc3 deletions removed')
        assert {i for i in before_ids if i not in del_ids and before_ids[i] != after[i]} == set(), (lang, 'existing text changed')
        assert after[fb_id] == before_ids[fb_id], (lang, 'fallback changed')
        assert dict(back.block2) == {**kept_keys, **{h(k): i for k, i in ids.items()}}
        for k, i in ids.items():
            assert after[i] == strings[k], (lang, k)
        (dst / f'{lang}.w3strings').write_bytes(raw)
        report['languages'][lang] = {'records': len(back.blocks), 'sha256': hashlib.sha256(raw).hexdigest(),
                                     'parent_sha256': hashlib.sha256(original).hexdigest()}
    for p in dst.glob('*.w3strings'):
        assert p.stem in LANGS, p
    (ROOT / 'build' / 'LOCALIZATION_REPORT.json').write_text(json.dumps(report, indent=1, ensure_ascii=False) + '\n')
    print('PASS 18 locales: every remaining V4.3.1 ID/key/text preserved (fallback untouched); '
          '%d obsolete Shading Rays records removed; 15 new V5.1 keys each' % len(DELETED_V431_KEYS))

if __name__ == '__main__':
    main()
