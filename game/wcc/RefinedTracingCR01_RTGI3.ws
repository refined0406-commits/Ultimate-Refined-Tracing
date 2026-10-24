// C15 FIX2 private Shared Utilities 4.1 script replacement. Compile from matched game+REDkit source; NEVER install loose.
// CR01 LIFECYCLE01 integrated event transport + native disabled-row lifecycle.
// Patch 5.01 + Shared Utilities 4.1. Compile ONLY via the proven wcc_lite patch
// workflow into precompiled.rsblob. Never install this source loosely.
//
// UI policy:
//   Master Off -> all 15 dependent controls disabled=true.
//   Master On  -> all 15 dependent controls disabled=false.
// Renderer eligibility remains native/ASI-side and never greys PT preferences.
//
// Event transport:
//   The genuine OnOptionValueChanged event writes the config first. CR01 then
//   explicitly saves user settings only for CR01 values and renderer-mode/
//   native-shadow values that the lifecycle owner needs to observe. The ASI
//   consumes the resulting exact file-change event and diffs CR01 values.

class RefinedTracingCR01_Descriptor extends SU_MenuDescriptor {
  public function build() {
    this.onHover('MasterEnabled', "", "tooltip_cr01_masterenabled");
    this.onHoverDynamic('QuickPresetSelect', "", "tooltip_cr01_quick_preset_off");
    this.onHover('CPUFastBoneReads', "", "tooltip_cr01_cpu_fast_bone_reads");
    this.onHover('CPUDistantTrees', "", "tooltip_cr01_cpu_distant_trees");
    this.onHover('CPUPhysicsLOD', "", "tooltip_cr01_cpu_physics_lod");
    this.onHover('PTShadows', "", "tooltip_cr01_ptshadows");
    this.onHover('PTBillboards', "", "tooltip_cr01_ptbillboards");
    this.onHover('GeometryBounces', "", "tooltip_cr01_geometrybounces");
    this.onHover('HairBounces', "", "tooltip_cr01_hairbounces");
    this.onHover('IndirectLight', "", "tooltip_cr01_indirectlight");
    this.onHover('SunImpact', "", "tooltip_cr01_sunimpact");
    this.onHover('SkyImpact', "", "tooltip_cr01_skyimpact");
    this.onHover('AmbientOcclusion', "", "tooltip_cr01_ambientocclusion");
    this.onHover('AOStrength', "", "tooltip_cr01_aostrength");
    this.onHover('AORadius', "", "tooltip_cr01_aoradius");
    this.onHover('AOContrast', "", "tooltip_cr01_aocontrast");
    this.onHover('PTRTAOEnabled', "", "tooltip_cr01_rtao_enabled");
    this.onHover('PTRTAORayCount', "", "tooltip_cr01_rtao_count");
    this.onHover('PTRTAORadius', "", "tooltip_cr01_rtao_radius");
    this.onHover('PTRTAOStrength', "", "tooltip_cr01_rtao_strength");
    this.onHover('GrassDay', "", "tooltip_cr01_grassday");
    this.onHover('GrassNight', "", "tooltip_cr01_grassnight");
    this.onHover('SpecularScreenSpace', "", "tooltip_cr01_specularscreenspace");
    this.onHover('SpecularCheckerboard', "", "tooltip_cr01_specularcheckerboard");
    this.onHoverDynamic('RayTracedOptimizations', "", "tooltip_cr01_rt_opt_off");
    this.onHover('RTPerfClassificationPrimary', "", "tooltip_cr01_rt_perf_primary");
    this.onHover('Virtual_RTPerfClassificationSecondary', "", "tooltip_cr01_rt_perf_secondary");
    this.onHover('RTPerfMaxProbes', "", "tooltip_cr01_rt_perf_cap");
    this.onHover('RTPerfAOQuality', "", "tooltip_cr01_rt_perf_ao");
    this.onHover('RTPerfShadingBudget', "", "tooltip_cr01_rt_perf_budget");
    this.onHover('RTPerfGlobalShadingLimit', "", "tooltip_cr01_rt_perf_global_shading_limit");
    this.onHover('RTPerfGrid1ShadingLimit', "", "tooltip_cr01_rt_perf_grid1_shading_limit");
    this.onHover('RTQualityClassificationPrimary', "", "tooltip_cr01_rt_quality_primary");
    this.onHover('Virtual_RTQualityClassificationSecondary', "", "tooltip_cr01_rt_quality_secondary");
    this.onHover('RTQualityMaxProbes', "", "tooltip_cr01_rt_quality_cap");
    this.onHover('RTQualityAOQuality', "", "tooltip_cr01_rt_quality_ao");
    this.onHover('RTQualityShadingBudget', "", "tooltip_cr01_rt_quality_budget");
    this.onHover('RTQualityGlobalShadingLimit', "", "tooltip_cr01_rt_quality_global_shading_limit");
    this.onHover('RTQualityGrid1ShadingLimit', "", "tooltip_cr01_rt_quality_grid1_shading_limit");
    // V5.1 Normal RT performance controls
    this.onHover('RTReflLocalLights', "", "tooltip_cr01_rt_refl_local_lights");
    this.onHover('RTShadowLocalLights', "", "tooltip_cr01_rt_shadow_local_lights");
    this.onHover('RTShadowFoliageDetail', "", "tooltip_cr01_rt_shadow_foliage_detail");
    this.withFallbackMessage("", "tooltip_cr01_fallback");
  }

  // Official Shared Utilities Menu Descriptor dynamicDescription contract.
  // Read current saved values at hover time, not at menu-entry time, to avoid
  // stale descriptions after preset switching. All keys are owned and localized
  // in each of the 18 Quick Preset .w3strings packs.
  protected function dynamicDescription(menu: string, option: name, description: string): string {
    var rawValue: string;
    if (option == 'QuickPresetSelect') {
      rawValue = theGame.GetInGameConfigWrapper().GetVarValue('RefinedTracingCR01', 'QuickPresetSelect');
      if (rawValue == "1") return GetLocStringByKey("tooltip_cr01_quick_preset_performance");
      if (rawValue == "2") return GetLocStringByKey("tooltip_cr01_quick_preset_balanced");
      if (rawValue == "3") return GetLocStringByKey("tooltip_cr01_quick_preset_extreme");
      return GetLocStringByKey("tooltip_cr01_quick_preset_off");
    }
    if (option == 'RayTracedOptimizations') {
      rawValue = theGame.GetInGameConfigWrapper().GetVarValue('RefinedTracingCR01', 'RayTracedOptimizations');
      // Old V4 persisted 2/3 are interpreted as Auto until menu migration.
      if (rawValue == "1" || rawValue == "2" || rawValue == "3")
        return GetLocStringByKey("tooltip_cr01_rt_opt_auto");
      return GetLocStringByKey("tooltip_cr01_rt_opt_off");
    }
    return description;
  }
}

@wrapMethod(CR4IngameMenu)
function SU_onMenuEntered(menu: string, out descriptors: array<SU_MenuDescriptor>) {
  wrappedMethod(menu, descriptors);
  if (menu == "refinedtracing") {
    descriptors.PushBack(new RefinedTracingCR01_Descriptor in this);
  }
}

@addMethod(CR4IngameMenu)
function RT_CR01_DisableDependents(masterDisabled: bool, rtMode: string, rtaoDisabled: bool, quickPreset: string) {
  var tags: array<name>;
  var rtPerfTags: array<name>;
  var rtQualityTags: array<name>;
  var rtaoTags: array<name>;
  var changes: CScriptedFlashArray;
  var change: CScriptedFlashObject;
  var i: int;

  // Editable preset policy: a preset fills the 39 values but never locks them.
  // Master, RTGI Off/Auto and PT RTAO applicability still own their dependencies.
  // quickPreset remains in the signature to preserve the existing caller contract.

  tags.PushBack('PTShadows');
  tags.PushBack('CPUFastBoneReads');
  tags.PushBack('CPUDistantTrees');
  tags.PushBack('CPUPhysicsLOD');
  tags.PushBack('PTBillboards');
  tags.PushBack('GeometryBounces');
  tags.PushBack('HairBounces');
  tags.PushBack('IndirectLight');
  tags.PushBack('SunImpact');
  tags.PushBack('SkyImpact');
  tags.PushBack('AmbientOcclusion');
  tags.PushBack('AOStrength');
  tags.PushBack('AORadius');
  tags.PushBack('AOContrast');
  tags.PushBack('PTRTAOEnabled');
  tags.PushBack('GrassDay');
  tags.PushBack('GrassNight');
  tags.PushBack('SpecularScreenSpace');
  tags.PushBack('SpecularCheckerboard');
  tags.PushBack('RTReflLocalLights');
  tags.PushBack('RTShadowLocalLights');
  tags.PushBack('RTShadowFoliageDetail');
  rtPerfTags.PushBack('RTPerfClassificationPrimary');
  rtPerfTags.PushBack('Virtual_RTPerfClassificationSecondary');
  rtPerfTags.PushBack('RTPerfMaxProbes');
  rtPerfTags.PushBack('RTPerfAOQuality');
  rtPerfTags.PushBack('RTPerfShadingBudget');
  rtPerfTags.PushBack('RTPerfGlobalShadingLimit');
  rtPerfTags.PushBack('RTPerfGrid1ShadingLimit');
  rtQualityTags.PushBack('RTQualityClassificationPrimary');
  rtQualityTags.PushBack('Virtual_RTQualityClassificationSecondary');
  rtQualityTags.PushBack('RTQualityMaxProbes');
  rtQualityTags.PushBack('RTQualityAOQuality');
  rtQualityTags.PushBack('RTQualityShadingBudget');
  rtQualityTags.PushBack('RTQualityGlobalShadingLimit');
  rtQualityTags.PushBack('RTQualityGrid1ShadingLimit');

  rtaoTags.PushBack('PTRTAORayCount');
  rtaoTags.PushBack('PTRTAORadius');
  rtaoTags.PushBack('PTRTAOStrength');
  changes = m_flashValueStorage.CreateTempFlashArray();
  // Master-only dependency for existing PT and CPU controls.
  for (i = 0; i < tags.Size(); i += 1) {
    change = m_flashValueStorage.CreateTempFlashObject();
    change.SetMemberFlashUInt("tag", NameToFlashUInt(tags[i]));
    change.SetMemberFlashBool("disabled", masterDisabled);
    changes.PushBackFlashObject(change);
  }
  // Normal RTGI dependents respect Master and RT Auto, not the preset label.
  change = m_flashValueStorage.CreateTempFlashObject();
  change.SetMemberFlashUInt("tag", NameToFlashUInt('RayTracedOptimizations'));
  change.SetMemberFlashBool("disabled", masterDisabled);
  changes.PushBackFlashObject(change);
  // Off / Auto has UI IDs and stored Entries 0/0 and 1/1, respectively.
  // Both Quality and Performance rows are editable in Auto; Off greys both.
  // Legacy persisted RTGI choices 2/3 are migrated to stored Auto=1 at
  // menu entry. The native config reader also treats 2/3 as Auto immediately.
  for (i = 0; i < rtPerfTags.Size(); i += 1) {
    change = m_flashValueStorage.CreateTempFlashObject();
    change.SetMemberFlashUInt("tag", NameToFlashUInt(rtPerfTags[i]));
    change.SetMemberFlashBool("disabled", masterDisabled || rtMode != "1");
    changes.PushBackFlashObject(change);
  }
  for (i = 0; i < rtQualityTags.Size(); i += 1) {
    change = m_flashValueStorage.CreateTempFlashObject();
    change.SetMemberFlashUInt("tag", NameToFlashUInt(rtQualityTags[i]));
    change.SetMemberFlashBool("disabled", masterDisabled || rtMode != "1");
    changes.PushBackFlashObject(change);
  }
  for (i = 0; i < rtaoTags.Size(); i += 1) {
    change = m_flashValueStorage.CreateTempFlashObject();
    change.SetMemberFlashUInt("tag", NameToFlashUInt(rtaoTags[i]));
    change.SetMemberFlashBool("disabled", masterDisabled || rtaoDisabled);
    changes.PushBackFlashObject(change);
  }
  // Master stays accessible. Quick Preset is selectable whenever Master is On.
  change = m_flashValueStorage.CreateTempFlashObject();
  change.SetMemberFlashUInt("tag", NameToFlashUInt('QuickPresetSelect'));
  change.SetMemberFlashBool("disabled", masterDisabled);
  changes.PushBackFlashObject(change);
  m_flashValueStorage.SetFlashArray("options.update_disabled", changes);
  theGame.GetGuiManager().ForceProcessFlashStorage();
}

// PRIVATE Quick Preset integration. Runs only on a user's selector change.
// SetVarValue writes the existing CR01 menu values; SaveUserSettings happens
// ONCE in OnOptionValueChanged after the complete update. No per-frame writer.
// No recursive OnOptionValueChanged calls are issued by this method.
@addMethod(CR4IngameMenu)
function RT_CR01_ApplyQuickPreset(selected: string): bool {
  var values: CScriptedFlashArray;
  var update: CScriptedFlashObject;
  if (selected != "1" && selected != "2" && selected != "3") return false;
  values = m_flashValueStorage.CreateTempFlashArray();
  // CPUFastBoneReads
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'CPUFastBoneReads', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'CPUFastBoneReads', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'CPUFastBoneReads', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('CPUFastBoneReads'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'CPUFastBoneReads'));
  values.PushBackFlashObject(update);
  // CPUDistantTrees
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'CPUDistantTrees', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'CPUDistantTrees', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'CPUDistantTrees', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('CPUDistantTrees'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'CPUDistantTrees'));
  values.PushBackFlashObject(update);
  // CPUPhysicsLOD
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'CPUPhysicsLOD', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'CPUPhysicsLOD', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'CPUPhysicsLOD', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('CPUPhysicsLOD'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'CPUPhysicsLOD'));
  values.PushBackFlashObject(update);
  // PTBillboards
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTBillboards', "2");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTBillboards', "2");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTBillboards', "1");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('PTBillboards'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'PTBillboards'));
  values.PushBackFlashObject(update);
  // PTShadows
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTShadows', "2");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTShadows', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTShadows', "1");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('PTShadows'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'PTShadows'));
  values.PushBackFlashObject(update);
  // PTRTAOEnabled
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTRTAOEnabled', "0");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTRTAOEnabled', "0");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTRTAOEnabled', "1");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('PTRTAOEnabled'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'PTRTAOEnabled'));
  values.PushBackFlashObject(update);
  // PTRTAORayCount
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTRTAORayCount', "0");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTRTAORayCount', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTRTAORayCount', "3");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('PTRTAORayCount'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'PTRTAORayCount'));
  values.PushBackFlashObject(update);
  // PTRTAORadius
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTRTAORadius', "0");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTRTAORadius', "4");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTRTAORadius', "6");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('PTRTAORadius'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'PTRTAORadius'));
  values.PushBackFlashObject(update);
  // PTRTAOStrength
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTRTAOStrength', "0");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTRTAOStrength', "2");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTRTAOStrength', "5");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('PTRTAOStrength'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'PTRTAOStrength'));
  values.PushBackFlashObject(update);
  // GeometryBounces
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'GeometryBounces', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'GeometryBounces', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'GeometryBounces', "6");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('GeometryBounces'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'GeometryBounces'));
  values.PushBackFlashObject(update);
  // HairBounces
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'HairBounces', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'HairBounces', "4");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'HairBounces', "6");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('HairBounces'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'HairBounces'));
  values.PushBackFlashObject(update);
  // IndirectLight
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'IndirectLight', "1.20");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'IndirectLight', "1.10");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'IndirectLight', "1.00");
  }
  // Flash OPTIONS current uses XML Option IDs, never stored Entry values.
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('IndirectLight'));
  if (selected == "1") {
    update.SetMemberFlashString("current", "4");
  } else if (selected == "2") {
    update.SetMemberFlashString("current", "2");
  } else {
    update.SetMemberFlashString("current", "0");
  }
  values.PushBackFlashObject(update);
  // SunImpact
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'SunImpact', "1.00");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'SunImpact', "1.00");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'SunImpact', "1.00");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('SunImpact'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'SunImpact'));
  values.PushBackFlashObject(update);
  // SkyImpact
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'SkyImpact', "1.00");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'SkyImpact', "1.00");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'SkyImpact', "1.00");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('SkyImpact'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'SkyImpact'));
  values.PushBackFlashObject(update);
  // AmbientOcclusion
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'AmbientOcclusion', "0");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'AmbientOcclusion', "0");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'AmbientOcclusion', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('AmbientOcclusion'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'AmbientOcclusion'));
  values.PushBackFlashObject(update);
  // AOStrength
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'AOStrength', "0");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'AOStrength', "0");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'AOStrength', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('AOStrength'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'AOStrength'));
  values.PushBackFlashObject(update);
  // AORadius
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'AORadius', "0");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'AORadius', "0");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'AORadius', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('AORadius'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'AORadius'));
  values.PushBackFlashObject(update);
  // AOContrast
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'AOContrast', "0");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'AOContrast', "0");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'AOContrast', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('AOContrast'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'AOContrast'));
  values.PushBackFlashObject(update);
  // GrassDay
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'GrassDay', "0");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'GrassDay', "0");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'GrassDay', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('GrassDay'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'GrassDay'));
  values.PushBackFlashObject(update);
  // GrassNight
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'GrassNight', "0");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'GrassNight', "0");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'GrassNight', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('GrassNight'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'GrassNight'));
  values.PushBackFlashObject(update);
  // SpecularScreenSpace
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'SpecularScreenSpace', "2");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'SpecularScreenSpace', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'SpecularScreenSpace', "1");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('SpecularScreenSpace'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'SpecularScreenSpace'));
  values.PushBackFlashObject(update);
  // SpecularCheckerboard
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'SpecularCheckerboard', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'SpecularCheckerboard', "2");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'SpecularCheckerboard', "2");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('SpecularCheckerboard'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'SpecularCheckerboard'));
  values.PushBackFlashObject(update);
  // RayTracedOptimizations
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RayTracedOptimizations', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RayTracedOptimizations', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RayTracedOptimizations', "1");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('RayTracedOptimizations'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'RayTracedOptimizations'));
  values.PushBackFlashObject(update);
  // RTQualityClassificationPrimary
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityClassificationPrimary', "32");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityClassificationPrimary', "64");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityClassificationPrimary', "128");
  }
  // Flash OPTIONS current uses XML Option IDs, never stored Entry values.
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('RTQualityClassificationPrimary'));
  if (selected == "1") {
    update.SetMemberFlashString("current", "1");
  } else if (selected == "2") {
    update.SetMemberFlashString("current", "3");
  } else {
    update.SetMemberFlashString("current", "5");
  }
  values.PushBackFlashObject(update);
  // RTQualityClassificationSecondary
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityClassificationSecondary', "12");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityClassificationSecondary', "16");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityClassificationSecondary', "32");
  }
  // Flash OPTIONS current uses XML Option IDs, never stored Entry values.
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('Virtual_RTQualityClassificationSecondary'));
  if (selected == "1") {
    update.SetMemberFlashString("current", "4");
  } else if (selected == "2") {
    update.SetMemberFlashString("current", "5");
  } else {
    update.SetMemberFlashString("current", "7");
  }
  values.PushBackFlashObject(update);
  // RTQualityMaxProbes
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityMaxProbes', "8192");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityMaxProbes', "65536");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityMaxProbes', "262144");
  }
  // Flash OPTIONS current uses XML Option IDs, never stored Entry values.
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('RTQualityMaxProbes'));
  if (selected == "1") {
    update.SetMemberFlashString("current", "1");
  } else if (selected == "2") {
    update.SetMemberFlashString("current", "3");
  } else {
    update.SetMemberFlashString("current", "5");
  }
  values.PushBackFlashObject(update);
  // RTQualityAOQuality
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityAOQuality', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityAOQuality', "2");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityAOQuality', "4");
  }
  // Flash OPTIONS current uses XML Option IDs, never stored Entry values.
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('RTQualityAOQuality'));
  if (selected == "1") {
    update.SetMemberFlashString("current", "1");
  } else if (selected == "2") {
    update.SetMemberFlashString("current", "2");
  } else {
    update.SetMemberFlashString("current", "4");
  }
  values.PushBackFlashObject(update);
  // RTQualityShadingBudget
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityShadingBudget', "65536");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityShadingBudget', "262144");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityShadingBudget', "524288");
  }
  // Flash OPTIONS current uses XML Option IDs, never stored Entry values.
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('RTQualityShadingBudget'));
  if (selected == "1") {
    update.SetMemberFlashString("current", "1");
  } else if (selected == "2") {
    update.SetMemberFlashString("current", "4");
  } else {
    update.SetMemberFlashString("current", "6");
  }
  values.PushBackFlashObject(update);
  // RTQualityGlobalShadingLimit
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityGlobalShadingLimit', "4096");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityGlobalShadingLimit', "12288");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityGlobalShadingLimit', "32768");
  }
  // Flash OPTIONS current uses XML Option IDs, never stored Entry values.
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('RTQualityGlobalShadingLimit'));
  if (selected == "1") {
    update.SetMemberFlashString("current", "1");
  } else if (selected == "2") {
    update.SetMemberFlashString("current", "3");
  } else {
    update.SetMemberFlashString("current", "6");
  }
  values.PushBackFlashObject(update);
  // RTQualityGrid1ShadingLimit
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityGrid1ShadingLimit', "16384");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityGrid1ShadingLimit', "49152");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTQualityGrid1ShadingLimit', "262144");
  }
  // Flash OPTIONS current uses XML Option IDs, never stored Entry values.
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('RTQualityGrid1ShadingLimit'));
  if (selected == "1") {
    update.SetMemberFlashString("current", "1");
  } else if (selected == "2") {
    update.SetMemberFlashString("current", "3");
  } else {
    update.SetMemberFlashString("current", "6");
  }
  values.PushBackFlashObject(update);
  // RTPerfClassificationPrimary
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfClassificationPrimary', "32");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfClassificationPrimary', "64");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfClassificationPrimary', "128");
  }
  // Flash OPTIONS current uses XML Option IDs, never stored Entry values.
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('RTPerfClassificationPrimary'));
  if (selected == "1") {
    update.SetMemberFlashString("current", "1");
  } else if (selected == "2") {
    update.SetMemberFlashString("current", "3");
  } else {
    update.SetMemberFlashString("current", "5");
  }
  values.PushBackFlashObject(update);
  // RTPerfClassificationSecondary
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfClassificationSecondary', "12");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfClassificationSecondary', "16");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfClassificationSecondary', "32");
  }
  // Flash OPTIONS current uses XML Option IDs, never stored Entry values.
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('Virtual_RTPerfClassificationSecondary'));
  if (selected == "1") {
    update.SetMemberFlashString("current", "4");
  } else if (selected == "2") {
    update.SetMemberFlashString("current", "5");
  } else {
    update.SetMemberFlashString("current", "7");
  }
  values.PushBackFlashObject(update);
  // RTPerfMaxProbes
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfMaxProbes', "8192");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfMaxProbes', "65536");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfMaxProbes', "262144");
  }
  // Flash OPTIONS current uses XML Option IDs, never stored Entry values.
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('RTPerfMaxProbes'));
  if (selected == "1") {
    update.SetMemberFlashString("current", "1");
  } else if (selected == "2") {
    update.SetMemberFlashString("current", "3");
  } else {
    update.SetMemberFlashString("current", "5");
  }
  values.PushBackFlashObject(update);
  // RTPerfAOQuality
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfAOQuality', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfAOQuality', "2");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfAOQuality', "4");
  }
  // Flash OPTIONS current uses XML Option IDs, never stored Entry values.
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('RTPerfAOQuality'));
  if (selected == "1") {
    update.SetMemberFlashString("current", "1");
  } else if (selected == "2") {
    update.SetMemberFlashString("current", "2");
  } else {
    update.SetMemberFlashString("current", "4");
  }
  values.PushBackFlashObject(update);
  // RTPerfShadingBudget
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfShadingBudget', "65536");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfShadingBudget', "262144");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfShadingBudget', "524288");
  }
  // Flash OPTIONS current uses XML Option IDs, never stored Entry values.
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('RTPerfShadingBudget'));
  if (selected == "1") {
    update.SetMemberFlashString("current", "1");
  } else if (selected == "2") {
    update.SetMemberFlashString("current", "4");
  } else {
    update.SetMemberFlashString("current", "6");
  }
  values.PushBackFlashObject(update);
  // RTPerfGlobalShadingLimit
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfGlobalShadingLimit', "4096");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfGlobalShadingLimit', "12288");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfGlobalShadingLimit', "32768");
  }
  // Flash OPTIONS current uses XML Option IDs, never stored Entry values.
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('RTPerfGlobalShadingLimit'));
  if (selected == "1") {
    update.SetMemberFlashString("current", "1");
  } else if (selected == "2") {
    update.SetMemberFlashString("current", "3");
  } else {
    update.SetMemberFlashString("current", "6");
  }
  values.PushBackFlashObject(update);
  // RTPerfGrid1ShadingLimit
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfGrid1ShadingLimit', "16384");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfGrid1ShadingLimit', "49152");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTPerfGrid1ShadingLimit', "262144");
  }
  // Flash OPTIONS current uses XML Option IDs, never stored Entry values.
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('RTPerfGrid1ShadingLimit'));
  if (selected == "1") {
    update.SetMemberFlashString("current", "1");
  } else if (selected == "2") {
    update.SetMemberFlashString("current", "3");
  } else {
    update.SetMemberFlashString("current", "6");
  }
  values.PushBackFlashObject(update);
  // RTReflLocalLights (V5.1)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTReflLocalLights', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTReflLocalLights', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTReflLocalLights', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('RTReflLocalLights'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'RTReflLocalLights'));
  values.PushBackFlashObject(update);
  // RTShadowLocalLights (V5.1)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTShadowLocalLights', "4");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTShadowLocalLights', "2");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTShadowLocalLights', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('RTShadowLocalLights'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'RTShadowLocalLights'));
  values.PushBackFlashObject(update);
  // RTShadowFoliageDetail (V5.1)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTShadowFoliageDetail', "3");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTShadowFoliageDetail', "2");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RTShadowFoliageDetail', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('RTShadowFoliageDetail'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'RTShadowFoliageDetail'));
  values.PushBackFlashObject(update);
  // Single refresh of the complete 39-control transaction. All OPTIONS rows
  // use their existing XML Option IDs, with stored configuration values intact.
  // The existing callback restores preset locks after this refresh.
  m_flashValueStorage.SetFlashArray("options.force_update_values", values);
  theGame.GetGuiManager().ForceProcessFlashStorage();
  return true;
}

@addMethod(CR4IngameMenu)
function RT_CR01_IsDecimalText(raw: string): bool {
  var i: int;
  var dotCount: int;
  var c: string;
  if (StrLen(raw) == 0 || StrLen(raw) > 24) return false;
  dotCount = 0;
  for (i = 0; i < StrLen(raw); i += 1) {
    c = StrMid(raw, i, 1);
    if (c == ".") {
      dotCount += 1;
      if (dotCount > 1) return false;
    } else if (c != "0" && c != "1" && c != "2" && c != "3" && c != "4"
            && c != "5" && c != "6" && c != "7" && c != "8" && c != "9") {
      return false;
    }
  }
  if (StrLeft(raw, 1) == "." || StrRight(raw, 1) == ".") return false;
  return true;
}

@addMethod(CR4IngameMenu)
function RT_CR01_RepairLiveIndirectLight(): bool {
  var raw: string;
  var value: float;
  var corrected: string;
  raw = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'IndirectLight');
  // OPTIONS GetVarValue returns the selector ID, not its decimal Entry.
  // Valid IDs are 0..10. In particular, 1=1.05, 2=1.10, 7=1.35.
  // Previously IDs 2..10 were misread as >1.50 and 'repaired'.
  if (raw == "0" || raw == "1" || raw == "2" || raw == "3" || raw == "4"
      || raw == "5" || raw == "6" || raw == "7" || raw == "8"
      || raw == "9" || raw == "10") return false;
  if (this.RT_CR01_IsDecimalText(raw)) {
    value = StringToFloat(raw);
    if (value >= 1.0 && value <= 1.5) return false;
    if (value > 1.5) {
      corrected = "1.50";
    } else {
      corrected = "1.00";
    }
  } else {
    corrected = "1.00";
  }
  // SetVarValue for OPTIONS expects the option ID, never its Entry decimal.
  // Keep the existing invalid-value fallback, now expressed in valid IDs.
  if (corrected == "1.50") corrected = "10";
  else corrected = "0";
  // Repair only the in-memory menu field. Persist on the next genuine user edit,
  // not while opening a menu before the game config writer is confirmed ready.
  mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'IndirectLight', corrected);
  return true;
}

@wrapMethod(CR4IngameMenu)
function OnShowOptionSubmenu(actionType: int, menuTag: int, id: string) {
  var rawMaster: string;
  var rawRT: string;
  var rawRTAO: string;
  var rawPreset: string;
  var repaired: bool;
  var corrected: string;
  var updates: CScriptedFlashArray;
  var update: CScriptedFlashObject;

  repaired = false;
  if (id == "refinedtracing") {
    // One-shot V4 manual-mode migration: 2=Quality, 3=Performance -> Auto=1.
    // No reset of the sixteen separately saved RTGI configuration values.
    // Run BEFORE wrappedMethod so the displayed selector has a valid entry.
    rawRT = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'RayTracedOptimizations');
    if (rawRT == "2" || rawRT == "3") {
      // Display canonical Auto now; defer disk persistence until an explicit
      // user menu action. Native reader already normalizes historic 2/3.
      mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'RayTracedOptimizations', "1");
    }
    repaired = this.RT_CR01_RepairLiveIndirectLight();
  }

  wrappedMethod(actionType, menuTag, id);

  if (id == "refinedtracing") {
    if (repaired) {
      corrected = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'IndirectLight');
      updates = m_flashValueStorage.CreateTempFlashArray();
      update = m_flashValueStorage.CreateTempFlashObject();
      update.SetMemberFlashUInt("tag", NameToFlashUInt('IndirectLight'));
      // The OPTIONS widget expects XML Option id, not numeric Entry text.
      if (corrected == "10") {
        update.SetMemberFlashString("current", "10");
      } else {
        update.SetMemberFlashString("current", "0");
      }
      updates.PushBackFlashObject(update);
      m_flashValueStorage.SetFlashArray("options.force_update_values", updates);
      theGame.GetGuiManager().ForceProcessFlashStorage();
    }

    rawMaster = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'MasterEnabled');
    rawRT = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'RayTracedOptimizations');
    rawRTAO = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'PTRTAOEnabled');
    rawPreset = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'QuickPresetSelect');
    this.RT_CR01_DisableDependents(rawMaster != "true" && rawMaster != "1", rawRT, rawRTAO != "true" && rawRTAO != "1", rawPreset);


  }
}

// V5.0.0 hotfix: URT reacts only to its own options. Vanilla options that are
// not ordinary config variables (e.g. Game Difficulty, Gwent Difficulty) reach
// OnOptionValueChanged with a group id that is not a real config group index;
// resolving that id with GetGroupName is the reported crash path (Nexus report,
// Options > Gameplay > difficulty). Ownership is therefore decided
// by URT's own option names (the RefinedTracingCR01 Var ids of RefinedTracing.xml)
// BEFORE any group lookup, and every other option goes straight to wrappedMethod.
@addMethod(CR4IngameMenu)
function RT_CR01_IsOwnedOption(optionName: name): bool {
  if (optionName == 'MasterEnabled'
      || optionName == 'QuickPresetSelect'
      || optionName == 'CPUFastBoneReads'
      || optionName == 'CPUDistantTrees'
      || optionName == 'CPUPhysicsLOD'
      || optionName == 'PTBillboards'
      || optionName == 'PTShadows'
      || optionName == 'PTRTAOEnabled'
      || optionName == 'PTRTAORayCount'
      || optionName == 'PTRTAORadius'
      || optionName == 'PTRTAOStrength'
      || optionName == 'GeometryBounces'
      || optionName == 'HairBounces'
      || optionName == 'IndirectLight'
      || optionName == 'SunImpact'
      || optionName == 'SkyImpact'
      || optionName == 'AmbientOcclusion'
      || optionName == 'AOStrength'
      || optionName == 'AORadius'
      || optionName == 'AOContrast'
      || optionName == 'GrassDay'
      || optionName == 'GrassNight'
      || optionName == 'SpecularScreenSpace'
      || optionName == 'SpecularCheckerboard'
      || optionName == 'RayTracedOptimizations'
      || optionName == 'RTQualityClassificationPrimary'
      || optionName == 'Virtual_RTQualityClassificationSecondary'
      || optionName == 'RTQualityMaxProbes'
      || optionName == 'RTQualityAOQuality'
      || optionName == 'RTQualityShadingBudget'
      || optionName == 'RTQualityGlobalShadingLimit'
      || optionName == 'RTQualityGrid1ShadingLimit'
      || optionName == 'RTPerfClassificationPrimary'
      || optionName == 'Virtual_RTPerfClassificationSecondary'
      || optionName == 'RTPerfMaxProbes'
      || optionName == 'RTPerfAOQuality'
      || optionName == 'RTPerfShadingBudget'
      || optionName == 'RTPerfGlobalShadingLimit'
      || optionName == 'RTPerfGrid1ShadingLimit'
      || optionName == 'RTReflLocalLights'
      || optionName == 'RTShadowLocalLights'
      || optionName == 'RTShadowFoliageDetail') {
    return true;
  }
  return false;
}

@wrapMethod(CR4IngameMenu)
function OnOptionValueChanged(groupId: int, optionName: name, optionValue: string) {
  var result: bool;
  var groupName: name;
  var rawMaster: string;
  var rawRT: string;
  var rawRTAO: string;
  var rawPreset: string;
  var presetUiValues: CScriptedFlashArray;
  var presetUiUpdate: CScriptedFlashObject;

  groupName = '';
  if (this.RT_CR01_IsOwnedOption(optionName)) {
    // Only URT's own rows (real config group ids) are resolved; the group
    // check also guards against a same-named variable in another group.
    groupName = mInGameConfigWrapper.GetGroupName(groupId);
  }
  if (groupName == 'RefinedTracingCR01') {
    rawPreset = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'QuickPresetSelect');
    rawMaster = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'MasterEnabled');
    // Do not process a stale selector event when Master is already Off.
    if (optionName == 'QuickPresetSelect' && rawMaster != "true" && rawMaster != "1") return false;
    // Preset-applied rows remain editable. A genuine user edit will disengage
    // the preset after wrappedMethod stores the new value (below).
  }
  // The inherited handler writes the selector first. Applying its existing
  // settings before the single SaveUserSettings makes the persisted snapshot
  // coherent. Never reapply on menu entry/restart; saved values stay authoritative.
  result = wrappedMethod(groupId, optionName, optionValue);
  // A manual tweak becomes a custom configuration. Change the selector to Off
  // directly, without passing through Performance or reapplying ANY preset.
  // SetVarValue does not issue a user OnOptionValueChanged callback; save once
  // at the end alongside the edited field, preserving all other 38 values.
  if (groupName == 'RefinedTracingCR01'
      && optionName != 'MasterEnabled' && optionName != 'QuickPresetSelect') {
    rawPreset = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'QuickPresetSelect');
    if (rawPreset == "1" || rawPreset == "2" || rawPreset == "3") {
      mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'QuickPresetSelect', "0");
      presetUiValues = m_flashValueStorage.CreateTempFlashArray();
      presetUiUpdate = m_flashValueStorage.CreateTempFlashObject();
      presetUiUpdate.SetMemberFlashUInt("tag", NameToFlashUInt('QuickPresetSelect'));
      presetUiUpdate.SetMemberFlashString("current", "0");
      presetUiValues.PushBackFlashObject(presetUiUpdate);
      m_flashValueStorage.SetFlashArray("options.force_update_values", presetUiValues);
      theGame.GetGuiManager().ForceProcessFlashStorage();
    }
  }
  if (groupName == 'RefinedTracingCR01' && optionName == 'QuickPresetSelect') {
    rawMaster = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'MasterEnabled');
    rawPreset = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'QuickPresetSelect');
    // Master Off should not be able to apply a preset via stale Flash events.
    if ((rawMaster == "true" || rawMaster == "1") && rawPreset != "0") {
      this.RT_CR01_ApplyQuickPreset(rawPreset);
    }
  }

  if (groupName == 'RefinedTracingCR01') {
    if (optionName == 'MasterEnabled' || optionName == 'QuickPresetSelect'
        || optionName == 'RayTracedOptimizations' || optionName == 'PTRTAOEnabled') {
      // The wrapped handler has already written the latest config values.
      rawMaster = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'MasterEnabled');
      rawRT = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'RayTracedOptimizations');
      rawRTAO = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'PTRTAOEnabled');
      rawPreset = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'QuickPresetSelect');
      this.RT_CR01_DisableDependents(rawMaster != "true" && rawMaster != "1", rawRT, rawRTAO != "true" && rawRTAO != "1", rawPreset);
    }
    // Exact CR01 menu event -> durable menu mirror event for the native owner.
    theGame.SaveUserSettings();
  } else if (optionName == 'PTEnable' || optionName == 'EnableRT'
          || optionName == 'EnableRtRadiance' || optionName == 'Virtual_RTShadows') {
    // Renderer-eligibility/native-shadow changes use the same event path.
    // The ASI still verifies live CVars and never trusts this text as renderer truth.
    theGame.SaveUserSettings();
  }

  return result;
}
