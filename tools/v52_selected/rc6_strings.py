#!/usr/bin/env python3
"""One-shot, asserted RC6 update of tools/v52_selected/locale_records.py and locale_mapping.json.

Adds, in all 18 languages, the strings of the RC6 rows (labels, option labels, tooltips) taken
verbatim from the private research build R5 (their texts were already reviewed: short, no
third-party names; caustics keep their "Experimental" caution in the tooltip), the four new
named headings, the requested heading rename ("Textures / Rendering") and aligns the other
heading texts with the approved English names (several languages still showed research-build
titles such as "CPU and World" or "Frame Generation (Experimental)").
Headings keep the RC5 padding: ' ' + title + U+00A0.
Usage: rc6_strings.py <r5strings.json>"""
import json, pprint, re, sys
from pathlib import Path
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent))
from v52_selected import locale_records as L
R5 = json.loads(Path(sys.argv[1]).read_text())
LANGS = sorted(L.RECORDS)
assert len(LANGS) == 18 and sorted(R5) == LANGS

NEW_ROWS = ['urtx_ptcaustics', 'urtx_ptcausticsinpt', 'urtx_ptcausticstypes', 'urtx_ptcausticsres', 'urtx_ptcausticsblur',
            'urtx_ptcausticsarea', 'urtx_ptskinbouncedesat', 'urtx_rtreflfog', 'urtx_terrainpreload', 'urtx_treedistance']
NEW_OPTS = ['urtx_w_cau_water', 'urtx_w_cau_surface', 'urtx_w_cau_reflect', 'urtx_n_512', 'urtx_n_1024', 'urtx_n_4096',
            'urtx_n_4', 'urtx_n_20_m', 'urtx_n_40_m', 'urtx_n_100_m', 'urtx_n_150_m', 'urtx_n_200_m', 'urtx_n_0p25',
            'urtx_n_0p75', 'urtx_n_10_m', 'urtx_n_400_m', 'urtx_n_600_m', 'urtx_n_1p5', 'urtx_n_2p6', 'urtx_n_5']

def strip_paren(s):
    return re.sub(r'\s*[（(][^)）]*[)）]\s*$', '', s.strip())

SHARED = {'ar': 'مشترك', 'br': 'compartilhado', 'cn': '共用', 'cz': 'společné', 'de': 'Gemeinsam', 'en': 'Shared',
          'es': 'compartido', 'esmx': 'compartido', 'fr': 'partagé', 'hu': 'közös', 'it': 'condiviso', 'jp': '共通',
          'kr': '공통', 'pl': 'wspólne', 'ru': 'общие', 'tr': 'ortak', 'ua': 'спільні', 'zh': '共用'}
TEXREN = {'ar': 'الخامات / العرض', 'br': 'Texturas / Renderização', 'cn': '纹理 / 渲染', 'cz': 'Textury / vykreslování',
          'de': 'Texturen / Rendering', 'en': 'Textures / Rendering', 'es': 'Texturas / Renderizado',
          'esmx': 'Texturas / Renderizado', 'fr': 'Textures / rendu', 'hu': 'Textúrák / renderelés',
          'it': 'Texture / rendering', 'jp': 'テクスチャ / レンダリング', 'kr': '텍스처 / 렌더링', 'pl': 'Tekstury / renderowanie',
          'ru': 'Текстуры / рендеринг', 'tr': 'Dokular / işleme', 'ua': 'Текстури / рендеринг', 'zh': '材質貼圖 / 渲染'}
CJK_COLON = {'cn', 'zh'}

def heading(lang, key):
    if key == 'urt52_sep_cpu': t = 'CPU'
    elif key == 'urt52_sep_pt': t = 'Path Tracing'
    elif key == 'urt52_sep_rt': t = 'Ray Tracing'
    elif key == 'urt52_sep_fg': t = 'Frame Generation'
    elif key == 'urt52_sep_tex': t = TEXREN[lang]
    elif key == 'urt52_sep_ptcaus': t = strip_paren(R5[lang]['urt_sep_ptcaus'])
    elif key == 'urt52_sep_rtgiq': t = R5[lang]['urt_sep_rtgiq'].strip()
    elif key == 'urt52_sep_rtgip': t = R5[lang]['urt_sep_rtgip'].strip()
    elif key == 'urt52_sep_rtpt':
        sep = '：' if lang in CJK_COLON else (' : ' if lang == 'fr' else ': ')
        t = 'RT/PT' + sep + SHARED[lang]
    else: raise KeyError(key)
    t = t.replace(' ', ' ').strip()
    assert t and '(' not in t and '（' not in t and 'xperiment' not in t.lower(), (lang, key, t)
    return ' ' + t + ' '

HEADS_OLD = ['urt52_sep_cpu', 'urt52_sep_pt', 'urt52_sep_rt', 'urt52_sep_tex', 'urt52_sep_fg']
HEADS_NEW = ['urt52_sep_ptcaus', 'urt52_sep_rtgiq', 'urt52_sep_rtgip', 'urt52_sep_rtpt']
added = []
for lang in LANGS:
    rec = L.RECORDS[lang]; r5 = R5[lang]
    for k in HEADS_OLD:
        rec[k] = rec['option_' + k] = heading(lang, k)
    for k in HEADS_NEW:
        assert k not in rec
        rec[k] = rec['option_' + k] = heading(lang, k)
    lbl = r5['cr01_pt_scene_range']; assert lbl == 'PT: Scene Range'
    rec['cr01_pt_scene_range'] = rec['option_cr01_pt_scene_range'] = lbl
    rec['tooltip_cr01_pt_scene_range'] = L.CORE_TIPS[lang]['tooltip_cr01_pt_scene_range']
    for k in NEW_ROWS:
        for kk in (k, 'option_' + k, 'tooltip_' + k):
            assert kk not in rec, (lang, kk)
            v = r5[kk] if kk in r5 else r5[k]
            assert v and 'VAXIS' not in v and 'Beardmo' not in v and 'Immersive' not in v, (lang, kk)
            rec[kk] = v
    for k in NEW_OPTS:
        assert k not in rec, (lang, k)
        rec[k] = r5[k]
    if lang == 'en':
        added = [k for k in rec]
# the caustics tooltips must keep their caution in every language (R5 text ends with the localised "Experimental.")
for lang in LANGS:
    for k in NEW_ROWS[:6]:
        assert L.RECORDS[lang]['tooltip_' + k] == R5[lang]['tooltip_' + k]
    assert L.RECORDS[lang]['tooltip_urtx_terrainpreload'] == R5[lang]['tooltip_urtx_terrainpreload']
keysets = {tuple(sorted(L.RECORDS[l])) for l in LANGS}
assert len(keysets) == 1, 'every language has exactly the same keys'

# IDs: existing ones unchanged; new keys from 1194780000 upwards
MAP = json.loads((HERE / 'locale_mapping.json').read_text())
used = set(MAP['selected_key_ids'].values()) | set(MAP['heading_ids'].values())
nid = 1194780000
for k in sorted(L.RECORDS['en']):
    if k in MAP['selected_key_ids'] or k in MAP['heading_ids']:
        continue
    assert nid not in used
    if k.startswith(('urt52_sep_', 'option_urt52_sep_')):
        MAP['heading_ids'][k] = nid
    else:
        MAP['selected_key_ids'][k] = nid; MAP['selected_keys'].append(k)
    used.add(nid); nid += 1
MAP['selected_keys'] = sorted(set(MAP['selected_keys']))
assert sorted(set(MAP['selected_key_ids']) | set(MAP['heading_ids'])) == sorted(L.RECORDS['en'])
(HERE / 'locale_mapping.json').write_text(json.dumps(MAP, indent=2, ensure_ascii=False) + '\n')
src = (HERE / 'locale_records.py').read_text('utf-8')
a = src.index('RECORDS = ')
b = src.index('CORE_TIPS = ')
hdr = '# Selected V5.2 controls (RC3 + RC6 rows) and nine translated, optically-padded native separators.\n'
(HERE / 'locale_records.py').write_text(hdr + 'RECORDS = ' + pprint.pformat(L.RECORDS, width=120) + '\n' + src[b:], 'utf-8')
print('PASS RC6 strings: %d keys per language x 18; %d new IDs from 1194780000' % (len(L.RECORDS['en']), nid - 1194780000))
