/* Windows-only game-owned settings directory, write-temp-then-replace persistence.
 * No startup file IO from DllMain. Caller initializes on the verified owner. */
#include "cr01_settings_store_windows.h"
#define WINAPI __stdcall
#define IMPORT __declspec(dllimport)
typedef void* HANDLE;
typedef uint16_t WCHAR;
typedef unsigned long DWORD;
typedef int BOOL;
#define INVALID_HANDLE_VALUE ((HANDLE)(intptr_t)-1)
#define GENERIC_READ 0x80000000ul
#define GENERIC_WRITE 0x40000000ul
#define FILE_SHARE_READ 1ul
#define OPEN_EXISTING 3ul
#define CREATE_ALWAYS 2ul
#define FILE_ATTRIBUTE_NORMAL 0x80ul
#define MOVEFILE_REPLACE_EXISTING 1ul
#define MOVEFILE_WRITE_THROUGH 8ul
#define ERROR_FILE_NOT_FOUND 2ul
#define ERROR_PATH_NOT_FOUND 3ul
#define ERROR_ALREADY_EXISTS 183ul
IMPORT DWORD WINAPI GetLastError(void);
IMPORT HANDLE WINAPI CreateFileW(const WCHAR*,DWORD,DWORD,void*,DWORD,DWORD,HANDLE);
IMPORT BOOL WINAPI ReadFile(HANDLE,void*,DWORD,DWORD*,void*);
IMPORT BOOL WINAPI WriteFile(HANDLE,const void*,DWORD,DWORD*,void*);
IMPORT BOOL WINAPI FlushFileBuffers(HANDLE);
IMPORT BOOL WINAPI CloseHandle(HANDLE);
IMPORT BOOL WINAPI MoveFileExW(const WCHAR*,const WCHAR*,DWORD);
IMPORT BOOL WINAPI DeleteFileW(const WCHAR*);
static int app(WCHAR*d,unsigned cap,const WCHAR*s){
 unsigned i=0,k=0;
 if(!d||!s)return 0;
 while(i<cap&&d[i])i++;
 if(i==cap)return 0;
 while(s[k]){if(i+1>=cap)return 0;d[i++]=s[k++];}
 d[i]=0;return 1;
}
#define ERROR_SHARING_VIOLATION 32ul
#define ERROR_LOCK_VIOLATION 33ul
IMPORT void WINAPI Sleep(DWORD);
IMPORT BOOL WINAPI ReplaceFileW(const WCHAR*,const WCHAR*,const WCHAR*,DWORD,void*,void*);
/* Open for reading, riding out short sharing violations (antivirus, backup
 * tools, cloud sync) instead of treating them as missing/corrupt. */
static HANDLE open_read(const WCHAR*path,DWORD*err){
 HANDLE h=INVALID_HANDLE_VALUE;
 for(unsigned i=0;i<10;i++){
  h=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
  if(h!=INVALID_HANDLE_VALUE){*err=0;return h;}
  *err=GetLastError();
  if(*err!=ERROR_SHARING_VIOLATION&&*err!=ERROR_LOCK_VIOLATION)break;
  Sleep(50);
 }
 return INVALID_HANDLE_VALUE;
}
static int read_all(HANDLE h,char*buf,size_t cap,size_t*out){
 DWORD read=0;
 /* One read of capacity+1 prevents silently ignoring appended data.
  * The codec rejects a file longer than 4096 bytes. */
 if(!ReadFile(h,buf,(DWORD)cap,&read,0)){CloseHandle(h);return -1;}
 *out=(size_t)read;
 if(!CloseHandle(h))return -1;
 return 1;
}
static int missing(DWORD e){return e==ERROR_FILE_NOT_FOUND||e==ERROR_PATH_NOT_FOUND;}
static int windows_read(void*opaque,char*buf,size_t cap,size_t*out){
 CR01WindowsStore*s=(CR01WindowsStore*)opaque;HANDLE h;DWORD err=0;
 if(!s||!s->initialized||!buf||!out||cap>0xffffffffu)return -1;
 *out=0;s->legacy_loaded=0;s->primary_bytes_read=0;
 h=open_read(s->final_path,&err);
 if(h!=INVALID_HANDLE_VALUE){
  int r=read_all(h,buf,cap,out);
  if(r==1)s->primary_bytes_read=1;else s->commit_blocked=1;
  return r;
 }
 /* Never fall back over an unreadable existing canonical file. */
 if(!missing(err)){s->commit_blocked=1;return -1;}
 /* Read-only migration chain: V4.x name, then early-V4 name. Old files kept. */
 const uint16_t*legacy[2]={s->legacy_path,s->legacy2_path};
 for(unsigned i=0;i<2;i++){
  h=open_read(legacy[i],&err);
  if(h==INVALID_HANDLE_VALUE){if(missing(err))continue;return -1;}
  int r=read_all(h,buf,cap,out);
  if(r==1)s->legacy_loaded=1u+i;
  return r;
 }
 return 0;
}
static int windows_read_backup(void*opaque,char*buf,size_t cap,size_t*out){
 CR01WindowsStore*s=(CR01WindowsStore*)opaque;HANDLE h;DWORD err=0;
 if(!s||!s->initialized||!buf||!out||cap>0xffffffffu)return -1;
 *out=0;
 h=open_read(s->backup_path,&err);
 if(h==INVALID_HANDLE_VALUE)return missing(err)?0:-1;
 return read_all(h,buf,cap,out);
}
static int windows_quarantine(void*opaque){
 CR01WindowsStore*s=(CR01WindowsStore*)opaque;
 uint16_t path[1040];unsigned n=0;
 if(!s||!s->initialized||!s->primary_bytes_read)return 0; /* only proven-invalid content */
 /* Never overwrite earlier evidence: .corrupt, then .corrupt.1 ... .corrupt.99,
  * each without MOVEFILE_REPLACE_EXISTING. If every name is taken, keep the
  * invalid primary in place and block saving for this session, so neither it
  * nor the good .bak is replaced (menu values stay live from dx12user.settings). */
 while(s->corrupt_path[n]&&n<1024u){path[n]=s->corrupt_path[n];n++;}
 for(unsigned i=0;i<100u;i++){
  unsigned k=n;
  if(i){path[k++]='.';if(i>=10u)path[k++]=(uint16_t)('0'+i/10u);path[k++]=(uint16_t)('0'+i%10u);}
  path[k]=0;
  if(MoveFileExW(s->final_path,path,MOVEFILE_WRITE_THROUGH))return 1;
  if(GetLastError()!=ERROR_ALREADY_EXISTS&&GetLastError()!=80ul)break; /* 80 = ERROR_FILE_EXISTS */
 }
 s->commit_blocked=1;
 return 0;
}
static int windows_commit(void*opaque,const char*buf,size_t n){
 CR01WindowsStore*s=(CR01WindowsStore*)opaque;HANDLE h;DWORD wrote=0;
 if(!s||!s->initialized||!buf||!n||n>4096u||s->commit_blocked)return 0;
 h=CreateFileW(s->temp_path,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
 if(h==INVALID_HANDLE_VALUE)return 0;
 BOOL ok=WriteFile(h,buf,(DWORD)n,&wrote,0)&&wrote==(DWORD)n&&FlushFileBuffers(h);
 if(!CloseHandle(h))ok=0;
 if(!ok){(void)DeleteFileW(s->temp_path);return 0;}
 if(s->legacy_loaded){
  /* First migration: fail rather than overwrite a canonical file created
   * by another process after our missing-file read. */
  if(!MoveFileExW(s->temp_path,s->final_path,MOVEFILE_WRITE_THROUGH)){(void)DeleteFileW(s->temp_path);return 0;}
  s->legacy_loaded=0;return 1;
 }
 /* Normal save: atomic replace that keeps the previous good file as .bak. */
 if(ReplaceFileW(s->final_path,s->temp_path,s->backup_path,0,0,0))return 1;
 if(GetLastError()==ERROR_FILE_NOT_FOUND&&
    MoveFileExW(s->temp_path,s->final_path,MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH))return 1;
 (void)DeleteFileW(s->temp_path);
 return 0;
}
/* PRODUCTION PERSISTENCE: the calling owner resolves Documents\The Witcher 3
 * using its existing SHGetFolderPathW flow. Private preferences are stored in
 * that exact game-owned user-settings directory, alongside dx12user.settings,
 * in RefinedTracing.settings. No QA env vars or AppData fallback.
 * If the new file is missing, the old RefinedTracingCR01.settings is read-only
 * imported after full codec validation, then atomically copied to the new
 * file without overwriting a concurrently created file. Old file is kept.
 * No DX12 or other game-owned file is written through this store. This is
 * only the native PT preference subset, NOT yet the full menu configuration.
 */
int cr01_store_windows_init(CR01WindowsStore*s,CR01PrivateStore*out,
                            const WCHAR*witcher_settings_dir){
 static const WCHAR slash[]={ '\\',0};
 static const WCHAR filename[]={ 'U','l','t','i','m','a','t','e','R','e','f','i','n','e','d','T','r','a','c','i','n','g','.','s','e','t','t','i','n','g','s',0};
 static const WCHAR legacyname[]={ 'R','e','f','i','n','e','d','T','r','a','c','i','n','g','.','s','e','t','t','i','n','g','s',0};
 static const WCHAR legacyname2[]={ 'R','e','f','i','n','e','d','T','r','a','c','i','n','g','C','R','0','1','.','s','e','t','t','i','n','g','s',0};
 static const WCHAR baksuffix[]={ '.','b','a','k',0};
 static const WCHAR corruptsuffix[]={ '.','c','o','r','r','u','p','t',0};
 static const WCHAR tmpsuffix[]={ '.','t','m','p',0};
 unsigned i=0,j;
 if(!s||!out||!witcher_settings_dir)return 0;
 s->initialized=0;s->legacy_loaded=0;s->final_path[0]=0;s->temp_path[0]=0;s->legacy_path[0]=0;
 s->legacy2_path[0]=0;s->backup_path[0]=0;s->corrupt_path[0]=0;s->primary_bytes_read=0;s->commit_blocked=0;
 /* Require an absolute Windows path returned by the owner's game directory
  * resolver. Valid redirected Documents UNC paths are also accepted. */
 while(i<1024u&&witcher_settings_dir[i])i++;
 if(!i||i>=1024u)return 0;
 if(!(i>=3u&&((witcher_settings_dir[0]>=L'A'&&witcher_settings_dir[0]<=L'Z')||
        (witcher_settings_dir[0]>=L'a'&&witcher_settings_dir[0]<=L'z'))&&
       witcher_settings_dir[1]==L':'&&(witcher_settings_dir[2]==L'\\'||witcher_settings_dir[2]==L'/'))&&
    !(i>=5u&&witcher_settings_dir[0]==L'\\'&&witcher_settings_dir[1]==L'\\'&&
      witcher_settings_dir[2]!=L'\\'))return 0;
 for(j=0;j<=i;j++)s->final_path[j]=witcher_settings_dir[j];
 if(s->final_path[i-1]!=L'\\'&&s->final_path[i-1]!=L'/')
  if(!app(s->final_path,1024,slash))return 0;
 if(!app(s->legacy_path,1024,s->final_path)||
    !app(s->legacy_path,1024,legacyname))return 0;
 if(!app(s->legacy2_path,1024,s->final_path)||
    !app(s->legacy2_path,1024,legacyname2))return 0;
 if(!app(s->final_path,1024,filename))return 0;
 if(!app(s->temp_path,1024,s->final_path)||
    !app(s->temp_path,1024,tmpsuffix))return 0;
 if(!app(s->backup_path,1024,s->final_path)||!app(s->backup_path,1024,baksuffix))return 0;
 if(!app(s->corrupt_path,1024,s->final_path)||!app(s->corrupt_path,1024,corruptsuffix))return 0;
 s->initialized=1;out->ctx=s;
 out->read_private=windows_read;out->commit_private=windows_commit;
 out->read_backup=windows_read_backup;out->quarantine=windows_quarantine;
 return 1;
}
