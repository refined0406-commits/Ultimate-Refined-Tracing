#!/usr/bin/env python3
"""V5.0.0 hotfix + V5.1 regression: CR4IngameMenu.OnOptionValueChanged routing.

Static checks of the WitcherScript source that the precompiled.rsblob is built
from (game/wcc/RefinedTracingCR01_RTGI3.ws):

1. GetGroupName is reachable only inside `if (this.RT_CR01_IsOwnedOption(...))`,
   i.e. never for a vanilla option such as Game/Gwent Difficulty.
2. The owned-name set equals exactly the interactive (non-separator) Var ids
   of the RefinedTracingCR01 group in menu/RefinedTracing.xml.
3. wrappedMethod is reached on every path except the pre-existing owned
   stale-QuickPresetSelect early return (Master Off).
4. Removing the two hotfix hunks restores the protected V5.0.0 source byte for
   byte (proves nothing else changed), and every protected count is unchanged.
5. A tiny control-flow model of the wrapper, driven by the parsed source,
   confirms vanilla options never call GetGroupName and still call wrappedMethod.

Proves source structure only. It does not prove WCC compilation or in-game
behaviour; both need Windows.
"""
import hashlib, os, re, sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
WS = os.path.join(ROOT, 'game', 'wcc', 'RefinedTracingCR01_RTGI3.ws')
XML = os.path.join(ROOT, 'build', 'menu', 'RefinedTracing.xml')   # shipped (V5.1) menu
if not os.path.exists(XML):
    XML = os.path.join(ROOT, 'menu', 'RefinedTracing.xml')
PROTECTED_SHA = '07f8b823ee944916ca8074f487b75a063abfa8f88f7e6df0f69ce079d9907a7a'   # V5.0.0 (pre-hotfix)
HOTFIX_SHA = '90509bc1b8d4f49db1badb31fb3b82db2edaf2c4c84a0d8fdd9705d93e1707f1'      # V5.0.0 hotfix1 (shipped)
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import v51_ws_patch as V51P
V51 = V51P.NAMES

fails = []
def check(cond, msg):
    print(('PASS ' if cond else 'FAIL ') + msg)
    if not cond:
        fails.append(msg)

raw = open(WS, 'rb').read()
check(raw.startswith(b'\xef\xbb\xbf') and b'\r\n' not in raw, 'source keeps UTF-8 BOM + LF')
src = raw.decode('utf-8-sig')

# --- owned set vs menu XML -------------------------------------------------
m = re.search(r"function RT_CR01_IsOwnedOption\(optionName: name\): bool \{\n(.*?)\n\}\n", src, re.S)
check(m is not None, 'RT_CR01_IsOwnedOption defined')
owned = re.findall(r"optionName == '([^']+)'", m.group(1)) if m else []
xml = open(XML, encoding='utf-8-sig').read()
grp = re.search(r'<Group id="RefinedTracingCR01".*?</Group>', xml, re.S)
check(grp is not None, 'menu XML group RefinedTracingCR01 present')
xml_ids = [i for i, t in re.findall(r'<Var id="([^"]+)"[^>]*displayType="([A-Z_]+)', grp.group(0))
           if t != 'SUBTLE_SEPARATOR'] if grp else []
check(len(owned) == 42 and len(set(owned)) == 42, f'42 unique owned names: 41 V5.0.0 - 2 Shading Rays + 3 V5.1 (got {len(owned)})')
check(sorted(owned) == sorted(xml_ids), 'owned set == interactive XML Var ids (no extra, none missing)')
hovers = re.findall(r"this\.onHover(?:Dynamic)?\('([^']+)'\s*,", src)
check(sorted(set(hovers)) == sorted(owned), 'owned set == the 42 tooltip-hover controls')

# --- wrapper structure ---------------------------------------------------
w = re.search(r"@wrapMethod\(CR4IngameMenu\)\nfunction OnOptionValueChanged\(.*?\n\}\n", src, re.S)
check(w is not None, 'OnOptionValueChanged wrapper present')
body = w.group(0) if w else ''
check(src.count('GetGroupName(') == 1, 'exactly one GetGroupName( call in the whole script')
gate = ("  groupName = '';\n  if (this.RT_CR01_IsOwnedOption(optionName)) {\n"
        "    // Only URT's own rows (real config group ids) are resolved; the group\n"
        "    // check also guards against a same-named variable in another group.\n"
        "    groupName = mInGameConfigWrapper.GetGroupName(groupId);\n  }\n")
check(body.count(gate) == 1, 'GetGroupName sits only inside the ownership gate')
check(body.index(gate) < body.index('wrappedMethod(') if gate in body else False,
      'gate precedes wrappedMethod')
check(body.count('result = wrappedMethod(groupId, optionName, optionValue);') == 1, 'wrappedMethod called once')
pre = body[:body.index('wrappedMethod(')] if 'wrappedMethod(' in body else ''
returns = re.findall(r'return [^;]+;', pre)
check(returns == ['return false;'], 'only one early return before wrappedMethod (pre-existing)')
check("if (optionName == 'QuickPresetSelect' && rawMaster != \"true\" && rawMaster != \"1\") return false;" in pre
      and pre.index("if (groupName == 'RefinedTracingCR01') {") < pre.index('return false;'),
      'that early return is owned-only (inside groupName == RefinedTracingCR01)')
check(body.rstrip().endswith('return result;\n}') or body.rstrip().endswith('return result;\n}'.rstrip()),
      'wrapper returns wrappedMethod result')

# --- exact reversibility: V5.1 hunks -> hotfix1 -> protected V5.0.0 --------
v51 = V51P.revert(src)
v51_sha = hashlib.sha256(b'\xef\xbb\xbf' + v51.encode('utf-8')).hexdigest()
check(v51_sha == HOTFIX_SHA, f'reverting the {len(V51P.ops())} V5.1 operations == shipped V5.0.0 hotfix1 source ({v51_sha[:12]})')
check(V51P.apply(v51) == src, 'source == hotfix1 + exactly the V5.1-rc3 operations (nothing else changed)')
# --- V5.1 Quick Preset assignments (stored value == XML Option id) --------
want = {'RTReflLocalLights': ('1','1','0'),
        'RTShadowLocalLights': ('4','2','0'), 'RTShadowFoliageDetail': ('3','2','0')}
import xml.etree.ElementTree as _ET
_opts = {v.get('id'): {o.get('id') for o in v.iter('Option')} for v in _ET.fromstring(open(XML,encoding='utf-8-sig').read().encode()).iter('Var')}
for n, (pv, bv, ev) in want.items():
    m = re.search(r"  // " + n + r" \(V5\.1\)\n  if \(selected == \"1\"\) \{\n    mInGameConfigWrapper\.SetVarValue\('RefinedTracingCR01', '" + n +
                  r"', \"(\d+)\"\);\n  \} else if \(selected == \"2\"\) \{\n    mInGameConfigWrapper\.SetVarValue\('RefinedTracingCR01', '" + n +
                  r"', \"(\d+)\"\);\n  \} else \{\n    mInGameConfigWrapper\.SetVarValue\('RefinedTracingCR01', '" + n + r"', \"(\d+)\"\);", src)
    check(m is not None and m.groups() == (pv, bv, ev), f'preset {n}: Performance={pv} Balanced={bv} Extreme={ev}')
    check(m is not None and set(m.groups()) <= _opts.get(n, set()), f'preset {n}: values are valid menu option ids')
fn = src[src.index('function RT_CR01_ApplyQuickPreset'):src.index('function RT_CR01_IsDecimalText')]
check(all(fn.index("// " + n + " (V5.1)") < fn.index('options.force_update_values') for n in V51),
      'new preset assignments are inside the single preset transaction (one UI refresh, one save)')
# --- rc3: removed controls are absent from every script path -------------
for n in ('RTGIConsoleGrid', 'RTQualityShadingRaysPerProbe', 'RTPerfShadingRaysPerProbe', 'shading_rays', 'console_grid'):
    check(n not in src, f'{n}: no hover, tag, owned name or Quick Preset assignment')
# --- rc3: exact RTGI Quick Preset values (stored value, flash Option id) ---
def block(var, tag):
    m = re.search(r"  // " + var + r"\n  if \(selected == \"1\"\) \{\n    mInGameConfigWrapper\.SetVarValue\('RefinedTracingCR01', '" + var +
                  r"', \"(\d+)\"\);\n  \} else if \(selected == \"2\"\) \{\n    mInGameConfigWrapper\.SetVarValue\('RefinedTracingCR01', '" + var +
                  r"', \"(\d+)\"\);\n  \} else \{\n    mInGameConfigWrapper\.SetVarValue\('RefinedTracingCR01', '" + var +
                  r"', \"(\d+)\"\);\n  \}\n  // Flash OPTIONS current uses XML Option IDs, never stored Entry values\.\n"
                  r"  update = m_flashValueStorage\.CreateTempFlashObject\(\);\n  update\.SetMemberFlashUInt\(\"tag\", NameToFlashUInt\('" + tag +
                  r"'\)\);\n  if \(selected == \"1\"\) \{\n    update\.SetMemberFlashString\(\"current\", \"(\d+)\"\);\n  \} else if \(selected == \"2\"\) \{\n"
                  r"    update\.SetMemberFlashString\(\"current\", \"(\d+)\"\);\n  \} else \{\n    update\.SetMemberFlashString\(\"current\", \"(\d+)\"\);", src)
    return m.groups() if m else None
_vals = {v.get('id'): {o.get('id'): [e.get('value') for e in o.iter('Entry')][0] for o in v.iter('Option')}
         for v in _ET.fromstring(open(XML,encoding='utf-8-sig').read().encode()).iter('Var') if v.get('id') != 'QuickPresetSelect'}
RTGI = {  # var: (Performance, Balanced, Extreme) stored values -- rc3 user values
 'ClassificationPrimary': ('32', '64', '128'), 'ClassificationSecondary': ('12', '16', '32'),
 'MaxProbes': ('8192', '65536', '262144'), 'AOQuality': ('1', '2', '4'), 'ShadingBudget': ('65536', '262144', '524288'),
 'GlobalShadingLimit': ('4096', '12288', '32768'), 'Grid1ShadingLimit': ('16384', '49152', '262144')}
for g in ('RTQuality', 'RTPerf'):
    for f, exp in RTGI.items():
        var = g + f; tag = ('Virtual_' + var) if f == 'ClassificationSecondary' else var
        got = block(var, tag)
        check(got is not None and got[:3] == exp, f'preset {var}: Performance/Balanced/Extreme = {"/".join(exp)} (got {got and got[:3]})')
        check(got is not None and all(_vals[tag][fid] == sv for fid, sv in zip(got[3:], got[:3])),
              f'preset {var}: flash Option ids {got and got[3:]} display exactly those values')
qps = {o.get('id'): dict((e.get('varId'), e.get('value')) for e in o.iter('Entry'))
       for v in _ET.fromstring(open(XML,encoding='utf-8-sig').read().encode()).iter('Var') if v.get('id') == 'QuickPresetSelect' for o in v.iter('Option')}
for g in ('RTQuality', 'RTPerf'):
    for f, exp in RTGI.items():
        check(tuple(qps[i].get(g + f) for i in '123') == exp, f'menu XML Quick Preset entries {g}{f} match the script')
rev = v51
fn = re.search(r"// V5\.0\.0 hotfix: URT reacts only to its own options\..*?\n\}\n\n(?=@wrapMethod\(CR4IngameMenu\)\nfunction OnOptionValueChanged)", rev, re.S)
check(fn is not None, 'hotfix helper block located')
if fn:
    rev = rev[:fn.start()] + rev[fn.end():]
rev = rev.replace(gate, '  groupName = mInGameConfigWrapper.GetGroupName(groupId);\n', 1)
rev_sha = hashlib.sha256(b'\xef\xbb\xbf' + rev.encode('utf-8')).hexdigest()
check(rev_sha == PROTECTED_SHA, f'reverting the 2 hotfix hunks == protected V5.0.0 source ({rev_sha[:12]})')

# --- protected counts (same values as the V4.3.0 build kit asserted) -------
check(len(re.findall(r'theGame\.SaveUserSettings\(\);', src)) == 2, 'SaveUserSettings count unchanged (2)')
check(len(re.findall(r"mInGameConfigWrapper\.SetVarValue\('RefinedTracingCR01'", src)) == 123, 'config setters: 120 V5.0.0 - 6 Shading Rays + 9 V5.1 preset assignments')
check(len(re.findall(r'values\.PushBackFlashObject\(update\);', src)) == 40, 'Quick Preset UI rows: 39 V5.0.0 - 2 Shading Rays + 3 V5.1')
check(len(hovers) == 42, '42 hover controls (41 - 2 + 3 V5.1)')

# --- control-flow model ----------------------------------------------------
def model(option, group_name_of_id, master='1'):
    trace = []
    gn = ''
    if option in owned:
        trace.append('GetGroupName'); gn = group_name_of_id
    if gn == 'RefinedTracingCR01' and option == 'QuickPresetSelect' and master not in ('true', '1'):
        return trace
    trace.append('wrappedMethod')
    if gn == 'RefinedTracingCR01':
        trace.append('SaveUserSettings')
    elif option in ('PTEnable', 'EnableRT', 'EnableRtRadiance', 'Virtual_RTShadows'):
        trace.append('SaveUserSettings')
    return trace
for vanilla in ('GameDifficulty', 'Virtual_GwentDifficulty', 'Difficulty', 'GwentDifficulty', 'Subtitles', 'PTEnable'):
    t = model(vanilla, None)
    check('GetGroupName' not in t and 'wrappedMethod' in t, f'vanilla {vanilla}: no group lookup, wrappedMethod called')
for own in owned:
    t = model(own, 'RefinedTracingCR01')
    check(t[0] == 'GetGroupName' and 'wrappedMethod' in t and t[-1] == 'SaveUserSettings', f'URT {own}: processed + saved')
t = model('QuickPresetSelect', 'RefinedTracingCR01', master='0')
check(t == ['GetGroupName'], 'stale preset with Master Off still ignored (unchanged)')
t = model('PTBillboards', 'Graphics')
check('SaveUserSettings' not in t and 'wrappedMethod' in t, 'same-named var in another group not processed by URT')

print('TOTAL', 'FAIL' if fails else 'PASS', len(fails))
sys.exit(1 if fails else 0)
