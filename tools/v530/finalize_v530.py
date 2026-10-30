#!/usr/bin/env python3
"""Adopt the WCC blob from URT_V530_PRIVATE_RESULTS.zip (kit v530rel-rev13) only if every receipt check passes.
Writes game/wcc/precompiled.rsblob(+.sha256), game/wcc/V52_VERIFIED_WCC.json (the receipt file name build.py
reads) and game/wcc/V52_WCC_MENU_AT_COMPILE.xml (the kit's menu reference), and copies the receipt and the
read-only evidence to docs/evidence_v520rel/. Nothing is adopted on any failure.
Usage: finalize_v520.py <URT_RUN_*.zip or URT_V520_RELEASE_RESULTS.zip> <workdir>"""
import hashlib, io, json, shutil, sys, zipfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools' / 'v52_selected'))
import spec as S
RESULTS = 'URT_V530_PRIVATE_RESULTS.zip'; KIT = 'v530rel-rev13'
EV = ROOT / 'docs/evidence_v530'
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
kit_menu = (EV / 'REFINEDTRACING_V520R_MENU_REFERENCE.xml').read_bytes()
check(('kit %s' % KIT) in rtxt, 'receipt from kit ' + KIT)
check(rec.get('CandidateSourceSHA256') == sha(cand_b) == kit['candidate_sha256'], 'receipt names the exact V5.3.0 private script ' + sha(cand_b)[:12])
check(rec.get('BaselineV520FinalSourceSHA256') == kit['baseline_sha256'], 'baseline = V5.2.0 final script')
check(rec.get('V520RMenuXMLSHA256') == sha(kit_menu) == kit['menu_sha256'], 'receipt names the kit menu reference ' + sha(kit_menu)[:12])
check(sha(kit_menu) == sha((ROOT / 'build/menu/RefinedTracing.xml').read_bytes()), 'kit menu == menu built from this source')
check(rec.get('CompiledStaticChecksPassed') == 'True' and rec.get('WccExitCode') == '0', 'WCC compiled, static checks passed')
check(rec.get('WorkspaceMatchesPreRunManifest') == 'True' and rec.get('NoGameInstallationPerformed') == 'True', 'workspaces restored exactly; nothing installed')
# R12: REDkit's wcc_lite.exe was updated by Steam between the R11 run (15:06 UTC, 9f448e0c...) and the R12 run (23:33 UTC) on
# 29 October 2026, the day of game build 1090142. That updated compiler is accepted by its exact hash; any other is refused.
KNOWN_WCC = {'9f448e0c8b9ea1ea803d0ae543f2fcd6e905088b8b71f54da2bff22c6cf318fc': 'REDkit WCC of every build up to R11',
             '1ea22410ccca10c44b3d8a083ae8b862cdd79fa1e5cf4d028120c078078717fe': 'REDkit WCC as updated on 29 October 2026'}
check(rec.get('CompilerMatchesV430Build') == 'True' or rec.get('WindowsWCCSHA256') in KNOWN_WCC,
      'known REDkit WCC: ' + KNOWN_WCC.get(rec.get('WindowsWCCSHA256', ''), 'UNKNOWN ' + rec.get('WindowsWCCSHA256', '')))
blob = (wd / 'precompiled.rsblob').read_bytes() if (wd / 'precompiled.rsblob').exists() else b''
check(bool(blob) and sha(blob) == rec.get('BlobSHA256') and len(blob) == int(rec.get('BlobBytes', '0')), 'blob file == receipt hash and size')
check(sha(blob) not in ('b21e4dc007b57a30092032bc8bb865fc07e3a35b2bf431345364e58c5a2e2cc2', '2f3927cc4a3ab4200e5bdbfa726a7e329d2cb6cd03143e334b886191469eb5a7', 'b4f03464f401d9e6c3f2b5249afd9eb7c2d7f9dff140b00a8ba3a2a5330dcf43', 'f13fdbb68b748541dc73cffc1ff86a7d813661cbc6cf6e7ee54a9fac0972d308', 'fa1ac2f85e2ff3a5226cd043c114026ee0e90992b63dde6f14fe0ec8cb283feb', '4d7b396c8b026ab29614f1e1113133f427d350cfd117c1668a9ea1044e2982d9', 'ea4edb1c888a02a3dcb63a53923e974b87b2fcb9d50db3994d2d47f5fb5c1b73', '4ba66ba145b04ffa0bc85795c28261ade0606d1d149319ab7621695d3a2171ab', '5f5adb2badc4bbd090f3490d8009e63a17f2deba38549ee0f85a8f24f55b9910', 'b8b56a56bd5e39566ae9fb2cac5a39426f9a08fdfc333bdd30a6f70b66a47458', '8c48a00bda5276fb58b817080a053ce889575122c72150385083b427612f71eb', '9656b905ac6aae113a5608e280dea25eeced9102057e665258a923c3d60d91c9', '813e64d0bcc97d500c62d413de602ceaa9742548dcfff384d91729e393583568', 'a14a9a4ad70b408a003ddedfe5eb14feaf8ce831b256e79525da31f9468d8821'), 'blob is not any earlier blob (approved V5.2.0, V5.3.0 rev1, rev3, R7, R8, R9, R11, R12, R12 FIX2, FIX3, FIX4): new source compiled')
need = [b'PTSceneRange', b'tooltip_cr01_pt_scene_range', b'RT_CR01_IsOwnedOption', b'RT_CR01_ApplyQuickPreset', b'RTShadowFoliageDetail', b'RT_CR01_IsPresetURTX', b'URTExtended']
need += [r['id'].encode() for r in S.MENU_ROWS] + [('tooltip_' + r['id'].lower()).encode() for r in S.MENU_ROWS]
gone = [b'RTQualityShadingBudget', b'RTPerfShadingBudget', b'tooltip_cr01_rt_quality_budget', b'tooltip_cr01_rt_perf_budget',
        b'PTRTAOEnabled', b'URTX_PTTextureDetail', b'URTX_TextureBudget', b'URTX_TextureSharpness']
check(all(n in blob for n in need) and not any(n in blob for n in gone), 'compiled script: every row and tooltip present (incl. the 9 expanded rows); withdrawn rows absent')
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
    'kit': 'URT_V5.3.0_PRIVATE_WCC_KIT_' + KIT, 'receipt_label': rtxt.splitlines()[0].strip(),
    'user_results_zip': zp.name, 'user_results_zip_sha256': sha(zp.read_bytes()),
    'candidate_source_sha256': sha(cand_b), 'menu_sha256': sha(kit_menu),
    'blob_sha256': sha(blob), 'blob_bytes': len(blob), 'wcc_exit_code': 0, 'workspace_restored': True,
    'static_checks_passed': True, 'no_game_installation_performed': True, 'gameplay_validated': False,
}
(ROOT / 'game/wcc/V52_VERIFIED_WCC.json').write_text(json.dumps(receipt, indent=2) + '\n')
print('ADOPTED: blob', sha(blob), '- now run: python3 build.py --out <dir>')
