#include "cr01_menu_bridge.h"
#include "cr01_gate_live_adapter.h"
#include "cr01_shadow_policy.h"
#include "cr01_settings_codec.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t fb(float v){ union { float f; uint32_t u; } x; x.f=v; return x.u; }
static uint32_t rd32(const unsigned char*p){
 return (uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;
}
static int same_settings(const CR01Settings*a,const CR01Settings*b){
 if(a->master!=b->master)return 0;
 for(unsigned i=0;i<CR01_FEATURE_COUNT;i++)if(a->v[i]!=b->v[i])return 0;
 return 1;
}

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
 const uint32_t generic_mask=(1u<<F_SUN)|(1u<<F_SKY)|(1u<<F_AO)|
   (1u<<F_AO_STRENGTH)|(1u<<F_AO_RADIUS)|(1u<<F_AO_CONTRAST)|
   (1u<<F_GRASS_DAY)|(1u<<F_GRASS_NIGHT)|(1u<<F_SPEC_SS)|
   (1u<<F_SPEC_CHECKER)|(1u<<F_BILLBOARDS);
 const uint32_t special_mask=(1u<<F_GEOM)|(1u<<F_HAIR)|(1u<<F_INDIRECT)|(1u<<F_SHADOWS);

 cr01_menu_bridge_init(&b);
 for(unsigned f=0;f<CR01_FEATURE_COUNT;f++)slots[f]=native[f];
 for(unsigned f=0;f<CR01_FEATURE_COUNT;f++)
   if(f!=F_GEOM&&f!=F_HAIR&&f!=F_INDIRECT&&f!=F_SHADOWS)
     cr01_native_slot(&b.controller,f,&slots[f]);

 /* All 15 preferences can change while Master Off without runtime writes. */
 assert(cr01_menu_bridge_mode(&b,CR01_RENDER_NORMAL_RT));
 for(unsigned f=0;f<CR01_FEATURE_COUNT;f++)assert(cr01_menu_bridge_event(&b,keys[f],values[f]));
 for(unsigned f=0;f<CR01_FEATURE_COUNT;f++)assert(slots[f]==native[f]);

 /* Master On remains dormant outside PT. */
 assert(cr01_menu_bridge_event(&b,"MasterEnabled","1"));
 for(unsigned f=0;f<CR01_FEATURE_COUNT;f++)assert(slots[f]==native[f]);

 /* PT applies all 11 direct CVar carriers. Four special carriers are pending
    here because they are deliberately owned by independent production paths. */
 assert(!cr01_menu_bridge_mode(&b,CR01_RENDER_PT));
 assert(b.controller.active_mask==generic_mask);
 assert(b.controller.pending_mask==special_mask);
 assert(slots[F_SUN]==fb(1.25f));
 assert(slots[F_SKY]==fb(.90f));
 assert(slots[F_AO]==1u);
 assert(slots[F_AO_STRENGTH]==fb(2.0f));
 assert(slots[F_AO_RADIUS]==fb(4.0f));
 assert(slots[F_AO_CONTRAST]==fb(3.0f));
 assert(slots[F_GRASS_DAY]==fb(1.5f));
 assert(slots[F_GRASS_NIGHT]==fb(.75f));
 assert(slots[F_SPEC_SS]==0u);
 assert(slots[F_SPEC_CHECKER]==1u);
 assert(slots[F_BILLBOARDS]==0u);

 /* The real serializer gate carries Geometry, Hair and Indirect live. */
 CR01GateState gate={0};
 unsigned char scratch[0x400];
 memset(scratch,0xa5,sizeof(scratch));
 assert(cr01_gate_publish_e06(&gate,&b.controller.saved,1,1,1,11));
 /* Staged values MUST remain native until the protected custom registration
    pair is live on the render-side serializer call. */
 assert(cr01_gate_post_serialize(&gate,scratch,0)==0);
 assert(rd32(scratch+CR01_GEOM_OFFSET)==0xa5a5a5a5u);
 assert(rd32(scratch+CR01_HAIR_OFFSET)==0xa5a5a5a5u);
 assert(cr01_gate_post_serialize(&gate,scratch,1)==1);
 assert(rd32(scratch+CR01_GEOM_OFFSET)==(6u|(135u<<8)));
 assert(rd32(scratch+CR01_HAIR_OFFSET)==5u);

 /* The real PT-shadow policy preserves native, forces On and forces Off
    without ever writing the native shadow CVar itself. */
 unsigned force=99;uint32_t effective=99;
 assert(cr01_shadow_policy(&b.controller.saved,1,1u,&force,&effective));
 assert(force==0u&&effective==2u); /* saved manual On */
 CR01Settings shadow_off=b.controller.saved;
 shadow_off.v[F_SHADOWS]=2u;
 assert(cr01_shadow_policy(&shadow_off,1,1u,&force,&effective));
 assert(force==1u&&effective==0u);
 CR01Settings shadow_native=b.controller.saved;
 shadow_native.v[F_SHADOWS]=0u;
 assert(cr01_shadow_policy(&shadow_native,1,3u,&force,&effective));
 assert(force==0u&&effective==3u);

 /* Master Off restores exact captured CVar native values, leaves serializer
    output untouched, uses native shadow policy, and preserves preferences. */
 CR01Settings remembered=b.controller.saved;
 assert(cr01_menu_bridge_event(&b,"MasterEnabled","0"));
 for(unsigned f=0;f<CR01_FEATURE_COUNT;f++)assert(slots[f]==native[f]);
 memset(scratch,0xa5,sizeof(scratch));
 assert(cr01_gate_publish_e06(&gate,&b.controller.saved,1,1,1,12));
 assert(cr01_gate_post_serialize(&gate,scratch,1)==0);
 assert(rd32(scratch+CR01_GEOM_OFFSET)==0xa5a5a5a5u);
 assert(rd32(scratch+CR01_HAIR_OFFSET)==0xa5a5a5a5u);
 assert(cr01_shadow_policy(&b.controller.saved,1,1u,&force,&effective));
 assert(force==0u&&effective==1u);
 for(unsigned i=0;i<CR01_FEATURE_COUNT;i++)assert(b.controller.saved.v[i]==remembered.v[i]);

 /* Off-state persistence contains Master Off plus the exact remembered F00-F14. */
 char encoded[2048];size_t encoded_n=0;CR01Settings decoded;
 assert(cr01_settings_serialize(&b.controller.saved,encoded,sizeof(encoded),&encoded_n));
 assert(cr01_settings_parse(encoded,encoded_n,&decoded));
 assert(same_settings(&decoded,&b.controller.saved));
 assert(decoded.master==0u);
 for(unsigned i=0;i<CR01_FEATURE_COUNT;i++)assert(decoded.v[i]==remembered.v[i]);

 /* On again restores the same values. RT/raster then releases them. */
 assert(cr01_menu_bridge_event(&b,"MasterEnabled","1")==2);
 assert(b.controller.active_mask==generic_mask&&b.controller.pending_mask==special_mask);
 assert(slots[F_SUN]==fb(1.25f)&&slots[F_BILLBOARDS]==0u);
 assert(cr01_menu_bridge_mode(&b,CR01_RENDER_RASTER));
 for(unsigned f=0;f<CR01_FEATURE_COUNT;f++)assert(slots[f]==native[f]);
 memset(scratch,0xa5,sizeof(scratch));
 assert(cr01_gate_publish_e06(&gate,&b.controller.saved,0,1,1,13));
 assert(cr01_gate_post_serialize(&gate,scratch,1)==0);

 /* Untouched Master On defaults are true pass-through: no CVar ownership,
    no CameraCB overwrite and shadow Native follows the game's native input. */
 CR01Settings defaults;cr01_defaults(&defaults);defaults.master=1;
 CR01GateState neutral={0};
 memset(scratch,0xa5,sizeof(scratch));
 assert(cr01_gate_publish_e06(&neutral,&defaults,1,1,1,14));
 assert(cr01_gate_post_serialize(&neutral,scratch,1)==0);
 assert(cr01_shadow_policy(&defaults,1,2u,&force,&effective));
 assert(force==0u&&effective==2u);

 assert(cr01_native_shutdown(&b.controller));
 puts("PASS FINAL ALL15: 11 CVar carriers + Geometry/Hair/Indirect serializer + PT-shadow policy, Off-On-Off-On, persistence, PT/RT/raster isolation");
 return 0;
}
