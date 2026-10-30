#!/usr/bin/env python3
"""Adopt the WCC blob from URT_V52_RC9_RESULTS.zip (kit rev12) only if every receipt check passes.
Writes game/wcc/precompiled.rsblob(+.sha256), game/wcc/V52_VERIFIED_WCC.json and
game/wcc/V52_WCC_MENU_AT_COMPILE.xml (the kit's menu reference), and copies the receipt and the
read-only native graphics-menu evidence to docs/evidence_rc9/. Nothing is adopted on any failure.
Usage: finalize_rc6.py <URT_RUN_*.zip or URT_V52_RC9_RESULTS.zip> <workdir>"""
import hashlib, io, json, shutil, sys, zipfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools' / 'v52_selected'))
import spec as S
zp, wd = Path(sys.argv[1]), Path(sys.argv[2])
sha = lambda b: hashlib.sha256(b).hexdigest()
if wd.exists():
    shutil.rmtree(wd)
wd.mkdir(parents=True)
outer = zipfile.ZipFile(zp)
names = outer.namelist()
inner = [n for n in names if n.endswith('URT_V52_RC9_RESULTS.zip')]
z = zipfile.ZipFile(io.BytesIO(outer.read(inner[0]))) if inner else outer
for n in z.namelist():
    assert not n.startswith(('/', '\\')) and '..' not in n, n
    if n.endswith(('/', '\\')):
        continue
    dst = wd / n.replace('\\', '/')          # the Windows kit writes backslash entry names
    dst.parent.mkdir(parents=True, exist_ok=True)
    dst.write_bytes(z.read(n))
rtxt = (wd / 'BUILD_RECEIPT.txt').read_text('utf-8-sig')
rec = {}
for line in rtxt.splitlines():
    if '=' in line:
        k, v = line.split('=', 1); rec.setdefault(k.strip(), v.strip())
fails = []
def check(c, m):
    print(('PASS ' if c else 'FAIL ') + m)
    if not c:
        fails.append(m)
kit = json.loads((ROOT / 'docs/evidence_rc9/KIT_rev12_MANIFEST.json').read_text())
cand_b = (ROOT / 'game/wcc/RefinedTracingCR01_RTGI3.ws').read_bytes()
kit_menu = (ROOT / 'docs/evidence_rc9/REFINEDTRACING_V52_MENU_REFERENCE.xml').read_bytes()
check('kit rev12' in rtxt, 'receipt from kit rev12')
check(rec.get('CandidateSourceSHA256') == sha(cand_b) == kit['candidate_sha256'], 'receipt names the exact final script ' + sha(cand_b)[:12])
check(rec.get('V52MenuXMLSHA256') == sha(kit_menu) == kit['menu_sha256'], 'receipt names the kit menu reference ' + sha(kit_menu)[:12])
check(rec.get('CompiledStaticChecksPassed') == 'True' and rec.get('WccExitCode') == '0', 'WCC compiled, static checks passed')
check(rec.get('WorkspaceMatchesPreRunManifest') == 'True' and rec.get('NoGameInstallationPerformed') == 'True', 'workspaces restored exactly; nothing installed')
check(rec.get('CompilerMatchesV430Build') == 'True', 'same REDkit WCC as every earlier build')
blob = (wd / 'precompiled.rsblob').read_bytes() if (wd / 'precompiled.rsblob').exists() else b''
check(bool(blob) and sha(blob) == rec.get('BlobSHA256') and len(blob) == int(rec.get('BlobBytes', '0')), 'blob file == receipt hash and size')
need = [b'PTSceneRange', b'tooltip_cr01_pt_scene_range', b'RT_CR01_IsOwnedOption', b'RT_CR01_ApplyQuickPreset', b'RTShadowFoliageDetail', b'RT_CR01_IsPresetURTX', b'URTExtended']
need += [r['id'].encode() for r in S.ROWS] + [('tooltip_' + r['id'].lower()).encode() for r in S.ROWS]
gone = [b'RTQualityShadingBudget', b'RTPerfShadingBudget', b'tooltip_cr01_rt_quality_budget', b'tooltip_cr01_rt_perf_budget',
        b'PTRTAOEnabled', b'URTX_PTTextureDetail', b'URTX_TextureBudget', b'URTX_TextureSharpness']
check(all(n in blob for n in need) and not any(n in blob for n in gone),
      'compiled script: all RC6 rows and tooltips present; Shading Frame Budget and withdrawn rows absent')
if fails:
    print('FINALIZE REFUSED: %d check(s) failed; nothing adopted' % len(fails)); sys.exit(1)
(ROOT / 'game/wcc/precompiled.rsblob').write_bytes(blob)
(ROOT / 'game/wcc/precompiled.rsblob.sha256').write_text(sha(blob) + '  precompiled.rsblob\n')
(ROOT / 'game/wcc/V52_WCC_MENU_AT_COMPILE.xml').write_bytes(kit_menu)
ev = ROOT / 'docs/evidence_rc9'
shutil.copy2(zp, ev / zp.name)
shutil.copy2(wd / 'BUILD_RECEIPT.txt', ev / 'WCC_BUILD_RECEIPT.txt')
if (wd / 'NATIVE_MENU_EVIDENCE').is_dir():  # incl. MENU_MOVIES.txt (menu movies defining W3OptionsSeparator)
    dst = ev / 'NATIVE_MENU_EVIDENCE'
    if dst.exists():
        shutil.rmtree(dst)
    shutil.copytree(wd / 'NATIVE_MENU_EVIDENCE', dst)
receipt = {
    'kit': 'URT_V5.2.0_FINAL_PRIVATE_WCC_KIT_rev12', 'receipt_label': rtxt.splitlines()[0].strip(),
    'user_results_zip': zp.name, 'user_results_zip_sha256': sha(zp.read_bytes()),
    'candidate_source_sha256': sha(cand_b), 'menu_sha256': sha(kit_menu),
    'blob_sha256': sha(blob), 'blob_bytes': len(blob), 'wcc_exit_code': 0, 'workspace_restored': True,
    'static_checks_passed': True, 'no_game_installation_performed': True, 'gameplay_validated': False,
}
(ROOT / 'game/wcc/V52_VERIFIED_WCC.json').write_text(json.dumps(receipt, indent=2) + '\n')
print('ADOPTED: blob', sha(blob))
