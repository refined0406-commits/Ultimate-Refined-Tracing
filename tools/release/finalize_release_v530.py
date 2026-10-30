#!/usr/bin/env python3
"""V5.3.0 release: verify the built archives and write the public DEVELOPMENT_SOURCE zip and SHA256SUMS.txt.
Usage: python3 tools/release/finalize_release_v530.py OUT   (OUT = the folder written by `python3 build.py --out OUT`)
Checks (every one must pass; nothing is published):
 * MAIN = ASI-ONLY + the Ultimate ASI Loader (winmm.dll, pinned hash) + its licence; exact file lists, no development files;
 * the same UltimateRefinedTracing.asi in MAIN, ASI-ONLY, OUT/bin and build_manifest.json; its version string is "5.3.0";
 * precompiled.rsblob = the WCC receipt's blob (game/wcc/V52_VERIFIED_WCC.json); menu = build/menu; 18 w3strings;
 * README is the public V5.3.0 text (no test-build wording);
 * the public source = `git archive HEAD` without docs/evidence* (raw WCC run captures: local Windows paths and game
   files) and without private history; scanned for local paths, user names and credentials."""
import hashlib, io, json, re, subprocess, sys, zipfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
OUT = Path(sys.argv[1]).resolve()
V = '5.3.0'
fails = 0
def check(c, m):
    global fails
    print(('PASS ' if c else 'FAIL ') + m)
    if not c: fails += 1
sha = lambda b: hashlib.sha256(b).hexdigest()
MAIN, ASIO = OUT / f'Ultimate_Refined_Tracing_V{V}_MAIN.zip', OUT / f'Ultimate_Refined_Tracing_V{V}_ASI_ONLY.zip'
LANGS = ['ar', 'br', 'cn', 'cz', 'de', 'en', 'es', 'esmx', 'fr', 'hu', 'it', 'jp', 'kr', 'pl', 'ru', 'tr', 'ua', 'zh']
COMMON = {'Readme.txt', 'bin/x64_dx12/UltimateRefinedTracing.asi', 'bin/config/r4game/user_config_matrix/pc/RefinedTracing.xml',
          'mods/modRefinedTracing/content/precompiled.rsblob'} | \
         {f'mods/modRefinedTracing/content/{l}.w3strings' for l in LANGS}
EXTRA = {'bin/x64_dx12/winmm.dll', 'LICENSES/Ultimate-ASI-Loader-LICENSE.txt'}
zm, za = zipfile.ZipFile(MAIN), zipfile.ZipFile(ASIO)
check(set(za.namelist()) == COMMON, 'ASI-ONLY: exactly the %d install files (ASI, menu, compiled script, 18 languages, README)' % len(COMMON))
check(set(zm.namelist()) == COMMON | EXTRA, 'MAIN: the same files + Ultimate ASI Loader (winmm.dll) + its licence')
check(all(zm.read(n) == za.read(n) for n in COMMON), 'MAIN and ASI-ONLY: every shared file byte-identical (incl. the ASI)')
asi = za.read('bin/x64_dx12/UltimateRefinedTracing.asi')
man = json.loads((OUT / 'build_manifest.json').read_text())
check(asi == (OUT / 'bin/UltimateRefinedTracing.asi').read_bytes(), 'the archived ASI is the one this build produced (OUT/bin)')
check(sha(asi) in json.dumps(man), 'build_manifest.json records this ASI hash (the reproducible double build)')
check(b'Ultimate Refined Tracing 5.3.0 | session started' in asi and not re.search(rb'5\.3\.0_|PRIVATE[ _](R[0-9]|TEST|BUILD)|R12', asi),
      'ASI version string is exactly "5.3.0"; no test-build identifier (5.3.0_PRIVATE..., R12) left in the binary')
rc = json.loads((ROOT / 'game/wcc/V52_VERIFIED_WCC.json').read_text())
blob = za.read('mods/modRefinedTracing/content/precompiled.rsblob')
check(sha(blob) == rc['blob_sha256'] and len(blob) == rc['blob_bytes'] and rc['wcc_exit_code'] == 0 and rc['workspace_restored'] is True,
      'compiled script = the WCC receipt blob %s (%s)' % (rc['blob_sha256'][:8], rc['kit']))
check(sha(za.read('bin/config/r4game/user_config_matrix/pc/RefinedTracing.xml')) == sha((ROOT / 'build/menu/RefinedTracing.xml').read_bytes()) and
      rc['menu_sha256'] == sha((ROOT / 'docs/evidence_v530/REFINEDTRACING_V520R_MENU_REFERENCE.xml').read_bytes()),
      'menu = the generated menu; the receipt names the kit menu reference')
check(all((ROOT / 'build/loc' / f'{l}.w3strings').read_bytes() == za.read(f'mods/modRefinedTracing/content/{l}.w3strings') for l in LANGS), 'all 18 w3strings = the generated ones')
check(sha(zm.read('bin/x64_dx12/winmm.dll')) == '031a3e5576d91dce1e438d36b9a3d462c7334ab4791990a8ff1e3ddc0e132daf', 'winmm.dll = unmodified Ultimate ASI Loader 9.7.4 (pinned hash)')
rd = za.read('Readme.txt').decode('utf-8')
check(rd.startswith('ULTIMATE REFINED TRACING V5.3.0 - README') and rd == zm.read('Readme.txt').decode('utf-8') and not re.search(r'PRIVATE|TEST BUILD|R12|FIX[0-9]|HourLines|W3LoadFix|VRAM Relief|Memory Stabilizer|Claude|Anthropic|ChatGPT|\bAI\b', rd) and
      not any('IMPORTANT README' in n for z in (zm, za) for n in z.namelist()), 'Readme.txt: public V5.3.0 text in MAIN and ASI-ONLY (identical); no test-build wording, no unrelated-mod or AI-tool references; no old README')
check(not any(re.search(r'\.(redswf|swf|bundle|w2[a-z]*|xbm|dds|cache)$', n, re.I) for z in (zm, za) for n in z.namelist()),
      'no game asset (menu movie, bundles, textures, caches) in the installable archives')
dev = [n for z in (zm, za) for n in z.namelist() if re.search(r'\.(log|pdb|bak|ll|py|ws|obj|lib|exp|tmp|dmp|json|md)$', n, re.I)]
check(not dev, 'no development, debug or temporary files in the installable archives %s' % dev[:3])
# ---- public source ----
tar = subprocess.run(['git', '-C', str(ROOT), 'archive', '--format=zip', '--prefix=URT5/', 'HEAD'], capture_output=True, check=True).stdout
src = zipfile.ZipFile(io.BytesIO(tar))
keep = [i for i in src.infolist() if not re.match(r'URT5/docs/evidence|URT5/game/wcc/panel_ingamemenu\.redswf$', i.filename)]
# built from parts so that this file does not match itself
_P = [b'/home/' + b'claude', b'/tmp/' + b'claude', b'claude' + b'-0', b'users\\\\' + b'refin', b'refined' + b'0406',
      b'ghp_' + b'[A-Za-z0-9]{20}', b'github' + b'_pat_', b'BEGIN (RSA|OPENSSH) ' + b'PRIVATE KEY']
leak = re.compile(b'|'.join(_P), re.I)
bad = [i.filename for i in keep if not i.is_dir() and leak.search(src.read(i))]
check(not bad, 'public source: no local path, user name or credential in %d files %s' % (len(keep), bad[:3]))
dst = OUT / f'Ultimate_Refined_Tracing_V{V}_DEVELOPMENT_SOURCE.zip'
with zipfile.ZipFile(dst, 'w', zipfile.ZIP_DEFLATED, compresslevel=9) as z:
    for i in sorted(keep, key=lambda i: i.filename):
        if i.is_dir(): continue
        info = zipfile.ZipInfo(i.filename, (2026, 10, 30, 0, 0, 0)); info.compress_type = zipfile.ZIP_DEFLATED
        info.external_attr = i.external_attr
        z.writestr(info, src.read(i))
dz = zipfile.ZipFile(dst)
check(dz.read('URT5/game/wcc/precompiled.rsblob') == blob and dz.read('URT5/build.py') == (ROOT / 'build.py').read_bytes(),
      'public source carries the exact compiled script and build script of this release')
check(not any('evidence' in n for n in dz.namelist() if n.startswith('URT5/docs/')), 'public source: raw WCC run captures excluded (hashes in docs/release_v5.3.0/BUILD_PROVENANCE.md)')
check(not any(n.lower().endswith(('.redswf', '.swf')) for n in dz.namelist()), 'public source: the game menu movie (CD PROJEKT RED asset) excluded')
names = [MAIN.name, ASIO.name, dst.name]
(OUT / 'SHA256SUMS.txt').write_text(''.join('%s  %s\n' % (sha((OUT / n).read_bytes()), n) for n in names))
print((OUT / 'SHA256SUMS.txt').read_text(), end='')
print('ASI %s' % sha(asi))
print('TOTAL %s %d' % ('FAIL' if fails else 'PASS', fails))
sys.exit(1 if fails else 0)
