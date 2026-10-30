# Ultimate Refined Tracing V5.0.0 PRIVATE HOTFIX (difficulty-menu option routing), kit revision 2.
# Entry points: 1_CHECK_ONLY.cmd (-Mode Check, read-only) and 2_COMPILE.cmd (-Mode Compile).
# Windows PowerShell 5.1. Same compiler, workspaces and wcc command line as the V4.3.0 build.
#
# Revision 2 change (root cause of run 20261021_190256): the full workspace backup was written
# inside the kit folder. Under "D:\Chrome Downloads\New folder\..." the longest mirrored file
# path reached 262 characters, above the Windows 259-character file limit, so the copy failed
# (the folder, 224 characters, was still under the 248 limit). Nothing had been staged.
# Now: backup + compiler output live under the short fixed folder C:\URT_BK, every backup path
# is length-checked BEFORE anything is written, and a read-only Check mode exists.
param([Parameter(Mandatory=$true)][ValidateSet('Check','Compile')][string]$Mode)
$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest

$Root = $PSScriptRoot
$Wcc = 'G:\SteamLibrary\steamapps\common\The Witcher 3 REDkit\bin\x64_RedKit\wcc_lite.exe'
$Base = 'C:\RT_BLOB\base'
$Patch = 'C:\RT_BLOB\patch'
$Stage = 'C:\RT_BLOB\patch\local\RefinedTracingCR01.ws'
$BkRoot = 'C:\URT_BK'
$BkBase = 'C:\URT_BK\B'
$BkPatch = 'C:\URT_BK\P'
$BkOut = 'C:\URT_BK\OUT'
$BkMarker = 'C:\URT_BK\RESTORE_REQUIRED.txt'
$Candidate = Join-Path $Root 'RefinedTracingCR01.ws'
$Baseline = Join-Path $Root 'BASELINE_V500_DO_NOT_STAGE.ws'
$Xml = Join-Path $Root 'REFINEDTRACING_V500_MENU_REFERENCE.xml'
$CandidateHash = '90509bc1b8d4f49db1badb31fb3b82db2edaf2c4c84a0d8fdd9705d93e1707f1'
$BaselineHash = '07f8b823ee944916ca8074f487b75a063abfa8f88f7e6df0f69ce079d9907a7a'
$XmlHash = '2aa6bd1a32a0215ee08f3658f7cf72d288c08d26b289a5ebab6da3fb3c8af8ff'
$ProtectedBlobHash = '3332ffc9dbe51676d41599177528344b97b77f54110b48869670104f47fcb97a'
$OlderBlobHash = 'fca12ffd1de39bc636d19a4a47d44bd3eda98f0e230fa74c46cf9f323c780693'
$KnownCompilerHash = '9f448e0c8b9ea1ea803d0ae543f2fcd6e905088b8b71f54da2bff22c6cf318fc'
# c32de181... is the staged file your previous runs left (and restored) in the patch workspace.
$KnownStageHashes = @(
 'c32de181ec45b488f4986834f5d68206bdc700147e1f4e5f10705733ec4e1fad',
 '9d1631f6eab8da959f8ef316e16be99cf4f55e033a8990dc16e6c35d30f325ed',
 'aea2dae9fc4abd88413024b4b5425aa8c56ffc4792eeb6f0d89bd44b171049f3',
 '07f8b823ee944916ca8074f487b75a063abfa8f88f7e6df0f69ce079d9907a7a'
)
$MaxFile = 259   # Windows MAX_PATH file-name limit without long-path support
$MaxDir = 247    # Windows directory limit (248 minus terminator)
$ExpectedResultZip = 'URT_V500_HOTFIX_WCC_RESULTS.zip'

$run = $null; $reportDir = $null
$success = $false; $restoreOK = $false; $stageAttempted = $false; $backupStarted = $false
$snaps = @(); $wccExit = -1; $blobHash = '(none)'; $blobBytes = 0
$stageOldHash = '(not captured)'; $compilerHash = '(not captured)'; $errorDetail = '(none)'
$changed = 0; $vanillaCopied = '(not attempted)'; $pre = $null

function FileHash([string]$Path) { return (Get-FileHash -LiteralPath $Path -Algorithm SHA256 -ErrorAction Stop).Hash.ToLowerInvariant() }
function RequireFile([string]$Path) { if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) { throw "Required file missing: $Path" } }
function RequireDir([string]$Path) { if (-not (Test-Path -LiteralPath $Path -PathType Container)) { throw "Required directory missing: $Path" } }
function AssertEqualHash([string]$Path,[string]$Expected) { RequireFile $Path; if ((FileHash $Path) -ne $Expected) { throw "SHA-256 mismatch: $Path" } }
function MakeDir([string]$Path) { New-Item -Path $Path -ItemType Directory -ErrorAction Stop | Out-Null }
function HasReparse([string]$Path) { return (([int](Get-Item -LiteralPath $Path -Force).Attributes -band [int][IO.FileAttributes]::ReparsePoint) -ne 0) }

function VerifySourceAndMenu {
  AssertEqualHash $Candidate $CandidateHash
  AssertEqualHash $Baseline $BaselineHash
  AssertEqualHash $Xml $XmlHash
  $a = [IO.File]::ReadAllText($Baseline)
  $b = [IO.File]::ReadAllText($Candidate)
  $nl = "`n"
  $oldGate = '  groupName = mInGameConfigWrapper.GetGroupName(groupId);' + $nl
  $newGate = "  groupName = '';" + $nl + '  if (this.RT_CR01_IsOwnedOption(optionName)) {' + $nl +
    "    // Only URT's own rows (real config group ids) are resolved; the group" + $nl +
    '    // check also guards against a same-named variable in another group.' + $nl +
    '    groupName = mInGameConfigWrapper.GetGroupName(groupId);' + $nl + '  }' + $nl
  $anchor = '@wrapMethod(CR4IngameMenu)' + $nl + 'function OnOptionValueChanged('
  foreach ($needle in @($oldGate,$anchor)) {
    if (([regex]::Matches($a,[regex]::Escape($needle))).Count -ne 1) { throw 'Protected V5.0.0 anchor differs or duplicated.' }
  }
  $hs = $b.IndexOf('// V5.0.0 hotfix: URT reacts only to its own options.', [StringComparison]::Ordinal)
  $he = $b.IndexOf($anchor, [StringComparison]::Ordinal)
  if ($hs -lt 0 -or $he -le $hs) { throw 'Hotfix helper block not found.' }
  $helper = $b.Substring($hs, $he - $hs)
  $expected = $a.Replace($anchor, $helper + $anchor).Replace($oldGate, $newGate)
  if ($b -cne $expected) { throw 'Candidate differs from the EXACT approved 2-hunk hotfix of V5.0.0.' }
  $owned = [regex]::Matches($helper,"optionName == '([^']+)'")
  if ($owned.Count -ne 41) { throw "Expected 41 owned names, found $($owned.Count)" }
  if (([regex]::Matches($b,'GetGroupName\(')).Count -ne 1) { throw 'GetGroupName call count changed.' }
  if (([regex]::Matches($b,'theGame\.SaveUserSettings\(\);')).Count -ne 2) { throw 'Unexpected change to save-call count.' }
  if (([regex]::Matches($b,"mInGameConfigWrapper\.SetVarValue\('RefinedTracingCR01'")).Count -ne 120) { throw 'Unexpected change to configuration setters.' }
  if (([regex]::Matches($b,'values\.PushBackFlashObject\(update\);')).Count -ne 39) { throw 'Unexpected change to Quick Preset UI assignments.' }
  $controls = [regex]::Matches($b,"this\.onHover(?:Dynamic)?\('([^']+)'\s*,")
  if ($controls.Count -ne 41) { throw "Expected 41 menu controls, found $($controls.Count)" }
  $ownedSet = @{}
  foreach ($o in $owned) { $ownedSet[$o.Groups[1].Value] = $true }
  foreach ($c in $controls) { if (-not $ownedSet.ContainsKey($c.Groups[1].Value)) { throw "Menu control not owned: $($c.Groups[1].Value)" } }
  $xmlText = [IO.File]::ReadAllText($Xml)
  foreach ($k in $ownedSet.Keys) { if (-not $xmlText.Contains('<Var id="' + $k + '"')) { throw "Owned name not in menu XML: $k" } }
  return $controls
}

# Read-only scan of one workspace: refuses links, measures size, hashes every file and checks
# that every mirrored backup path fits within the Windows limits. Writes nothing.
function ScanWorkspace([string]$Path,[string]$BackupDir,[string]$Tag) {
  RequireDir $Path
  if (HasReparse $Path) { throw "Workspace root is a reparse point: $Path" }
  $objects = @(Get-ChildItem -LiteralPath $Path -Force -Recurse -ErrorAction Stop)
  if ($objects.Count -gt 30000) { throw "Unexpectedly large workspace (more than 30000 entries): $Tag" }
  $files = @(); $bytes = [double]0; $longest = 0; $longestPath = ''
  $entries = @{}
  foreach ($obj in $objects) {
    if (($obj.Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0) { throw "Workspace contains link/reparse point; refusing: $($obj.FullName)" }
    $rel = $obj.FullName.Substring($Path.Length).TrimStart('\')
    $dst = Join-Path $BackupDir $rel
    if ($obj.PSIsContainer) {
      if ($dst.Length -gt $MaxDir) { throw "Backup folder path would be too long ($($dst.Length) > $MaxDir): $dst" }
    } else {
      if ($dst.Length -gt $MaxFile) { throw "Backup file path would be too long ($($dst.Length) > $MaxFile): $dst" }
      $parentLen = (Split-Path -Parent $dst).Length
      if ($parentLen -gt $MaxDir) { throw "Backup folder path would be too long ($parentLen > $MaxDir): $dst" }
      if ($dst.Length -gt $longest) { $longest = $dst.Length; $longestPath = $dst }
      $bytes += $obj.Length
      $entries[$rel] = [pscustomobject]@{ Relative = $rel; SHA256 = (FileHash $obj.FullName); Original = $obj.FullName; Backup = $dst }
    }
  }
  if ($bytes -gt 2147483648) { throw "Workspace $Tag exceeds 2 GiB protected backup limit." }
  return [pscustomobject]@{ Root = $Path; Tag = $Tag; BackupDir = $BackupDir; Entries = $entries; FileCount = $entries.Count; Bytes = $bytes; Longest = $longest; LongestPath = $longestPath }
}

function ManifestLines($Scan) {
  $lines = @()
  foreach ($k in ($Scan.Entries.Keys | Sort-Object)) { $lines += ($Scan.Entries[$k].SHA256 + '  ' + $k) }
  return $lines
}

# Every check that must pass before anything is written outside the kit folder.
function Preflight {
  if ($Root -match '(?i)\\steamapps\\common\\The Witcher 3\\' -or $Root -match '(?i)\\bin\\x64_dx12\\') { throw 'Extract the kit outside the installed Witcher 3 directory.' }
  foreach ($forbidden in @($Base,$Patch,$BkRoot,(Split-Path -Parent $Wcc))) {
    if ($Root.StartsWith($forbidden.TrimEnd('\')+'\',[StringComparison]::OrdinalIgnoreCase) -or $Root.Equals($forbidden,[StringComparison]::OrdinalIgnoreCase)) {
      throw 'Kit must be OUTSIDE the REDkit compiler folder, the WCC workspaces and C:\URT_BK.'
    }
  }
  if (Test-Path -LiteralPath $BkRoot) {
    throw "$BkRoot already exists. It may hold an unrestored workspace backup from an earlier run. Nothing changed; send the folder listing instead of deleting it."
  }
  foreach ($old in @(Get-ChildItem -LiteralPath $Root -Directory -ErrorAction Stop | Where-Object { $_.Name -like 'URT_V500_HOTFIX_RUN_*' -or $_.Name -like 'URT_RUN_*' })) {
    if (Test-Path -LiteralPath (Join-Path $old.FullName 'WORKSPACE_RESTORE_REQUIRED.txt')) { throw "Earlier run needs workspace restoration: $($old.FullName). Refusing." }
  }
  $controls = VerifySourceAndMenu
  RequireFile $Wcc
  if (HasReparse $Wcc) { throw 'REDkit WCC executable is a link/reparse point; refusing.' }
  if (@(Get-Process -Name 'wcc_lite' -ErrorAction SilentlyContinue).Count -gt 0) { throw 'A WCC compiler process is already running.' }
  RequireDir $Base; RequireDir $Patch; RequireDir (Split-Path -Parent $Stage); RequireFile $Stage
  $sh = FileHash $Stage
  if ($sh -notin $KnownStageHashes) { throw "Staged WCC source $sh is not a known historical file. Workspace unmatched; nothing changed." }
  $extra = @(Get-ChildItem -LiteralPath $Patch -Recurse -File -Filter '*.ws' -ErrorAction Stop | Where-Object { $_.FullName -ine $Stage })
  if ($extra.Count -gt 0) { throw "Unexpected extra WCC patch scripts: $($extra.Count). Workspace unmatched." }
  $sb = ScanWorkspace $Base $BkBase 'base'
  $sp = ScanWorkspace $Patch $BkPatch 'patch'
  $drive = New-Object IO.DriveInfo 'C'
  $need = [double]($sb.Bytes + $sp.Bytes) + 268435456
  if ($drive.AvailableFreeSpace -lt $need) { throw "Not enough free space on C: for the backup (need $([math]::Ceiling($need/1MB)) MB)." }
  return [pscustomobject]@{ Controls = $controls; StageHash = $sh; Base = $sb; Patch = $sp; CompilerHash = (FileHash $Wcc); FreeMB = [math]::Floor($drive.AvailableFreeSpace/1MB) }
}

function BackupWorkspace($Scan) {
  foreach ($key in ($Scan.Entries.Keys | Sort-Object)) {
    $e = $Scan.Entries[$key]
    $parent = Split-Path -Parent $e.Backup
    if (-not (Test-Path -LiteralPath $parent -PathType Container)) { New-Item -ItemType Directory -Path $parent -Force -ErrorAction Stop | Out-Null }
    Copy-Item -LiteralPath $e.Original -Destination $e.Backup -ErrorAction Stop
    if ((FileHash $e.Backup) -ne $e.SHA256) { throw "Workspace backup failed verification (or file changed during backup): $($e.Original)" }
  }
}

function RestoreWorkspace($Scan) {
  $count = 0
  foreach ($item in @(Get-ChildItem -LiteralPath $Scan.Root -Force -Recurse -File -ErrorAction Stop)) {
    $rel = $item.FullName.Substring($Scan.Root.Length).TrimStart('\')
    if (-not $Scan.Entries.ContainsKey($rel)) { Remove-Item -LiteralPath $item.FullName -Force -ErrorAction Stop; $count++ }
  }
  foreach ($key in $Scan.Entries.Keys) {
    $e = $Scan.Entries[$key]
    if (-not (Test-Path -LiteralPath $e.Original -PathType Leaf) -or (FileHash $e.Original) -ne $e.SHA256) {
      $parent = Split-Path -Parent $e.Original
      if (-not (Test-Path -LiteralPath $parent -PathType Container)) { New-Item -Path $parent -ItemType Directory -Force -ErrorAction Stop | Out-Null }
      Copy-Item -LiteralPath $e.Backup -Destination $e.Original -Force -ErrorAction Stop
      $count++
    }
    if ((FileHash $e.Original) -ne $e.SHA256) { throw "CRITICAL: post-restore mismatch: $($e.Original)" }
  }
  if (@(Get-ChildItem -LiteralPath $Scan.Root -Force -Recurse -File -ErrorAction Stop).Count -ne $Scan.FileCount) { throw "CRITICAL: workspace file count differs after restore: $($Scan.Tag)" }
  return $count
}

# ------------------------------------------------------------------ CHECK (read-only)
if ($Mode -eq 'Check') {
  $report = Join-Path $Root 'CHECK_REPORT.txt'
  $lines = @('Ultimate Refined Tracing V5.0.0 hotfix WCC kit rev2 - READ-ONLY CHECK', "UTC=$((Get-Date).ToUniversalTime().ToString('o'))", "KitFolder=$Root")
  $ok = $false
  try {
    Write-Host 'Read-only check: nothing outside this kit folder is changed...' -ForegroundColor Cyan
    $p = Preflight
    $lines += @("StagedSourceSHA256=$($p.StageHash)", "WindowsWCCSHA256=$($p.CompilerHash)", "CompilerMatchesV430Build=$($p.CompilerHash -eq $KnownCompilerHash)",
      "BaseFiles=$($p.Base.FileCount)", "BaseMB=$([math]::Round($p.Base.Bytes/1MB,1))", "PatchFiles=$($p.Patch.FileCount)",
      "LongestBackupPath=$($p.Base.Longest) chars (limit $MaxFile): $($p.Base.LongestPath)", "LongestPatchBackupPath=$($p.Patch.Longest) chars",
      "FreeSpaceC_MB=$($p.FreeMB)", "BackupFolderFree=$(-not (Test-Path -LiteralPath $BkRoot))")
    $lines += '--- patch workspace manifest ---'
    $lines += (ManifestLines $p.Patch)
    $ok = $true
  } catch {
    $lines += "Error=$($_.Exception.Message)"
  }
  $lines += "Result=$(if ($ok) { 'SAFE TO COMPILE' } else { 'NOT SAFE - DO NOT RUN 2_COMPILE' })"
  $lines | Set-Content -LiteralPath $report -Encoding UTF8
  if ($ok) {
    Write-Host 'RESULT: SAFE TO COMPILE. Both workspaces match the expected state; no long paths; nothing was changed.' -ForegroundColor Green
    exit 0
  }
  Write-Host "RESULT: NOT SAFE. $($lines[-2])" -ForegroundColor Red
  Write-Host 'Nothing was changed. Send CHECK_REPORT.txt.' -ForegroundColor Yellow
  exit 1
}

# ------------------------------------------------------------------ COMPILE
try {
  $stamp = (Get-Date).ToUniversalTime().ToString('yyyyMMdd_HHmmss')
  $run = Join-Path $Root ("URT_RUN_$stamp")
  MakeDir $run
  $reportDir = Join-Path $run 'RESULT_PAYLOAD'
  MakeDir $reportDir
  Write-Host 'Checking source, compiler, workspaces and backup path lengths (read-only)...' -ForegroundColor Cyan
  $pre = Preflight
  $stageOldHash = $pre.StageHash
  $compilerHash = $pre.CompilerHash
  (ManifestLines $pre.Base) | Set-Content -LiteralPath (Join-Path $reportDir 'BASE_MANIFEST_BEFORE.txt') -Encoding UTF8
  (ManifestLines $pre.Patch) | Set-Content -LiteralPath (Join-Path $reportDir 'PATCH_MANIFEST_BEFORE.txt') -Encoding UTF8
  Copy-Item -LiteralPath $Stage -Destination (Join-Path $reportDir 'ORIGINAL_STAGED_SOURCE_BACKUP.ws') -ErrorAction Stop
  AssertEqualHash (Join-Path $reportDir 'ORIGINAL_STAGED_SOURCE_BACKUP.ws') $stageOldHash

  Write-Host 'Backing up both WCC workspaces to C:\URT_BK (hash-verified)...' -ForegroundColor Cyan
  MakeDir $BkRoot
  $backupStarted = $true
  ("Workspace backup of $Base and $Patch. Kit run: $run. If this file exists after the kit finished, restoration did NOT complete: do not delete C:\URT_BK and do not rerun.") |
    Set-Content -LiteralPath $BkMarker -Encoding UTF8
  'Restoration pending; see C:\URT_BK.' | Set-Content -LiteralPath (Join-Path $run 'WORKSPACE_RESTORE_REQUIRED.txt') -Encoding UTF8
  MakeDir $BkBase; MakeDir $BkPatch; MakeDir $BkOut
  BackupWorkspace $pre.Base
  BackupWorkspace $pre.Patch
  $snaps = @($pre.Base, $pre.Patch)
  if ((FileHash $Stage) -ne $stageOldHash) { throw 'Workspace changed during backup; refusing to stage.' }
  AssertEqualHash $Candidate $CandidateHash

  $stageAttempted = $true
  Copy-Item -LiteralPath $Candidate -Destination $Stage -Force -ErrorAction Stop
  AssertEqualHash $Stage $CandidateHash
  $raw = Join-Path $BkOut 'blob.rsblob'
  $log = Join-Path $reportDir 'WCC_NATIVE_OUTPUT.txt'
  Write-Host 'Running the V4.3.0 REDkit WCC command (unchanged)...' -ForegroundColor Cyan
  Push-Location (Split-Path -Parent $Wcc)
  try {
    $savedEap = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    try {
      & $Wcc compilescripts $Base "-patch=$Patch" "-out=$BkOut" 2>&1 | Out-File -LiteralPath $log -Encoding UTF8 -ErrorAction Stop
      $wccExit = $LASTEXITCODE
    } finally { $ErrorActionPreference = $savedEap }
  } finally { Pop-Location }
  if ($wccExit -ne 0) { throw "WCC returned nonzero exit code: $wccExit" }
  RequireFile $raw
  $blobBytes = (Get-Item -LiteralPath $raw).Length
  if ($blobBytes -le 6000 -or $blobBytes -ge 5000000) { throw "Unexpected WCC blob size: $blobBytes" }
  $blobHash = FileHash $raw
  if ($blobHash -eq $ProtectedBlobHash -or $blobHash -eq $OlderBlobHash) { throw 'Compiler produced the unchanged V5.0.0 blob: stale or ignored source.' }
  $blobText = [Text.Encoding]::GetEncoding(28591).GetString([IO.File]::ReadAllBytes($raw))
  foreach ($h in $pre.Controls) { if (-not $blobText.Contains($h.Groups[1].Value)) { throw "Compiled menu control missing: $($h.Groups[1].Value)" } }
  foreach ($sym in @('RT_CR01_IsOwnedOption','GetGroupName','OnOptionValueChanged','RefinedTracingCR01',
    'IndirectLight','RT_CR01_ApplyQuickPreset','RT_CR01_RepairLiveIndirectLight','OnShowOptionSubmenu','QuickPresetSelect',
    'options.force_update_values','options.update_disabled','RTQualityClassificationSecondary','RTPerfClassificationSecondary',
    'PTBillboards','CPUDistantTrees','CPUPhysicsLOD')) {
    if (-not $blobText.Contains($sym)) { throw "Compiled symbol missing: $sym" }
  }
  Copy-Item -LiteralPath $raw -Destination (Join-Path $reportDir 'precompiled.rsblob') -ErrorAction Stop
  AssertEqualHash (Join-Path $reportDir 'precompiled.rsblob') $blobHash
  $success = $true
} catch {
  $errorDetail = $_.Exception.ToString()
  Write-Host "BUILD CHECK FAILED: $($_.Exception.Message)" -ForegroundColor Red
} finally {
  try {
    if ($backupStarted) {
      if ($stageAttempted -or $snaps.Count -eq 2) {
        foreach ($s in $snaps) { $changed += RestoreWorkspace $s }
        if ($snaps.Count -ne 2) { throw 'Workspace backup incomplete after staging; cannot restore.' }
      }
      # Independent post-check: both workspaces must again match the manifests taken before anything was written.
      foreach ($s in @($pre.Base, $pre.Patch)) {
        $now = @(Get-ChildItem -LiteralPath $s.Root -Force -Recurse -File -ErrorAction Stop)
        if ($now.Count -ne $s.FileCount) { throw "CRITICAL: $($s.Tag) file count differs from the pre-run manifest." }
        foreach ($k in $s.Entries.Keys) { if ((FileHash $s.Entries[$k].Original) -ne $s.Entries[$k].SHA256) { throw "CRITICAL: differs from pre-run manifest: $($s.Entries[$k].Original)" } }
      }
      if ((FileHash $Stage) -ne $stageOldHash) { throw 'Staged source not restored exactly.' }
      $restoreOK = $true
      # The verified backup is now redundant; remove it so C:\URT_BK never lingers.
      Remove-Item -LiteralPath $BkMarker -Force -ErrorAction Stop
      Remove-Item -LiteralPath $BkRoot -Recurse -Force -ErrorAction Stop
      Remove-Item -LiteralPath (Join-Path $run 'WORKSPACE_RESTORE_REQUIRED.txt') -Force -ErrorAction SilentlyContinue
    } else {
      $restoreOK = $true   # nothing outside the kit folder was written
    }
  } catch {
    $restoreOK = $false; $success = $false
    $errorDetail += "`nCRITICAL RESTORE FAILURE: " + $_.Exception.ToString()
    Write-Host 'CRITICAL: WORKSPACE RESTORATION FAILED. DO NOT RERUN. Keep C:\URT_BK and the run folder.' -ForegroundColor Red
  }
  if ($reportDir -and (Test-Path -LiteralPath $reportDir)) {
    try {
      if (-not (Test-Path -LiteralPath (Join-Path $reportDir 'WCC_NATIVE_OUTPUT.txt'))) {
        'WCC did not run; see BUILD_RECEIPT.txt.' | Set-Content -LiteralPath (Join-Path $reportDir 'WCC_NATIVE_OUTPUT.txt') -Encoding UTF8
      }
      if (-not $success) { Remove-Item -LiteralPath (Join-Path $reportDir 'precompiled.rsblob') -Force -ErrorAction SilentlyContinue }
      @(
        'Ultimate Refined Tracing V5.0.0 PRIVATE HOTFIX (difficulty-menu option routing) WCC receipt, kit rev2',
        'This is NOT an installable mod, not a public release, and not gameplay validated.',
        "UTC=$((Get-Date).ToUniversalTime().ToString('o'))",
        "CandidateSourceSHA256=$CandidateHash", "BaselineV500SourceSHA256=$BaselineHash", "V500MenuXMLSHA256=$XmlHash",
        "V500ProtectedBlobSHA256=$ProtectedBlobHash", "StagedSourceOriginalSHA256=$stageOldHash",
        "WindowsWCCExecutable=$Wcc", "WindowsWCCSHA256=$compilerHash", "CompilerMatchesV430Build=$($compilerHash -eq $KnownCompilerHash)",
        "BaseWorkspace=$Base", "PatchWorkspace=$Patch", "BackupFolder=$BkRoot",
        "BaseFiles=$(if ($pre) { $pre.Base.FileCount } else { '-' })", "LongestBackupPath=$(if ($pre) { $pre.Base.Longest } else { '-' })",
        "WccExitCode=$wccExit", "BlobBytes=$blobBytes", "BlobSHA256=$blobHash",
        "WorkspaceFilesRestoredOrRemoved=$changed", "WorkspaceMatchesPreRunManifest=$restoreOK",
        "CompiledStaticChecksPassed=$success", "NoGameInstallationPerformed=True", "Error=$errorDetail"
      ) | Set-Content -LiteralPath (Join-Path $reportDir 'BUILD_RECEIPT.txt') -Encoding UTF8
      Add-Type -AssemblyName System.IO.Compression.FileSystem -ErrorAction Stop
      $zip = Join-Path $run $ExpectedResultZip
      [IO.Compression.ZipFile]::CreateFromDirectory($reportDir,$zip,[IO.Compression.CompressionLevel]::Optimal,$false)
      Write-Host "RESULT ZIP: $zip" -ForegroundColor Cyan
    } catch {
      $success = $false
      Write-Host "FAILED to package results: $($_.Exception.Message)" -ForegroundColor Red
    }
  }
}
if (-not $success -or -not $restoreOK -or $wccExit -ne 0) {
  Write-Host 'FAILURE: NO BUILD APPROVED. Send the results ZIP if one was created.' -ForegroundColor Red
  exit 1
}
Write-Host 'SUCCESS: compiled, checked, both workspaces restored and verified, C:\URT_BK removed.' -ForegroundColor Green
exit 0
