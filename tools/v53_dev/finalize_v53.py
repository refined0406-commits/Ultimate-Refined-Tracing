#!/usr/bin/env python3
"""Adopt the WCC blob from URT_V53_DEV1_RESULTS.zip (kit v53dev-rev1) only if every receipt check passes.
Writes game/wcc/precompiled.rsblob(+.sha256), game/wcc/V52_VERIFIED_WCC.json (the receipt file name build.py
reads) and game/wcc/V52_WCC_MENU_AT_COMPILE.xml (the kit's menu reference), and copies the receipt and the
read-only V5.3 evidence to docs/evidence_v53dev1/. Nothing is adopted on any failure.
Usage: finalize_v53.py <URT_RUN_*.zip or URT_V53_DEV1_RESULTS.zip> <workdir>"""
import hashlib, io, json, shutil, sys, zipfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools' / 'v52_selected'))
import spec as S
RESULTS = 'URT_V53_DEV1_RESULTS.zip'; KIT = 'v53dev-rev1'
EV = ROOT / 'docs/evidence_v53dev1'
zp, wd = Path(sys.argv[1]), Path(sys.argv[2])
sha = lambda b: hashlib.sha256(b).hexdigest()
if wd.exists():
    shutil.rmtree(wd)
wd.mkdir(parents=True)
outer = zipfile.ZipFile(zp)
inner = [n for n in outer.namelist() if n.endswith(RESULTS)]
z = zipfile.ZipFile(io.BytesIO(outer.read(inner[0]))) if inner else outer
for n in z.namelist():
    assert not n.startswith(('/', '\\')) and '..' not in n, n
    if n.endswith(('/', '\\')):
        continue
    dst = wd / n.replace('\\', '/')
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
kit = json.loads((EV / ('KIT_%s_MANIFEST.json' % KIT)).read_text())
cand_b = (ROOT / 'game/wcc/RefinedTracingCR01_RTGI3.ws').read_bytes()
kit_menu = (EV / 'REFINEDTRACING_V53_MENU_REFERENCE.xml').read_bytes()
check(('kit %s' % KIT) in rtxt, 'receipt from kit ' + KIT)
check(rec.get('CandidateSourceSHA256') == sha(cand_b) == kit['candidate_sha256'], 'receipt names the exact V5.3 dev script ' + sha(cand_b)[:12])
check(rec.get('BaselineV520FinalSourceSHA256') == kit['baseline_sha256'], 'baseline = V5.2.0 final script')
check(rec.get('V53MenuXMLSHA256') == sha(kit_menu) == kit['menu_sha256'], 'receipt names the kit menu reference ' + sha(kit_menu)[:12])
check(sha(kit_menu) == sha((ROOT / 'build/menu/RefinedTracing.xml').read_bytes()), 'kit menu == menu built from this source')
check(rec.get('CompiledStaticChecksPassed') == 'True' and rec.get('WccExitCode') == '0', 'WCC compiled, static checks passed')
check(rec.get('WorkspaceMatchesPreRunManifest') == 'True' and rec.get('NoGameInstallationPerformed') == 'True', 'workspaces restored exactly; nothing installed')
check(rec.get('CompilerMatchesV430Build') == 'True', 'same REDkit WCC as every earlier build')
blob = (wd / 'precompiled.rsblob').read_bytes() if (wd / 'precompiled.rsblob').exists() else b''
check(bool(blob) and sha(blob) == rec.get('BlobSHA256') and len(blob) == int(rec.get('BlobBytes', '0')), 'blob file == receipt hash and size')
check(sha(blob) != 'b21e4dc007b57a30092032bc8bb865fc07e3a35b2bf431345364e58c5a2e2cc2', 'blob is not the V5.2.0 final blob (new source compiled)')
need = [b'PTSceneRange', b'tooltip_cr01_pt_scene_range', b'RT_CR01_IsOwnedOption', b'RT_CR01_ApplyQuickPreset', b'RTShadowFoliageDetail', b'RT_CR01_IsPresetURTX', b'URTExtended']
need += [r['id'].encode() for r in S.ROWS] + [('tooltip_' + r['id'].lower()).encode() for r in S.ROWS]
gone = [b'RTQualityShadingBudget', b'RTPerfShadingBudget', b'tooltip_cr01_rt_quality_budget', b'tooltip_cr01_rt_perf_budget',
        b'PTRTAOEnabled', b'URTX_PTTextureDetail', b'URTX_TextureBudget', b'URTX_TextureSharpness']
check(all(n in blob for n in need) and not any(n in blob for n in gone), 'compiled script: every V5.2.0 and V5.3 row and tooltip present; withdrawn rows absent')
if fails:
    print('FINALIZE REFUSED: %d check(s) failed; nothing adopted' % len(fails)); sys.exit(1)
(ROOT / 'game/wcc/precompiled.rsblob').write_bytes(blob)
(ROOT / 'game/wcc/precompiled.rsblob.sha256').write_text(sha(blob) + '  precompiled.rsblob\n')
(ROOT / 'game/wcc/V52_WCC_MENU_AT_COMPILE.xml').write_bytes(kit_menu)
shutil.copy2(zp, EV / zp.name)
shutil.copy2(wd / 'BUILD_RECEIPT.txt', EV / 'WCC_BUILD_RECEIPT.txt')
if (wd / 'V53_READONLY_EVIDENCE').is_dir():
    dst = EV / 'V53_READONLY_EVIDENCE'
    if dst.exists():
        shutil.rmtree(dst)
    shutil.copytree(wd / 'V53_READONLY_EVIDENCE', dst)
receipt = {
    'kit': 'URT_V5.3_DEV1_PRIVATE_WCC_KIT_' + KIT, 'receipt_label': rtxt.splitlines()[0].strip(),
    'user_results_zip': zp.name, 'user_results_zip_sha256': sha(zp.read_bytes()),
    'candidate_source_sha256': sha(cand_b), 'menu_sha256': sha(kit_menu),
    'blob_sha256': sha(blob), 'blob_bytes': len(blob), 'wcc_exit_code': 0, 'workspace_restored': True,
    'static_checks_passed': True, 'no_game_installation_performed': True, 'gameplay_validated': False,
}
(ROOT / 'game/wcc/V52_VERIFIED_WCC.json').write_text(json.dumps(receipt, indent=2) + '\n')
print('ADOPTED: blob', sha(blob), '- now run: python3 build.py --out <dir>')
