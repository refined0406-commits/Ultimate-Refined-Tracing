#include "cr01_menu_bridge.h"
#include "cr01_settings_codec.h"
#include "cr01_rtao_config.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(void){
 CR01MenuBridge a,b;char blob[512];size_t n;
 cr01_menu_bridge_init(&a);
 assert(!cr01_menu_bridge_event(&a,"PTRTAOEnabled","1"));
 assert(!cr01_menu_bridge_event(&a,"PTRTAORayCount","3"));
 assert(!cr01_menu_bridge_event(&a,"PTRTAORadius","4"));
 assert(!cr01_menu_bridge_event(&a,"PTRTAOStrength","5"));
 assert(cr01_menu_bridge_event(&a,"AmbientOcclusion","1"));
 assert(cr01_menu_bridge_save(&a,blob,sizeof blob,&n));
 /* Legacy RTAO values in a V2 preference store must be ignored in RAM and never
    reactivated by a newly installed V5.2. Do not rewrite the old file on read. */
 char legacy[512];memcpy(legacy,blob,n+1);
 const char *fields[]={"RTAO0=","RTAO1=","RTAO2=","RTAO3="};
 for(int i=0;i<4;i++){char *p=strstr(legacy,fields[i]);assert(p);p+=strlen(fields[i]);assert(*p=='0');*p='1';}
 cr01_menu_bridge_init(&b);assert(cr01_menu_bridge_load(&b,legacy,strlen(legacy)));
 assert(!b.controller.saved.rtao.mode&&!b.controller.saved.rtao.rays&&!b.controller.saved.rtao.radius&&!b.controller.saved.rtao.strength);
 assert(cr01_rtao_apply(25602,&b.controller.saved.rtao,1,1,1,1,1)==25602);
 assert(cr01_menu_bridge_event(&b,"AOStrength","2"));
 assert(!b.controller.saved.rtao.mode);
 puts("PASS V5.2 four retired RTAO controls inert, V2 legacy data neutralized, remaining AO operates");
 return 0;
}
