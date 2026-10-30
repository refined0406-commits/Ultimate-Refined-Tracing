#include "cr01_menu_bridge.h"
#include "cr01_settings_codec.h"
#include "cr01_rtao_config.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(void) {
 CR01MenuBridge a,b;uint32_t o;char blob[512], old[512];size_t n,oldn;int migrated;
 cr01_menu_bridge_init(&a);assert(a.controller.saved.rtao.mode==0);
 assert(cr01_menu_bridge_event(&a,"PTRTAOEnabled","1"));
 assert(cr01_menu_bridge_event(&a,"PTRTAORayCount","3"));
 assert(cr01_menu_bridge_event(&a,"PTRTAORadius","6"));
 assert(cr01_menu_bridge_event(&a,"PTRTAOStrength","5"));
 assert(a.controller.saved.rtao.mode==1 && a.controller.saved.rtao.rays==3 && a.controller.saved.rtao.radius==6 && a.controller.saved.rtao.strength==5);
 assert(!cr01_menu_bridge_event(&a,"PTRTAORayCount","4"));
 assert(!cr01_menu_bridge_event(&a,"PTRTAORadius","7"));
 assert(!cr01_menu_bridge_event(&a,"PTRTAOStrength","6"));
 assert(!cr01_menu_bridge_event(&a,"PTRTAOEnabled","3"));
 assert(cr01_menu_bridge_save(&a,blob,sizeof(blob),&n));
 assert(strncmp(blob,"CR01_SETTINGS_V2\n",17)==0);
 cr01_menu_bridge_init(&b);assert(cr01_menu_bridge_load(&b,blob,n));
 assert(b.controller.saved.rtao.mode==1&&b.controller.saved.rtao.rays==3&&b.controller.saved.rtao.radius==6&&b.controller.saved.rtao.strength==5);
 o=cr01_rtao_apply(25602,&b.controller.saved.rtao,1,1,1,1,1);assert((o&(1u<<24))!=0 && (o&0xffff)==25602);
 assert(cr01_rtao_apply(25602,&b.controller.saved.rtao,1,1,1,2,1)==25602);
 assert(cr01_menu_bridge_event(&b,"PTRTAOEnabled","0"));
 assert(cr01_rtao_apply(25602,&b.controller.saved.rtao,1,1,1,1,1)==25602);
 assert(cr01_menu_bridge_event(&b,"PTRTAOEnabled","2"));
 assert(cr01_rtao_apply(25602,&b.controller.saved.rtao,1,1,1,1,1)==25602);
 /* Convert a genuine V2 blob to the historical V1 shape and ensure lossless
  * migration for all original C17 values, with new defaults OFF. */
 memcpy(old,blob,n); old[n]=0;
 assert(strstr(old,"CR01_SETTINGS_V2\n")==old);
 memcpy(old+14,"V1",2); /* header length identical; only V2 -> V1 */
 char *start=strstr(old,"RTAO0=");assert(start);
 char *finish=strstr(start,"END\n");assert(finish);
 memmove(start,finish,strlen(finish)+1);oldn=strlen(old);
 CR01Settings x;assert(cr01_settings_parse_migrated(old,oldn,&x,&migrated));assert(migrated==1);
 assert(x.rtao.mode==0&&x.rtao.rays==0&&x.rtao.radius==0&&x.rtao.strength==0);
 assert(cr01_menu_bridge_load(&b,old,oldn));
 assert(b.controller.saved.rtao.mode==0);
 puts("PASS RTAO four-field valid/invalid menu values, V2 persistence, V1 migration, epoch isolation, Native Auto/Off strict neutral");
 return 0;
}
