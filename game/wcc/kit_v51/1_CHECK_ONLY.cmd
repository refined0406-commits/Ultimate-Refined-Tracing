@echo off
setlocal EnableExtensions DisableDelayedExpansion
cd /d "%~dp0"
echo.
echo ==============================================================
echo  URT V5.1 WCC KIT (rev3) - STEP 1: READ-ONLY CHECK
echo ==============================================================
echo.
set "PS_EXE=%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe"
if not exist "%PS_EXE%" (
  echo [FAILURE] Windows PowerShell 5.1 not available. Nothing was changed.
  goto :end
)
"%PS_EXE%" -NoLogo -NoProfile -NonInteractive -ExecutionPolicy Bypass -File "%~dp0URT_V51_WCC.ps1" -Mode Check
if errorlevel 1 (
  echo.
  echo Step 1 did NOT pass. Do not run 2_COMPILE.cmd. Send CHECK_REPORT.txt.
) else (
  echo.
  echo Step 1 passed. You may now run 2_COMPILE.cmd.
)
:end
echo.
echo Press any key to close this window.
pause >nul
endlocal
