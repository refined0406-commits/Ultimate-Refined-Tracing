URT V5.0.0 HOTFIX - WCC COMPILE KIT (revision 2)
Nothing to install. Nothing in the game folder is touched.

1. Extract this ZIP to any new folder (any drive or path is fine now).
2. Double-click 1_CHECK_ONLY.cmd
   - Read-only: it changes nothing outside this folder.
   - Wait for "SAFE TO COMPILE". If it says NOT SAFE, stop and send CHECK_REPORT.txt.
3. Double-click 2_COMPILE.cmd and wait for SUCCESS or FAILURE.
4. Send back URT_V500_HOTFIX_WCC_RESULTS.zip from the new URT_RUN_... folder (also on failure).

What changed since the failed run
- The earlier kit put its workspace backup inside the kit folder. From
  "D:\Chrome Downloads\New folder\..." one backup file path became 262 characters,
  over the Windows limit of 259, so the copy failed. Nothing had been staged or compiled.
- The backup now goes to the short folder C:\URT_BK, every backup path is length-checked
  before anything is written, and the backup is deleted automatically only after both
  workspaces are verified identical to their state before the run.
- If C:\URT_BK already exists, the kit refuses to run (it would mean an unfinished restore).

You can delete the old URT_V500_HOTFIX_RUN_... folder from the failed run; it contains only
copies, and no restoration is pending from it.
