/* V5.1 Normal RT performance controls: pure-logic tests (mappings, CAS owner,
 * RTGI Performance console probe grid state machine). */
#include "../../src/urt_rtperf_core.h"
#include <stdio.h>
#include <string.h>
static int fails;
#define CHECK(c,m) do{ if(c) printf("PASS %s\n",m); else {printf("FAIL %s\n",m); fails++;} }while(0)
static uint32_t fb(float f){uint32_t u;memcpy(&u,&f,4);return u;}

static UrtRpPrefs P(uint32_t cg,uint32_t rl,uint32_t ss,uint32_t sf){
  (void)cg; UrtRpPrefs p={{rl,ss,sf}};return p;   /* rc3: console grid removed */
}
int main(void){
  /* ---- preferences (rc2: four controls) ---- */
  UrtRpPrefs nat=P(0,0,0,0);
  CHECK(URT_RP_COUNT==3&&URT_RF_COUNT==3,"rc3: exactly 3 controls / 3 owned engine fields");
  CHECK(urt_rp_prefs_valid(&nat),"all-Native preferences valid");
  UrtRpPrefs bad;
  bad=P(0,2,0,0); CHECK(!urt_rp_prefs_valid(&bad),"out-of-range local lights rejected");
  bad=P(0,0,5,0); CHECK(!urt_rp_prefs_valid(&bad),"out-of-range shadow slots rejected");
  bad=P(0,0,0,4); CHECK(!urt_rp_prefs_valid(&bad),"out-of-range foliage detail rejected");
  int removed=1; for(unsigned i=0;i<URT_RP_COUNT;i++){
    if(!strcmp(urt_rp_keys[i],"RTGIProbeRayDistance")||!strcmp(urt_rp_keys[i],"RTReflTextureDetail")||!strcmp(urt_rp_keys[i],"RTAOFoliageDetail")||
       !strcmp(urt_rp_keys[i],"RTGIConsoleGrid"))removed=0;}
  CHECK(removed,"removed controls are not read at all");

  /* ---- mappings ---- */
  uint32_t t;
  CHECK(!urt_rf_target(&nat,URT_RF_REFL_LOCAL,1u,&t)&&!urt_rf_target(&nat,URT_RF_SHADOW_SLOTS,8u,&t)&&
        !urt_rf_target(&nat,URT_RF_SHADOW_ALPHA_MIP,fb(2.0f),&t),"Native writes nothing");
  /* Quick Preset values: Performance (1,1,4,3), Balanced (1,1,2,2) */
  UrtRpPrefs perf=P(1,1,4,3), bal=P(1,1,2,2);
  CHECK(urt_rf_target(&perf,URT_RF_REFL_LOCAL,1u,&t)&&t==0u,"Performance: reflection local lights off");
  CHECK(urt_rf_target(&perf,URT_RF_SHADOW_SLOTS,8u,&t)&&t==0u,"Performance: local light shadows Off = 0 (engine-supported)");
  CHECK(urt_rf_target(&perf,URT_RF_SHADOW_ALPHA_MIP,fb(2.0f),&t)&&t==fb(6.0f),"Performance: foliage Very Low = mip 6");
  CHECK(urt_rf_target(&bal,URT_RF_SHADOW_SLOTS,8u,&t)&&t==4u,"Balanced: local light shadows 4");
  CHECK(urt_rf_target(&bal,URT_RF_SHADOW_ALPHA_MIP,fb(2.0f),&t)&&t==fb(4.0f),"Balanced: foliage Low = mip 4");
  UrtRpPrefs red=P(0,0,1,1);
  CHECK(urt_rf_target(&red,URT_RF_SHADOW_SLOTS,8u,&t)&&t==6u,"6 local light shadows");
  CHECK(urt_rf_target(&red,URT_RF_SHADOW_ALPHA_MIP,fb(2.0f),&t)&&t==fb(3.0f),"foliage Reduced = mip 3");
  CHECK(urt_rf_target(&perf,URT_RF_REFL_LOCAL,0x00000001u,&t)&&t==0u,"bool byte only");
  CHECK(!urt_rf_target(&perf,URT_RF_REFL_LOCAL,0u,&t),"already off natively: no ownership");
  CHECK(!urt_rf_target(&perf,URT_RF_REFL_LOCAL,0x101u,&t),"non-bool state refused");
  CHECK(!urt_rf_target(&bal,URT_RF_SHADOW_SLOTS,2u,&t),"never more shadowed lights than native");
  CHECK(!urt_rf_target(&bal,URT_RF_SHADOW_ALPHA_MIP,fb(7.0f),&t),"never sharper than native mip");
  CHECK(!urt_rf_target(&perf,URT_RF_SHADOW_ALPHA_MIP,0u,&t),"uninitialised (0) mip refused");
  CHECK(!urt_rf_target(&perf,URT_RF_SHADOW_ALPHA_MIP,0x7fc00000u,&t),"NaN native refused");
  CHECK(!urt_rf_target(&perf,URT_RF_SHADOW_SLOTS,1000u,&t),"implausible native refused");

  /* ---- owner ---- */
  uint32_t mem[URT_RF_COUNT]={1u,8u,fb(2.0f)};
  uint32_t native[URT_RF_COUNT];memcpy(native,mem,sizeof mem);
  UrtRfOwner o;urt_rf_init(&o);for(unsigned i=0;i<URT_RF_COUNT;i++)o.slot[i]=&mem[i];
  urt_rf_update(&o,&nat,1);
  CHECK(!memcmp(mem,native,sizeof mem)&&o.mask==0,"Native + eligible: memory untouched");
  urt_rf_update(&o,&perf,0);
  CHECK(!memcmp(mem,native,sizeof mem)&&o.mask==0,"not eligible (PT/raster/Master Off): nothing applied");
  urt_rf_update(&o,&perf,1);
  CHECK(mem[0]==0u&&mem[1]==0u&&mem[2]==fb(6.0f),"Performance preset values applied");
  urt_rf_update(&o,&perf,1);
  CHECK(mem[1]==0u&&o.conflicts==0,"steady state: idempotent (no repeated writes)");
  urt_rf_update(&o,&bal,1);
  CHECK(mem[0]==0u&&mem[1]==4u&&mem[2]==fb(4.0f),"switch preset Performance -> Balanced live");
  UrtRpPrefs manual=P(1,1,1,2);                       /* user edits one row after Balanced */
  urt_rf_update(&o,&manual,1);
  CHECK(mem[0]==0u&&mem[1]==6u&&mem[2]==fb(4.0f),"manual edit changes only that row; others kept");
  urt_rf_update(&o,&manual,0);
  CHECK(!memcmp(mem,native,sizeof mem)&&o.mask==0,"leaving Normal RT restores every native value exactly");
  urt_rf_update(&o,&manual,1); mem[1]=7u;
  urt_rf_update(&o,&manual,1);
  CHECK(mem[1]==7u&&!(o.mask&(1u<<URT_RF_SHADOW_SLOTS))&&o.conflicts==1,"foreign write is never clobbered");
  urt_rf_update(&o,&manual,0);
  CHECK(mem[1]==7u&&mem[0]==1u,"release keeps the foreign value, restores ours");
  mem[1]=8u;
  UrtRpPrefs ext=P(0,0,0,0);                          /* Extreme preset = all Native */
  urt_rf_update(&o,&perf,1);urt_rf_update(&o,&ext,1);
  CHECK(!memcmp(mem,native,sizeof mem)&&o.mask==0,"Extreme (all Native): native values restored, nothing owned");
  urt_rf_update(&o,&perf,1);urt_rf_release(&o);
  CHECK(!memcmp(mem,native,sizeof mem),"shutdown release restores native");

  printf("TOTAL %s %d\n",fails?"FAIL":"PASS",fails);
  return fails?1:0;
}
