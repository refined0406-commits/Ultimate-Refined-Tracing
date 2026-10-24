/* CR01 experimental synchronous DirectInput main gateway.
 * No work in DllMain except storing instance and disabling notifications.
 * The game DirectInput8Create call is forwarded to System32 regardless of ASI
 * readiness, and preparation executes synchronously ONCE before forwarding.
 * Build is NOT ready for user installation until the companion ASI's safe
 * registration and immutable shader creation are proven. */
#define IMP __declspec(dllimport)
#define EXP __declspec(dllexport)
#define WIN __stdcall
typedef unsigned long DWORD;typedef long HRESULT;typedef int BOOL;
typedef unsigned short WCHAR;typedef void* HMODULE;typedef void* HANDLE;
typedef void* HINSTANCE;typedef void* FARPROC;typedef void* LPVOID;
#define E_FAIL ((HRESULT)0x80004005L)
#define DLL_PROCESS_ATTACH 1ul
IMP DWORD WIN GetSystemDirectoryW(WCHAR*,DWORD);
IMP DWORD WIN GetModuleFileNameW(HMODULE,WCHAR*,DWORD);
IMP HMODULE WIN LoadLibraryW(const WCHAR*);
IMP FARPROC WIN GetProcAddress(HMODULE,const char*);
IMP BOOL WIN DisableThreadLibraryCalls(HMODULE);
IMP DWORD WIN GetCurrentThreadId(void);
typedef struct CR01_ONCE {void* p;}CR01_ONCE;
typedef BOOL(WIN*ONCE_CB)(CR01_ONCE*,void*,void**);
IMP BOOL WIN InitOnceExecuteOnce(CR01_ONCE*,ONCE_CB,void*,void**);
static CR01_ONCE g_di_once={0},g_asi_once={0};
static HMODULE g_self,g_system;
static void*g_real_create;
static int g_prepare_success;
static volatile DWORD g_asi_initializer_tid;
static volatile DWORD g_reentry_detected;
static unsigned wlength(const WCHAR*s){unsigned n=0;while(s[n])n++;return n;}
static int wappend(WCHAR*s,unsigned cap,const WCHAR*suffix){
 unsigned i=wlength(s),j=0;
 while(suffix[j]){if(i+1>=cap)return 0;s[i++]=suffix[j++];}
 s[i]=0;return 1;
}
static BOOL WIN load_system_once(CR01_ONCE*u,void*a,void**c){
 WCHAR path[1024];DWORD n;(void)u;(void)a;(void)c;
 n=GetSystemDirectoryW(path,990);
 if(!n||n>=990 || !wappend(path,1024,L"\\dinput8.dll"))return 1;
 g_system=LoadLibraryW(path);
 if(g_system)g_real_create=(void*)GetProcAddress(g_system,"DirectInput8Create");
 return 1;
}
static int ready_real(void){
 if(!InitOnceExecuteOnce(&g_di_once,load_system_once,0,0))return 0;
 return g_real_create!=0;
}
static BOOL WIN load_asi_and_prepare_inner(CR01_ONCE*u,void*a,void**c){
 WCHAR path[1024];DWORD n;HMODULE asi;FARPROC proc;
 (void)u;(void)a;(void)c;
 /* Game path is already authenticated by the ASI itself before ANY mutation.
  * LoadLibrary failure leaves the actual system DInput operation unaffected. */
 n=GetModuleFileNameW(g_self,path,1024);
 if(!n||n>=1024)return 1;
 while(n>0 && path[n-1]!=L'\\' && path[n-1]!=L'/')n--;
 if(!n || n+24>=1024)return 1;
 path[n]=0;
 if(!wappend(path,1024,L"RefinedTracing.asi"))return 1;
 asi=LoadLibraryW(path);
 if(!asi)return 1;
 proc=GetProcAddress(asi,"CR01EarlyPrepare");
 if(!proc)return 1;
 { typedef int(WIN*EARLY)(void); g_prepare_success=((EARLY)proc)()!=0; }
 return 1;
}
/* Unlike InitOnceExecuteOnce recursion on the same thread, this guarded
 * forwarding has no deadlock. Recursive calls mark the launch as unsafe and
 * cannot count as proof of early ASI readiness. */
static BOOL WIN load_asi_and_prepare(CR01_ONCE*u,void*a,void**c){
 __atomic_store_n(&g_asi_initializer_tid,GetCurrentThreadId(),__ATOMIC_RELEASE);
 BOOL r=load_asi_and_prepare_inner(u,a,c);
 if(__atomic_load_n(&g_reentry_detected,__ATOMIC_ACQUIRE))g_prepare_success=0;
 __atomic_store_n(&g_asi_initializer_tid,0u,__ATOMIC_RELEASE);
 return r;
}
/* A future production build must not use the preparation Boolean alone as
   permission to patch the game. The ASI independently establishes ownership,
   exact EXE identity, immutable DXIL validation and pre-registration state. */
EXP HRESULT WIN DirectInput8Create(HINSTANCE hinst,DWORD version,const void*riid,
                                   LPVOID*out,void*outer){
 typedef HRESULT(WIN*REAL)(HINSTANCE,DWORD,const void*,LPVOID*,void*);
 if(!ready_real())return E_FAIL;
 DWORD owner=__atomic_load_n(&g_asi_initializer_tid,__ATOMIC_ACQUIRE);
 if(owner && owner==GetCurrentThreadId()){
  __atomic_store_n(&g_reentry_detected,1u,__ATOMIC_RELEASE);
  return ((REAL)g_real_create)(hinst,version,riid,out,outer);
 }
 (void)InitOnceExecuteOnce(&g_asi_once,load_asi_and_prepare,0,0);
 return ((REAL)g_real_create)(hinst,version,riid,out,outer);
}
static FARPROC export_system(const char*name){
 if(!ready_real()||!g_system)return 0;
 return GetProcAddress(g_system,name);
}
EXP HRESULT WIN DllCanUnloadNow(void){typedef HRESULT(WIN*FN)(void);
 FARPROC p=export_system("DllCanUnloadNow");return p?((FN)p)():E_FAIL;}
EXP HRESULT WIN DllGetClassObject(const void*c,const void*r,LPVOID*out){
 typedef HRESULT(WIN*FN)(const void*,const void*,LPVOID*);
 FARPROC p=export_system("DllGetClassObject");return p?((FN)p)(c,r,out):E_FAIL;}
EXP HRESULT WIN DllRegisterServer(void){typedef HRESULT(WIN*FN)(void);
 FARPROC p=export_system("DllRegisterServer");return p?((FN)p)():E_FAIL;}
EXP HRESULT WIN DllUnregisterServer(void){typedef HRESULT(WIN*FN)(void);
 FARPROC p=export_system("DllUnregisterServer");return p?((FN)p)():E_FAIL;}
EXP const void* WIN GetdfDIJoystick(void){typedef const void*(WIN*FN)(void);
 FARPROC p=export_system("GetdfDIJoystick");return p?((FN)p)():0;}
EXP void WIN CR01ProxyIdentityTestOnly(void){}
BOOL WIN DllMain(HINSTANCE instance,DWORD reason,void*unused){
 (void)unused;
 if(reason==DLL_PROCESS_ATTACH){g_self=(HMODULE)instance;DisableThreadLibraryCalls(g_self);}
 return 1;
}
