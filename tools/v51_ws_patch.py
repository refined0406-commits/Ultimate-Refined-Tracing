#!/usr/bin/env python3
"""Single source of truth for the V5.1 WitcherScript patch (rc3) on the shipped
V5.0.0-hotfix1 script. Used to generate game/wcc/RefinedTracingCR01_RTGI3.ws, by the
regression test, and mirrored exactly by the Windows WCC kit (one OLD/NEW file pair
per operation, applied in order; every OLD must occur exactly once when applied and
every NEW exactly once when reverted, so the patch is exact and reversible).

rc3 = rc2 minus RTGIConsoleGrid, plus the user's rc3 RTGI changes:
  * three additive V5.1 controls (tooltips, Master-only tags, owned names, presets);
  * both RTGI Shading Rays Per Probe rows removed everywhere (hover, RT tag lists,
    owned names, Quick Preset blocks) -> never written by the menu again;
  * Quick Preset Performance: Secondary Classification Rays 4 -> 12 (both groups);
  * Quick Preset Balanced: Secondary 12 -> 16, Shading Frame Budget 196608 -> 262144.
"""
NAMES = ['RTReflLocalLights', 'RTShadowLocalLights', 'RTShadowFoliageDetail']
TIPS = ['tooltip_cr01_rt_refl_local_lights', 'tooltip_cr01_rt_shadow_local_lights',
        'tooltip_cr01_rt_shadow_foliage_detail']
# Quick Preset values (stored value == XML Option id): Performance "1", Balanced "2", Extreme "3".
PRESET = {'RTReflLocalLights': ('1', '1', '0'), 'RTShadowLocalLights': ('4', '2', '0'),
          'RTShadowFoliageDetail': ('3', '2', '0')}
REMOVED = ['RTQualityShadingRaysPerProbe', 'RTPerfShadingRaysPerProbe']
NL = '\n'
HOVER_ANCHOR = "    this.onHover('RTQualityGrid1ShadingLimit', \"\", \"tooltip_cr01_rt_quality_grid1_shading_limit\");" + NL
TAG_ANCHOR = "  tags.PushBack('SpecularCheckerboard');" + NL
OWN_OLD = "      || optionName == 'RTPerfGrid1ShadingLimit') {"
PRESET_ANCHOR = "  // Single refresh of the complete 39-control transaction. All OPTIONS rows" + NL


def _flash(tag, cur):
    p, b, e = cur
    return ("  // Flash OPTIONS current uses XML Option IDs, never stored Entry values." + NL +
            "  update = m_flashValueStorage.CreateTempFlashObject();" + NL +
            "  update.SetMemberFlashUInt(\"tag\", NameToFlashUInt('" + tag + "'));" + NL +
            "  if (selected == \"1\") {" + NL + "    update.SetMemberFlashString(\"current\", \"" + p + "\");" + NL +
            "  } else if (selected == \"2\") {" + NL + "    update.SetMemberFlashString(\"current\", \"" + b + "\");" + NL +
            "  } else {" + NL + "    update.SetMemberFlashString(\"current\", \"" + e + "\");" + NL +
            "  }" + NL + "  values.PushBackFlashObject(update);" + NL)


def _block(var, tag, vals, cur):
    """One existing (V4.3.1/V5.0) Quick Preset block, exactly as in hotfix1."""
    p, b, e = vals
    s = lambda v: "    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', '" + var + "', \"" + v + "\");" + NL
    return ("  // " + var + NL + "  if (selected == \"1\") {" + NL + s(p) + "  } else if (selected == \"2\") {" + NL + s(b) +
            "  } else {" + NL + s(e) + "  }" + NL + _flash(tag, cur))


def ops():
    """Ordered (old, new) exact replacements turning hotfix1 into V5.1-rc3."""
    o = []
    # --- additive V5.1 controls (3) ---
    o.append((HOVER_ANCHOR, HOVER_ANCHOR + '    // V5.1 Normal RT performance controls' + NL + ''.join(
        "    this.onHover('" + n + "', \"\", \"" + t + "\");" + NL for n, t in zip(NAMES, TIPS))))
    o.append((TAG_ANCHOR, TAG_ANCHOR + ''.join("  tags.PushBack('" + n + "');" + NL for n in NAMES)))
    o.append((OWN_OLD, "      || optionName == 'RTPerfGrid1ShadingLimit'" + NL + ''.join(
        "      || optionName == '" + n + "'" + NL for n in NAMES[:-1]) + "      || optionName == '" + NAMES[-1] + "') {"))
    pre = ''
    for n in NAMES:
        p, b, e = PRESET[n]
        pre += ("  // " + n + " (V5.1)" + NL +
                "  if (selected == \"1\") {" + NL +
                "    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', '" + n + "', \"" + p + "\");" + NL +
                "  } else if (selected == \"2\") {" + NL +
                "    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', '" + n + "', \"" + b + "\");" + NL +
                "  } else {" + NL +
                "    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', '" + n + "', \"" + e + "\");" + NL +
                "  }" + NL +
                "  update = m_flashValueStorage.CreateTempFlashObject();" + NL +
                "  update.SetMemberFlashUInt(\"tag\", NameToFlashUInt('" + n + "'));" + NL +
                "  update.SetMemberFlashString(\"current\", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', '" + n + "'));" + NL +
                "  values.PushBackFlashObject(update);" + NL)
    o.append((PRESET_ANCHOR, pre + PRESET_ANCHOR))
    # --- rc3: remove both Shading Rays Per Probe rows from every script path ---
    for g, lg in (('Perf', 'perf'), ('Quality', 'quality')):
        budget_hover = "    this.onHover('RT" + g + "ShadingBudget', \"\", \"tooltip_cr01_rt_" + lg + "_budget\");" + NL
        o.append((budget_hover + "    this.onHover('RT" + g + "ShadingRaysPerProbe', \"\", \"tooltip_cr01_rt_" + lg + "_shading_rays\");" + NL,
                  budget_hover))
        t = "  rt" + g + "Tags.PushBack('RT" + g + "ShadingBudget');" + NL
        o.append((t + "  rt" + g + "Tags.PushBack('RT" + g + "ShadingRaysPerProbe');" + NL, t))
        own = "      || optionName == 'RT" + g + "ShadingBudget'" + NL
        o.append((own + "      || optionName == 'RT" + g + "ShadingRaysPerProbe'" + NL, own))
        nxt = "  // RT" + g + "GlobalShadingLimit" + NL
        o.append((_block('RT' + g + 'ShadingRaysPerProbe', 'RT' + g + 'ShadingRaysPerProbe', ('16', '24', '32'), ('1', '2', '3')) + nxt, nxt))
        # --- rc3 Quick Preset value changes (stored value + XML Option id) ---
        sec, vsec = 'RT' + g + 'ClassificationSecondary', 'Virtual_RT' + g + 'ClassificationSecondary'
        o.append((_block(sec, vsec, ('4', '12', '32'), ('1', '4', '7')), _block(sec, vsec, ('12', '16', '32'), ('4', '5', '7'))))
        bud = 'RT' + g + 'ShadingBudget'
        o.append((_block(bud, bud, ('65536', '196608', '524288'), ('1', '3', '6')),
                  _block(bud, bud, ('65536', '262144', '524288'), ('1', '4', '6'))))
    return o


def apply(base):
    s = base
    for i, (old, new) in enumerate(ops()):
        assert s.count(old) == 1, ('apply', i, old[:80])
        s = s.replace(old, new)
    return s


def revert(v51):
    s = v51
    for i, (old, new) in reversed(list(enumerate(ops()))):
        assert s.count(new) == 1, ('revert', i, new[:80])
        s = s.replace(new, old)
    return s


if __name__ == '__main__':
    import sys
    base = open(sys.argv[1], 'rb').read(); assert base.startswith(b'\xef\xbb\xbf') and b'\r\n' not in base
    out = apply(base.decode('utf-8-sig'))
    assert revert(out) == base.decode('utf-8-sig')
    open(sys.argv[2], 'wb').write(b'\xef\xbb\xbf' + out.encode('utf-8'))
    print('PASS V5.1 rc3 script = hotfix1 + %d exact reversible operations' % len(ops()))
