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
    this.onHover('PTSceneRange', "", "tooltip_cr01_pt_scene_range");
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
    this.onHover('GrassDay', "", "tooltip_cr01_grassday");
    this.onHover('GrassNight', "", "tooltip_cr01_grassnight");
    this.onHover('SpecularScreenSpace', "", "tooltip_cr01_specularscreenspace");
    this.onHover('SpecularCheckerboard', "", "tooltip_cr01_specularcheckerboard");
    this.onHoverDynamic('RayTracedOptimizations', "", "tooltip_cr01_rt_opt_off");
    this.onHover('RTPerfClassificationPrimary', "", "tooltip_cr01_rt_perf_primary");
    this.onHover('Virtual_RTPerfClassificationSecondary', "", "tooltip_cr01_rt_perf_secondary");
    this.onHover('RTPerfMaxProbes', "", "tooltip_cr01_rt_perf_cap");
    this.onHover('RTPerfAOQuality', "", "tooltip_cr01_rt_perf_ao");
    this.onHover('RTPerfGlobalShadingLimit', "", "tooltip_cr01_rt_perf_global_shading_limit");
    this.onHover('RTPerfGrid1ShadingLimit', "", "tooltip_cr01_rt_perf_grid1_shading_limit");
    this.onHover('RTQualityClassificationPrimary', "", "tooltip_cr01_rt_quality_primary");
    this.onHover('Virtual_RTQualityClassificationSecondary', "", "tooltip_cr01_rt_quality_secondary");
    this.onHover('RTQualityMaxProbes', "", "tooltip_cr01_rt_quality_cap");
    this.onHover('RTQualityAOQuality', "", "tooltip_cr01_rt_quality_ao");
    this.onHover('RTQualityGlobalShadingLimit', "", "tooltip_cr01_rt_quality_global_shading_limit");
    this.onHover('RTQualityGrid1ShadingLimit', "", "tooltip_cr01_rt_quality_grid1_shading_limit");
    // V5.1 Normal RT performance controls
    this.onHover('RTReflLocalLights', "", "tooltip_cr01_rt_refl_local_lights");
    this.onHover('RTShadowLocalLights', "", "tooltip_cr01_rt_shadow_local_lights");
    this.onHover('RTShadowFoliageDetail', "", "tooltip_cr01_rt_shadow_foliage_detail");
    this.onHover('URTX_PTAOQuality', "", "tooltip_urtx_ptaoquality");
    this.onHover('URTX_RTAOScreenSpace', "", "tooltip_urtx_rtaoscreenspace");
    this.onHover('URTX_RTHBIL', "", "tooltip_urtx_rthbil");
    this.onHover('URTX_RTAOPretrace', "", "tooltip_urtx_rtaopretrace");
    this.onHover('URTX_RTReflScreenSpace', "", "tooltip_urtx_rtreflscreenspace");
    this.onHover('URTX_TextureStreams', "", "tooltip_urtx_texturestreams");
    this.onHover('URTX_PTCaustics', "", "tooltip_urtx_ptcaustics");
    this.onHover('URTX_PTCausticsInPT', "", "tooltip_urtx_ptcausticsinpt");
    this.onHover('URTX_PTCausticsTypes', "", "tooltip_urtx_ptcausticstypes");
    this.onHover('URTX_PTCausticsRes', "", "tooltip_urtx_ptcausticsres");
    this.onHover('URTX_PTCausticsBlur', "", "tooltip_urtx_ptcausticsblur");
    this.onHover('URTX_PTCausticsArea', "", "tooltip_urtx_ptcausticsarea");
    this.onHover('URTX_PTSkinBounceDesat', "", "tooltip_urtx_ptskinbouncedesat");
    this.onHover('URTX_RTReflFog', "", "tooltip_urtx_rtreflfog");
    this.onHover('URTX_TerrainPreload', "", "tooltip_urtx_terrainpreload");
    this.onHover('URTX_TreeDistance', "", "tooltip_urtx_treedistance");
    this.onHover('URTX_FGMenuResources', "", "tooltip_urtx_fgmenuresources");
    // V5.2.0 expanded rows (Native / Auto by default; eight are assigned by the Quick Presets, not No Blue Glow)
    this.onHover('URTX_TexUnloadSpeed', "", "tooltip_urtx_texunloadspeed");
    this.onHover('URTX_MeshStreamLead', "", "tooltip_urtx_meshstreamlead");
    this.onHover('URTX_GenericStreamLead', "", "tooltip_urtx_genericstreamlead");
    this.onHover('URTX_GCDiscardThreshold', "", "tooltip_urtx_gcdiscardthreshold");
    this.onHover('URTX_GCIdleFrames', "", "tooltip_urtx_gcidleframes");
    this.onHover('URTX_RTASDefrag', "", "tooltip_urtx_rtasdefrag");
    this.onHover('URTX_RTASBatch', "", "tooltip_urtx_rtasbatch");
    this.onHover('URTX_RTGIAlphaTest', "", "tooltip_urtx_rtgialphatest");
    this.onHover('URTX_PTNoBlueGlow', "", "tooltip_urtx_ptnoblueglow");
    // V5.3.0 performance rows (Native / Auto by default; Quick Preset assignments in RT_CR01_ApplyQuickPreset)
    this.onHover('URTX_PTRadianceCacheDensity', "", "tooltip_urtx_ptradiancecachedensity");
    this.onHover('URTX_RTAsyncAO', "", "tooltip_urtx_rtasyncao");
    this.onHover('URTX_RTAsyncGIShading', "", "tooltip_urtx_rtasyncgishading");
    this.onHover('URTX_RTTreeWindDistance', "", "tooltip_urtx_rttreewinddistance");
    this.onHover('URTX_RTAsyncGIClassify', "", "tooltip_urtx_rtasyncgiclassify");
    this.onHover('URTX_RTASAsyncCompact', "", "tooltip_urtx_rtasasynccompact");
    this.onHover('URTX_RTASFastTrace', "", "tooltip_urtx_rtasfasttrace");
    this.onHover('URTX_PTMoonLight', "", "tooltip_urtx_ptmoonlight");
    // V5.3.0 EXPERIMENTAL section (Native / Auto by default; no Quick Preset). R11: PT Bounce Optimisation Layer
    this.onHover('URTX_PTDiffuseCoverage', "", "tooltip_urtx_ptdiffusecoverage");
    this.onHover('URTX_ExpPTSunShadowCoverage', "", "tooltip_urtx_expptsunshadowcoverage");
    this.onHover('URTX_ExpPTLocalShadowCoverage', "", "tooltip_urtx_expptlocalshadowcoverage");
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
  var changes: CScriptedFlashArray;
  var change: CScriptedFlashObject;
  var i: int;

  // Editable preset policy: a preset fills the 39 values but never locks them.
  // Master and RTGI Off/Auto retain their established dependency rules.
  // quickPreset remains in the signature to preserve the existing caller contract.

  tags.PushBack('PTShadows');
  tags.PushBack('PTSceneRange');
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
  tags.PushBack('GrassDay');
  tags.PushBack('GrassNight');
  tags.PushBack('SpecularScreenSpace');
  tags.PushBack('SpecularCheckerboard');
  tags.PushBack('RTReflLocalLights');
  tags.PushBack('RTShadowLocalLights');
  tags.PushBack('RTShadowFoliageDetail');
  tags.PushBack('URTX_PTAOQuality');
  tags.PushBack('URTX_RTAOScreenSpace');
  tags.PushBack('URTX_RTHBIL');
  tags.PushBack('URTX_RTAOPretrace');
  tags.PushBack('URTX_RTReflScreenSpace');
  tags.PushBack('URTX_TextureStreams');
  tags.PushBack('URTX_PTCaustics');
  tags.PushBack('URTX_PTCausticsInPT');
  tags.PushBack('URTX_PTCausticsTypes');
  tags.PushBack('URTX_PTCausticsRes');
  tags.PushBack('URTX_PTCausticsBlur');
  tags.PushBack('URTX_PTCausticsArea');
  tags.PushBack('URTX_PTSkinBounceDesat');
  tags.PushBack('URTX_RTReflFog');
  tags.PushBack('URTX_TerrainPreload');
  tags.PushBack('URTX_TreeDistance');
  tags.PushBack('URTX_FGMenuResources');
  tags.PushBack('URTX_TexUnloadSpeed');
  tags.PushBack('URTX_MeshStreamLead');
  tags.PushBack('URTX_GenericStreamLead');
  tags.PushBack('URTX_GCDiscardThreshold');
  tags.PushBack('URTX_GCIdleFrames');
  tags.PushBack('URTX_RTASDefrag');
  tags.PushBack('URTX_RTASBatch');
  tags.PushBack('URTX_RTGIAlphaTest');
  tags.PushBack('URTX_PTNoBlueGlow');
  tags.PushBack('URTX_PTRadianceCacheDensity');
  tags.PushBack('URTX_RTAsyncAO');
  tags.PushBack('URTX_RTAsyncGIShading');
  tags.PushBack('URTX_RTTreeWindDistance');
  tags.PushBack('URTX_RTAsyncGIClassify');
  tags.PushBack('URTX_RTASAsyncCompact');
  tags.PushBack('URTX_RTASFastTrace');
  tags.PushBack('URTX_PTMoonLight');
  tags.PushBack('URTX_PTDiffuseCoverage');
  tags.PushBack('URTX_ExpPTSunShadowCoverage');
  tags.PushBack('URTX_ExpPTLocalShadowCoverage');
  rtPerfTags.PushBack('RTPerfClassificationPrimary');
  rtPerfTags.PushBack('Virtual_RTPerfClassificationSecondary');
  rtPerfTags.PushBack('RTPerfMaxProbes');
  rtPerfTags.PushBack('RTPerfAOQuality');
  rtPerfTags.PushBack('RTPerfGlobalShadingLimit');
  rtPerfTags.PushBack('RTPerfGrid1ShadingLimit');
  rtQualityTags.PushBack('RTQualityClassificationPrimary');
  rtQualityTags.PushBack('Virtual_RTQualityClassificationSecondary');
  rtQualityTags.PushBack('RTQualityMaxProbes');
  rtQualityTags.PushBack('RTQualityAOQuality');
  rtQualityTags.PushBack('RTQualityGlobalShadingLimit');
  rtQualityTags.PushBack('RTQualityGrid1ShadingLimit');

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
  // Master stays accessible. Quick Preset is selectable whenever Master is On.
  change = m_flashValueStorage.CreateTempFlashObject();
  change.SetMemberFlashUInt("tag", NameToFlashUInt('QuickPresetSelect'));
  change.SetMemberFlashBool("disabled", masterDisabled);
  changes.PushBackFlashObject(change);
  m_flashValueStorage.SetFlashArray("options.update_disabled", changes);
  theGame.GetGuiManager().ForceProcessFlashStorage();
}

// Quick Preset integration. Runs only on a user's selector change.
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
  // GeometryBounces (V5.2.0 final: Extreme = 4)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'GeometryBounces', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'GeometryBounces', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'GeometryBounces', "4");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('GeometryBounces'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'GeometryBounces'));
  values.PushBackFlashObject(update);
  // HairBounces (V5.2.0 final: Extreme = 10)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'HairBounces', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'HairBounces', "4");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'HairBounces', "10");
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
  // AmbientOcclusion (V5.2.0 final presets: Performance = Off)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'AmbientOcclusion', "2");
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
  // PTSceneRange (V5.2.0 final presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTSceneRange', "4");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTSceneRange', "0");
  } else {
    mInGameConfigWrapper.SetVarValue('RefinedTracingCR01', 'PTSceneRange', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('PTSceneRange'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'PTSceneRange'));
  values.PushBackFlashObject(update);
  // URTX_PTAOQuality (V5.2.0 final presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_PTAOQuality', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_PTAOQuality', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_PTAOQuality', "4");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_PTAOQuality'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_PTAOQuality'));
  values.PushBackFlashObject(update);
  // URTX_RTAOScreenSpace (V5.2.0 final presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTAOScreenSpace', "2");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTAOScreenSpace', "2");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTAOScreenSpace', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_RTAOScreenSpace'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_RTAOScreenSpace'));
  values.PushBackFlashObject(update);
  // URTX_RTHBIL (V5.2.0 final presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTHBIL', "2");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTHBIL', "2");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTHBIL', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_RTHBIL'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_RTHBIL'));
  values.PushBackFlashObject(update);
  // URTX_RTAOPretrace (V5.2.0 final presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTAOPretrace', "2");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTAOPretrace', "2");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTAOPretrace', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_RTAOPretrace'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_RTAOPretrace'));
  values.PushBackFlashObject(update);
  // URTX_RTReflScreenSpace (V5.2.0 final presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTReflScreenSpace', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTReflScreenSpace', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTReflScreenSpace', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_RTReflScreenSpace'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_RTReflScreenSpace'));
  values.PushBackFlashObject(update);
  // URTX_RTReflFog (V5.2.0 final presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTReflFog', "2");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTReflFog', "0");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTReflFog', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_RTReflFog'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_RTReflFog'));
  values.PushBackFlashObject(update);
  // URTX_TextureStreams (V5.2.0 final presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_TextureStreams', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_TextureStreams', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_TextureStreams', "2");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_TextureStreams'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_TextureStreams'));
  values.PushBackFlashObject(update);
  // URTX_TerrainPreload (V5.2.0 final presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_TerrainPreload', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_TerrainPreload', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_TerrainPreload', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_TerrainPreload'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_TerrainPreload'));
  values.PushBackFlashObject(update);
  // URTX_FGMenuResources (V5.2.0 final presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_FGMenuResources', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_FGMenuResources', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_FGMenuResources', "1");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_FGMenuResources'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_FGMenuResources'));
  values.PushBackFlashObject(update);
  // URTX_RTGIAlphaTest (V5.2.0 expanded presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTGIAlphaTest', "4");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTGIAlphaTest', "2");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTGIAlphaTest', "1");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_RTGIAlphaTest'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_RTGIAlphaTest'));
  values.PushBackFlashObject(update);
  // URTX_TexUnloadSpeed (V5.2.0 expanded presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_TexUnloadSpeed', "3");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_TexUnloadSpeed', "2");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_TexUnloadSpeed', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_TexUnloadSpeed'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_TexUnloadSpeed'));
  values.PushBackFlashObject(update);
  // URTX_MeshStreamLead (V5.2.0 expanded presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_MeshStreamLead', "0");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_MeshStreamLead', "0");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_MeshStreamLead', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_MeshStreamLead'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_MeshStreamLead'));
  values.PushBackFlashObject(update);
  // URTX_GenericStreamLead (V5.2.0 expanded presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_GenericStreamLead', "0");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_GenericStreamLead', "0");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_GenericStreamLead', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_GenericStreamLead'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_GenericStreamLead'));
  values.PushBackFlashObject(update);
  // URTX_GCDiscardThreshold (V5.2.0 expanded presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_GCDiscardThreshold', "0");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_GCDiscardThreshold', "0");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_GCDiscardThreshold', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_GCDiscardThreshold'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_GCDiscardThreshold'));
  values.PushBackFlashObject(update);
  // URTX_GCIdleFrames (V5.2.0 expanded presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_GCIdleFrames', "0");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_GCIdleFrames', "0");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_GCIdleFrames', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_GCIdleFrames'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_GCIdleFrames'));
  values.PushBackFlashObject(update);
  // URTX_RTASDefrag (V5.2.0 expanded presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTASDefrag', "0");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTASDefrag', "0");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTASDefrag', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_RTASDefrag'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_RTASDefrag'));
  values.PushBackFlashObject(update);
  // URTX_RTASBatch (V5.2.0 expanded presets)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTASBatch', "0");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTASBatch', "0");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTASBatch', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_RTASBatch'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_RTASBatch'));
  values.PushBackFlashObject(update);
  // URTX_PTRadianceCacheDensity (V5.3.0 final presets: 8 / 6 / Native)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_PTRadianceCacheDensity', "3");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_PTRadianceCacheDensity', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_PTRadianceCacheDensity', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_PTRadianceCacheDensity'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_PTRadianceCacheDensity'));
  values.PushBackFlashObject(update);
  // URTX_RTAsyncAO (V5.3.0 final presets: On / On / Native)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTAsyncAO', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTAsyncAO', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTAsyncAO', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_RTAsyncAO'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_RTAsyncAO'));
  values.PushBackFlashObject(update);
  // URTX_RTAsyncGIShading (V5.3.0 final presets: On / On / Native)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTAsyncGIShading', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTAsyncGIShading', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTAsyncGIShading', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_RTAsyncGIShading'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_RTAsyncGIShading'));
  values.PushBackFlashObject(update);
  // URTX_RTTreeWindDistance (V5.3.0 final presets: 10 m / 15 m / Native)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTTreeWindDistance', "2");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTTreeWindDistance', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTTreeWindDistance', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_RTTreeWindDistance'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_RTTreeWindDistance'));
  values.PushBackFlashObject(update);
  // URTX_PTDiffuseCoverage (V5.3.0 final presets, owner's table 2026-10-30: 50% / 50% / Native; applies at the next game start)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_PTDiffuseCoverage', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_PTDiffuseCoverage', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_PTDiffuseCoverage', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_PTDiffuseCoverage'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_PTDiffuseCoverage'));
  values.PushBackFlashObject(update);
  // URTX_ExpPTSunShadowCoverage (V5.3.0 final presets, owner's table 2026-10-30: 50% / 50% / Native; applies at the next game start)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_ExpPTSunShadowCoverage', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_ExpPTSunShadowCoverage', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_ExpPTSunShadowCoverage', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_ExpPTSunShadowCoverage'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_ExpPTSunShadowCoverage'));
  values.PushBackFlashObject(update);
  // URTX_ExpPTLocalShadowCoverage (V5.3.0 final presets, owner's table 2026-10-30: 50% / 50% / Native; applies at the next game start)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_ExpPTLocalShadowCoverage', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_ExpPTLocalShadowCoverage', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_ExpPTLocalShadowCoverage', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_ExpPTLocalShadowCoverage'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_ExpPTLocalShadowCoverage'));
  values.PushBackFlashObject(update);
  // URTX_PTMoonLight (V5.3.0 final presets, owner's table 2026-10-30: Native in all three)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_PTMoonLight', "0");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_PTMoonLight', "0");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_PTMoonLight', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_PTMoonLight'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_PTMoonLight'));
  values.PushBackFlashObject(update);
  // URTX_RTAsyncGIClassify (V5.3.0 final presets, owner's table 2026-10-30: On / On / Native; applies at the next game start)
  if (selected == "1") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTAsyncGIClassify', "1");
  } else if (selected == "2") {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTAsyncGIClassify', "1");
  } else {
    mInGameConfigWrapper.SetVarValue('URTExtended', 'URTX_RTAsyncGIClassify', "0");
  }
  update = m_flashValueStorage.CreateTempFlashObject();
  update.SetMemberFlashUInt("tag", NameToFlashUInt('URTX_RTAsyncGIClassify'));
  update.SetMemberFlashString("current", mInGameConfigWrapper.GetVarValue('URTExtended', 'URTX_RTAsyncGIClassify'));
  values.PushBackFlashObject(update);
  // Single refresh of the complete preset transaction. All OPTIONS rows
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
    rawRTAO = "0";
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
// V5.2.0: hidden-storage (URTExtended) rows assigned by the Quick Presets.
@addMethod(CR4IngameMenu)
function RT_CR01_IsPresetURTX(optionName: name): bool {
  return optionName == 'URTX_PTAOQuality'
      || optionName == 'URTX_RTAOScreenSpace'
      || optionName == 'URTX_RTHBIL'
      || optionName == 'URTX_RTAOPretrace'
      || optionName == 'URTX_RTReflScreenSpace'
      || optionName == 'URTX_RTReflFog'
      || optionName == 'URTX_TextureStreams'
      || optionName == 'URTX_TerrainPreload'
      || optionName == 'URTX_FGMenuResources'
      || optionName == 'URTX_RTGIAlphaTest'
      || optionName == 'URTX_TexUnloadSpeed'
      || optionName == 'URTX_MeshStreamLead'
      || optionName == 'URTX_GenericStreamLead'
      || optionName == 'URTX_GCDiscardThreshold'
      || optionName == 'URTX_GCIdleFrames'
      || optionName == 'URTX_RTASDefrag'
      || optionName == 'URTX_RTASBatch'
      || optionName == 'URTX_PTRadianceCacheDensity'
      || optionName == 'URTX_RTAsyncAO'
      || optionName == 'URTX_RTAsyncGIShading'
      || optionName == 'URTX_RTTreeWindDistance'
      || optionName == 'URTX_PTDiffuseCoverage'
      || optionName == 'URTX_ExpPTSunShadowCoverage'
      || optionName == 'URTX_ExpPTLocalShadowCoverage'
      || optionName == 'URTX_PTMoonLight'
      || optionName == 'URTX_RTAsyncGIClassify';
}

@addMethod(CR4IngameMenu)
function RT_CR01_IsOwnedOption(optionName: name): bool {
  if (optionName == 'MasterEnabled'
      || optionName == 'QuickPresetSelect'
      || optionName == 'CPUFastBoneReads'
      || optionName == 'CPUDistantTrees'
      || optionName == 'CPUPhysicsLOD'
      || optionName == 'PTBillboards'
      || optionName == 'PTShadows'
      || optionName == 'PTSceneRange'
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
      || optionName == 'RTQualityGlobalShadingLimit'
      || optionName == 'RTQualityGrid1ShadingLimit'
      || optionName == 'RTPerfClassificationPrimary'
      || optionName == 'Virtual_RTPerfClassificationSecondary'
      || optionName == 'RTPerfMaxProbes'
      || optionName == 'RTPerfAOQuality'
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
        || optionName == 'RayTracedOptimizations') {
      // The wrapped handler has already written the latest config values.
      rawMaster = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'MasterEnabled');
      rawRT = mInGameConfigWrapper.GetVarValue('RefinedTracingCR01', 'RayTracedOptimizations');
      rawRTAO = "0";
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

  // The selected 7 new controls use hidden URTExtended storage; native save once
  // after the user's menu change, without changing a Quick Preset or vanilla key.
  if (StrLeft(NameToString(optionName), 5) == "URTX_") {
    // V5.2.0: a manual edit of a preset-controlled row makes the configuration custom (Quick Preset Off),
    // exactly like the RefinedTracingCR01 rows above; every other value is kept.
    if (this.RT_CR01_IsPresetURTX(optionName)) {
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
    theGame.SaveUserSettings();
  }

  return result;
}
