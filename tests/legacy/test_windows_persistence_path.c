#include "cr01_settings_store_windows.h"
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
/* Native-host Win32 API simulation, no game files or processes touched. */
typedef uint16_t WCHAR;
typedef unsigned long DWORD;
typedef int BOOL;
typedef void* HANDLE;
typedef struct {char bytes[4097];size_t size;int valid;} File;
static File final_file,old_file,temp_file;
static unsigned moved,flushed,deleted,commit_error,collision;
static DWORD last_error,last_move_flags;
static int ends(const WCHAR*s,const char*e){
 size_t n=0,m=strlen(e);while(s[n])n++;if(n<m)return 0;
 for(size_t j=0;j<m;j++)if(s[n-m+j]!=(WCHAR)e[j])return 0;
 return 1;
}
static int eq(const WCHAR*s,const char*x){
 size_t i=0;while(x[i]){if(s[i]!=(unsigned char)x[i])return 0;i++;}return s[i]==0;
}
static File*target(const WCHAR*p){
 if(ends(p,"RefinedTracing.settings.tmp"))return &temp_file;
 if(ends(p,"RefinedTracingCR01.settings"))return &old_file;
 assert(ends(p,"RefinedTracing.settings"));return &final_file;
}
DWORD __attribute__((stdcall)) GetLastError(void){return last_error;}
HANDLE __attribute__((stdcall)) CreateFileW(const WCHAR*p,DWORD access,DWORD share,void*sa,DWORD disposition,DWORD attrs,HANDLE templ){
 (void)access;(void)share;(void)sa;(void)attrs;(void)templ;
 File*f=target(p);
 if(disposition==3ul&&!f->valid){last_error=2ul;return (HANDLE)(intptr_t)-1;}
 if(disposition==2ul){f->size=0;f->valid=1;}
 return f;
}
BOOL __attribute__((stdcall)) ReadFile(HANDLE h,void*p,DWORD cap,DWORD*out,void*ov){
 (void)ov;File*f=h;if(!f||!f->valid)return 0;
 size_t n=f->size<cap?f->size:cap;memcpy(p,f->bytes,n);*out=(DWORD)n;return 1;
}
BOOL __attribute__((stdcall)) WriteFile(HANDLE h,const void*p,DWORD n,DWORD*out,void*ov){
 (void)ov;File*f=h;
 if(commit_error||n>sizeof(f->bytes))return 0;
 memcpy(f->bytes,p,n);f->size=n;f->valid=1;*out=n;return 1;
}
BOOL __attribute__((stdcall)) FlushFileBuffers(HANDLE h){(void)h;flushed++;return 1;}
BOOL __attribute__((stdcall)) CloseHandle(HANDLE h){(void)h;return 1;}
BOOL __attribute__((stdcall)) MoveFileExW(const WCHAR*src,const WCHAR*dst,DWORD flags){
 assert(ends(src,"RefinedTracing.settings.tmp")&&ends(dst,"RefinedTracing.settings"));
 assert(flags==8ul||flags==9ul);last_move_flags=flags;
 if(commit_error||!temp_file.valid)return 0;
 if(collision){final_file.valid=1;memcpy(final_file.bytes,"OTHER",5);final_file.size=5;collision=0;}
 if(flags==8ul&&final_file.valid)return 0;
 final_file=temp_file;temp_file.valid=0;moved++;return 1;
}
BOOL __attribute__((stdcall)) DeleteFileW(const WCHAR*p){
 assert(ends(p,".tmp"));temp_file.valid=0;deleted++;return 1;
}
static void wide(WCHAR*out,size_t cap,const char*s){size_t i=0;while(s[i]){assert(i+1<cap);out[i]=(uint8_t)s[i];i++;}out[i]=0;}
static void reset_files(void){memset(&final_file,0,sizeof(final_file));memset(&old_file,0,sizeof(old_file));memset(&temp_file,0,sizeof(temp_file));}
int main(void){
 CR01WindowsStore w={0};CR01PrivateStore io={0};WCHAR p[1024];char b[4097];size_t n=0;
 wide(p,1024,"C:\\Users\\Example\\Documents\\The Witcher 3");
 assert(cr01_store_windows_init(&w,&io,p));
 assert(eq(w.final_path,"C:\\Users\\Example\\Documents\\The Witcher 3\\RefinedTracing.settings"));
 assert(eq(w.temp_path,"C:\\Users\\Example\\Documents\\The Witcher 3\\RefinedTracing.settings.tmp"));
 assert(eq(w.legacy_path,"C:\\Users\\Example\\Documents\\The Witcher 3\\RefinedTracingCR01.settings"));
 assert(io.read_private(io.ctx,b,sizeof(b),&n)==0);
 assert(io.commit_private(io.ctx,"DATA",4));
 assert(last_move_flags==9ul&&moved==1&&flushed==1&&final_file.valid&&final_file.size==4);
 assert(io.read_private(io.ctx,b,sizeof(b),&n)==1&&n==4&&!memcmp(b,"DATA",4)&&!w.legacy_loaded);
 commit_error=1;assert(!io.commit_private(io.ctx,"FAIL",4));
 assert(final_file.size==4&&!memcmp(final_file.bytes,"DATA",4));assert(deleted==1);
 commit_error=0;
 /* Canonical file always wins: existing is never discarded for legacy. */
 memcpy(old_file.bytes,"LEGACY",6);old_file.size=6;old_file.valid=1;
 assert(io.read_private(io.ctx,b,sizeof(b),&n)==1&&n==4&&!memcmp(b,"DATA",4)&&!w.legacy_loaded);
 /* Absent canonical imports legacy read-only, then atomically creates canonical. */
 final_file.valid=0;
 assert(io.read_private(io.ctx,b,sizeof(b),&n)==1&&n==6&&!memcmp(b,"LEGACY",6)&&w.legacy_loaded);
 assert(io.commit_private(io.ctx,b,n)&&!w.legacy_loaded&&last_move_flags==8ul);
 assert(old_file.valid&&old_file.size==6&&!memcmp(old_file.bytes,"LEGACY",6));
 assert(final_file.valid&&final_file.size==6&&!memcmp(final_file.bytes,"LEGACY",6));
 assert(io.commit_private(io.ctx,"NEW",3)&&last_move_flags==9ul);
 assert(old_file.size==6&&!memcmp(old_file.bytes,"LEGACY",6));
 /* A new canonical file created during migration is NEVER overwritten. */
 reset_files();memcpy(old_file.bytes,"OLD",3);old_file.size=3;old_file.valid=1;
 assert(io.read_private(io.ctx,b,sizeof(b),&n)==1&&w.legacy_loaded);
 collision=1;assert(!io.commit_private(io.ctx,"OLD",3));
 assert(final_file.valid&&final_file.size==5&&!memcmp(final_file.bytes,"OTHER",5));
 assert(old_file.valid&&old_file.size==3&&!memcmp(old_file.bytes,"OLD",3));
 assert(!temp_file.valid);
 /* Empty canonical stays authoritative and must fail codec validation; never fall back. */
 final_file.valid=1;final_file.size=0;
 assert(io.read_private(io.ctx,b,sizeof(b),&n)==1&&n==0&&!w.legacy_loaded);
 /* Full-format migration: validate genuine CR01_V2 bytes before the first
  * canonical commit. Invalid legacy must not create any replacement file. */
 wide(p,1024,"C:\\Users\\Example\\Documents\\The Witcher 3");
 assert(cr01_store_windows_init(&w,&io,p));
 reset_files();
 CR01MenuBridge original,restored;
 char serialized[512];size_t used=0;
 cr01_menu_bridge_init(&original);
 assert(cr01_menu_bridge_event(&original,"MasterEnabled","1"));
 assert(cr01_menu_bridge_event(&original,"IndirectLight","1.20"));
 assert(cr01_menu_bridge_save(&original,serialized,sizeof(serialized),&used));
 memcpy(old_file.bytes,serialized,used);old_file.size=used;old_file.valid=1;
 cr01_menu_bridge_init(&restored);
 assert(cr01_store_load(&restored,&io)&&w.legacy_loaded);
 assert(restored.controller.saved.master==1&&restored.controller.saved.v[F_INDIRECT]==120);
 restored.dirty_generation++;
 assert(cr01_store_flush(&restored,&io));
 assert(!w.legacy_loaded&&final_file.valid);
 assert(old_file.size==used&&!memcmp(old_file.bytes,serialized,used));
 assert(final_file.size==used&&!memcmp(final_file.bytes,serialized,used));
 reset_files();memcpy(old_file.bytes,"BROKEN",6);old_file.size=6;old_file.valid=1;
 cr01_menu_bridge_init(&restored);
 assert(!cr01_store_load(&restored,&io)&&!final_file.valid);
 assert(old_file.valid&&old_file.size==6&&!memcmp(old_file.bytes,"BROKEN",6));
 wide(p,1024,"C:\\Users\\Example\\Documents\\The Witcher 3\\");
 assert(cr01_store_windows_init(&w,&io,p));assert(eq(w.final_path,"C:\\Users\\Example\\Documents\\The Witcher 3\\RefinedTracing.settings"));
 wide(p,1024,"\\\\SERVER\\share\\Witcher 3");
 assert(cr01_store_windows_init(&w,&io,p));assert(eq(w.legacy_path,"\\\\SERVER\\share\\Witcher 3\\RefinedTracingCR01.settings"));
 wide(p,1024,"relative\\dir");assert(!cr01_store_windows_init(&w,&io,p));
 wide(p,1024,"C:relative");assert(!cr01_store_windows_init(&w,&io,p));
 wide(p,1024,"C");assert(!cr01_store_windows_init(&w,&io,p));
 assert(!cr01_store_windows_init(&w,&io,0));
 memset(p,'A',sizeof(p));assert(!cr01_store_windows_init(&w,&io,p));
 puts("PASS private store migration: canonical precedence, legacy read-only, atomic new file, collision refusal, retention, UNC/absolute path");
 return 0;
}
