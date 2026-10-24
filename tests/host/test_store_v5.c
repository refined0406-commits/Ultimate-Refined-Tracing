/* V5 private preference store: naming/migration chain, ReplaceFileW backup,
 * corruption recovery and transient I/O behaviour, against a simulated Win32
 * file system (no real files touched). */
#include "cr01_settings_store_windows.h"
#include "cr01_settings_store.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
typedef uint16_t WCHAR; typedef unsigned long DWORD; typedef int BOOL; typedef void* HANDLE;
typedef struct {const char*suffix;char bytes[4097];size_t size;int valid;int locked;int share_violations;} File;
/* suffix order matters: longest first */
static File fs[]={{"UltimateRefinedTracing.settings.tmp"},{"UltimateRefinedTracing.settings.bak"},
 {"UltimateRefinedTracing.settings.corrupt"},{"UltimateRefinedTracing.settings"},
 {"RefinedTracingCR01.settings"},{"RefinedTracing.settings"},
 {"UltimateRefinedTracing.settings.corrupt.1"},{"UltimateRefinedTracing.settings.corrupt.2"}};
enum{TMP,BAK,COR,FIN,OLD2,OLD1,COR1,COR2,NF};
static File occupied={"occupied",{0},0,1,0,0}; /* every further .corrupt.N already exists */
static int all_taken;
static DWORD last_error;static unsigned sleeps,replaces;
static size_t wl(const WCHAR*s){size_t n=0;while(s[n])n++;return n;}
static File*target(const WCHAR*p){
 for(int i=0;i<NF;i++){size_t m=strlen(fs[i].suffix),n=wl(p);if(n<m+1)continue;int ok=p[n-m-1]=='\\';
  for(size_t j=0;ok&&j<m;j++)if(p[n-m+j]!=(WCHAR)fs[i].suffix[j])ok=0; if(ok)return &fs[i];}
 if(all_taken)return &occupied;
 assert(0);return 0;}
DWORD GetLastError(void){return last_error;}
void Sleep(DWORD ms){(void)ms;sleeps++;}
HANDLE CreateFileW(const WCHAR*p,DWORD a,DWORD s,void*sa,DWORD disp,DWORD at,HANDLE t){
 (void)a;(void)s;(void)sa;(void)at;(void)t;File*f=target(p);
 if(f->share_violations){f->share_violations--;last_error=32;return (HANDLE)(intptr_t)-1;}
 if(f->locked){last_error=5;return (HANDLE)(intptr_t)-1;}
 if(disp==3&&!f->valid){last_error=2;return (HANDLE)(intptr_t)-1;}
 if(disp==2){f->size=0;f->valid=1;}
 return f;}
BOOL ReadFile(HANDLE h,void*p,DWORD cap,DWORD*o,void*ov){(void)ov;File*f=h;size_t n=f->size<cap?f->size:cap;memcpy(p,f->bytes,n);*o=(DWORD)n;return 1;}
BOOL WriteFile(HANDLE h,const void*p,DWORD n,DWORD*o,void*ov){(void)ov;File*f=h;memcpy(f->bytes,p,n);f->size=n;f->valid=1;*o=n;return 1;}
BOOL FlushFileBuffers(HANDLE h){(void)h;return 1;}
BOOL CloseHandle(HANDLE h){(void)h;return 1;}
BOOL DeleteFileW(const WCHAR*p){target(p)->valid=0;return 1;}
BOOL MoveFileExW(const WCHAR*s,const WCHAR*d,DWORD fl){File*a=target(s),*b=target(d);
 if(!a->valid){last_error=2;return 0;}
 if(!(fl&1)&&b->valid){last_error=183;return 0;}
 const char*bs=b->suffix;*b=*a;b->suffix=bs;a->valid=0;return 1;}
BOOL ReplaceFileW(const WCHAR*rep,const WCHAR*with,const WCHAR*bak,DWORD fl,void*x,void*y){
 (void)fl;(void)x;(void)y;File*r=target(rep),*w=target(with),*k=target(bak);
 if(!r->valid){last_error=2;return 0;}
 const char*ks=k->suffix,*rs=r->suffix;*k=*r;k->suffix=ks;*r=*w;r->suffix=rs;w->valid=0;replaces++;return 1;}
static void reset(void){for(int i=0;i<NF;i++){const char*s=fs[i].suffix;memset(&fs[i],0,sizeof fs[i]);fs[i].suffix=s;}}
static void put(int i,const char*b,size_t n){memcpy(fs[i].bytes,b,n);fs[i].size=n;fs[i].valid=1;}
static int eqw(const WCHAR*s,const char*x){size_t i=0;while(x[i]){if(s[i]!=(unsigned char)x[i])return 0;i++;}return s[i]==0;}
static size_t prefs(char*out,unsigned master,const char*indirect){
 CR01MenuBridge m;size_t used=0;cr01_menu_bridge_init(&m);
 assert(cr01_menu_bridge_event(&m,"MasterEnabled",master?"1":"0"));
 assert(cr01_menu_bridge_event(&m,"IndirectLight",indirect));
 assert(cr01_menu_bridge_save(&m,out,512,&used));return used;}
int main(void){
 CR01WindowsStore w;CR01PrivateStore io;WCHAR dir[64];const char*d="C:\\Docs\\The Witcher 3";
 for(size_t i=0;;i++){dir[i]=(unsigned char)d[i];if(!d[i])break;}
 memset(&w,0,sizeof w);memset(&io,0,sizeof io);
 assert(cr01_store_windows_init(&w,&io,dir));
 assert(eqw(w.final_path,"C:\\Docs\\The Witcher 3\\UltimateRefinedTracing.settings"));
 assert(eqw(w.legacy_path,"C:\\Docs\\The Witcher 3\\RefinedTracing.settings"));
 assert(eqw(w.legacy2_path,"C:\\Docs\\The Witcher 3\\RefinedTracingCR01.settings"));
 assert(eqw(w.backup_path,"C:\\Docs\\The Witcher 3\\UltimateRefinedTracing.settings.bak"));
 char a[512],b[512];size_t na=prefs(a,1,"1.20"),nb=prefs(b,1,"1.35");CR01MenuBridge m;
 /* 1. fresh install: defaults, nothing written */
 reset();cr01_menu_bridge_init(&m);assert(cr01_store_load_resilient(&m,&io)==0&&!m.controller.saved.master&&!fs[FIN].valid);
 /* 2. V4.3.x user: RefinedTracing.settings migrates, old file kept, no overwrite flag */
 reset();put(OLD1,a,na);cr01_menu_bridge_init(&m);
 assert(cr01_store_load_resilient(&m,&io)==0&&w.legacy_loaded==1&&m.controller.saved.master==1&&m.controller.saved.v[F_INDIRECT]==120);
 m.dirty_generation++;assert(cr01_store_flush(&m,&io));
 assert(fs[FIN].valid&&fs[FIN].size==na&&!memcmp(fs[FIN].bytes,a,na)&&fs[OLD1].valid&&!w.legacy_loaded);
 /* 3. early-V4 user: RefinedTracingCR01.settings migrates when the V4.x name is absent */
 reset();put(OLD2,b,nb);cr01_menu_bridge_init(&m);
 assert(cr01_store_load_resilient(&m,&io)==0&&w.legacy_loaded==2&&m.controller.saved.v[F_INDIRECT]==135);
 /* 4. V4.x name wins over the early name; canonical wins over both */
 reset();put(OLD1,a,na);put(OLD2,b,nb);cr01_menu_bridge_init(&m);assert(cr01_store_load_resilient(&m,&io)==0&&m.controller.saved.v[F_INDIRECT]==120);
 reset();put(FIN,b,nb);put(OLD1,a,na);cr01_menu_bridge_init(&m);assert(cr01_store_load_resilient(&m,&io)==0&&m.controller.saved.v[F_INDIRECT]==135&&!w.legacy_loaded);
 /* 5. normal save uses ReplaceFileW and keeps the previous file as .bak */
 m.dirty_generation++;m.controller.saved.v[F_INDIRECT]=120;replaces=0;
 assert(cr01_store_flush(&m,&io)&&replaces==1&&fs[BAK].valid&&fs[BAK].size==nb&&!memcmp(fs[BAK].bytes,b,nb));
 /* 6. corrupt primary + valid .bak -> backup restored, corrupt moved aside, rewrite pending */
 reset();put(FIN,"GARBAGE",7);put(BAK,b,nb);cr01_menu_bridge_init(&m);
 assert(cr01_store_load_resilient(&m,&io)==1&&m.controller.saved.v[F_INDIRECT]==135);
 assert(fs[COR].valid&&fs[COR].size==7&&!fs[FIN].valid&&m.dirty_generation!=m.save_ack);
 assert(cr01_store_flush(&m,&io)&&fs[FIN].valid&&fs[FIN].size==nb);
 /* 7. corrupt primary, no backup -> defaults, runtime NOT passive, file quarantined */
 reset();put(FIN,"",0);cr01_menu_bridge_init(&m);
 assert(cr01_store_load_resilient(&m,&io)==2&&!m.controller.saved.master&&fs[COR].valid);
 /* 8. locked primary (I/O error): defaults for the session, NEVER quarantined or overwritten */
 reset();put(FIN,a,na);fs[FIN].locked=1;memset(&w,0,sizeof w);assert(cr01_store_windows_init(&w,&io,dir));cr01_menu_bridge_init(&m);
 assert(cr01_store_load_resilient(&m,&io)==2&&w.commit_blocked&&!fs[COR].valid);
 m.dirty_generation++;assert(!cr01_store_flush(&m,&io));fs[FIN].locked=0;assert(fs[FIN].size==na&&!memcmp(fs[FIN].bytes,a,na));
 /* 9. short sharing violations are ridden out */
 reset();put(FIN,a,na);fs[FIN].share_violations=3;memset(&w,0,sizeof w);assert(cr01_store_windows_init(&w,&io,dir));sleeps=0;cr01_menu_bridge_init(&m);
 assert(cr01_store_load_resilient(&m,&io)==0&&sleeps==3&&m.controller.saved.v[F_INDIRECT]==120);
 /* 10. migration never overwrites a canonical file that appeared meanwhile */
 reset();put(OLD1,a,na);memset(&w,0,sizeof w);assert(cr01_store_windows_init(&w,&io,dir));cr01_menu_bridge_init(&m);
 assert(cr01_store_load_resilient(&m,&io)==0&&w.legacy_loaded);put(FIN,b,nb);m.dirty_generation++;
 assert(!cr01_store_flush(&m,&io)&&fs[FIN].size==nb&&!memcmp(fs[FIN].bytes,b,nb)&&!fs[TMP].valid);
 /* 11. evidence preserved: an existing .corrupt is never overwritten; next free name is used */
 reset();put(COR,"FIRST",5);put(FIN,"SECOND",6);memset(&w,0,sizeof w);assert(cr01_store_windows_init(&w,&io,dir));cr01_menu_bridge_init(&m);
 assert(cr01_store_load_resilient(&m,&io)==2);
 assert(fs[COR].size==5&&!memcmp(fs[COR].bytes,"FIRST",5)&&fs[COR1].valid&&fs[COR1].size==6&&!memcmp(fs[COR1].bytes,"SECOND",6)&&!fs[FIN].valid);
 /* 12. every evidence name taken: primary kept in place, saving blocked, good .bak untouched */
 reset();put(COR,"A",1);put(COR1,"B",1);put(COR2,"C",1);put(FIN,"THIRD",5);put(BAK,"JUNK",4);all_taken=1;
 memset(&w,0,sizeof w);assert(cr01_store_windows_init(&w,&io,dir));cr01_menu_bridge_init(&m);
 assert(cr01_store_load_resilient(&m,&io)==2&&w.commit_blocked&&fs[FIN].valid&&fs[FIN].size==5);
 m.dirty_generation++;assert(!cr01_store_flush(&m,&io)&&fs[FIN].size==5&&fs[BAK].size==4);all_taken=0;
 puts("PASS V5 store: 3-name migration chain, precedence, .bak via ReplaceFileW, corrupt->bak, corrupt->defaults, locked file untouched, sharing retry, collision refusal, .corrupt evidence never overwritten");
 return 0;
}
