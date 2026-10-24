#!/usr/bin/env python3
"""Static verification of the built V5 archives against the V4.3.1 baseline.
Usage: verify_release.py <out dir> <V4.3.1 MAIN zip> <V4.3.1 ASI_ONLY zip>"""
import sys, zipfile, hashlib, subprocess, re, json, io
from pathlib import Path
import xml.etree.ElementTree as ET
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools'))
import rc1_w3codec as codec
from build_v5_locales import h
import build_v5_locales as LOC
out = Path(sys.argv[1]); old_main = zipfile.ZipFile(sys.argv[2]); old_asi = zipfile.ZipFile(sys.argv[3])
sha = lambda b: hashlib.sha256(b).hexdigest()
fails = []
def check(cond, msg):
    print(('  ok   ' if cond else '  FAIL ') + msg)
    if not cond: fails.append(msg)
main = zipfile.ZipFile(next(out.glob('*_MAIN.zip'))); asi = zipfile.ZipFile(next(out.glob('*_ASI_ONLY.zip')))
check(main.testzip() is None and asi.testzip() is None, 'archives pass CRC test')
mn, an = set(main.namelist()), set(asi.namelist())
check(mn - an == {'bin/x64_dx12/winmm.dll', 'LICENSES/Ultimate-ASI-Loader-LICENSE.txt'}, 'MAIN = ASI-ONLY + loader + licence')
check(not any('Beardmo' in n or 'BlackHair' in n or 'Immersive' in n for n in mn), 'no Beardmo file or asset shipped')
check(all(main.read(n) == asi.read(n) for n in an), 'shared files byte-identical in both archives')
check(sha(main.read('bin/x64_dx12/winmm.dll')) == '031a3e5576d91dce1e438d36b9a3d462c7334ab4791990a8ff1e3ddc0e132daf', 'winmm.dll is the unmodified UAL 9.7.4 x64 build')
check(not any(n.endswith('dinput8.dll') for n in mn), 'no dinput8.dll shipped (no collision with Witcher Native Bridge)')
check(not any(n.endswith('RefinedTracing.asi') and not n.endswith('UltimateRefinedTracing.asi') for n in mn), 'no legacy-named ASI shipped')
# unchanged game-facing payload vs V4.3.1
for n in ['mods/modRefinedTracing/content/gameplay/gui_new/swf/mainmenu/panel_ingamemenu.redswf']:
    check(main.read(n) == old_main.read(n), 'unchanged vs V4.3.1: ' + n.split('/')[-1])
# V5.0.0 difficulty-menu hotfix: the only intended script change. Exact Windows WCC blob of
# hotfix source 90509bc1... (receipt docs/evidence_hotfix/RUN2_BUILD_RECEIPT.txt).
blob = main.read('mods/modRefinedTracing/content/precompiled.rsblob')
# V5.1.0-rc3: Windows WCC blob 71c07eff... of source 9fb19652... (hotfix1 + 16 exact rc3 operations), receipt docs/evidence_v51rc3/
check(sha(blob) == open(ROOT / 'game' / 'wcc' / 'precompiled.rsblob.sha256').read().split()[0], 'precompiled.rsblob is the approved WCC blob (receipt-pinned)')
REMOVED = (b'RTGIProbeRayDistance', b'RTReflTextureDetail', b'RTAOFoliageDetail',            # rc1 -> rc2
           b'RTGIConsoleGrid', b'RTQualityShadingRaysPerProbe', b'RTPerfShadingRaysPerProbe')  # rc2 -> rc3
check(all(n in blob for n in (b'RTReflLocalLights', b'RTShadowLocalLights', b'RTShadowFoliageDetail', b'tooltip_cr01_rt_shadow_foliage_detail')) and
      not any(n in blob for n in REMOVED) and b'console_grid' not in blob and b'shading_rays' not in blob,
      'compiled script has exactly the rc3 V5.1 rows (Console Probe Grid and both Shading Rays rows absent)')
check(b'RT_CR01_IsOwnedOption' in blob, 'compiled script contains the option-ownership gate')
# PE checks on the ASI
asi_bytes = main.read('bin/x64_dx12/UltimateRefinedTracing.asi')
tmp = out / '_verify.asi'; tmp.write_bytes(asi_bytes)
# RC4 Tree Shadow Distance withdrawn: the runtime must never read the old key or touch the SpeedTree distance CVars
for needle in ('RTTreeShadowDistance', 'RTSTConfigInstancingDistance', 'RTSTConfigUpdateDistance', 'RTSTConfigUpdateWindSimBlendDistance'):
    check(needle.encode() not in asi_bytes and needle.encode('utf-16-le') not in asi_bytes, 'ASI has no reference to ' + needle)
check(b'RTTreeShadowDistance' not in main.read('bin/config/r4game/user_config_matrix/pc/RefinedTracing.xml'), 'menu has no RTTreeShadowDistance row')
ro = lambda *a: subprocess.run(['llvm-readobj', *a, str(tmp)], capture_output=True, text=True).stdout
hdr = ro('--file-headers')
check('Machine: IMAGE_FILE_MACHINE_AMD64' in hdr and 'IMAGE_FILE_DLL' in hdr, 'ASI is an x64 DLL')
exports = re.findall(r'Name: (\S+)', ro('--coff-exports'))
check(sorted(exports) == ['CR01EarlyPrepare', 'InitializeASI'], 'exports exactly InitializeASI + CR01EarlyPrepare (UAL + legacy bootstrap)')
imp = ro('--coff-imports'); dlls = re.findall(r'^\s+Name: (\S+\.dll)', imp, re.M)
check(sorted(set(d.upper() for d in dlls)) == ['ADVAPI32.DLL', 'KERNEL32.DLL'], 'imports only KERNEL32 + ADVAPI32 (no CRT, no D3D/DXGI hooks)')
syms = set(re.findall(r'Symbol: (\w+)', imp))
allowed = set('''CloseHandle CreateFileW CreateMutexW CreateThread DeleteFileW DisableThreadLibraryCalls FindCloseChangeNotification
FindFirstChangeNotificationW FindNextChangeNotification FlushFileBuffers FlushInstructionCache GetCurrentProcess GetCurrentProcessId
GetFileAttributesExW GetFileAttributesW GetFileSizeEx GetLastError GetModuleFileNameW GetModuleHandleExW GetModuleHandleW
GetPrivateProfileStringW GetProcAddress GetProcessMitigationPolicy GetSystemTime InitOnceExecuteOnce LoadLibraryW MoveFileExW
ReadFile ReplaceFileW Sleep VirtualAlloc VirtualFree VirtualProtect WaitForSingleObject WriteFile CryptAcquireContextW
CryptCreateHash CryptDestroyHash CryptGetHashParam CryptHashData CryptReleaseContext'''.split())
check(syms <= allowed, 'imported APIs within the audited allow-list' + ('' if syms <= allowed else ': ' + str(syms - allowed)))
check('WritePrivateProfileString' not in imp, 'no INI writer imported (never writes dx12user.settings)')
check('tls' not in ro('--sections').lower().split('name: .tls') [0] if False else '.tls' not in ro('--sections'), 'no TLS section')
tmp.unlink()
# RC9: Black Hair Fix layering removed - no ReShade add-on, no layering exports or strings
check(not any(n.endswith('.addon64') or n.endswith('.addon') for n in mn), 'no ReShade add-on shipped (no ReShade dependency)')
for needle in (b'ZERO-OPT', b'RC1 runtime', b'RT private', b'C17 private', b'runtime-attested', b'private, pinned', b'native-passive', b'one-shot entry'):
    check(needle not in asi_bytes, 'no obsolete development log wording in the ASI: ' + needle.decode())
readme = main.read('IMPORTANT README!.txt').decode('utf-8')
zipver = re.search(r'Ultimate_Refined_Tracing_V(.+)_MAIN\.zip', next(out.glob('*_MAIN.zip')).name).group(1)
if 'rc' in zipver:   # private V5.1 test candidate
    check('PRIVATE TEST BUILD' in readme and ('V' + zipver) in readme, 'README marks the private test build ' + zipver)
else:
    check('RELEASE CANDIDATE' not in readme and ('V' + zipver) in readme, 'README is release text (no candidate wording)')
check(b'Normal RT performance controls ready' in asi_bytes and b'console probe grid' not in asi_bytes, 'V5.1 Normal RT performance module linked (no console probe grid code)')
check(not any(n in asi_bytes for n in REMOVED) and b'BalanceModeRT' not in asi_bytes,
      'ASI never reads a removed key (saved Console Probe Grid / Shading Rays values cannot reach the engine)')
for needle in (b'URTLayer', b'ReShadeRegister', b'layering', b'cr01_build_layered_dxil'):
    check(needle not in asi_bytes, 'ASI contains no layering/ReShade code or text: ' + needle.decode())
# menu
xmlv5 = main.read('bin/config/r4game/user_config_matrix/pc/RefinedTracing.xml'); xmlv4 = old_main.read('bin/config/r4game/user_config_matrix/pc/RefinedTracing.xml')
r5, r4 = ET.fromstring(xmlv5), ET.fromstring(xmlv4)
def rows(r):
    out = []
    for v in r.iter('Var'):
        v.tail = None; out.append(ET.tostring(v))
    return out
GONE431 = ('RTQualityShadingRaysPerProbe', 'RTPerfShadingRaysPerProbe')
def rows_kept(r):
    return [x for v, x in zip(r.iter('Var'), rows(r)) if v.get('id') not in GONE431 + ('QuickPresetSelect',)]
n431 = len(rows_kept(r4))
check(len(rows(r5)) == len(rows(r4)) - 2 + 4 and
      [x for v, x in zip(r5.iter('Var'), rows(r5)) if v.get('id') != 'QuickPresetSelect'][:n431] == rows_kept(r4),
      'all %d other V4.3.1 menu rows unchanged and in place; both Shading Rays rows removed' % n431)
V51 = ['URTSpaceRTPerf_01','RTReflLocalLights','RTShadowLocalLights','RTShadowFoliageDetail']
added = [v for v in r5.iter('Var')][len(rows(r4)) - 2:]
check([v.get('id') for v in added] == V51, 'exactly the V5.1 separator + 3 Normal RT rows appended at the end')
check(all(v.get('value') in ('0', '') for v in added), 'every new row defaults to Native / Auto (0)')
check(all(e.get('varId') == v.get('id') and e.get('value') == o.get('id') for v in added for o in v.iter('Option') for e in o.iter('Entry')),
      'new rows only set themselves (option index == stored value)')
check(not any(e.get('varId') in V51 for v in list(r5.iter('Var'))[:len(rows(r4)) - 2] for e in v.iter('Entry')),
      'XML preset entries never name the new rows: they are preset by the compiled script only')
for gone in REMOVED:
    check(gone not in xmlv5 and gone not in asi_bytes, 'removed control absent from menu and ASI: ' + gone.decode())
check(r5.find('Group').get('id') == 'RefinedTracingCR01', 'group id kept (compiled WCC + saved preferences compatibility)')
def qp(r):
    q = next(v for v in r.iter('Var') if v.get('id') == 'QuickPresetSelect')
    return {o.get('id'): [(e.get('varId'), e.get('value')) for e in o.iter('Entry')] for o in q.iter('Option')}
RC3 = {'1': {'RTQualityClassificationSecondary': '12', 'RTPerfClassificationSecondary': '12'},
       '2': {'RTQualityClassificationSecondary': '16', 'RTPerfClassificationSecondary': '16',
             'RTQualityShadingBudget': '262144', 'RTPerfShadingBudget': '262144'}}
q4, q5 = qp(r4), qp(r5)
check(all(q5[o] == [(k, RC3.get(o, {}).get(k, v)) for k, v in q4[o] if k not in GONE431] for o in q4),
      'Quick Preset XML entries: only the rc3 changes (Perf Secondary 12; Balanced Secondary 16 + Budget 262144; Shading Rays dropped)')
check(all(dict(q5['2']).get(g + k) == v for g in ('RTQuality', 'RTPerf') for k, v in
          (('ClassificationPrimary', '64'), ('ClassificationSecondary', '16'), ('MaxProbes', '65536'), ('AOQuality', '2'),
           ('ShadingBudget', '262144'), ('GlobalShadingLimit', '12288'), ('Grid1ShadingLimit', '49152'))),
      'Balanced = 64 / 16 / 65536 / 2 / 262144 / 12288 / 49152 in both RTGI groups')
presets = sum(1 for o in r5.iter('Option') for e in o.iter('Entry') if e.get('varId') != o.find('Entry').get('varId'))
presets4 = sum(1 for o in r4.iter('Option') for e in o.iter('Entry') if e.get('varId') != o.find('Entry').get('varId'))
check(presets == presets4 - 6, 'preset assignment entries: %d V4.3.1 - 6 Shading Rays = %d' % (presets4, presets))
# locales
names = {n.get('displayName') for n in r5.iter() if n.get('displayName')} - {'Mods.refinedtracing'}
var_labels = {v.get('displayName') for v in r5.iter('Var')}
for lang in sorted(codec.MAGIC):
    p = f'mods/modRefinedTracing/content/{lang}.w3strings'
    new = codec.decode(main.read(p), lang); old = codec.decode(old_main.read(p), lang)
    nk, ok_ = dict(new.block2), dict(old.block2); nt = {b[0]: b[3] for b in new.blocks}; ot = {b[0]: b[3] for b in old.blocks}
    kept = all(nk.get(k) == v for k, v in ok_.items())
    changed = {i for i in ot if ot[i] != nt.get(i)}
    resolv = all(h(n) in nk for n in names)
    # the options menu draws row labels from "option_" + displayName (RC2 UI defect)
    twins = all(h('option_' + d) in nk for d in var_labels)
    gone = not any(h(k) in nk for k in ('cr01_ptshadercompat', 'option_cr01_ptshadercompat', 'preset_value_ptshadercompat_0',
                                         'preset_value_ptshadercompat_1', 'preset_value_ptshadercompat_2'))
    del_ids = {ok_[h(k)] for k in LOC.DELETED_V431_KEYS}
    kept = all(nk.get(k) == v for k, v in ok_.items() if v not in del_ids) and not any(h(k) in nk for k in LOC.DELETED_V431_KEYS)
    changed = {i for i in ot if i not in del_ids and ot[i] != nt.get(i)}
    added_ids = set(nt) - set(ot)
    check(kept and not changed and set(ot) - set(nt) == del_ids and resolv and twins and gone and len(added_ids) == 15 and all(nt[i].strip() for i in added_ids),
          f'{lang}: every other V4.3.1 ID/key/text preserved, 12 Shading Rays records removed, +15 V5.1 strings, every row label resolves with its option_ twin')
print('FAIL %d' % len(fails) if fails else 'PASS release verification')
sys.exit(1 if fails else 0)
