#include "cr01_settings_store.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
typedef struct {char b[512];size_t n;int commits;int fail; } FakeStore;
static int rd(void*ctx,char*buf,size_t cap,size_t*count){
 FakeStore *f=(FakeStore*)ctx;
 if(!f->n)return 0;
 if(f->n>cap)return -1;
 memcpy(buf,f->b,f->n);*count=f->n;return 1;
}
static int wr(void*ctx,const char*buf,size_t n){
 FakeStore*f=(FakeStore*)ctx;
 if(f->fail||n>sizeof(f->b))return 0;
 memcpy(f->b,buf,n);f->n=n;f->commits++;return 1;
}
int main(void){
 FakeStore m={0};CR01PrivateStore io={rd,wr,&m};CR01MenuBridge a,b;
 cr01_menu_bridge_init(&a);assert(cr01_store_load(&a,&io));
 assert(a.controller.saved.master==0&&a.controller.saved.v[F_INDIRECT]==100);
 assert(cr01_menu_bridge_event(&a,"MasterEnabled","1"));
 assert(cr01_menu_bridge_event(&a,"SunImpact","1.25"));
 m.fail=1;assert(!cr01_store_flush(&a,&io));assert(m.commits==0);
 m.fail=0;assert(cr01_store_flush(&a,&io));assert(m.commits==1);
 assert(cr01_store_flush(&a,&io));assert(m.commits==1);
 cr01_menu_bridge_init(&b);assert(cr01_store_load(&b,&io));
 assert(b.controller.saved.master==1&&b.controller.saved.v[F_SUN]==125);
 /* Persist Master Off without erasing the remembered Sun preference. */
 assert(cr01_menu_bridge_event(&b,"MasterEnabled","0"));
 assert(cr01_store_flush(&b,&io));
 CR01MenuBridge c;cr01_menu_bridge_init(&c);assert(cr01_store_load(&c,&io));
 assert(c.controller.saved.master==0&&c.controller.saved.v[F_SUN]==125);
 /* And persist On again with the same preference. */
 assert(cr01_menu_bridge_event(&c,"MasterEnabled","1"));
 assert(cr01_store_flush(&c,&io));
 CR01MenuBridge d;cr01_menu_bridge_init(&d);assert(cr01_store_load(&d,&io));
 assert(d.controller.saved.master==1&&d.controller.saved.v[F_SUN]==125);
 m.b[0]='X';assert(!cr01_store_load(&a,&io));
 assert(a.controller.saved.master==1&&a.controller.saved.v[F_SUN]==125);
 puts("PASS private settings: missing defaults, atomic commit contract, reload, corrupt refusal");
 return 0;
}
