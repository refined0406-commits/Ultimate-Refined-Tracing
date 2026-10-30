#!/usr/bin/env python3
"""Generate the URT V5.3.0 PRIVATE Windows WCC kit (v530priv-rev3): approved V5.2.0 + seven performance rows and PT: Moon Light (descriptions, Master greying) + the owner's
final Quick Preset assignments for the four gameplay-approved rows.

Same proven workflow as every kit since V5.1 (template: game/wcc/kit_v51, the rev3 kit; same compiler path,
C:\\RT_BLOB workspaces and command line, C:\\URT_BK hash-verified backup, restore and post-run manifest check,
read-only CHECK step). Only the inputs and the pinned expectations change:
  * baseline = the V5.2.0 FINAL script (a43cec54..., compiled by the owner with kit rev13 into blob b21e4dc0);
  * candidate = game/wcc/RefinedTracingCR01_RTGI3.ws, proven equal to the baseline plus N hash-pinned exact
    replacements (OP_nn_OLD -> OP_nn_NEW, each OLD occurring exactly once when applied in order);
  * menu reference = build/menu/RefinedTracing.xml (V5.3 dev);
  * the V5.2.0 final blob and every earlier blob are rejected as stale;
  * NEW read-only evidence (best effort, never fails the build): which game XML/INI files name the nine V5.3
    engine variables (with the matching lines), the game's graphics.xml and gc.ini (copied), and the W3LoadFix
    / loader files present in bin\\x64_dx12 (names, sizes, hashes; w3loadfix.ini and w3loadfix.log copied).
Usage: make_kit_v53.py <out dir>"""
import difflib, hashlib, json, re, shutil, sys, zipfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools' / 'v52_selected'))
import spec as S
sha = lambda b: hashlib.sha256(b).hexdigest()
out = Path(sys.argv[1])
KIT = 'v530rel-rev13'   # V5.3.0 release (rev12 = R12 FIX4, rev11 = R12 FIX3, rev10 = R12 FIX2, rev9 = R12, rev8 = R11, rev7 = R9/R10, rev6 = R8, rev5 = R7; rev4 = R6 alone, never run)
RESULTS = 'URT_V530_PRIVATE_RESULTS.zip'
V520_BLOB = 'b21e4dc007b57a30092032bc8bb865fc07e3a35b2bf431345364e58c5a2e2cc2'
DEV1_BLOB = '2f3927cc4a3ab4200e5bdbfa726a7e329d2cb6cd03143e334b886191469eb5a7'   # V5.3 DEV1/DEV2 (no new presets): stale
REL1_BLOB = 'b4f03464f401d9e6c3f2b5249afd9eb7c2d7f9dff140b00a8ba3a2a5330dcf43'   # rev1 (8 rows with values; superseded): stale
APPROVED_BLOB = 'f13fdbb68b748541dc73cffc1ff86a7d813661cbc6cf6e7ee54a9fac0972d308'   # gameplay-approved V5.2.0: stale for V5.3.0
V530R1_BLOB = 'fa1ac2f85e2ff3a5226cd043c114026ee0e90992b63dde6f14fe0ec8cb283feb'    # V5.3.0 private rev1 (no presets, raw titles): stale
V530R3_BLOB = '4d7b396c8b026ab29614f1e1113133f427d350cfd117c1668a9ea1044e2982d9'    # V5.3.0 private rev3 (R3..R5 installed blob): stale for R7
V530R7_BLOB = 'ea4edb1c888a02a3dcb63a53923e974b87b2fcb9d50db3994d2d47f5fb5c1b73'    # V5.3.0 private R7 (kit rev5): stale for R8
V530R8_BLOB = '4ba66ba145b04ffa0bc85795c28261ade0606d1d149319ab7621695d3a2171ab'    # V5.3.0 private R8 (kit rev6): stale for R9
V530R9_BLOB = '5f5adb2badc4bbd090f3490d8009e63a17f2deba38549ee0f85a8f24f55b9910'    # V5.3.0 private R9/R10 (kit rev7): stale for R11
V530R11_BLOB = 'b8b56a56bd5e39566ae9fb2cac5a39426f9a08fdfc333bdd30a6f70b66a47458'   # V5.3.0 private R11 (kit rev8): stale for R12
V530R12_BLOB = '8c48a00bda5276fb58b817080a053ce889575122c72150385083b427612f71eb'   # V5.3.0 private R12 / R12 FIX1 (kit rev9): stale for R12 FIX2
V530R12F2_BLOB = '9656b905ac6aae113a5608e280dea25eeced9102057e665258a923c3d60d91c9'   # V5.3.0 private R12 FIX2 (kit rev10): stale for R12 FIX3
V530R12F3_BLOB = '813e64d0bcc97d500c62d413de602ceaa9742548dcfff384d91729e393583568'   # V5.3.0 private R12 FIX3 (kit rev11): stale for the release
V530R12F4_BLOB = 'a14a9a4ad70b408a003ddedfe5eb14feaf8ce831b256e79525da31f9468d8821'   # V5.3.0 private R12 FIX4 (kit rev12): stale for the release (the script changed: Quick Presets)
# Safety (R7): the output folder is deleted and recreated; refuse anything that is not a dedicated kit folder
# (an earlier run with the work folder as argument deleted the whole work tree).
if out.resolve() in (ROOT.resolve(), ROOT.parent.resolve(), Path.home().resolve(), Path('/').resolve()) or not out.name.startswith('kit_') \
        or (out.exists() and any((out / x).exists() for x in ('.git', 'src', 'urt', 'exe', 'tools'))):
    raise SystemExit('REFUSED: output must be a dedicated folder named kit_* (it is deleted and recreated): %s' % out)
if out.exists():
    shutil.rmtree(out)
out.mkdir(parents=True)
base_b = (ROOT / 'tools/v53_dev/kit_base/V520_FINAL_RefinedTracingCR01.ws').read_bytes()
assert sha(base_b) == 'a43cec54c1528c525b3c0c0c32414a5faaf447683f6fee8e63119461584d4ac7'
cand_b = (ROOT / 'game/wcc/RefinedTracingCR01_RTGI3.ws').read_bytes()
base = base_b.decode('utf-8-sig'); cand = cand_b.decode('utf-8-sig')
assert '\r' not in base and '\r' not in cand and all(ord(c) < 128 for c in cand)
# ---- exact, unique, ordered replacement ops ----
bl = base.splitlines(keepends=True); cl = cand.splitlines(keepends=True)
ops = []
for g in difflib.SequenceMatcher(None, bl, cl, autojunk=False).get_grouped_opcodes(1):
    i1, i2, j1, j2 = g[0][1], g[-1][2], g[0][3], g[-1][4]
    ctx = 0
    while True:
        a, b = max(0, i1 - ctx), min(len(bl), i2 + ctx)
        c, d = j1 - (i1 - a), j2 + (b - i2)
        old, new = ''.join(bl[a:b]), ''.join(cl[c:d])
        if old and base.count(old) == 1:
            break
        ctx += 1; assert ctx < 40
    ops.append((old, new))
t = base
for old, new in ops:
    assert t.count(old) == 1; t = t.replace(old, new)
assert t == cand and len(ops) >= 2, 'ops do not reproduce the candidate'
(out / 'BASELINE_V520_FINAL_DO_NOT_STAGE.ws').write_bytes(base_b)
(out / 'RefinedTracingCR01.ws').write_bytes(cand_b)
xml = (ROOT / 'build/menu/RefinedTracing.xml').read_bytes()
(out / 'REFINEDTRACING_V520R_MENU_REFERENCE.xml').write_bytes(xml)
ops_ps = []
for i, (old, new) in enumerate(ops, 1):
    o, n = 'OP_%02d_OLD.txt' % i, 'OP_%02d_NEW.txt' % i
    (out / o).write_bytes(old.encode()); (out / n).write_bytes(new.encode())
    ops_ps.append(" @('%s','%s','%s','%s')" % (o, sha(old.encode()), n, sha(new.encode())))
(out / 'V520R_SOURCE_DIFF.patch').write_text(''.join(difflib.unified_diff(bl, cl, 'V5.2.0-final/RefinedTracingCR01.ws', 'V5.2.0-release/RefinedTracingCR01.ws')))
# ---- pinned expectations, measured on the candidate ----
retired = list(S.RETIRED_AFTER_COMPILE)
hovers = re.findall(r"this\.onHover(?:Dynamic)?\('([^']+)'\s*,", cand)
NH = len(hovers); assert NH == 54 + len(S.ROWS_V53) + len([r for r in S.ROWS_V530 if r.get('menu', True)]) + len(S.RETIRED_V530) and len(set(hovers)) == NH
xml_t = xml.decode('utf-8')
assert all(('id="%s"' % h) in xml_t for h in hovers if h not in retired)
NSET = len(re.findall(r"mInGameConfigWrapper\.SetVarValue\('RefinedTracingCR01'", cand))
NURTX = len(re.findall(r"mInGameConfigWrapper\.SetVarValue\('URTExtended'", cand))
NUI = cand.count('values.PushBackFlashObject(update);')
assert (NSET, NURTX, NUI) == (109, 27 + 3 * 17, 44 + 17), (NSET, NURTX, NUI)   # V5.2.0 final + the owner's 8 + 4 preset rows + the 5 V5.3.0 release rows
assert cand.count('theGame.SaveUserSettings();') == 3 and cand.count('GetGroupName(') == 1
V53IDS = [r['id'] for r in S.ROWS_V53] + [r['id'] for r in S.ROWS_V530 if r.get('menu', True)]
ps = (ROOT / 'game/wcc/kit_v51/URT_V51_WCC.ps1').read_text()
def rep(a, b, c=1):
    global ps
    assert ps.count(a) == c, (a[:80], ps.count(a)); ps = ps.replace(a, b)
rep('# Ultimate Refined Tracing V5.1 PRIVATE (Normal RT performance controls) WCC kit rev3.\n'
    '# Same proven rev2 workflow; inputs: V5.0.0-hotfix1 source + the exact 3-hunk V5.1 patch.\n',
    '# Ultimate Refined Tracing V5.3.0 PRIVATE WCC kit %s (no shader change).\n'
    '# Same proven rev2-rev13 workflow; inputs: V5.2.0 FINAL source + exactly %d hash-pinned exact replacements.\n' % (KIT, len(ops)))
rep("$Baseline = Join-Path $Root 'BASELINE_V500_HOTFIX1_DO_NOT_STAGE.ws'", "$Baseline = Join-Path $Root 'BASELINE_V520_FINAL_DO_NOT_STAGE.ws'")
rep("$Xml = Join-Path $Root 'REFINEDTRACING_V51_MENU_REFERENCE.xml'", "$Xml = Join-Path $Root 'REFINEDTRACING_V520R_MENU_REFERENCE.xml'")
rep("$CandidateHash = '9797e4b12d3899d088fe84d520f341bd3b08d8f67dae50458dd7091adbcc7df3'", "$CandidateHash = '%s'" % sha(cand_b))
rep("$BaselineHash = '90509bc1b8d4f49db1badb31fb3b82db2edaf2c4c84a0d8fdd9705d93e1707f1'", "$BaselineHash = '%s'" % sha(base_b))
rep("$XmlHash = '848fa37ec000f06814597abdd2fc0ae4a71de9b4d1d2e0e63166c354a6601cc1'", "$XmlHash = '%s'" % sha(xml))
STALE = {
    'V510BlobHash': '71c07eff388b3fca5da46fb1afcccf1d325f5f8719e9cf03f10f8822c8909899',
    'V52R1BlobHash': 'fdd10a278e1143b57d1a4f017d6be874dc9ab7164330482f82350d2f97fe7d73',
    'V52R2BlobHash': 'd2a3fb16ccbd32093fe45a48ecee31383506ea680ba8582be381c0e408b04bf9',
    'V52RC5BlobHash': '27d0a30851a006602cc3c8c0574bb46865aeb172228f7bdb8b9ff8822f12095e',
    'V52RC6BlobHash': '0d39070a00e107dd577c9e5caf3e26f4313208a173962d19e1fb8ca137230ebb',
    'V52RC9BlobHash': '238db3831a984828fab892edc15c7617fe92d5d3ef1210d28a84cab434a4d652',
    'V520FinalBlobHash': V520_BLOB,
    'V53DevBlobHash': DEV1_BLOB,
    'V520Rel1BlobHash': REL1_BLOB,
    'V520ApprovedBlobHash': APPROVED_BLOB,
    'V530Rev1BlobHash': V530R1_BLOB,
    'V530Rev3BlobHash': V530R3_BLOB,
    'V530R7BlobHash': V530R7_BLOB,
    'V530R8BlobHash': V530R8_BLOB,
    'V530R9BlobHash': V530R9_BLOB,
    'V530R11BlobHash': V530R11_BLOB,
    'V530R12BlobHash': V530R12_BLOB,
    'V530R12F2BlobHash': V530R12F2_BLOB,
    'V530R12F3BlobHash': V530R12F3_BLOB,
    'V530R12F4BlobHash': V530R12F4_BLOB,
}
rep("$KnownCompilerHash = '9f448e0c8b9ea1ea803d0ae543f2fcd6e905088b8b71f54da2bff22c6cf318fc'\n",
    "$KnownCompilerHash = '9f448e0c8b9ea1ea803d0ae543f2fcd6e905088b8b71f54da2bff22c6cf318fc'\n" +
    ''.join("$%s = '%s'\n" % kv for kv in STALE.items()) +
    '$Ops = @(\n' + ',\n'.join(ops_ps) + '\n)\n')
rep("$ExpectedResultZip = 'URT_V51_WCC_RESULTS.zip'", "$ExpectedResultZip = '%s'" % RESULTS)
# ---- VerifySourceAndMenu: replaced wholesale (exact ops, counts, menu) ----
a = ps.index('function VerifySourceAndMenu {'); b = ps.index('\n}\n', a) + 3
VERIFY = r'''function VerifySourceAndMenu {
  AssertEqualHash $Candidate $CandidateHash
  AssertEqualHash $Baseline $BaselineHash
  AssertEqualHash $Xml $XmlHash
  $a = [IO.File]::ReadAllText($Baseline)
  $b = [IO.File]::ReadAllText($Candidate)
  # Exact V5.2.0 release patch of the V5.2.0 FINAL source: hash-pinned replacements, each OLD exactly once, in order.
  $t = $a
  foreach ($op in $Ops) {
    $oldPath = Join-Path $Root $op[0]; $newPath = Join-Path $Root $op[2]
    AssertEqualHash $oldPath $op[1]; AssertEqualHash $newPath $op[3]
    $old = [IO.File]::ReadAllText($oldPath); $new = [IO.File]::ReadAllText($newPath)
    if (([regex]::Matches($t,[regex]::Escape($old))).Count -ne 1) { throw "Replacement anchor missing or duplicated: $($op[0])" }
    $t = $t.Replace($old, $new)
  }
  if ($b -cne $t) { throw 'Candidate differs from the EXACT approved V5.2.0 release patch of the V5.2.0 final source.' }
  if (([regex]::Matches($b,'GetGroupName\(')).Count -ne 1) { throw 'GetGroupName call count changed.' }
  if (([regex]::Matches($b,'theGame\.SaveUserSettings\(\);')).Count -ne 3) { throw 'Unexpected change to save-call count.' }
  if (([regex]::Matches($b,"mInGameConfigWrapper\.SetVarValue\('RefinedTracingCR01'")).Count -ne __NSET__ -or ([regex]::Matches($b,"mInGameConfigWrapper\.SetVarValue\('URTExtended'")).Count -ne __NURTX__) { throw 'Unexpected configuration setter count (Quick Presets must equal V5.2.0 final + the 17 approved rows).' }
  if (([regex]::Matches($b,'values\.PushBackFlashObject\(update\);')).Count -ne __NUI__) { throw 'Unexpected Quick Preset UI row count (must equal V5.2.0 final + 17).' }
  $controls = [regex]::Matches($b,"this\.onHover(?:Dynamic)?\('([^']+)'\s*,")
  if ($controls.Count -ne __NH__) { throw "Expected __NH__ described controls, found $($controls.Count)" }
  $xmlText = [IO.File]::ReadAllText($Xml)
  $held = @(__HELD__)
  foreach ($c in $controls) { $cid = $c.Groups[1].Value; if ($held -contains $cid) { continue }; if (-not $xmlText.Contains('id="' + $cid + '"')) { throw "Control not in menu XML: $cid" } }
  foreach ($gone in @('RTGIConsoleGrid','ShadingRaysPerProbe','ShadingBudget','SUBTLE_SEPARATOR','PTRTAOEnabled','URTX_TextureBudget','URTX_TextureSharpness','URTX_PTTextureDetail','URTX_PTCaustics','URTX_TreeDistance')) { if ($xmlText.Contains($gone)) { throw "Removed item still in menu XML: $gone" } }
  foreach ($n in @(__V53__)) { if (-not $xmlText.Contains('id="' + $n + '"')) { throw "Expanded row not in menu XML: $n" } }
  return $controls
}
'''
VERIFY = (VERIFY.replace('__NSET__', str(NSET)).replace('__NURTX__', str(NURTX)).replace('__NUI__', str(NUI))
          .replace('__NH__', str(NH)).replace('__HELD__', ','.join("'%s'" % h for h in retired))
          .replace('__V53__', ','.join("'%s'" % h for h in V53IDS)))
ps = ps[:a] + VERIFY + ps[b:]
rep("if ($blobHash -eq $ProtectedBlobHash -or $blobHash -eq $Pre500BlobHash -or $blobHash -eq $OlderBlobHash) {",
    "if ($blobHash -eq $ProtectedBlobHash -or $blobHash -eq $Pre500BlobHash -or $blobHash -eq $OlderBlobHash -or " +
    ' -or '.join('$blobHash -eq $%s' % k for k in STALE) + ') {')
SYMS = ['RT_CR01_IsOwnedOption', 'GetGroupName', 'OnOptionValueChanged', 'RefinedTracingCR01', 'IndirectLight', 'RT_CR01_ApplyQuickPreset',
        'RT_CR01_RepairLiveIndirectLight', 'OnShowOptionSubmenu', 'QuickPresetSelect', 'options.force_update_values', 'options.update_disabled',
        'RTQualityClassificationSecondary', 'RTPerfClassificationSecondary', 'PTBillboards', 'CPUDistantTrees', 'CPUPhysicsLOD',
        'tooltip_cr01_rt_refl_local_lights', 'tooltip_cr01_rt_shadow_foliage_detail', 'PTSceneRange', 'tooltip_cr01_pt_scene_range',
        'RT_CR01_IsPresetURTX', 'URTExtended'] + V53IDS + ['tooltip_' + x.lower() for x in V53IDS]
a = ps.index("  foreach ($sym in @('tooltip_cr01_rtgi_console_grid'"); b = ps.index('\n', ps.index("    if (-not $blobText.Contains($sym)) { throw \"Compiled symbol missing: $sym\" }", a)) + 1
b = ps.index('\n', b) + 1   # closing brace line of the foreach
ps = ps[:a] + "  foreach ($sym in @(%s)) {\n    if (-not $blobText.Contains($sym)) { throw \"Compiled symbol missing: $sym\" }\n  }\n" % ','.join("'%s'" % s for s in SYMS) + ps[b:]
NAMES = ','.join("'%s'" % r['cvar'] for r in S.ROWS_V53)
EV_PS = r'''
  # ---- V5.3 read-only evidence (best effort; reads and copies, never modifies; never fails the build) ----
  try {
    $evDir = Join-Path $reportDir 'V53_READONLY_EVIDENCE'
    MakeDir $evDir
    $names = @(__NAMES__)
    $gameRoot = $Wcc -replace '(?i)\\The Witcher 3 REDkit\\bin\\x64_RedKit\\wcc_lite\.exe$', '\The Witcher 3'
    $lines = @("GameRoot=$gameRoot")
    if ($gameRoot -eq $Wcc -or -not (Test-Path -LiteralPath $gameRoot -PathType Container)) { $lines += 'GameRootNotFound' } else {
      foreach ($rel in @('bin\config\r4game\user_config_matrix\pc','bin\config\base','bin\config\platform\pc')) {
        $dir = Join-Path $gameRoot $rel
        if (-not (Test-Path -LiteralPath $dir -PathType Container)) { $lines += "FolderNotFound=$rel"; continue }
        foreach ($f in @(Get-ChildItem -LiteralPath $dir -File -ErrorAction Stop | Where-Object { $_.Extension -eq '.xml' -or $_.Extension -eq '.ini' })) {
          if ($f.Length -gt 4194304) { continue }
          $section = ''; $hit = @()
          foreach ($ln in ([IO.File]::ReadAllText($f.FullName) -split "`r?`n")) {
            $s = $ln.Trim()
            if ($s.StartsWith('[') -and $s.EndsWith(']')) { $section = $s }
            foreach ($n in $names) { if ($s.Contains($n)) { $hit += "$section $s"; break } }
          }
          $lines += "$rel\$($f.Name) sha256=$(FileHash $f.FullName) mentions=$($hit.Count)"
          foreach ($h in $hit) { $lines += "    $h" }
        }
      }
      $x64 = Join-Path $gameRoot 'bin\x64_dx12'
      foreach ($n in @('dsound.dll','w3loadfix.dll','w3loadfix.asi','w3loadfix.ini','w3loadfix_disabled','winmm.dll','dinput8.dll','xinput9_1_0.dll','UltimateRefinedTracing.asi','RefinedTracing.asi')) {
        $p = Join-Path $x64 $n
        if (Test-Path -LiteralPath $p -PathType Leaf) { $lines += "bin\x64_dx12\$n bytes=$((Get-Item -LiteralPath $p).Length) sha256=$(FileHash $p)" }
      }
      foreach ($c in @(@('bin\x64_dx12\w3loadfix.ini','w3loadfix.ini'),@('bin\x64_dx12\w3loadfix.log','w3loadfix.log'),@('bin\config\r4game\user_config_matrix\pc\graphics.xml','graphics.xml'),@('bin\config\base\gc.ini','gc.ini'))) {
        $p = Join-Path $gameRoot $c[0]
        if (Test-Path -LiteralPath $p -PathType Leaf) { Copy-Item -LiteralPath $p -Destination (Join-Path $evDir $c[1]) -ErrorAction Stop }
      }
    }
    $lines | Set-Content -LiteralPath (Join-Path $evDir 'EVIDENCE_INDEX.txt') -Encoding UTF8
  } catch { "Evidence step skipped: $($_.Exception.Message)" | Set-Content -LiteralPath (Join-Path $reportDir 'V53_EVIDENCE_SKIPPED.txt') -Encoding UTF8 }
'''.replace('__NAMES__', NAMES)
rep("  Copy-Item -LiteralPath $raw -Destination (Join-Path $reportDir 'precompiled.rsblob') -ErrorAction Stop\n",
    "  Copy-Item -LiteralPath $raw -Destination (Join-Path $reportDir 'precompiled.rsblob') -ErrorAction Stop\n" + EV_PS)
rep("  $lines = @('Ultimate Refined Tracing V5.1 WCC kit rev3 - READ-ONLY CHECK', ",
    "  $lines = @('Ultimate Refined Tracing V5.3.0 private WCC kit %s - READ-ONLY CHECK', " % KIT)
rep("        'Ultimate Refined Tracing V5.1 PRIVATE (Normal RT performance controls) WCC receipt, kit rev3',",
    "        'Ultimate Refined Tracing V5.3.0 PRIVATE WCC receipt, kit %s'," % KIT)
rep('"CandidateSourceSHA256=$CandidateHash", "BaselineV500Hotfix1SourceSHA256=$BaselineHash", "V51MenuXMLSHA256=$XmlHash",',
    '"CandidateSourceSHA256=$CandidateHash", "BaselineV520FinalSourceSHA256=$BaselineHash", "V520RMenuXMLSHA256=$XmlHash", "V520FinalBlobSHA256=$V520FinalBlobHash",')
_bad = [l for l in ps.splitlines() if 'V51' in l.replace('URT_V51', '').replace('V510BlobHash', '') or 'Hotfix1Source' in l]
assert not _bad, _bad
assert all(ord(c) < 128 for c in ps) and 'Shader' not in ps
(out / 'URT_V530_PRIVATE_KIT.ps1').write_bytes(ps.replace('\n', '\r\n').encode('ascii'))
for name in ('1_CHECK_ONLY.cmd', '2_COMPILE.cmd'):
    t = (ROOT / 'game/wcc/kit_v51' / name).read_text()
    t = (t.replace('URT V5.1 WCC KIT (rev3)', 'URT V5.3.0 PRIVATE WCC KIT (%s)' % KIT).replace('URT_V51_WCC.ps1', 'URT_V530_PRIVATE_KIT.ps1')
          .replace('URT_V51_WCC_RESULTS.zip', RESULTS))
    assert 'V51' not in t and 'URT_V530_PRIVATE_KIT.ps1' in t, name
    (out / name).write_bytes(t.replace('\r\n', '\n').replace('\n', '\r\n').encode('ascii'))
readme = """URT V5.3.0 PRIVATE - WCC COMPILE KIT (%s)
Nothing to install. Nothing in the game folder is changed. No SDK or developer software
is needed. Same two steps as every earlier kit.

1. Extract this ZIP to any new folder (NOT inside the game folder).
2. Double-click 1_CHECK_ONLY.cmd (read-only; wait for "SAFE TO COMPILE").
   If it says NOT SAFE, stop and send CHECK_REPORT.txt.
3. Double-click 2_COMPILE.cmd and wait for SUCCESS or FAILURE.
4. Send back %s from the new URT_RUN_... folder (also on failure).

What is compiled: your V5.2.0 final script (the one kit rev13 compiled) plus exactly %d
hash-pinned exact replacements: the approved V5.2.0 changes (descriptions, Master greying and
Quick Presets of the expanded rows) and, new in V5.3.0, the descriptions and Master greying of
seven performance rows and PT: Moon Light, plus your final Quick Preset values for
four of them (Moon Light is in no preset). Every other
Quick Preset value is exactly the approved V5.2.0 one; any other difference is refused. Same compiler, C:\\RT_BLOB workspaces, command, C:\\URT_BK backup and
verification. No Path Tracing shader change, so there is no shader step.

READ-ONLY EVIDENCE: step 2 also records (never changes) which game XML/INI files name the
nine expanded engine variables (with those lines), copies graphics.xml and gc.ini, and lists the
W3LoadFix / loader files in bin\\x64_dx12 with sizes and hashes (w3loadfix.ini and, if
present, w3loadfix.log are copied; that log has no personal data per its README).
""" % (KIT, RESULTS, len(ops))
(out / 'README_START_HERE.txt').write_bytes(readme.replace('\n', '\r\n').encode('ascii'))
z = out.parent / ('URT_V5.3.0_PRIVATE_WCC_KIT_%s.zip' % KIT)
with zipfile.ZipFile(z, 'w', zipfile.ZIP_DEFLATED) as zf:
    for p in sorted(out.iterdir()):
        info = zipfile.ZipInfo(p.name, (2026, 10, 26, 0, 0, 0)); info.compress_type = zipfile.ZIP_DEFLATED; info.external_attr = 0o644 << 16
        zf.writestr(info, p.read_bytes())
man = {'kit': KIT, 'ops': len(ops), 'baseline_sha256': sha(base_b), 'candidate_sha256': sha(cand_b), 'menu_sha256': sha(xml),
       'described_controls': NH, 'results_zip': RESULTS, 'zip': z.name, 'zip_sha256': sha(z.read_bytes())}
(out.parent / ('KIT_%s_MANIFEST.json' % KIT)).write_text(json.dumps(man, indent=2) + '\n')
print('PASS kit %s: %d ops; candidate %s; menu %s; %s %s' % (KIT, len(ops), sha(cand_b)[:12], sha(xml)[:12], z.name, sha(z.read_bytes())[:12]))
