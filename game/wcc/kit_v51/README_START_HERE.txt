URT V5.1.0-rc1 - WCC COMPILE KIT (revision 3)
Nothing to install. Nothing in the game folder is touched. Same steps as rev2.

1. Extract this ZIP to any new folder.
2. Double-click 1_CHECK_ONLY.cmd  (read-only; wait for "SAFE TO COMPILE").
   If it says NOT SAFE, stop and send CHECK_REPORT.txt.
3. Double-click 2_COMPILE.cmd and wait for SUCCESS or FAILURE.
4. Send back URT_V51_WCC_RESULTS.zip from the new URT_RUN_... folder (also on failure).

What is compiled
- The shipped V5.0.0-hotfix1 script plus exactly three additive V5.1 hunks
  (see V51_SOURCE_DIFF.patch): tooltips for the 7 new Normal RT rows, Master-Off
  greying for them, and their names in the option-ownership list so a change is
  saved and applied immediately. The kit refuses any other difference.
- Same compiler, same C:\RT_BLOB workspaces, same command, backup to C:\URT_BK,
  workspaces verified identical afterwards and the backup removed.
