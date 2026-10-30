# V5.2.0 in-game acceptance checklist

Before you start, back up `Documents\The Witcher 3\dx12user.settings`. Install `Ultimate_Refined_Tracing_V5.2.0_MAIN.zip` (or ASI-ONLY) over your current URT.

1. **Version.** `bin\x64_dx12\UltimateRefinedTracing.log` starts with `Ultimate Refined Tracing 5.2.0`.
2. **Separators (release blocker).** Every named heading must show `white line | title | white line` at each of these points:
   * a. when the mod menu first opens;
   * b. after scrolling to the bottom and back to the top, several times;
   * c. after leaving and re-entering the mod menu (repeat 5 times);
   * d. in the main-menu Options and in the in-game Options (both);
   * e. after selecting each Quick Preset;
   * f. with Master Off, then On;
   * g. after restarting the game.
3. **Performance preset.** Select Performance. GI Alpha-Test Texture Detail = 8 and Texture Unloading Speed = 4x. Mesh Streaming Distance, General Streaming Distance, Memory Cleanup Threshold, Memory Cleanup Interval, RT AS Defragmentation and RT AS Build Batch Size = Native / Auto. PT: No Blue Glow on Armour unchanged; all earlier preset rows show their usual values.
4. **Balanced, then Extreme.** Balanced: 6 / 2x, the six rows Native / Auto. Extreme: PT: Geometry Bounces 4 Bounces, PT: Hair Bounces 10 Bounces, GI Alpha-Test 4, Texture Unloading Speed and the six rows Native / Auto. Changing a bounce row by hand switches Quick Preset to Off.
5. **Manual edit.** With a preset active, set Memory Cleanup Interval to 600: Quick Preset switches to Off, every other value stays. Re-selecting a preset puts it back to Native / Auto.
6. **Restart rows.** Choose RT AS Build Batch Size 64 by hand, restart, load a save: the log shows `RT memory settings applied before ray tracing initialised` and `RT acceleration-structure manager is using URT's RT memory settings`. With every preset these rows stay Native (no RT memory line in the log).
7. **Skin Bounce Light Desaturation.** Its description gives the example of Geralt's skin looking too orange near firelight.
8. **Unchanged behaviour.**
   * Native graphics menu: Texture Quality and Foliage Visibility Range are unchanged after all of the above.
   * Orange Hair Fix RT+PT and Immersive World behave as with the earlier V5.2.0 candidate.
   * Frame Generation is still the last section.

If anything fails, send the log plus a screenshot. For separators, include which heading failed and after which step.
