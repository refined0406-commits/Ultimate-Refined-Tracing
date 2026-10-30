#!/usr/bin/env python3
"""Generate the URT V5.2.0 RC6 Windows WCC kit (rev11).

Same proven workflow as every kit since V5.1 rev5 (same compiler path, C:\\RT_BLOB workspaces,
command line, C:\\URT_BK hash-verified backup, restore and post-run manifest check, read-only
CHECK step). Only the inputs and the pinned expectations change:
  * candidate = game/wcc/RefinedTracingCR01_RTGI3.ws (RC6), proven equal to the shipped
    V5.0.0-hotfix1 script plus N hash-pinned exact replacements (OP_nn_OLD -> OP_nn_NEW, each OLD
    occurring exactly once when applied in order) - the ops are derived mechanically here;
  * menu reference = build/menu/RefinedTracing.xml (held rows excluded);
  * counts: 54 described controls (2 of them held out of the menu until native-menu evidence),
    105 Quick Preset/setter writes, 34 Quick Preset UI rows, 3 saves, 1 GetGroupName;
  * the RC5 blob and every earlier blob are rejected as stale;
  * NEW, read-only, best effort: graphics-menu evidence. The game's own user_config_matrix XML files
    that mention any engine variable URT may own are COPIED (never modified) into the results
    zip, so the native-menu ownership of Terrain Preload / Tree and Bush Draw Distance can be decided.
Usage: make_kit_rc6.py <rev5 kit dir> <hotfix1 .ws> <out dir>"""
import difflib, hashlib, json, re, shutil, sys, zipfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools' / 'v52_selected'))
import spec as S
sha = lambda b: hashlib.sha256(b).hexdigest()
rev5, hotfix, out = map(Path, sys.argv[1:4])
KIT = 'rev11'
if out.exists():
    shutil.rmtree(out)
out.mkdir(parents=True)
base_b = hotfix.read_bytes(); assert sha(base_b) == '90509bc1b8d4f49db1badb31fb3b82db2edaf2c4c84a0d8fdd9705d93e1707f1'
cand_b = (ROOT / 'game/wcc/RefinedTracingCR01_RTGI3.ws').read_bytes()
base = base_b.decode('utf-8-sig'); cand = cand_b.decode('utf-8-sig')
assert '\r' not in base and '\r' not in cand

# ---- derive exact, unique, ordered replacement ops ----
bl = base.splitlines(keepends=True); cl = cand.splitlines(keepends=True)
groups = list(difflib.SequenceMatcher(None, bl, cl, autojunk=False).get_grouped_opcodes(1))
def simulate(ops):
    t = base
    for old, new in ops:
        if not old or t.count(old) != 1:
            return None
        t = t.replace(old, new)
    return t
ops = []
for g in groups:
    i1, i2, j1, j2 = g[0][1], g[-1][2], g[0][3], g[-1][4]
    ctx = 0
    while True:
        a, b = max(0, i1 - ctx), min(len(bl), i2 + ctx)
        c, d = j1 - (i1 - a), j2 + (b - i2)
        old, new = ''.join(bl[a:b]), ''.join(cl[c:d])
        if old and base.count(old) == 1:
            break
        ctx += 1
        assert ctx < 40
    ops.append((old, new))
# merge any overlapping ops (context growth) by re-deriving on the union of ranges
assert simulate(ops) == cand, 'ops do not reproduce the candidate'
(out / 'BASELINE_V500_HOTFIX1_DO_NOT_STAGE.ws').write_bytes(base_b)
(out / 'RefinedTracingCR01.ws').write_bytes(cand_b)
xml = (ROOT / 'build/menu/RefinedTracing.xml').read_bytes()
(out / 'REFINEDTRACING_V52_MENU_REFERENCE.xml').write_bytes(xml)
ops_ps = []
for i, (old, new) in enumerate(ops, 1):
    o, n = 'OP_%02d_OLD.txt' % i, 'OP_%02d_NEW.txt' % i
    (out / o).write_bytes(old.encode()); (out / n).write_bytes(new.encode())
    ops_ps.append(" @('%s','%s','%s','%s')" % (o, sha(old.encode()), n, sha(new.encode())))
(out / 'RC6_SOURCE_DIFF.patch').write_text(''.join(difflib.unified_diff(bl, cl, 'V5.0.0-hotfix1/RefinedTracingCR01.ws', 'V5.2.0-RC6/RefinedTracingCR01.ws')))

held = list(S.NATIVE_MENU_HOLD)
hovers = re.findall(r"this\.onHover(?:Dynamic)?\('([^']+)'\s*,", cand)
xml_t = xml.decode('utf-8')
assert len(hovers) == 54 and len(set(hovers)) == 54
assert all(('id="%s"' % h) in xml_t for h in hovers if h not in held)
NSET = len(re.findall(r"mInGameConfigWrapper\.SetVarValue\('RefinedTracingCR01'", cand))
NUI = cand.count('values.PushBackFlashObject(update);')
assert (NSET, NUI) == (105, 34), (NSET, NUI)

# engine variable names whose presence in a game XML is evidence of native-menu ownership
EVID = sorted({r['cvar'] for r in S.ROWS if r['cvar']} | {'ProximityTLASSize', 'ProximityExtendedTLASSize', 'ShadingRaysPerFrameBudget'})

ps = (rev5 / 'URT_V51_WCC.ps1').read_text()
def rep(a, b, c=1):
    global ps
    assert ps.count(a) == c, (a[:70], ps.count(a)); ps = ps.replace(a, b)
rep('# Ultimate Refined Tracing V5.1.0-rc3 PRIVATE WCC kit rev5 (3 controls; Shading Rays + Console Grid removed).\n'
    '# Same proven rev2-rev4 workflow; inputs: V5.0.0-hotfix1 source + exactly 16 hash-pinned exact replacements\n',
    '# Ultimate Refined Tracing V5.2.0 RC6 PRIVATE WCC kit %s (no shader change).\n'
    '# Same proven rev2-rev10 WCC workflow; inputs: V5.0.0-hotfix1 source + exactly %d hash-pinned exact replacements\n' % (KIT, len(ops)))
rep("$Xml = Join-Path $Root 'REFINEDTRACING_V51_MENU_REFERENCE.xml'", "$Xml = Join-Path $Root 'REFINEDTRACING_V52_MENU_REFERENCE.xml'")
rep("$CandidateHash = '9fb1965265bbcf0c6ec32df97bc4bccd4dc615251cad21d716d21694f219988c'", "$CandidateHash = '%s'" % sha(cand_b))
rep("$XmlHash = '3e2bdb96e1c12e0b05a5561ead973adeec65a1d421fc3a794b2818ccfab2a45f'", "$XmlHash = '%s'" % sha(xml))
STALE = {
    'V510BlobHash': '71c07eff388b3fca5da46fb1afcccf1d325f5f8719e9cf03f10f8822c8909899',
    'V52R1BlobHash': 'fdd10a278e1143b57d1a4f017d6be874dc9ab7164330482f82350d2f97fe7d73',
    'V52R2BlobHash': 'd2a3fb16ccbd32093fe45a48ecee31383506ea680ba8582be381c0e408b04bf9',
    'V52R3BlobHash': '748defd1' ,
    'V52RC5BlobHash': '27d0a30851a006602cc3c8c0574bb46865aeb172228f7bdb8b9ff8822f12095e',
}
STALE.pop('V52R3BlobHash')   # only a prefix is on record; the full-hash entries below are enough
rep("$Rc2BlobHash = 'c10ec1b63c31c6dbc780f922119e7ebd1bdd26dc6e01214adb0f09da8c20c9d6'      # V5.1.0-rc2 blob\n",
    "$Rc2BlobHash = 'c10ec1b63c31c6dbc780f922119e7ebd1bdd26dc6e01214adb0f09da8c20c9d6'      # V5.1.0-rc2 blob\n" +
    ''.join("$%s = '%s'\n" % kv for kv in STALE.items()))
a = ps.index('$Ops = @(\n'); b = ps.index('\n)\n', a)
ps = ps[:a] + '$Ops = @(\n' + ',\n'.join(ops_ps) + ps[b:]
rep("$ExpectedResultZip = 'URT_V51_WCC_RESULTS.zip'", "$ExpectedResultZip = 'URT_V52_RC6_RESULTS.zip'")
rep("throw 'Candidate differs from the EXACT approved V5.1-rc3 patch of V5.0.0-hotfix1.'",
    "throw 'Candidate differs from the EXACT approved V5.2.0 RC6 patch of V5.0.0-hotfix1.'")
rep("  foreach ($gone in @('RTGIConsoleGrid','RTQualityShadingRaysPerProbe','RTPerfShadingRaysPerProbe','shading_rays','console_grid')) {",
    "  foreach ($gone in @('RTGIConsoleGrid','RTQualityShadingRaysPerProbe','RTPerfShadingRaysPerProbe','shading_rays','console_grid','RTQualityShadingBudget','RTPerfShadingBudget','_budget\"','PTRTAOEnabled','PTRTAORayCount','PTRTAORadius','PTRTAOStrength','URTX_TextureBudget','URTX_TextureSharpness','URTX_PTTextureDetail')) {")
rep("if (([regex]::Matches($b,'theGame\\.SaveUserSettings\\(\\);')).Count -ne 2) { throw 'Unexpected change to save-call count.' }",
    "if (([regex]::Matches($b,'theGame\\.SaveUserSettings\\(\\);')).Count -ne 3) { throw 'Unexpected change to save-call count (RC5: 3).' }")
rep("if (([regex]::Matches($b,\"mInGameConfigWrapper\\.SetVarValue\\('RefinedTracingCR01'\")).Count -ne 123) { throw 'Unexpected configuration setter count (expected 120 - 6 + 9).' }",
    "if (([regex]::Matches($b,\"mInGameConfigWrapper\\.SetVarValue\\('RefinedTracingCR01'\")).Count -ne %d) { throw 'Unexpected configuration setter count (RC5 111 - 6 Shading Frame Budget).' }" % NSET)
rep("if (([regex]::Matches($b,'values\\.PushBackFlashObject\\(update\\);')).Count -ne 40) { throw 'Unexpected Quick Preset UI row count (expected 39 - 2 + 3).' }",
    "if (([regex]::Matches($b,'values\\.PushBackFlashObject\\(update\\);')).Count -ne %d) { throw 'Unexpected Quick Preset UI row count (RC5 36 - 2 Shading Frame Budget).' }" % NUI)
rep("if ($controls.Count -ne 42) { throw \"Expected 42 menu controls, found $($controls.Count)\" }",
    "if ($controls.Count -ne 54) { throw \"Expected 54 described controls, found $($controls.Count)\" }")
rep("  foreach ($c in $controls) { if (-not $xmlText.Contains('<Var id=\"' + $c.Groups[1].Value + '\"')) { throw \"Control not in menu XML: $($c.Groups[1].Value)\" } }",
    "  $held = @(%s)\n"
    "  foreach ($c in $controls) { $cid = $c.Groups[1].Value; if ($held -contains $cid) { continue }; if (-not ($xmlText.Contains('<Var id=\"' + $cid + '\"') -or $xmlText.Contains('id=\"' + $cid + '\" displayName'))) { throw \"Control not in menu XML: $cid\" } }\n"
    "  if ($b.Contains(\"SetVarValue('URTExtended'\") -or $b.Contains(\"SetVarValue('RefinedTracingCR01', 'PTSceneRange'\")) { throw 'A new RC6 row is written by script (Quick Presets must not assign new rows).' }"
    % ','.join("'%s'" % h for h in held))
rep("  foreach ($gone in @('RTGIConsoleGrid','ShadingRaysPerProbe')) { if ($xmlText.Contains($gone)) { throw \"Removed control still in menu XML: $gone\" } }",
    "  foreach ($gone in @('RTGIConsoleGrid','ShadingRaysPerProbe','ShadingBudget','SUBTLE_SEPARATOR','PTRTAOEnabled','URTX_TextureBudget','URTX_TextureSharpness','URTX_PTTextureDetail')) { if ($xmlText.Contains($gone)) { throw \"Removed item still in menu XML: $gone\" } }")
rep("if ($blobHash -eq $ProtectedBlobHash -or $blobHash -eq $Rc1BlobHash -or $blobHash -eq $Rc2BlobHash -or",
    "if ($blobHash -eq $ProtectedBlobHash -or $blobHash -eq $Rc1BlobHash -or $blobHash -eq $Rc2BlobHash -or " +
    ' -or '.join('$blobHash -eq $%s' % k for k in STALE) + ' -or')
rep("  foreach ($gone in @('RTGIProbeRayDistance','RTReflTextureDetail','RTAOFoliageDetail','RTGIConsoleGrid','RTQualityShadingRaysPerProbe','RTPerfShadingRaysPerProbe','tooltip_cr01_rtgi_console_grid','shading_rays')) { if ($blobText.Contains($gone)) { throw \"Removed control still compiled: $gone\" } }",
    "  foreach ($gone in @('RTGIProbeRayDistance','RTReflTextureDetail','RTAOFoliageDetail','RTGIConsoleGrid','RTQualityShadingRaysPerProbe','RTPerfShadingRaysPerProbe','tooltip_cr01_rtgi_console_grid','shading_rays','RTQualityShadingBudget','RTPerfShadingBudget','tooltip_cr01_rt_quality_budget','tooltip_cr01_rt_perf_budget')) { if ($blobText.Contains($gone)) { throw \"Removed control still compiled: $gone\" } }")
rep("  foreach ($sym in @('tooltip_cr01_rt_refl_local_lights','tooltip_cr01_rt_shadow_foliage_detail',",
    "  foreach ($sym in @('tooltip_cr01_rt_refl_local_lights','tooltip_cr01_rt_shadow_foliage_detail',\n"
    "    'PTSceneRange','tooltip_cr01_pt_scene_range','URTX_PTCaustics','tooltip_urtx_ptcausticsarea','URTX_PTSkinBounceDesat','URTX_RTReflFog','URTX_TerrainPreload','URTX_TreeDistance','URTX_FGMenuResources',")
# read-only graphics-menu evidence (never fails the build)
EV_PS = r'''
  # ---- RC6 read-only graphics-menu evidence (best effort; copies, never modifies) ----
  try {
    $evDir = Join-Path $reportDir 'NATIVE_MENU_EVIDENCE'
    MakeDir $evDir
    $evNames = @(%s)
    $cands = @()
    $gameCfg = $Wcc -replace '(?i)\\The Witcher 3 REDkit\\bin\\x64_RedKit\\wcc_lite\.exe$', '\The Witcher 3\bin\config\r4game\user_config_matrix\pc'
    if ($gameCfg -ne $Wcc) { $cands += $gameCfg }
    $evLines = @("SearchedFolder=$gameCfg")
    foreach ($dir in $cands) {
      if (Test-Path -LiteralPath $dir -PathType Container) {
        foreach ($f in @(Get-ChildItem -LiteralPath $dir -File -Filter '*.xml' -ErrorAction Stop)) {
          if ($f.Length -gt 4194304) { continue }
          $t = [IO.File]::ReadAllText($f.FullName)
          $hit = @($evNames | Where-Object { $t.Contains($_) })
          $evLines += "$($f.Name) sha256=$(FileHash $f.FullName) mentions=$($hit -join ',')"
          if ($hit.Count -gt 0 -and $f.Name -ine 'RefinedTracing.xml') { Copy-Item -LiteralPath $f.FullName -Destination (Join-Path $evDir $f.Name) -ErrorAction Stop }
        }
      } else { $evLines += "FolderNotFound=$dir" }
    }
    $evLines | Set-Content -LiteralPath (Join-Path $evDir 'EVIDENCE_INDEX.txt') -Encoding UTF8
  } catch { "Evidence step skipped: $($_.Exception.Message)" | Set-Content -LiteralPath (Join-Path $reportDir 'NATIVE_MENU_EVIDENCE_SKIPPED.txt') -Encoding UTF8 }
''' % ','.join("'%s'" % n for n in EVID)
rep("  Copy-Item -LiteralPath $raw -Destination (Join-Path $reportDir 'precompiled.rsblob') -ErrorAction Stop\n",
    "  Copy-Item -LiteralPath $raw -Destination (Join-Path $reportDir 'precompiled.rsblob') -ErrorAction Stop\n" + EV_PS)
rep("  $lines = @('Ultimate Refined Tracing V5.1.0-rc3 WCC kit rev5 - READ-ONLY CHECK', ",
    "  $lines = @('Ultimate Refined Tracing V5.2.0 RC6 WCC kit %s - READ-ONLY CHECK', " % KIT)
rep("        'Ultimate Refined Tracing V5.1.0-rc3 PRIVATE WCC receipt, kit rev5',",
    "        'Ultimate Refined Tracing V5.2.0 RC6 PRIVATE WCC receipt, kit %s'," % KIT)
rep("\"V51MenuXMLSHA256=$XmlHash\",", "\"V52MenuXMLSHA256=$XmlHash\",")
assert 'V5.1-rc3' not in ps
assert all(ord(c) < 128 for c in ps) and 'Shader' not in ps
(out / 'URT_V52_RC6_KIT.ps1').write_bytes(ps.replace('\n', '\r\n').encode('ascii'))
for name in ('1_CHECK_ONLY.cmd', '2_COMPILE.cmd'):
    t = (rev5 / name).read_text()
    t = t.replace('URT V5.1.0-rc3 WCC KIT (rev5)', 'URT V5.2.0 RC6 WCC KIT (%s)' % KIT).replace('URT_V51_WCC.ps1', 'URT_V52_RC6_KIT.ps1') \
         .replace('URT_V51_WCC_RESULTS.zip', 'URT_V52_RC6_RESULTS.zip')
    assert 'V51' not in t and 'URT_V52_RC6_KIT.ps1' in t, name
    (out / name).write_bytes(t.replace('\r\n', '\n').replace('\n', '\r\n').encode('ascii'))
readme = """URT V5.2.0 RC6 PRIVATE - WCC COMPILE KIT (%s)
Nothing to install. Nothing in the game folder is changed. No SDK or developer software
is needed. Same two steps as every earlier kit.

1. Extract this ZIP to any new folder (NOT inside the game folder).
2. Double-click 1_CHECK_ONLY.cmd (read-only; wait for "SAFE TO COMPILE").
   If it says NOT SAFE, stop and send CHECK_REPORT.txt.
3. Double-click 2_COMPILE.cmd and wait for SUCCESS or FAILURE.
4. Send back URT_V52_RC6_RESULTS.zip from the new URT_RUN_... folder (also on failure).

What is compiled: the shipped V5.0.0-hotfix1 script plus exactly %d hash-pinned exact
replacements that produce the RC6 script (RC5 + the RC6 rows: PT: Scene Range, the six
PT: Caustics rows, PT: Skin Bounce Light Desaturation, RT/PT: Fog in Ray-Traced Reflections,
Terrain Preload Distance, Tree and Bush Draw Distance; both RTGI Shading Frame Budget rows and
their Quick Preset assignments removed). Any other difference is refused.
Same compiler, C:\\RT_BLOB workspaces, command, C:\\URT_BK backup and verification.
RC6 does not change URT's Path Tracing shader, so there is no shader step.

NEW (read-only): step 2 also COPIES, without changing anything, the game's own graphics
menu XML files from ...\\The Witcher 3\\bin\\config\\r4game\\user_config_matrix\\pc that
mention an engine setting URT controls, into the results ZIP. They decide whether
Terrain Preload Distance and Tree and Bush Draw Distance can be shown without touching
your native graphics-menu selections.
""" % (KIT, len(ops))
(out / 'README_START_HERE.txt').write_bytes(readme.replace('\n', '\r\n').encode('ascii'))
z = out.parent / ('URT_V5.2.0_RC6_PRIVATE_WCC_KIT_%s.zip' % KIT)
with zipfile.ZipFile(z, 'w', zipfile.ZIP_DEFLATED) as zf:
    for p in sorted(out.iterdir()):
        zf.write(p, p.name)
json.dump({'kit': KIT, 'ops': len(ops), 'candidate_sha256': sha(cand_b), 'menu_sha256': sha(xml),
           'zip': z.name, 'zip_sha256': sha(z.read_bytes())}, open(out.parent / ('KIT_%s_MANIFEST.json' % KIT), 'w'), indent=2)
print('PASS kit %s: %d ops; candidate %s; menu %s; %s %s' % (KIT, len(ops), sha(cand_b)[:12], sha(xml)[:12], z.name, sha(z.read_bytes())))
