/* URT V5 environment / compatibility layer. See urt_environment.h. */
#include "urt_environment.h"
#include "cr01_release_log.h"
#include <stddef.h>
#define WIN __stdcall
#define IMP __declspec(dllimport)
typedef void *HANDLE; typedef void *HMODULE; typedef unsigned long DWORD; typedef int BOOL;
typedef uint16_t WCHAR;
IMP HMODULE WIN GetModuleHandleW(const WCHAR*);
IMP BOOL WIN GetModuleHandleExW(DWORD,const WCHAR*,HMODULE*);
IMP DWORD WIN GetModuleFileNameW(HMODULE,WCHAR*,DWORD);
IMP void* WIN GetProcAddress(HMODULE,const char*);
IMP HMODULE WIN LoadLibraryW(const WCHAR*);
IMP DWORD WIN GetFileAttributesW(const WCHAR*);
IMP HANDLE WIN CreateMutexW(void*,BOOL,const WCHAR*);
IMP DWORD WIN GetLastError(void);
IMP DWORD WIN GetCurrentProcessId(void);
IMP DWORD WIN GetPrivateProfileStringW(const WCHAR*,const WCHAR*,const WCHAR*,WCHAR*,DWORD,const WCHAR*);
IMP DWORD WIN GetPrivateProfileSectionW(const WCHAR*,WCHAR*,DWORD,const WCHAR*);
#define ERROR_ALREADY_EXISTS 183ul
#define INVALID_ATTR 0xFFFFFFFFul

static unsigned wlen(const WCHAR*s){unsigned n=0;while(s&&s[n])n++;return n;}
static int wcat(WCHAR*d,unsigned cap,const WCHAR*s){unsigned i=wlen(d),j=0;while(s[j]){if(i+1>=cap)return 0;d[i++]=s[j++];}d[i]=0;return 1;}
static int acat(WCHAR*d,unsigned cap,const char*s){unsigned i=wlen(d),j=0;while(s[j]){if(i+1>=cap)return 0;d[i++]=(WCHAR)(unsigned char)s[j++];}d[i]=0;return 1;}

/* Folder of witcher3.exe (bin\x64_dx12\) with trailing slash. */
static int exe_dir(WCHAR*out,unsigned cap){
  DWORD n=GetModuleFileNameW(0,out,cap);
  if(!n||n>=cap)return 0;
  while(n&&out[n-1]!='\\'&&out[n-1]!='/')n--;
  if(!n)return 0;
  out[n]=0;return 1;
}
static int exe_file_exists(const char*name){
  WCHAR p[1024];
  if(!exe_dir(p,1024)||!acat(p,1024,name))return 0;
  DWORD a=GetFileAttributesW(p);
  return a!=INVALID_ATTR&&!(a&0x10ul);
}
static HMODULE loaded(const char*name){
  WCHAR w[64];unsigned i=0;
  while(name[i]&&i<63){w[i]=(WCHAR)(unsigned char)name[i];i++;}w[i]=0;
  return GetModuleHandleW(w);
}
static HMODULE self_module(void){
  HMODULE h=0;
  (void)GetModuleHandleExW(4ul|2ul,(const WCHAR*)(const void*)&urt_instance_claim,&h);
  return h;
}

int urt_instance_claim(const char **reason){
  static const char*why="";
  /* A legacy V4.x RefinedTracing.asi next to us is loaded by the same ASI loader
   * (and by the legacy dinput8 bootstrap). Two runtimes must never both own
   * CVars, code sites or shader slots: the legacy one keeps the process. */
  HMODULE legacy=loaded("RefinedTracing.asi");
  if(legacy&&legacy!=self_module()){why="legacy V4 RefinedTracing.asi is loaded";goto dormant;}
  if(exe_file_exists("RefinedTracing.asi")&&legacy!=self_module()){
    why="legacy V4 RefinedTracing.asi file is installed next to UltimateRefinedTracing.asi";goto dormant;}
  {
    /* Process-scoped named mutex: a second V5 copy (e.g. one in plugins\) stays dormant. */
    WCHAR name[96]={0};char digits[12];unsigned k=0;DWORD pid=GetCurrentProcessId();
    do{digits[k++]=(char)('0'+pid%10u);pid/=10u;}while(pid&&k<11);
    (void)acat(name,96,"Local\\UltimateRefinedTracing.Runtime.");
    while(k){char c[2]={digits[--k],0};(void)acat(name,96,c);}
    HANDLE m=CreateMutexW(0,0,name);
    if(!m){why="instance mutex unavailable";goto dormant;}
    if(GetLastError()==ERROR_ALREADY_EXISTS){why="another UltimateRefinedTracing.asi instance already runs";goto dormant;}
    /* Intentionally never closed: process lifetime ownership marker. */
  }
  if(reason)*reason="";
  return 1;
dormant:
  if(reason)*reason=why;
  return 0;
}

int urt_witcher_user_dir(uint16_t*out,unsigned cap){
  static const WCHAR shell[]={'s','h','e','l','l','3','2','.','d','l','l',0};
  WCHAR doc[1024];
  HMODULE sh=GetModuleHandleW(shell);
  if(!sh)sh=LoadLibraryW(shell);   /* witcher3.exe statically imports SHELL32 */
  if(!sh||!out||cap<32)return 0;
  typedef long (WIN*GETDOC)(void*,int,void*,DWORD,WCHAR*);
  GETDOC f=(GETDOC)GetProcAddress(sh,"SHGetFolderPathW");
  doc[0]=0;
  if(!f||f(0,5,0,0,doc)<0||!doc[0])return 0;   /* CSIDL_PERSONAL */
  out[0]=0;
  return wcat(out,cap,doc)&&acat(out,cap,"\\The Witcher 3");
}

/* Diagnostic only: 1 if dx12user.settings still carries the PT Shader
 * Compatibility value saved by RC2-RC8. URT never uses it for any decision. */
static __declspec(noinline) int obsolete_selector_present(void){
  static const WCHAR section[]={'R','e','f','i','n','e','d','T','r','a','c','i','n','g','C','R','0','1',0};
  static const WCHAR key[]={'P','T','S','h','a','d','e','r','C','o','m','p','a','t',0};
  WCHAR file[1100],v[8];
  if(!urt_witcher_user_dir(file,1100)||!acat(file,1100,"\\dx12user.settings"))return 0;
  v[0]=0;return GetPrivateProfileStringW(section,key,0,v,8,file)>0;
}

/* ---- V5.3: optional W3LoadFix (Nexus 13881) coexistence, read-only ----
 * Installed = w3loadfix.dll (its own dsound.dll loader) or w3loadfix.asi (loaded by an ASI loader) next to
 * witcher3.exe. Disabled = the "w3loadfix_disabled" file, or Enabled=false in [W3LoadFix] (w3loadfix.ini). */
static int w3lf_installed(void){return exe_file_exists("w3loadfix.dll")||exe_file_exists("w3loadfix.asi");}
static int w3lf_ini_path(WCHAR*p,unsigned cap){return exe_dir(p,cap)&&acat(p,cap,"w3loadfix.ini");}
static int w3lf_enabled(void){
  WCHAR ini[1100],v[16];
  static const WCHAR sec[]={'W','3','L','o','a','d','F','i','x',0},key[]={'E','n','a','b','l','e','d',0};
  if(!w3lf_installed()||exe_file_exists("w3loadfix_disabled"))return 0;
  if(!w3lf_ini_path(ini,1100))return 1;
  v[0]=0;(void)GetPrivateProfileStringW(sec,key,0,v,16,ini);
  return !((v[0]=='f'||v[0]=='F')&&(v[1]=='a'||v[1]=='A'))&&!(v[0]=='0'&&!v[1]);
}
unsigned urt_w3lf_override_section(char*out,unsigned cap){
  static const WCHAR sec[]={'O','v','e','r','r','i','d','e',0};
  static WCHAR buf[8192];
  WCHAR ini[1100];
  if(!out||cap<2||!w3lf_enabled()||!w3lf_ini_path(ini,1100))return 0;
  DWORD n=GetPrivateProfileSectionW(sec,buf,8192,ini);
  if(!n||n>=8190u)return 0;                       /* empty, absent, or truncated: treat as none */
  unsigned k=0;
  for(DWORD i=0;i<n&&k+2<cap;i++){WCHAR c=buf[i];out[k++]=(char)(c<128u?c:'?');}
  out[k++]=0;out[k]=0;return k;
}
static void line(const char*a){cr01_release_log_event(a);}
void urt_environment_report(void){
  static volatile DWORD once;
  if(__sync_val_compare_and_swap(&once,0ul,1ul))return;
  line("URT: compatibility scan (read-only) of bin\\x64_dx12");
  HMODULE winmm=loaded("winmm.dll"),dinput=loaded("dinput8.dll");
  if(winmm&&GetProcAddress(winmm,"IsUltimateASILoader"))line("URT: loader: Ultimate ASI Loader as winmm.dll (V5 MAIN default)");
  if(exe_file_exists("dinput8.dll")){
    if(dinput&&GetProcAddress(dinput,"CR01ProxyIdentityTestOnly"))
      line("URT: loader: legacy V4 URT dinput8.dll bootstrap present; harmless but obsolete, safe to remove when no other mod needs it");
    else if(dinput&&GetProcAddress(dinput,"IsUltimateASILoader"))
      line("URT: loader: Ultimate ASI Loader as dinput8.dll (e.g. Witcher Native Bridge); coexists with winmm.dll, each ASI loads once");
    else line("URT: loader: a dinput8.dll is installed (not yet loaded or not an ASI loader)");
  }
  if(exe_file_exists("WitcherNativeBridge.asi"))line("URT: detected Witcher Native Bridge; no shared resources with URT");
  if(exe_file_exists("BeardmosImmersiveWorld.asi"))line("URT: detected Beardmo's Immersive World (wind/grass/MinHook + ReShade add-on); no URT-owned CVar or code site overlap found in static analysis");
  if(exe_file_exists("BeardmosBlackHairFix.addon64"))
    line("URT: Beardmo's Black Hair Fix is installed: it is not a supported combination. URT installs its own PT shader as normal, so Black Hair Fix's PT Final hair edit does not apply");
  if(exe_file_exists("HairLightFixPT.asi"))line("URT: detected Orange Hair Fix PT; it recognises URT's PT library by identity (unchanged in V5)");
  if(exe_file_exists("xinput9_1_0.dll")&&exe_file_exists("rt_optimizer.log"))
    line("URT: detected PT Optimization (xinput9_1_0.dll): it patches the same PT shader, RT shadow sites and Physics LOD; URT refuses any site it does not find pristine - use one of the two mods");
  if(exe_file_exists("d3d12.dll")){
    if(exe_file_exists("d3d12_proxy.ini"))line("URT: detected LessStutter d3d12.dll proxy (pipeline cache); URT does not hook D3D12, no overlap");
    else line("URT: a local d3d12.dll is installed (ReShade or proxy); URT does not hook D3D12");
  }
  if(exe_file_exists("dxgi.dll")||exe_file_exists("ReShade.ini"))line("URT: ReShade-style dxgi/ini detected; URT does not hook DXGI/D3D12");
  if(exe_file_exists("..\\config\\platform\\pc\\dx12_vram_relief.ini"))line("URT: detected DX12 VRAM Relief ini: its values are the game's starting values; URT's Memory & Streaming rows override them only when set away from Native / Auto - use one or the other");
  if(exe_file_exists("..\\config\\platform\\pc\\w3_dx12_rt_memory_stabilizer.ini"))line("URT: detected DX12 RT Memory Stabilizer ini: URT's RT Memory Management rows override it only when set away from Native / Auto - use one or the other");
  if(w3lf_installed()){
    HMODULE ds=loaded("dsound.dll");
    if(exe_file_exists("w3loadfix.asi"))line("URT: detected W3LoadFix as an ASI plugin (loaded once by the ASI loader); independent mod, no shared hooks or code sites with URT");
    else if(ds&&!GetProcAddress(ds,"IsUltimateASILoader"))line("URT: detected W3LoadFix with its own dsound.dll loader; it coexists with URT's winmm.dll loader; independent mod, no shared hooks or code sites with URT");
    else line("URT: detected W3LoadFix files; independent mod, no shared hooks or code sites with URT");
    if(!w3lf_enabled())line("URT: W3LoadFix is installed but switched off (w3loadfix_disabled or Enabled=false)");
    if(exe_file_exists("w3loadfix.asi")&&exe_file_exists("w3loadfix.dll"))line("URT: both w3loadfix.asi and w3loadfix.dll are present; W3LoadFix runs only one copy (see its README)");
  }
  if(obsolete_selector_present())
    line("URT: an old PT Shader Compatibility value (removed) is still saved in dx12user.settings; it is ignored");
  line("URT: compatibility scan complete");
}
