/* Native mock of shipped Win32 logger: clean session truncation, event append,
 * bounded messages, timestamp formatting, flush-on-event and missing path. */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define __declspec(x)
#include "cr01_release_log.c"
static const char *test_path="/tmp/refined_rc1_log_mock.log";
static unsigned flush_count,create_count;
DWORD GetModuleFileNameW(void*unused,WCHAR *out,DWORD cap){
 const char *s="C:\\Games\\Witcher 3\\bin\\x64_dx12\\witcher3.exe";
 unsigned n=(unsigned)strlen(s);(void)unused;if(cap<n+1)return 0;
 for(unsigned i=0;i<=n;i++)out[i]=(WCHAR)s[i];return n;
}
HANDLE CreateFileW(const WCHAR*path,DWORD a,DWORD share,void*sec,DWORD creation,DWORD flags,HANDLE tmpl){
 FILE*f=0;(void)path;(void)a;(void)share;(void)sec;(void)flags;(void)tmpl;
 if(creation==CREATE_ALWAYS){f=fopen(test_path,"wb");create_count++;}
 else if(creation==OPEN_ALWAYS)f=fopen(test_path,"ab");
 return f?(HANDLE)f:INVALID_HANDLE;
}
BOOL WriteFile(HANDLE h,const void *bytes,DWORD n,DWORD *written,void *ov){
 (void)ov;*written=(DWORD)fwrite(bytes,1,n,(FILE*)h);return *written==n;
}
BOOL FlushFileBuffers(HANDLE h){flush_count++;return fflush((FILE*)h)==0;}
BOOL CloseHandle(HANDLE h){return fclose((FILE*)h)==0;}
void GetSystemTime(CR01SystemTime*t){memset(t,0,sizeof(*t));t->year=2026;t->month=10;t->day=15;t->hour=1;t->minute=5;t->second=9;}
DWORD GetCurrentProcessId(void){return 100;}
int main(void){
 FILE*f=fopen(test_path,"wb");fputs("HISTORICAL ENTRY MUST VANISH",f);fclose(f);
 assert(cr01_release_log_begin());
 cr01_release_log_event("CR01: test startup\r\n");
 cr01_release_log_event("CR01: renderer transition -> PT\r\n");
 assert(cr01_release_log_begin()); /* never truncate again in one session */
 assert(create_count==1&&flush_count>=3);
 for(unsigned i=0;i<200;i++)cr01_release_log_event("CR01 RT private: Normal RT overrides applied (asynchronous settings worker)");
 for(unsigned i=0;i<200;i++)cr01_release_log_event("CR01 RT private: Normal RT overrides released (native values restored where owned)");
 /* Flood of informational events cannot crowd out a later actionable error. */
 for(unsigned i=0;i<3000;i++)cr01_release_log_event("CR01: informational setting transition; optional context; no fault");
 cr01_release_log_event("CR01: ownership refused: expected native slot missing");
 f=fopen(test_path,"rb");assert(f);char b[140000];size_t n=fread(b,1,sizeof(b)-1,f);fclose(f);b[n]=0;
 assert(n<=131072u);
 assert(strstr(b,"ownership refused: expected native slot missing"));
 char*status=strstr(b,"Normal RT overrides applied");assert(status);
 assert(!strstr(status+1,"Normal RT overrides applied"));
 assert(!strstr(b,"HISTORICAL"));assert(strstr(b,"Ultimate Refined Tracing V4.0.0 | session started"));
 assert(strstr(b,"2026-10-15 01:05:09 UTC"));
 assert(strstr(b,"renderer transition -> PT"));
 assert(strstr(b,"Refined Tracing | test startup"));
 assert(strstr(b,"Refined Tracing.log")==0); /* log content not path leakage */
 puts("V4_LOG_MOCK=PASS; fresh session, bounded info, fault reserved, duplicates suppressed");
 remove(test_path);return 0;
}
