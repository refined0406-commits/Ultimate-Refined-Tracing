#include "cr01_menu_bridge.h"
#include "cr01_settings_store.h"
#include "cr01_settings_codec.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct {char data[512];size_t size;int saved;int fail;} M;
static int rd(void*p,char*buf,size_t cap,size_t*n){M*m=(M*)p;if(!m->size)return 0;if(m->size>cap)return -1;memcpy(buf,m->data,m->size);*n=m->size;return 1;}
static int wr(void*p,const char*buf,size_t n){M*m=(M*)p;if(m->fail||n>sizeof(m->data))return 0;memcpy(m->data,buf,n);m->size=n;m->saved++;return 1;}
int main(void){
 CR01MenuBridge a,b;CR01Settings old,now;size_t n=0;char wire[512],str[32];int migrated=-1;
 M m={0};CR01PrivateStore io={rd,wr,&m};
 cr01_menu_bridge_init(&a);assert(a.controller.saved.master==0&&a.controller.saved.v[F_INDIRECT]==100);
 assert(cr01_menu_bridge_saved_text(&a,"IndirectLight",str,sizeof(str))&&strcmp(str,"1.00")==0);
 assert(cr01_menu_bridge_valid_input(&a,"IndirectLight","1.00"));
 assert(!cr01_menu_bridge_valid_input(&a,"IndirectLight","0"));
 assert(!cr01_menu_bridge_valid_input(&a,"IndirectLight",""));
 assert(!cr01_menu_bridge_valid_input(&a,"IndirectLight","garbage"));
 assert(!cr01_menu_bridge_valid_input(&a,"IndirectLight","0.99"));
 assert(!cr01_menu_bridge_valid_input(&a,"IndirectLight","1.51"));
 assert(!cr01_menu_bridge_valid_input(&a,"IndirectLight","2.55"));
 assert(cr01_menu_bridge_valid_input(&a,"IndirectLight","1.11")); /* old valid 0.01 preference */
 for(unsigned step=0;step<=10;step++){
  unsigned v=100+5*step;
  char menu[8];menu[0]='1';menu[1]='.';menu[2]=(char)('0'+(v%100)/10);menu[3]=(char)('0'+v%10);menu[4]=0;
  assert(cr01_menu_bridge_event(&a,"IndirectLight",menu));
  assert(a.controller.saved.v[F_INDIRECT]==v);
  assert(cr01_menu_bridge_saved_text(&a,"IndirectLight",str,sizeof(str)));
  assert(strcmp(str,menu)==0);
 }
 assert(a.controller.saved.v[F_INDIRECT]==150);
 assert(cr01_menu_bridge_event(&a,"SunImpact","3.00"));
 assert(cr01_menu_bridge_event(&a,"SkyImpact","0.00"));
 assert(!cr01_menu_bridge_event(&a,"IndirectLight","1.501"));
 assert(!cr01_menu_bridge_event(&a,"IndirectLight","1.51"));
 assert(a.controller.saved.v[F_INDIRECT]==150);
 cr01_defaults(&old);old.master=1;
 /* Build a valid historical V1 private-settings record without passing
  * the new 150 cap through the encoder. */
 old.v[F_INDIRECT]=150;
 assert(cr01_settings_serialize(&old,wire,sizeof(wire),&n));
 char *line=strstr(wire,"F02=150\n");assert(line);line[4]='2';line[5]='5';line[6]='5';
 assert(cr01_settings_parse_migrated(wire,n,&now,&migrated));
 assert(migrated==1&&now.v[F_INDIRECT]==150&&now.master==1);
 /* Corrupted old values remain rejected, even when migration is enabled. */
 line[4]='0';line[5]='9';line[6]='9';assert(!cr01_settings_parse_migrated(wire,n,&now,&migrated));
 line[4]='2';line[5]='5';line[6]='5';
 memcpy(m.data,wire,n);m.size=n;
 cr01_menu_bridge_init(&b);
 assert(cr01_store_load(&b,&io));
 assert(b.controller.saved.master==1&&b.controller.saved.v[F_INDIRECT]==150);
 assert(b.dirty_generation!=b.save_ack);
 m.fail=1;assert(!cr01_store_flush(&b,&io));assert(m.saved==0);
 m.fail=0;assert(cr01_store_flush(&b,&io));assert(m.saved==1);
 assert(cr01_store_flush(&b,&io));assert(m.saved==1); /* migrate once */
 assert(cr01_settings_parse_migrated(m.data,m.size,&now,&migrated)&&!migrated);
 assert(now.v[F_INDIRECT]==150);
 /* Historical within-range 1.11 survives without rounding or rewriting. */
 now.v[F_INDIRECT]=111;
 assert(cr01_settings_serialize(&now,wire,sizeof(wire),&n));
 memcpy(m.data,wire,n);m.size=n;m.saved=0;
 cr01_menu_bridge_init(&b);
 assert(cr01_store_load(&b,&io));
 assert(b.controller.saved.v[F_INDIRECT]==111);
 assert(cr01_store_flush(&b,&io));assert(m.saved==0);
 puts("PASS FIX3: 11 UI values, native 1.00, invalid zero bootstrap, old 1.11 preserved, old 2.55 clamped once, atomic reload");
 return 0;
}
