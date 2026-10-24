#include "cr01_menu_bridge.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t fb(float v){ union { float f; uint32_t u; } x; x.f=v; return x.u; }

int main(void){
  CR01MenuBridge b;
  uint32_t slots[CR01_FEATURE_COUNT]={0};
  const uint32_t native[CR01_FEATURE_COUNT]={
    0,0,0,fb(1.0f),fb(1.0f),0,fb(1.0f),fb(1.0f),fb(1.0f),
    fb(1.0f),fb(1.0f),1,0,1,0
  };
  const char *keys[CR01_FEATURE_COUNT]={
    "GeometryBounces","HairBounces","IndirectLight","SunImpact","SkyImpact",
    "AmbientOcclusion","AOStrength","AORadius","AOContrast","GrassDay",
    "GrassNight","SpecularScreenSpace","SpecularCheckerboard","PTBillboards","PTShadows"
  };
  const char *values[CR01_FEATURE_COUNT]={
    "6","5","1.35","1.25","0.90","1","5","6","6","4","2","2","1","2","1"
  };

  cr01_menu_bridge_init(&b);
  for(unsigned f=0;f<CR01_FEATURE_COUNT;f++) slots[f]=native[f];
  for(unsigned f=0;f<CR01_FEATURE_COUNT;f++)
    if(f!=F_GEOM && f!=F_HAIR && f!=F_INDIRECT && f!=F_SHADOWS)
      cr01_native_slot(&b.controller,f,&slots[f]);

  /* Preferences are editable and durable while Master is Off and renderer is
     ineligible. No PT CVar is written by those edits. */
  assert(cr01_menu_bridge_mode(&b,CR01_RENDER_NORMAL_RT));
  for(unsigned f=0;f<CR01_FEATURE_COUNT;f++){
    assert(cr01_menu_bridge_event(&b,keys[f],values[f]));
    assert(cr01_menu_bridge_value_matches_saved(&b,keys[f],values[f]));
  }
  for(unsigned f=0;f<CR01_FEATURE_COUNT;f++) assert(slots[f]==native[f]);
  assert(b.controller.saved.master==0);

  /* Off -> On while still Normal RT keeps every PT carrier dormant. */
  assert(cr01_menu_bridge_event(&b,"MasterEnabled","1"));
  assert(b.controller.saved.master==1);
  assert(cr01_menu_bridge_should_disable(&b,F_AO)==0);
  for(unsigned f=0;f<CR01_FEATURE_COUNT;f++) assert(slots[f]==native[f]);

  /* Enter PT: all eleven generic CVar carriers apply, while the three
     CameraCB controls and PT shadow remain explicitly pending for their
     independent carriers instead of being falsely counted as CVar-applied. */
  assert(!cr01_menu_bridge_mode(&b,CR01_RENDER_PT));
  const uint32_t cvar_mask=(1u<<F_SUN)|(1u<<F_SKY)|(1u<<F_AO)|(1u<<F_AO_STRENGTH)|
    (1u<<F_AO_RADIUS)|(1u<<F_AO_CONTRAST)|(1u<<F_GRASS_DAY)|(1u<<F_GRASS_NIGHT)|
    (1u<<F_SPEC_SS)|(1u<<F_SPEC_CHECKER)|(1u<<F_BILLBOARDS);
  const uint32_t special_mask=(1u<<F_GEOM)|(1u<<F_HAIR)|(1u<<F_INDIRECT)|(1u<<F_SHADOWS);
  assert(b.controller.active_mask==cvar_mask);
  assert(b.controller.pending_mask==special_mask);
  assert(slots[F_SUN]==fb(1.25f));
  assert(slots[F_SKY]==fb(.90f));
  assert(slots[F_AO]==1);
  assert(slots[F_AO_STRENGTH]==fb(2.0f));
  assert(slots[F_AO_RADIUS]==fb(4.0f));
  assert(slots[F_AO_CONTRAST]==fb(3.0f));
  assert(slots[F_GRASS_DAY]==fb(1.5f));
  assert(slots[F_GRASS_NIGHT]==fb(.75f));
  assert(slots[F_SPEC_SS]==0);
  assert(slots[F_SPEC_CHECKER]==1);
  assert(slots[F_BILLBOARDS]==0);

  /* PT Master On -> Off restores the exact captured native values without
     touching SavedPreferences. */
  assert(cr01_menu_bridge_event(&b,"MasterEnabled","0"));
  assert(cr01_menu_bridge_should_disable(&b,F_AO));
  for(unsigned f=0;f<CR01_FEATURE_COUNT;f++) assert(slots[f]==native[f]);
  assert(b.controller.saved.v[F_GEOM]==6);
  assert(b.controller.saved.v[F_HAIR]==5);
  assert(b.controller.saved.v[F_INDIRECT]==135);
  assert(b.controller.saved.v[F_SHADOWS]==1);

  /* Off -> On again restores the same saved selections, not defaults. */
  assert(cr01_menu_bridge_event(&b,"MasterEnabled","1")==2);
  assert(b.controller.active_mask==cvar_mask);
  assert(b.controller.pending_mask==special_mask);
  assert(slots[F_SUN]==fb(1.25f) && slots[F_BILLBOARDS]==0);

  /* Leaving PT releases all generic owners. Returning to PT reapplies them. */
  assert(cr01_menu_bridge_mode(&b,CR01_RENDER_RASTER));
  for(unsigned f=0;f<CR01_FEATURE_COUNT;f++) assert(slots[f]==native[f]);
  assert(!cr01_menu_bridge_mode(&b,CR01_RENDER_PT));
  assert(b.controller.active_mask==cvar_mask && b.controller.pending_mask==special_mask);

  /* A duplicate delivered value is accepted but cannot generate a new dirty
     preference revision. This is the runtime's exact-event de-duplication. */
  uint32_t dirty=b.dirty_generation;
  assert(cr01_menu_bridge_event(&b,"SunImpact","1.25"));
  assert(b.dirty_generation==dirty);
  assert(!cr01_menu_bridge_value_matches_saved(&b,"SunImpact","1.30"));

  assert(cr01_menu_bridge_event(&b,"MasterEnabled","0"));
  assert(cr01_native_shutdown(&b.controller));
  puts("PASS LIFECYCLE01: 15 preferences, Off-On-Off-On, native restore, mode isolation, exact duplicate suppression");
  return 0;
}
