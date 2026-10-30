@echo off
setlocal EnableExtensions DisableDelayedExpansion
cd /d "%~dp0"
echo.
echo ==============================================================
echo  URT V5.1 WCC KIT (rev3) - STEP 2: COMPILE
echo ==============================================================
echo.
set "PS_EXE=%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe"
if not exist "%PS_EXE%" (
  echo [FAILURE] Windows PowerShell 5.1 not available. Nothing was changed.
  goto :end
)
"%PS_EXE%" -NoLogo -NoProfile -NonInteractive -ExecutionPolicy Bypass -File "%~dp0URT_V51_WCC.ps1" -Mode Compile
if errorlevel 1 (
  echo.
  echo FAILED. Send URT_V51_WCC_RESULTS.zip from the new URT_RUN_* folder if it exists. If it says RESTORATION FAILED, keep C:\URT_BK and do not rerun.
) else (
  echo.
  echo Send URT_V51_WCC_RESULTS.zip from the new URT_RUN_* folder.
)
:end
echo.
echo Press any key to close this window.
pause >nul
endlocal
