/* V5.2 PT: Scene Range: pure logic (src/urt_ptperf_core.c). r2: Small Object Detail removed. */
#include "../../src/urt_ptperf_core.h"
#include <stdio.h>
#include <string.h>
static int fails;
#define CHECK(c,m) do{int _c=!!(c);printf("%s %s\n",_c?"PASS":"FAIL",m);if(!_c)fails++;}while(0)
static uint32_t u(float f){uint32_t x;memcpy(&x,&f,4);return x;}
static float f(uint32_t x){float v;memcpy(&v,&x,4);return v;}
int main(void){
  UrtPxPrefs p={{0}};uint32_t t;
  CHECK(URT_PX_COUNT==1&&!strcmp(urt_px_keys[0],"PTSceneRange"),"1 control (Small Object Detail removed), stable key");
  CHECK(URT_PF_COUNT==2,"only the two PT box values are ever owned");
  CHECK(urt_px_prefs_valid(&p),"Native: valid");
  for(unsigned i=0;i<URT_PF_COUNT;i++)CHECK(!urt_pf_target(&p,i,u(400.f),&t),"Native writes nothing");
  p.v[URT_PX_SCENE_RANGE]=1;CHECK(urt_pf_target(&p,URT_PF_RANGE,u(400.f),&t)&&f(t)==300.f,"range 300 m");
  CHECK(urt_pf_target(&p,URT_PF_RANGE_EXT,u(800.f),&t)&&f(t)==300.f,"extended box 300 m");
  CHECK(!urt_pf_target(&p,URT_PF_RANGE,u(250.f),&t),"never enlarges a smaller native box");
  CHECK(!urt_pf_target(&p,URT_PF_RANGE,0x7fc00000u,&t),"NaN native refused");
  p.v[URT_PX_SCENE_RANGE]=4;CHECK(urt_pf_target(&p,URT_PF_RANGE,u(400.f),&t)&&f(t)==150.f,"range 150 m");
  p.v[URT_PX_SCENE_RANGE]=5;CHECK(!urt_px_prefs_valid(&p),"range index 5 rejected");
  printf("TOTAL %s %d\n",fails?"FAIL":"PASS",fails);return fails?1:0;
}
