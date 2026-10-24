#include "cr01_menu_bridge.h"
#include "cr01_settings_codec.h"
#include <stdint.h>
static const char* const names[CR01_FEATURE_COUNT]={
 "GeometryBounces", "HairBounces", "IndirectLight", "SunImpact", "SkyImpact",
 "AmbientOcclusion", "AOStrength", "AORadius", "AOContrast",
 "GrassDay", "GrassNight", "SpecularScreenSpace", "SpecularCheckerboard",
 "PTBillboards", "PTShadows"
};
static const char* const rtao_names[4]={"PTRTAOEnabled","PTRTAORayCount","PTRTAORadius","PTRTAOStrength"};
static int same(const char*a,const char*b){if(!a||!b)return 0;while(*a && *a==*b){a++;b++;}return !*a&&!*b;}
static int numeric(const char*s,uint32_t *out,int fixed100){
 uint32_t integer=0,fraction=0,digits=0;
 if(!s||!*s||!out)return 0;
 while(*s>='0'&&*s<='9'){
  uint32_t d=(uint32_t)(*s++-'0');
  if(integer>(UINT32_MAX-d)/10)return 0;
  integer=integer*10+d;
 }
 if(fixed100&&*s=='.'){
  s++;
  if(!(*s>='0'&&*s<='9'))return 0;
  while(*s>='0'&&*s<='9'){
   if(digits>=2)return 0;
   fraction=fraction*10u+(uint32_t)(*s++-'0');digits++;
  }
  if(digits==1)fraction*=10;
 }
 if(*s|| (fixed100 && integer>(UINT32_MAX-fraction)/100))return 0;
 *out=fixed100?integer*100+fraction:integer;return 1;
}
void cr01_menu_bridge_init(CR01MenuBridge*b){
 if(!b)return;
 cr01_native_init(&b->controller);
 b->dirty_generation=0;b->save_ack=0;
}
static int apply_value(CR01Settings*draft,const char*key,const char*value){
 uint32_t parsed=0;int feature=-1,fixed=0;
 if(!draft||!key||!value)return 0;
 if(same(key,"MasterEnabled")){
  if(same(value,"true"))parsed=1;
  else if(same(value,"false"))parsed=0;
  else if(!numeric(value,&parsed,0))return 0;
  if(parsed>1)return 0;
  draft->master=parsed;
 }else{
  for(unsigned i=0;i<4;i++)if(same(key,rtao_names[i])){
   if(!numeric(value,&parsed,0))return 0;
   if(i==0)draft->rtao.mode=parsed;else if(i==1)draft->rtao.rays=parsed;
   else if(i==2)draft->rtao.radius=parsed;else draft->rtao.strength=parsed;
   return cr01_rtao_valid(&draft->rtao);
  }
  for(unsigned i=0;i<CR01_FEATURE_COUNT;i++)if(same(key,names[i])){feature=(int)i;break;}
  if(feature<0)return 0;
  fixed=(feature==F_INDIRECT||feature==F_SUN||feature==F_SKY);
  if(!fixed&&(feature==F_AO||feature==F_SPEC_SS||feature==F_SPEC_CHECKER||
      feature==F_BILLBOARDS||feature==F_SHADOWS)&&
      (same(value,"true")||same(value,"false")))
    parsed=same(value,"true")?1u:0u;
  else if(!numeric(value,&parsed,fixed))return 0;
  draft->v[feature]=parsed;
 }
 return 1;
}
static int draft_for_value(const CR01MenuBridge*b,const char*key,const char*value,CR01Settings*draft){
 if(!b||!b->controller.initialized||!draft)return 0;
 *draft=b->controller.saved;
 return apply_value(draft,key,value)&&cr01_native_validate(draft);
}
int cr01_menu_bridge_valid_input(const CR01MenuBridge*b,const char*key,const char*value){
 CR01Settings draft;return draft_for_value(b,key,value,&draft);
}
int cr01_menu_bridge_value_matches_saved(const CR01MenuBridge*b,const char*key,const char*value){
 CR01Settings draft;
 if(!draft_for_value(b,key,value,&draft))return 0;
 if(draft.master!=b->controller.saved.master)return 0;
 for(unsigned i=0;i<CR01_FEATURE_COUNT;i++)
  if(draft.v[i]!=b->controller.saved.v[i])return 0;
 return draft.rtao.mode==b->controller.saved.rtao.mode&&
  draft.rtao.rays==b->controller.saved.rtao.rays&&
  draft.rtao.radius==b->controller.saved.rtao.radius&&
  draft.rtao.strength==b->controller.saved.rtao.strength;
}
/* Formats exactly the saved/default value for CR01-owned dx12user.settings
   keys. No CRT dependency and no changes to settings in another namespace. */
static int decimal(char*out,size_t cap,uint32_t n){
 char rev[10];unsigned d=0,j=0;
 do{rev[d++]=(char)('0'+n%10u);n/=10u;}while(n&&d<10);
 if(n||cap<=d)return 0;
 while(d)out[j++]=rev[--d];out[j]=0;return (int)j;
}
int cr01_menu_bridge_saved_text(const CR01MenuBridge*b,const char*key,char*out,size_t cap){
 uint32_t v=0;int fixed=0,found=0;
 if(!b||!b->controller.initialized||!key||!out||cap<6)return 0;
 if(same(key,"MasterEnabled")){v=b->controller.saved.master;found=1;}
 else for(unsigned i=0;i<CR01_FEATURE_COUNT;i++)if(same(key,names[i])){
  v=b->controller.saved.v[i];fixed=(i==F_INDIRECT||i==F_SUN||i==F_SKY);
  found=1;break;
 }
 if(!found){
  for(unsigned i=0;i<4;i++)if(same(key,rtao_names[i])){
   v=i==0?b->controller.saved.rtao.mode:i==1?b->controller.saved.rtao.rays:
     i==2?b->controller.saved.rtao.radius:b->controller.saved.rtao.strength;
   found=1;break;
  }
 }
 if(!found)return 0;
 if(fixed){
  int n=decimal(out,cap,v/100u);
  if(!n||(size_t)n+3>=cap)return 0;
  out[n]='.';out[n+1]=(char)('0'+(v%100u)/10u);
  out[n+2]=(char)('0'+v%10u);out[n+3]=0;
 }else if(!decimal(out,cap,v))return 0;
 return cr01_menu_bridge_valid_input(b,key,out);
}
int cr01_menu_bridge_event(CR01MenuBridge*b,const char*key,const char*value){
 CR01Settings draft;
 if(!draft_for_value(b,key,value,&draft))return 0;
 /* Do not turn duplicate file notifications into persistent writes. */
 if(draft.master==b->controller.saved.master){
  unsigned unchanged=1;
  for(unsigned i=0;i<CR01_FEATURE_COUNT;i++)
   if(draft.v[i]!=b->controller.saved.v[i]){unchanged=0;break;}
  if(unchanged&&draft.rtao.mode==b->controller.saved.rtao.mode&&
    draft.rtao.rays==b->controller.saved.rtao.rays&&
    draft.rtao.radius==b->controller.saved.rtao.radius&&
    draft.rtao.strength==b->controller.saved.rtao.strength)return 1;
 }
 if(!cr01_native_set_saved(&b->controller,&draft))return 0;
 b->dirty_generation++;
 return cr01_native_event(&b->controller,b->controller.mode)?1:
  (b->controller.pending_mask?2:0);
}

static int cr01_menu_bridge_commit_snapshot(CR01MenuBridge*b,const char*const*keys,const char*const*values,const unsigned char*present,unsigned count){
 CR01Settings draft;
 if(!b||!b->controller.initialized||!keys||!values||!count)return 0;
 draft=b->controller.saved;
 for(unsigned i=0;i<count;i++){
  if(present&&!present[i])continue;
  if(!values[i]||!apply_value(&draft,keys[i],values[i]))return 0;
 }
 if(!cr01_native_validate(&draft))return 0;
 if(draft.master==b->controller.saved.master){
  unsigned unchanged=1;
  for(unsigned i=0;i<CR01_FEATURE_COUNT;i++)
   if(draft.v[i]!=b->controller.saved.v[i]){unchanged=0;break;}
  if(unchanged&&draft.rtao.mode==b->controller.saved.rtao.mode&&
    draft.rtao.rays==b->controller.saved.rtao.rays&&
    draft.rtao.radius==b->controller.saved.rtao.radius&&
    draft.rtao.strength==b->controller.saved.rtao.strength)return 1;
 }
 if(!cr01_native_set_saved(&b->controller,&draft))return 0;
 b->dirty_generation++;
 return cr01_native_event(&b->controller,b->controller.mode)?1:
  (b->controller.pending_mask?2:0);
}
int cr01_menu_bridge_snapshot_partial(CR01MenuBridge*b,const char*const*keys,const char*const*values,const unsigned char*present,unsigned count){
 if(!present)return 0;
 return cr01_menu_bridge_commit_snapshot(b,keys,values,present,count);
}
int cr01_menu_bridge_snapshot(CR01MenuBridge*b,const char*const*keys,const char*const*values,unsigned count){
 return cr01_menu_bridge_commit_snapshot(b,keys,values,0,count);
}
int cr01_menu_bridge_mode(CR01MenuBridge*b,CR01RenderMode mode){
 return b&&cr01_native_event(&b->controller,mode);
}
int cr01_menu_bridge_load(CR01MenuBridge*b,const char*s,size_t n){
 CR01Settings parsed;
 if(!b||!b->controller.initialized||!cr01_settings_parse(s,n,&parsed))return 0;
 if(!cr01_native_set_saved(&b->controller,&parsed))return 0;
 b->dirty_generation++;return cr01_native_event(&b->controller,b->controller.mode)?1:
  (b->controller.pending_mask?2:0);
}
int cr01_menu_bridge_save(const CR01MenuBridge*b,char*out,size_t cap,size_t*size){
 return b&&cr01_settings_serialize(&b->controller.saved,out,cap,size);
}
int cr01_menu_bridge_should_disable(const CR01MenuBridge*b,unsigned feature){
 return !b||!b->controller.initialized||feature>=CR01_FEATURE_COUNT||!b->controller.saved.master;
}
