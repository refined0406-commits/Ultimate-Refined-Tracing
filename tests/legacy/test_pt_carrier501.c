#include "cr01_pt_carrier501.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
typedef struct {int supported,shader,camera,rollback,shadow,committed,commit_ok,release_ok,
 shadows,billboards,fail_camera;unsigned geom,hair,indirect,updates,releases;} Host;
static int build(void*v){return ((Host*)v)->supported;}
static int shader(void*v){return ((Host*)v)->shader;}
static int camera(void*v){return ((Host*)v)->camera;}
static int rollback(void*v){return ((Host*)v)->rollback;}
static int shadow(void*v){return ((Host*)v)->shadow;}
static int commit(void*v){Host*h=v;if(!h->commit_ok)return 0;h->committed=1;return 1;}
static int release(void*v){Host*h=v;if(!h->release_ok)return 0;h->committed=0;h->releases++;return 1;}
static int publish(void*v,unsigned a,unsigned b,unsigned c){Host*h=v;
 if(h->fail_camera)return 0;h->geom=a;h->hair=b;h->indirect=c;h->updates++;return 1;}
static int setshadow(void*v,unsigned a){((Host*)v)->shadows=(int)a;return 1;}
static int setbill(void*v,unsigned a){((Host*)v)->billboards=(int)a;return 1;}
static CR01PTCarrier501Ops ops(Host*h){return (CR01PTCarrier501Ops){h,build,shader,camera,shadow,rollback,
 commit,release,publish,setshadow,setbill};}
int main(void){
 Host host={.supported=1,.shader=1,.camera=1,.rollback=1,.shadow=1,.commit_ok=1,.release_ok=1};
 CR01PTCarrier501 c;CR01PTCarrier501Ops o=ops(&host);CR01Settings s;
 assert(cr01_pt501_init(&c,&o));s=c.saved;
 assert(!s.master&&s.v[F_INDIRECT]==100);
 assert(cr01_pt501_update(&c,&s,CR01_RENDER_PT));
 assert(c.stage==CR01_CARRIER_NATIVE&&!host.committed&&!host.updates);
 s.master=1;
 assert(cr01_pt501_update(&c,&s,CR01_RENDER_PT));
 assert(!host.committed); /* untouched Master On is real pass-through */
 s.v[F_GEOM]=5;s.v[F_HAIR]=6;s.v[F_INDIRECT]=133;
 assert(cr01_pt501_update(&c,&s,CR01_RENDER_PT));
 assert(host.committed&&host.geom==5&&host.hair==6&&host.indirect==133);
 assert(cr01_pt501_update(&c,&s,CR01_RENDER_NORMAL_RT));
 assert(!host.committed&&host.geom==2&&host.hair==4&&host.indirect==100);
 assert(c.saved.v[F_GEOM]==5&&c.saved.v[F_INDIRECT]==133); /* preferences retained */
 assert(cr01_pt501_update(&c,&s,CR01_RENDER_PT));
 assert(host.geom==5&&host.committed);
 s.v[F_SHADOWS]=1;s.v[F_BILLBOARDS]=2;
 assert(cr01_pt501_update(&c,&s,CR01_RENDER_PT));
 assert(host.shadows==1&&host.billboards==2);
 s.v[F_SHADOWS]=0;
 assert(cr01_pt501_update(&c,&s,CR01_RENDER_PT));
 assert(host.shadows==0&&host.billboards==2);
 s.master=0;
 assert(cr01_pt501_update(&c,&s,CR01_RENDER_PT));
 assert(!host.committed&&!host.shadows&&!host.billboards);
 /* Strong refusal of unsupported 5.01, no writes. */
 Host bad=host;bad.supported=0;bad.committed=0;bad.updates=0;
 CR01PTCarrier501 rejected;CR01PTCarrier501Ops badops=ops(&bad);
 assert(cr01_pt501_init(&rejected,&badops));s.master=1;
 assert(!cr01_pt501_update(&rejected,&s,CR01_RENDER_PT));
 assert(rejected.stage==CR01_CARRIER_FAILED&&!bad.committed&&!bad.updates);
 /* Failure after successful install forces a neutral rollback, never active. */
 Host partial=host;partial.committed=0;partial.fail_camera=1;partial.updates=0;
 CR01PTCarrier501 failed;CR01PTCarrier501Ops partialops=ops(&partial);
 assert(cr01_pt501_init(&failed,&partialops));
 assert(!cr01_pt501_update(&failed,&s,CR01_RENDER_PT));
 assert(failed.stage==CR01_CARRIER_FAILED);
 assert(partial.committed); /* sticky owner: failed neutral publication can't drop ownership */
 /* Bad game mode is refused. */
 assert(!cr01_pt501_update(&c,&s,(CR01RenderMode)99));
 printf("PASS: 5.01 carrier gating, native default, PT/RT, save, 3 CameraCB controls, shadow/billboard independence, rollback fail-closed\n");
}
