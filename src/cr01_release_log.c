/* Ultimate Refined Tracing V4 support log. One freshly truncated log per
 * game process; bounded informational events, reserved space for faults.
 * Called from the verified DI/ASI bootstrap outside DllMain. */
#include "cr01_release_log.h"
#include <stdint.h>
#include <stddef.h>
#ifndef URT_VERSION
#define URT_VERSION "5.0.0"
#endif
#define IMP __declspec(dllimport)
#define WIN __stdcall
typedef uint16_t WCHAR;
typedef unsigned long DWORD;
typedef int BOOL;
typedef void *HANDLE;
typedef struct {uint16_t year,month,day_of_week,day,hour,minute,second,millisecond;} CR01SystemTime;
#define INVALID_HANDLE ((HANDLE)(intptr_t)-1)
#define GENERIC_WRITE 0x40000000ul
#define FILE_APPEND_DATA 4ul
#define FILE_SHARE_READ 1ul
#define FILE_SHARE_WRITE 2ul
#define CREATE_ALWAYS 2ul
#define OPEN_ALWAYS 4ul
#define FILE_ATTRIBUTE_NORMAL 0x80ul
IMP DWORD WIN GetModuleFileNameW(void*,WCHAR*,DWORD);
IMP HANDLE WIN CreateFileW(const WCHAR*,DWORD,DWORD,void*,DWORD,DWORD,HANDLE);
IMP BOOL WIN WriteFile(HANDLE,const void*,DWORD,DWORD*,void*);
IMP BOOL WIN FlushFileBuffers(HANDLE);
IMP BOOL WIN CloseHandle(HANDLE);
IMP void WIN GetSystemTime(CR01SystemTime*);
IMP DWORD WIN GetCurrentProcessId(void);
static WCHAR g_path[1024];
static volatile DWORD g_log_started;
/* 64 KiB normal events, 128 KiB total including diagnostics. This prevents
 * runaway session logs while leaving extra room for faults late in a session. */
static volatile DWORD g_log_bytes;
static volatile DWORD g_applied_time, g_released_time;
static int starts(const char*s,const char*prefix){
 while(*prefix){if(*s++!=*prefix++)return 0;}return 1;
}
static int contains(const char*s,const char*needle){
 for(;*s;s++)if(starts(s,needle))return 1;
 return 0;
}
static int important(const char*s){
 return contains(s,"refused")||contains(s,"invalid")||contains(s,"failed")||
 contains(s,"unavailable")||contains(s,"lost")||contains(s,"mismatch")||
 contains(s,"conflict")||contains(s,"unsupported")||contains(s,"rejected")||
 contains(s,"error")||contains(s,"Error")||contains(s,"corrupt");
}
static void dig2(char *s,unsigned v){s[0]=(char)('0'+(v/10)%10);s[1]=(char)('0'+v%10);}
static void dig4(char *s,unsigned v){dig2(s,(v/100)%100);dig2(s+2,v%100);}
static void copy_limited(char*dst,unsigned *p,unsigned cap,const char*src){
 while(*src&&*p<cap)dst[(*p)++]=*src++;
}
static void event_line(const char*msg){
 char out[1152];unsigned n=0,i=0;DWORD wrote=0;HANDLE h;CR01SystemTime t;
 if(!msg||g_log_started!=1ul)return;
 GetSystemTime(&t);
 /* These two native-owner notices may repeat during the same renderer
  * handoff. Preserve initial state and later changes, not every tick. */
 volatile DWORD*last=0;
 if(starts(msg,"URT: Normal RT: RTGI settings applied"))last=&g_applied_time;
 else if(starts(msg,"URT: Normal RT: RTGI settings released"))last=&g_released_time;
 if(last){
  const DWORD stamp=(((DWORD)t.year*13u+t.month)*32u+t.day)*86400u+
                    (DWORD)t.hour*3600u+(DWORD)t.minute*60u+t.second;
  DWORD prev=__atomic_load_n(last,__ATOMIC_RELAXED);
  if(prev&&stamp>=prev&&stamp-prev<30u)return;
  __atomic_store_n(last,stamp,__ATOMIC_RELAXED);
 }
 if(sizeof(out)<44)return;
 dig4(out,t.year);out[4]='-';dig2(out+5,t.month);out[7]='-';dig2(out+8,t.day);
 out[10]=' ';dig2(out+11,t.hour);out[13]=':';dig2(out+14,t.minute);out[16]=':';dig2(out+17,t.second);
 out[19]=' ';out[20]='U';out[21]='T';out[22]='C';out[23]=' ';n=24;
 /* V5 branding: legacy internal "CR01: "/"CR01 " and "URT: " prefixes are
  * folded into one product prefix in the user-facing log. */
 copy_limited(out,&n,1100,"Ultimate Refined Tracing | ");
 if(starts(msg,"CR01: ")||starts(msg,"URT: "))i=starts(msg,"URT: ")?5:6;
 else if(starts(msg,"CR01 "))i=5;
 while(msg[i]&&msg[i]!='\r'&&msg[i]!='\n'&&n<1100)out[n++]=msg[i++];
 out[n++]='\r';out[n++]='\n';
 const DWORD cap=important(msg)?131072ul:65536ul;
 DWORD old=__sync_fetch_and_add(&g_log_bytes,(DWORD)n);
 if(old>cap||(DWORD)n>cap-old){
  (void)__sync_fetch_and_sub(&g_log_bytes,(DWORD)n);
  return;
 }
 h=CreateFileW(g_path,FILE_APPEND_DATA,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
 if(h==INVALID_HANDLE)return;
 /* One short event, sync written so an application crash retains prior lines. */
 /* Keep the reserved byte budget even on a failed/partial disk write. */
 (void)WriteFile(h,out,n,&wrote,0);
 (void)FlushFileBuffers(h);(void)CloseHandle(h);
}
int cr01_release_log_begin(void){
 static const WCHAR name[]={ 'U','l','t','i','m','a','t','e','R','e','f','i','n','e','d','T','r','a','c','i','n','g','.','l','o','g',0};
 WCHAR path[1024];DWORD len;HANDLE h;unsigned i;
 if(__sync_val_compare_and_swap(&g_log_started,0ul,2ul)!=0ul)return 1;
 g_log_bytes=0;g_applied_time=0;g_released_time=0;
 len=GetModuleFileNameW(0,path,1024);
 if(!len||len>=1024u)goto fail;
 while(len&&path[len-1]!=L'\\'&&path[len-1]!=L'/')len--;
 if(!len||len+sizeof(name)/sizeof(name[0])>=1024u)goto fail;
 for(i=0;i<len;i++)g_path[i]=path[i];
 for(i=0;i<sizeof(name)/sizeof(name[0]);i++)g_path[len+i]=name[i];
 h=CreateFileW(g_path,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
 if(h==INVALID_HANDLE)goto fail;
 (void)CloseHandle(h);
 g_log_started=1ul;
 event_line("Ultimate Refined Tracing " URT_VERSION " | session started (UTC)");
 return 1;
fail:
 g_log_started=0ul;return 0;
}
void cr01_release_log_event(const char*msg){if(g_log_started==1ul)event_line(msg);}
