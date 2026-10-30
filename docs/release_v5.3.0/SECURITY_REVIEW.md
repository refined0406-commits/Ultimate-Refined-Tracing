# Ultimate Refined Tracing V5.3.0: technical summary for security review

**What it is:**
* An ASI plugin, `UltimateRefinedTracing.asi`: a 64-bit DLL loaded into `witcher3.exe` by Ultimate ASI Loader (`winmm.dll`, unmodified 9.7.4, MIT).
* A WitcherScript menu, compiled by CD PROJEKT RED's own REDkit compiler.
* A menu definition XML file.
* 18 string tables.

The source code is public, and the build is reproducible (built twice, byte-identical, `/Brepro`). Checksums are in `SHA256SUMS.txt`, provenance in `BUILD_PROVENANCE.md`.

## Imports (the complete list, read from the PE import table)
* **KERNEL32:**
  * file I/O: `CreateFileW`, `ReadFile`, `WriteFile`, `FlushFileBuffers`, `MoveFileExW`, `ReplaceFileW`, `DeleteFileW`, `GetFileAttributes(Ex)W`, `GetFileSizeEx`;
  * change notification: `Find*ChangeNotification`;
  * memory: `VirtualAlloc`, `VirtualFree`, `VirtualProtect`, `FlushInstructionCache`;
  * modules: `GetModuleHandle(Ex)W`, `GetModuleFileNameW`, `LoadLibraryW`, `GetProcAddress`, `GetSystemDirectoryW`;
  * threads: `CreateThread`, `WaitForSingleObject`, `Sleep`, `InitOnceExecuteOnce`, `CreateMutexW`;
  * time: `GetSystemTime`, `GetTickCount64`;
  * profile reading: `GetPrivateProfileStringW`, `GetPrivateProfileSectionW`;
  * process: `GetCurrentProcess(Id)`, `GetProcessMitigationPolicy`, `DisableThreadLibraryCalls`, `GetLastError`.
* **ADVAPI32:** `CryptAcquireContextW`, `CryptCreateHash`, `CryptHashData`, `CryptGetHashParam`, `CryptDestroyHash`, `CryptReleaseContext`. These are used **only** to compute SHA-256 of the game executable and its shader, to verify them before anything is changed.
* **Exports:** `InitializeASI` (the loader entry point) and `CR01EarlyPrepare`.

## What it does NOT do
* **No network access:** no socket, WinINet, WinHTTP or URL functions are imported.
* **No registry access:** no `Reg*` functions.
* **No process creation** (`CreateProcess`, `ShellExecute`), **no services**, **no drivers**, **no persistence:** nothing is installed outside the game, and there is no autostart.
* **No privilege changes**, no token functions, no debug privileges.
* **No code injection into other processes:** it works only inside `witcher3.exe`, which loaded it.
* **No obfuscation and no packing:** the code is plain C, compiled with clang. All log text is readable.

## Files it touches (all in the user's game or Documents folder)
* **Reads** `Documents\The Witcher 3\dx12user.settings` (the game's settings, never written by URT).
* **Reads and writes** URT's own files:
  * `Documents\The Witcher 3\UltimateRefinedTracing.settings` (+ `.bak`, written atomically);
  * `UltimateRefinedTracing.startupguard`;
  * `bin\x64_dx12\UltimateRefinedTracing.log`.
* **Reads** the game folder's file names, read-only, to report other mods and loaders in its log.

## Why it changes memory inside the game (the reason heuristic scanners may react)
* **Engine settings:** URT writes graphics and performance values (for example the number of Path Tracing bounces) into the game's own settings variables. Every write is a compare-and-swap of an exact, verified address. The game's value is restored on Master Off, on renderer change and at exit.
* **Code hooks:** a small number of exact instruction sites are redirected with `VirtualProtect` and `FlushInstructionCache`. Each site is checked byte for byte before it is changed, and anything that is not as expected is refused. These are:
  * the Path Tracing settings serializer;
  * the PT shadow guards;
  * the CPU Fast Bone Reads, Distant Trees and Physics LOD paths.
* **Path Tracing shader:** at game start, URT builds its variant of the game's own PT shader library with the game's own `dxcompiler.dll`. The game's DXC validator checks and signs the result, and URT writes it into the two shader slots inside the game's memory image. Nothing is written to disk.
* **Diagnostics:** every 5 minutes URT reads the game's own GPU memory use through the system `dxgi.dll` (read-only) and its process memory. These are written to URT's log.

These techniques are typical of game mods. Antivirus heuristics can flag new DLLs that use `VirtualProtect` on code, or that are loaded by a proxy DLL such as `winmm.dll`. URT does not attempt to hide any of this.

## Supported executables
URT checks `witcher3.exe` by SHA-256:
* Patch 5.01 (build 1048522);
* the Steam update of 29 October 2026 (build 1090142).

Any other executable gets a reduced, safe feature set: no code or shader changes, only engine settings resolved by name.
