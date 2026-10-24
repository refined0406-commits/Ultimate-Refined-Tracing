/* Patch 5.01 CR01 native-renderer event owner. No SBT/state-object ownership,
 * no PT/RT optimizers, no rendering worker or per-frame writer. All writes are
 * edge-driven, private and conditional on verified game slots. This module
 * runs ONLY after the DirectInput bootstrap outside DllMain. */
#include "cr01_menu_bridge.h"
#include "cr01_release_log.h"
#include "cr01_settings_store_windows.h"
#include "cr01_pt_carrier501.h"
#include "cr01_controls.h"
#include "cr01_shadow501.h"
#include "cr01_bone_hook501.h"
#include "cr01_distant_trees501.h"
#include "cr01_physics_lod501.h"
#include "cr01_indirect_live.h"
#include "cr01_rt_legacy501.h"
#include "cr01_dx12_safety.h"
#include "urt_cvar_index.h"
#include "urt_rtperf501.h"
#include "urt_exe_identity.h"
#include "urt_runtime.h"
#include "urt_environment.h"
#include <stdint.h>
#include <stddef.h>
#define WIN __stdcall
#define IMP __declspec(dllimport)
typedef uint16_t WCHAR;
typedef unsigned long DWORD;
typedef int BOOL;
typedef void *HANDLE;
typedef void *HMODULE;
typedef void *FARPROC;
#define NULL_HANDLE ((HANDLE)0)
#define INVALID_HANDLE ((HANDLE)(intptr_t)-1)
#define IMAGE_SCN_MEM_READ 0x40000000u
#define IMAGE_SCN_MEM_WRITE 0x80000000u
#define FILE_APPEND_DATA 4ul
#define GENERIC_READ 0x80000000ul
#define GENERIC_WRITE 0x40000000ul
#define CREATE_ALWAYS 2ul
#define OPEN_ALWAYS 4ul
#define OPEN_EXISTING 3ul
#define FILE_SHARE_READ 1ul
#define FILE_SHARE_WRITE 2ul
#define FILE_SHARE_DELETE 4ul
#define FILE_ATTRIBUTE_NORMAL 0x80ul
#define FILE_END 2ul
#define WAIT_OBJECT_0 0ul
#define FILE_NOTIFY_CHANGE_FILE_NAME 0x1ul
#define FILE_NOTIFY_CHANGE_LAST_WRITE 0x10ul
#define FILE_NOTIFY_CHANGE_SIZE 0x8ul
#define WAIT_TIMEOUT 258ul
#define CR01_MODE_FALLBACK_MS 50ul /* private experimental legacy RT transition latency */
#define CR01_SETTINGS_SETTLE_MS 15ul
#define CR01_SETTINGS_READ_RETRIES 8u
#define CR01_BIND_RETRIES 20u
#define CR01_BIND_RETRY_MS 100ul
#define FALSE_VALUE 0
IMP HMODULE WIN GetModuleHandleW(const WCHAR*);
IMP HMODULE WIN LoadLibraryW(const WCHAR*);
IMP FARPROC WIN GetProcAddress(HMODULE,const char*);
IMP DWORD WIN GetModuleFileNameW(HMODULE,WCHAR*,DWORD);
IMP HANDLE WIN CreateThread(void*,size_t,DWORD (WIN*)(void*),void*,DWORD,DWORD*);
IMP BOOL WIN CloseHandle(HANDLE);
IMP BOOL WIN ReadFile(HANDLE,void*,DWORD,DWORD*,void*);
IMP void* WIN VirtualAlloc(void*,size_t,DWORD,DWORD);
IMP DWORD WIN WaitForSingleObject(HANDLE,DWORD);
IMP void WIN Sleep(DWORD);
IMP HANDLE WIN FindFirstChangeNotificationW(const WCHAR*,BOOL,DWORD);
IMP BOOL WIN FindNextChangeNotification(HANDLE);
IMP BOOL WIN FindCloseChangeNotification(HANDLE);
IMP HANDLE WIN CreateFileW(const WCHAR*,DWORD,DWORD,void*,DWORD,DWORD,HANDLE);
IMP BOOL WIN WriteFile(HANDLE,const void*,DWORD,DWORD*,void*);
IMP DWORD WIN SetFilePointer(HANDLE,long,long*,DWORD);
IMP DWORD WIN GetFileAttributesW(const WCHAR*);
IMP BOOL WIN GetFileAttributesExW(const WCHAR*,int,void*);
IMP DWORD WIN GetPrivateProfileStringW(const WCHAR*,const WCHAR*,const WCHAR*,WCHAR*,DWORD,const WCHAR*);
extern int cr01_gate_verify_exe_file(void);
extern int cr01_shader_slots_active(void);
extern int cr01_gate_win_publish_settings(const CR01Settings*,uint32_t,uint32_t);
static int g_tier=URT_TIER_ADAPTIVE;
static int g_shader_state=URT_SHADER_NONE;
static CR01MenuBridge g_menu;
static CR01WindowsStore g_private_paths;
static CR01PrivateStore g_private_io;
static WCHAR g_userfile[1024],g_userdir[1024];
static DWORD g_epoch;
static volatile DWORD g_running;
static uint32_t *g_pt_slot,*g_rt_slot,*g_radiance_slot,*g_indirect_slot;
static CR01Owner g_indirect_owner;
static unsigned g_ready;
static unsigned g_mode_initialized;
static unsigned g_warn_save_once;
static unsigned g_warn_snapshot_once;
static unsigned g_live_event_confirmed;
static unsigned g_user_stamp_valid;
static DWORD g_user_stamp_lo,g_user_stamp_hi,g_user_size_lo,g_user_size_hi;
static uint32_t u32(const uint8_t*p){return (uint32_t)p[0]|(uint32_t)p[1]<<8|
  (uint32_t)p[2]<<16|(uint32_t)p[3]<<24;}

static int wide_append(WCHAR*buf,unsigned cap,const WCHAR*s){
 unsigned a=0,b=0;while(a<cap&&buf[a])a++;if(a==cap)return 0;
 while(s[b]){if(a+1>=cap)return 0;buf[a++]=s[b++];}buf[a]=0;return 1;
}
static int build_paths(void){
 static const WCHAR shell[]={ 's','h','e','l','l','3','2','.','d','l','l',0};
 static const WCHAR slash[]={ '\\',0};
 static const WCHAR userdir[]={ 'T','h','e',' ','W','i','t','c','h','e','r',' ','3',0};
 static const WCHAR settings[]={ 'd','x','1','2','u','s','e','r','.','s','e','t','t','i','n','g','s',0};
 static WCHAR doc[1024];
 HMODULE sh=LoadLibraryW(shell);
 if(!sh)return 0;
 typedef long (WIN *GETDOC)(void*,int,void*,DWORD,WCHAR*);
 GETDOC getdoc=(GETDOC)(void*)GetProcAddress(sh,"SHGetFolderPathW");
 if(!getdoc||getdoc(0,5,0,0,doc)<0)return 0;
 for(unsigned i=0;i<1024;i++){g_userdir[i]=doc[i];if(!doc[i])break;}
 if(!wide_append(g_userdir,1024,slash)||!wide_append(g_userdir,1024,userdir))return 0;
 for(unsigned i=0;i<1024;i++){g_userfile[i]=g_userdir[i];if(!g_userfile[i])break;}
 if(!wide_append(g_userfile,1024,slash)||!wide_append(g_userfile,1024,settings))return 0;
 return 1;
}
/* V5: CVar registry resolution uses a one-pass index (urt_cvar_index.c) with
 * the exact acceptance rules of the V4.x LC44 resolver (unique writable
 * game-owned DWORD backing, ambiguous names fail closed; never resolve NAPL
 * camera lights). V4.x re-scanned the whole image per name; the host test
 * tests/host/test_cvar_index.c proves identical results on Patch 5.01. */
#define URT_CVAR_CAPACITY 16384u
static UrtCvarIndex g_cvar_ix;
static UrtCvarEntry *g_cvar_store;
static int cvar_index_refresh(uint8_t*base,uint32_t image){
 if(!g_cvar_store)g_cvar_store=(UrtCvarEntry*)VirtualAlloc(0,URT_CVAR_CAPACITY*sizeof(UrtCvarEntry),0x3000ul,0x04ul);
 if(!g_cvar_store)return 0;
 return urt_cvar_index_build(&g_cvar_ix,base,image,g_cvar_store,URT_CVAR_CAPACITY);
}
static uint32_t *find_cvar(uint8_t*base,uint32_t image,const char*name){
 if(!g_cvar_ix.base&&!cvar_index_refresh(base,image))return 0;
 if(g_cvar_ix.base!=base||g_cvar_ix.image_size!=image)return 0;
 return urt_cvar_lookup(&g_cvar_ix,name);
}
const UrtCvarIndex *urt_runtime_cvar_index(void){return g_cvar_ix.base?&g_cvar_ix:0;}
/* True Witcher renderer state comes only from validated CDPR engine CVars.
 * A missing/unknown value keeps all CR01 writers passive. */
uint32_t*cr01_legacy_approved_find_cvar(uint8_t*base,uint32_t image,const char*name){return find_cvar(base,image,name);}
static int detect_mode(CR01RenderMode*mode){
 if(!mode||!g_pt_slot)return 0;
 uint32_t pt=__atomic_load_n(g_pt_slot,__ATOMIC_ACQUIRE);
 if(pt>1)return 0;
 if(pt){*mode=CR01_RENDER_PT;return 1;}
 if(!g_rt_slot||!g_radiance_slot)return 0;
 uint32_t rt=__atomic_load_n(g_rt_slot,__ATOMIC_ACQUIRE);
 uint32_t rad=__atomic_load_n(g_radiance_slot,__ATOMIC_ACQUIRE);
 if(rt>1||rad>1)return 0;
 *mode=rt&&rad?CR01_RENDER_NORMAL_RT:CR01_RENDER_RASTER;return 1;
}
static int ascii(WCHAR*s,char*out,unsigned cap){
 unsigned i=0;while(s[i]&&i+1<cap){if(s[i]>127)return 0;out[i]=(char)s[i];i++;}
 if(s[i])return 0;out[i]=0;return 1;
}
static const WCHAR kSection[]={ 'R','e','f','i','n','e','d','T','r','a','c','i','n','g','C','R','0','1',0};
/* ---- V5 coherent settings snapshot -------------------------------------
 * One read of dx12user.settings per event; every key answered from that
 * snapshot. g_snap_mode is cleared for the session if the startup self-check
 * finds ANY difference from GetPrivateProfileStringW (V4.3.1 path retained). */
#include "urt_ini.h"
static UrtIniSection g_snap;
static int g_snap_mode=1, g_snap_ok, g_snap_checked, g_snap_trusted;
static uint8_t *g_snap_buf;
#define URT_SNAP_CAP (1024u*1024u)
static int snapshot_refresh(void){
 HANDLE h;DWORD got=0,total=0;
 g_snap_ok=0;
 if(!g_snap_mode)return 0;
 if(!g_snap_buf)g_snap_buf=(uint8_t*)VirtualAlloc(0,URT_SNAP_CAP,0x3000ul,0x04ul);
 if(!g_snap_buf)return 0;
 h=CreateFileW(g_userfile,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
 if(h==INVALID_HANDLE||!h)return 0;
 for(;;){
  if(total>=URT_SNAP_CAP){CloseHandle(h);return 0;}   /* larger than expected: API path */
  if(!ReadFile(h,g_snap_buf+total,URT_SNAP_CAP-total,&got,0)){CloseHandle(h);return 0;}
  if(!got)break;total+=got;
 }
 CloseHandle(h);
 g_snap_ok=urt_ini_parse(g_snap_buf,total,"RefinedTracingCR01",&g_snap);
 return g_snap_ok;
}
__declspec(noinline) DWORD urt_cfg_get(const WCHAR*key,WCHAR*out,DWORD cap){
 char k[96],v[128];unsigned i=0;
 if(!key||!out||!cap)return 0;
 if(g_snap_mode&&g_snap_trusted&&g_snap_ok&&cap<=128u){
  while(key[i]&&i<95){if(key[i]>127)goto api;k[i]=(char)key[i];i++;}
  if(key[i])goto api;
  k[i]=0;
  unsigned n=urt_ini_get(&g_snap,k,v,(unsigned)cap);
  for(unsigned j=0;j<=n;j++)out[j]=(WCHAR)(unsigned char)v[j];
  return n;
 }
api:
 return GetPrivateProfileStringW(kSection,key,(const WCHAR*)0,out,cap,g_userfile);
}
static const char*const kSelfCheckKeys[]={"MasterEnabled","QuickPresetSelect","PTShadows","PTBillboards",
 "GeometryBounces","HairBounces","IndirectLight","SunImpact","SkyImpact","AmbientOcclusion","AOStrength",
 "AORadius","AOContrast","GrassDay","GrassNight","SpecularScreenSpace","SpecularCheckerboard","PTRTAOEnabled",
 "PTRTAORayCount","PTRTAORadius","PTRTAOStrength","CPUFastBoneReads","CPUDistantTrees","CPUPhysicsLOD",
 "RayTracedOptimizations","RTPerfClassificationPrimary","RTPerfClassificationSecondary","RTPerfMaxProbes",
 "RTPerfAOQuality","RTPerfShadingBudget","RTPerfGlobalShadingLimit",
 "RTPerfGrid1ShadingLimit","RTQualityClassificationPrimary","RTQualityClassificationSecondary",
 "RTQualityMaxProbes","RTQualityAOQuality","RTQualityShadingBudget",
 "RTQualityGlobalShadingLimit","RTQualityGrid1ShadingLimit"};
/* Startup: snapshot must agree with the Windows API for every key URT reads.
 * V5.1.0-rc3: the removed RTGI Shading Rays Per Probe keys are never read. */
static __declspec(noinline) void snapshot_self_check(void){
 if(g_snap_checked||!g_snap_mode)return;
 if(!snapshot_refresh())return;      /* retry at the next refresh */
 g_snap_checked=1;
 for(unsigned i=0;i<sizeof(kSelfCheckKeys)/sizeof(kSelfCheckKeys[0]);i++){
  WCHAR wk[64],a[80],b[80];unsigned j=0;DWORD na,nb;
  while(kSelfCheckKeys[i][j]){wk[j]=(WCHAR)kSelfCheckKeys[i][j];j++;}wk[j]=0;
  {char k[64],v[80];unsigned q2=0;while(kSelfCheckKeys[i][q2]){k[q2]=kSelfCheckKeys[i][q2];q2++;}k[q2]=0;
   na=urt_ini_get(&g_snap,k,v,80);for(unsigned z=0;z<=na;z++)a[z]=(WCHAR)(unsigned char)v[z];}
  nb=GetPrivateProfileStringW(kSection,wk,(const WCHAR*)0,b,80,g_userfile);
  int same=na==nb;
  for(DWORD q=0;same&&q<na;q++)if(a[q]!=b[q])same=0;
  if(!same){
   g_snap_mode=0;g_snap_ok=0;g_snap_trusted=0;
   cr01_release_log_event("URT: settings snapshot self-check differed from Windows INI API; using the V4.3.1 per-key read path this session");
   return;
  }
 }
 g_snap_trusted=1;
 cr01_release_log_event("URT: settings snapshot self-check passed; one coherent read per settings event");
}
/* Bone setting is intentionally separate from the 37-control codec V2.
 * Missing user_config key (Witcher prunes defaults) means Off, not an old
 * implicit On. Off still leaves the QA02 private-settings bytes unchanged. */
static int cpu_bone_preference(void){
 static const WCHAR key[]={ 'C','P','U','F','a','s','t','B','o','n','e','R','e','a','d','s',0};
 WCHAR val[16];DWORD n=urt_cfg_get(key,val,16);
 if(n==0)return 0; /* menu default Off, often omitted by the game */
 if(n==1&&val[0]=='0')return 0;
 if(n==1&&val[0]=='1')return 1;
 if(n==4&&val[0]=='t'&&val[1]=='r'&&val[2]=='u'&&val[3]=='e')return 1;
 return 0; /* unknown input never enables a native patch */
}
static void cpu_bone_refresh(void){
 static int previous=-1;
 int desired=(g_menu.controller.saved.master==1u && cpu_bone_preference());
 if(desired==previous)return;
 if(cr01_bone501_set((unsigned)desired)){
  previous=desired;
  cr01_release_log_event(desired?"URT: CPU Fast Bone Reads On":
                               "URT: CPU Fast Bone Reads Off (game code path restored)");
 }else if(desired){
  previous=-1;
  cr01_release_log_event("URT: CPU Fast Bone Reads refused (game code not in the expected state, e.g. owned by another mod); native behaviour kept");
 }else previous=-1; /* an Off-restoration failure requires another attempt */
}
/* CPU Distant Trees uses separate native billboard proximity eligibility,
 * live on the CR01 worker. Changing CVar is immediate for new instance
 * decisions; existing GPU geometry requires scene rebuild (area reload QA). */
static int dt_current_pref;
static int dt_last_report=-999;
static int cpu_dt_preference(void){
 static const WCHAR key[]={ 'C','P','U','D','i','s','t','a','n','t','T','r','e','e','s',0};
 WCHAR val[16];DWORD n=urt_cfg_get(key,val,16);
 if(n==1 && val[0]=='1')return 1;
 if(n==4 && val[0]=='t'&&val[1]=='r'&&val[2]=='u'&&val[3]=='e')return 1;
 return 0;
}
static void cpu_dt_refresh(unsigned mode){
 int st=cr01_dt501_refresh(mode,g_menu.controller.saved.master,(unsigned)dt_current_pref,
                         g_menu.controller.saved.v[F_BILLBOARDS]);
 if(st!=dt_last_report){
  dt_last_report=st;
  switch(st){
  case 0:cr01_release_log_event("CR01: CPU Distant Trees Off (native RT billboards restored)\r\n");break;
  case 1:cr01_release_log_event("CR01: CPU Distant Trees On (native RT billboard proximity policy; reload area if old scene persists)\r\n");break;
  case 2:cr01_release_log_event("CR01: CPU Distant Trees On (native proximity gate already active)\r\n");break;
  case 3:cr01_release_log_event("CR01: CPU Distant Trees temporarily suspended (PT Billboards On; preference preserved; reload area if existing scene persists)\r\n");break;
  default:cr01_release_log_event("URT: CPU Distant Trees refused (engine setting changed by something else); native behaviour kept\r\n");break;
  }
 }
}
/* Third CPU feature: physics LOD budget, independent of renderer.
 * The Patch 5.01 Physics/LODBudget registration has a pinned default of
 * 25.0f and a writable current-value field at image RVA 0x512EE58.
 * We use its exact native CVar slot; NO physics work is skipped. */
static int physics_pref,physics_last=-999;
static int cpu_physics_preference(void){
 static const WCHAR key[]={'C','P','U','P','h','y','s','i','c','s','L','O','D',0};
 WCHAR val[16];DWORD n=urt_cfg_get(key,val,16);
 if(n==1&&val[0]=='1')return 1;
 if(n==4&&val[0]=='t'&&val[1]=='r'&&val[2]=='u'&&val[3]=='e')return 1;
 return 0;
}
static void cpu_physics_refresh(void){
 int st=cr01_physics501_refresh(g_menu.controller.saved.master,(unsigned)physics_pref);
 if(st==physics_last)return;
 physics_last=st;
 switch(st){
 case 0: cr01_release_log_event("CR01: CPU Physics Level of Detail Off (original physics budget restored)\r\n");break;
 case 1: cr01_release_log_event("CR01: CPU Physics Level of Detail On (native budget 25 to 5)\r\n");break;
 case 2: cr01_release_log_event("CR01: CPU Physics Level of Detail already at target, not owned\r\n");break;
 default:cr01_release_log_event("URT: CPU Physics Level of Detail refused (engine setting changed by something else); native behaviour kept\r\n");break;
 }
}
static const char*const kNames[]={"MasterEnabled","PTShadows","PTBillboards","GeometryBounces","HairBounces",
 "IndirectLight","SunImpact","SkyImpact","AmbientOcclusion","AOStrength","AORadius","AOContrast",
 "GrassDay","GrassNight","SpecularScreenSpace","SpecularCheckerboard",
 "PTRTAOEnabled","PTRTAORayCount","PTRTAORadius","PTRTAOStrength"};
/* Read-only three-state menu import. Missing keys legitimately occur because
 * Witcher omits default-valued CR01 options. A malformed PRESENT value is an
 * error, not permission to overwrite game-owned configuration at startup. */
static int read_menu_value(unsigned j,char value[80]){
 WCHAR wideKey[80],result[80];unsigned i=0;
 if(j>=sizeof(kNames)/sizeof(kNames[0])||!value)return -1;
 while(kNames[j][i]&&i<79){wideKey[i]=(WCHAR)kNames[j][i];i++;}wideKey[i]=0;
 result[0]=0;
 DWORD n=urt_cfg_get(wideKey,result,80);
 if(!n)return 0;
 if(n>=79||!ascii(result,value,80)||
    !cr01_menu_bridge_valid_input(&g_menu,kNames[j],value))return -1;
 return 1;
}
static int dx12_settings_file_nonempty(void);
typedef struct CR01_FILETIME_TAG{DWORD lo,hi;} CR01_FILETIME;
typedef struct CR01_FILE_ATTR_TAG{
 DWORD attrs;CR01_FILETIME creation,access,write;DWORD size_hi,size_lo;
} CR01_FILE_ATTR;
/* DX12 settings safety gate (private FIX03). This file belongs to the game.
 * Missing/zero-byte/attribute-unreadable input is never permission for our
 * startup INI repair to create or populate a replacement. Require an existing,
 * readable, nonempty, ordinary file immediately before each startup mutation.
 * This is not evidence that Witcher itself has finished initialization;
 * Windows testing must establish that lifecycle before promoting this branch. */
static int dx12_settings_file_nonempty(void){
 CR01_FILE_ATTR d;
 int ok=GetFileAttributesExW(g_userfile,0,&d)!=0;
 HANDLE h;
 if(!cr01_dx12_preflight_safe(ok,ok?d.attrs:0u,ok?d.size_hi:0u,ok?d.size_lo:0u))return 0;
 /* Metadata alone cannot prove a file is readable. Read-only OPEN_EXISTING
  * must succeed; never create a missing game config as a side effect. */
 h=CreateFileW(g_userfile,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
               0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
 if(h==INVALID_HANDLE||!h)return 0;
 return CloseHandle(h)!=0;
}
/* The directory containing Witcher settings is noisy. Filter OS directory wakes
 * against the exact dx12user.settings identity before reading any CR01 values.
 * This is edge-driven metadata filtering, not periodic settings polling. */
static int settings_stamp_sample(int report_change){
 CR01_FILE_ATTR d;
 if(!GetFileAttributesExW(g_userfile,0,&d))return 0;
 if(!g_user_stamp_valid){
  g_user_stamp_lo=d.write.lo;g_user_stamp_hi=d.write.hi;
  g_user_size_lo=d.size_lo;g_user_size_hi=d.size_hi;g_user_stamp_valid=1u;
  return report_change?1:0;
 }
 if(d.write.lo==g_user_stamp_lo&&d.write.hi==g_user_stamp_hi&&
    d.size_lo==g_user_size_lo&&d.size_hi==g_user_size_hi)return 0;
 g_user_stamp_lo=d.write.lo;g_user_stamp_hi=d.write.hi;
 g_user_size_lo=d.size_lo;g_user_size_hi=d.size_hi;
 return 1;
}
/* V4.0.1 private DX12 owner-protection candidate. Import only already
 * persisted valid menu keys. Missing keys retain the separately persisted CR01
 * SavedPreferences; they MUST NOT trigger native INI repair/writes. This
 * preserves the established FIX5 valid-key-authoritative and partial-overlay
 * policies while removing native startup mutation of game-owned graphics data.
 * The in-game menu can still display defaults for omitted keys; Windows menu
 * QA must verify this edge, especially upgraded users. */
static int startup_sync_menu_once(void){
 char values[sizeof(kNames)/sizeof(kNames[0])][80];
 const char*ptrs[sizeof(kNames)/sizeof(kNames[0])];
 unsigned char present[sizeof(kNames)/sizeof(kNames[0])];
 unsigned count=(unsigned)(sizeof(kNames)/sizeof(kNames[0]));
 unsigned any_present=0;
 uint32_t before=g_menu.dirty_generation;
 if(!dx12_settings_file_nonempty()){
  cr01_release_log_event("URT: DX12 settings missing/empty/unreadable; startup synchronization deferred until the game writes a valid file\r\n");
  return 0;
 }
 snapshot_self_check();
 (void)snapshot_refresh();
 for(unsigned j=0;j<count;j++){
  int status=read_menu_value(j,values[j]);
  if(status<0){
   cr01_release_log_event("URT: invalid URT value in dx12user.settings; startup import refused (nothing written)\r\n");
   return 0;
  }
  present[j]=(unsigned char)(status==1);
  ptrs[j]=present[j]?values[j]:"";
  any_present+=present[j];
 }
 if(any_present){
  if(!cr01_menu_bridge_snapshot_partial(&g_menu,kNames,ptrs,present,count))return 0;
  if(g_menu.dirty_generation!=before && !cr01_store_flush(&g_menu,&g_private_io)){
   if(!g_warn_save_once++)cr01_release_log_event("URT: UltimateRefinedTracing.settings save refused; URT stays inactive this session\r\n");
   return 0;
  }
 }
 g_warn_save_once=0;
 return 1;
}
/* Witcher may omit default-valued user-config keys from dx12user.settings.
 * LC12's proven settings carrier therefore treated each persisted key as optional.
 * Preserve that behavior without restoring LC12 polling: after an exact file edge,
 * overlay only keys that are actually present on top of SavedPreferences, then
 * commit the merged state atomically. Present-but-invalid values still fail closed. */
static int check_menu_file_changes(void){
 if(!dx12_settings_file_nonempty())return -1; /* invalid/missing exact file */
 char values[sizeof(kNames)/sizeof(kNames[0])][80];
 char fallback[sizeof(kNames)/sizeof(kNames[0])][80];
 const char*ptrs[sizeof(kNames)/sizeof(kNames[0])];
 unsigned char present[sizeof(kNames)/sizeof(kNames[0])];
 uint32_t before=g_menu.dirty_generation;
 uint32_t old_master=g_menu.controller.saved.master;
 unsigned any_present=0;
 unsigned count=(unsigned)(sizeof(kNames)/sizeof(kNames[0]));
 for(unsigned j=0;j<count;j++){
  WCHAR wideKey[80],result[80];unsigned i=0;DWORD n;char asciiValue[80];
  while(kNames[j][i]&&i<79){wideKey[i]=(WCHAR)kNames[j][i];i++;}wideKey[i]=0;
  result[0]=0;asciiValue[0]=0;values[j][0]=0;present[j]=0;
  n=urt_cfg_get(wideKey,result,80);
  if(n){
   if(n>=79||!ascii(result,asciiValue,80)||!cr01_menu_bridge_valid_input(&g_menu,kNames[j],asciiValue))return -1;
   for(i=0;asciiValue[i]&&i<79;i++)values[j][i]=asciiValue[i];values[j][i]=0;
   ptrs[j]=values[j];present[j]=1u;any_present=1u;
  }else{
   if(!cr01_menu_bridge_saved_text(&g_menu,kNames[j],fallback[j],sizeof(fallback[j])))return -1;
   ptrs[j]=fallback[j];
  }
 }
 if(!any_present)return 0;
 (void)cr01_menu_bridge_snapshot_partial(&g_menu,kNames,ptrs,present,count);
 if(g_menu.dirty_generation==before)return 0;
 if(!cr01_store_flush(&g_menu,&g_private_io)){
  if(!g_warn_save_once++)cr01_release_log_event("URT: UltimateRefinedTracing.settings save refused; preferences stay active for this session only\r\n");
 }else g_warn_save_once=0;
 if(!g_live_event_confirmed++)cr01_release_log_event("URT: live settings changes confirmed\r\n");
 if(old_master!=g_menu.controller.saved.master)
  cr01_release_log_event(g_menu.controller.saved.master?
    "URT: Master On; saved preferences applied for the active renderer\r\n":
    "URT: Master Off; every URT-owned value restored to native\r\n");
 return 1;
}
static int consume_exact_settings_edge(void){
 int r;
 /* Retry only while Witcher is in a replacement/write stage. Missing CR01 keys
  * are not an error: they retain SavedPreferences, matching the old proven carrier. */
 for(unsigned attempt=0;attempt<CR01_SETTINGS_READ_RETRIES;attempt++){
  if(attempt)Sleep(CR01_SETTINGS_SETTLE_MS);
  snapshot_self_check();
  (void)snapshot_refresh();
  r=check_menu_file_changes();
  if(r>=0){g_warn_snapshot_once=0;return r;}
 }
 if(!g_warn_snapshot_once++)
  cr01_release_log_event("URT: dx12user.settings changed but a URT value was invalid/unreadable; current state kept\r\n");
 return 0;
}
static void publish_now(CR01RenderMode mode,int indirect_exclusive){
 /* Strictly CPU-side staging into the validated native serializer gate. The
  * game still owns DXR execution. Render-side authorization is checked at the
  * exact native serialize call, so no shader-readiness polling is required. */
 static unsigned warn_once;
 /* V5: shader-carried controls exist only while URT's PT shader is live. */
 if(g_shader_state!=URT_SHADER_ACTIVE)return;
 CR01Settings shader_delivery=g_menu.controller.saved;
 /* Preserve SavedPreferences while only suppressing an unsafe shader gain. */
 if(!indirect_exclusive)shader_delivery.v[F_INDIRECT]=100u;
 if(++g_epoch==0u)g_epoch=1u;
 if(!cr01_gate_win_publish_settings(&shader_delivery,
                      mode==CR01_RENDER_PT?1u:0u,g_epoch)){
   if(!warn_once++)cr01_release_log_event("URT: PT shader controls could not be staged (refused); native values kept\r\n");
 }else warn_once=0;
}
static unsigned resolve_renderer_bindings(uint8_t*base,uint32_t image){
 static const struct {unsigned f;const char*name;} bindings[]={
  {F_SUN,"PTSunImpact"},{F_SKY,"PTSkyImpact"},{F_AO,"PTAOEnable"},
  {F_AO_STRENGTH,"PTAOStrength"},{F_AO_RADIUS,"PTAORadius"},
  {F_AO_CONTRAST,"PTAOFinalValuePower"},
  {F_GRASS_DAY,"PTGrassScale"},{F_GRASS_NIGHT,"PTGrassNightScale"},
  {F_SPEC_SS,"PTLateSpecularScreenSpaceTrace"},
  {F_SPEC_CHECKER,"PTLateSpecularCheckerboard"},
  {F_BILLBOARDS,"RTSTBillboards"}
 };
 unsigned ready=0;
 if(!g_pt_slot)g_pt_slot=find_cvar(base,image,"PTEnable");
 if(!g_rt_slot)g_rt_slot=find_cvar(base,image,"EnableRT");
 if(!g_radiance_slot)g_radiance_slot=find_cvar(base,image,"EnableRtRadiance");
 if(!g_indirect_slot)g_indirect_slot=find_cvar(base,image,"PTIndirectLightMultiplier");
 if(g_pt_slot&&g_rt_slot&&g_radiance_slot)ready+=3;
 if(g_indirect_slot)ready++;
 for(unsigned i=0;i<sizeof(bindings)/sizeof(bindings[0]);i++){
  uint32_t*slot=g_menu.controller.slots[bindings[i].f];
  if(!slot)slot=find_cvar(base,image,bindings[i].name);
  cr01_native_slot(&g_menu.controller,bindings[i].f,slot);
  if(g_menu.controller.slots[bindings[i].f])ready++;
 }
 return ready;
}
static int resolve_renderer_bindings_startup(uint8_t*base,uint32_t image){
 /* LC12-proven pattern: renderer registry discovery may race early startup.
  * Retry is bounded and startup-only; there is no background discovery loop. */
 for(unsigned attempt=0;attempt<CR01_BIND_RETRIES;attempt++){
  if(attempt)(void)cvar_index_refresh(base,image);
  if(resolve_renderer_bindings(base,image)==15u)return 1;
  if(attempt+1u<CR01_BIND_RETRIES)Sleep(CR01_BIND_RETRY_MS);
 }
 return 0;
}

static DWORD WIN worker(void*unused){
 (void)unused;
 if(!build_paths())return 0;
 cr01_menu_bridge_init(&g_menu);
 if(!cr01_store_windows_init(&g_private_paths,&g_private_io,g_userdir)){
  cr01_release_log_event("URT: Documents\\The Witcher 3 unavailable for UltimateRefinedTracing.settings; URT stays inactive this session\r\n");
  return 0;
 }
 {
  /* V5: a damaged/unreadable private file no longer disables URT. The
   * preferences the menu persisted in dx12user.settings remain authoritative
   * and are overlaid at startup sync; the private file only mirrors keys the
   * game prunes at their defaults. */
  int source=cr01_store_load_resilient(&g_menu,&g_private_io);
  if(source==1)cr01_release_log_event("URT: UltimateRefinedTracing.settings was invalid; restored the last known-good .bak copy (invalid file kept as .corrupt)\r\n");
  else if(source==2)cr01_release_log_event(g_private_paths.commit_blocked?
    "URT: UltimateRefinedTracing.settings unreadable (locked/I-O error); using defaults + menu values this session, file left untouched\r\n":
    "URT: UltimateRefinedTracing.settings invalid and no valid backup; using defaults + menu values (invalid file kept as .corrupt)\r\n");
 }
 /* The migration read never writes the old file. Persist a complete,
  * validated copy into the new canonical path. A failed commit keeps the
  * migrated preferences live in memory and retries at the next save. */
 if(g_private_paths.legacy_loaded){
  unsigned from=g_private_paths.legacy_loaded;
  g_menu.dirty_generation++;
  if(!cr01_store_flush(&g_menu,&g_private_io))
   cr01_release_log_event("URT: legacy private preferences loaded; first copy to UltimateRefinedTracing.settings refused, will retry on next save\r\n");
  else cr01_release_log_event(from==1u?"URT: migrated RefinedTracing.settings to UltimateRefinedTracing.settings (old file kept for rollback)\r\n":
    "URT: migrated RefinedTracingCR01.settings to UltimateRefinedTracing.settings (old file kept for rollback)\r\n");
 }
 cr01_release_log_event("URT: preferences file ready: Documents\\The Witcher 3\\UltimateRefinedTracing.settings");
 cr01_release_log_event(cr01_shadow501_ready()?"URT: PT Ray-Traced Shadows control ready\r\n":
   "URT: PT Ray-Traced Shadows control unavailable (refused); the game's own shadow setting applies\r\n");
 urt_environment_report();
 cr01_release_log_event(g_tier==URT_TIER_FULL?"URT: runtime tier FULL (known Patch 5.01 code, early start)":
   g_tier==URT_TIER_LATE?"URT: runtime tier LATE (known code, late start: CVar/CPU features only)":
   "URT: runtime tier ADAPTIVE (unknown executable: name-resolved CVar features only; code patches and PT shader off)");
 cr01_release_log_event("URT: camera-light settings are never changed (compatible with camera-light mods such as NAPL)");
 uint8_t*base=(uint8_t*)GetModuleHandleW(0);
 if(!base||u32(base+0x3c)>0x1000)return 0;
 uint32_t image=u32(base+u32(base+0x3c)+24+56);
 if(g_tier!=URT_TIER_ADAPTIVE&&image!=0x6402000u)return 0;
 if(!image||image>0x20000000u)return 0;
 if(!resolve_renderer_bindings_startup(base,image))
  cr01_release_log_event("URT: some engine settings could not be resolved after retrying; those controls stay native\r\n");
 else
  cr01_release_log_event("URT: engine settings resolved; live controls available\r\n");
 if(cr01_shader_slots_active())
  cr01_release_log_event("URT: PT shader slots installed; validated DXC shader SHA-256 f1ad5c374ae1188508568d621e8be64ede3f2167e4ab06dcfca5a6781746e6ac (identity unchanged since V4.2.x)");
 else
  cr01_release_log_event("URT: PT shader slots not installed; shader-driven controls remain native (other controls unaffected)\r\n");
 CR01RenderMode last=CR01_RENDER_RASTER;
 if(detect_mode(&last)){
  g_mode_initialized=1u;
  (void)cr01_menu_bridge_mode(&g_menu,last);
  cr01_release_log_event(last==CR01_RENDER_PT ? "CR01: initial renderer=PT; isolation active" : (last==CR01_RENDER_NORMAL_RT ? "CR01: initial renderer=Normal RT; isolation active" : "CR01: initial renderer=raster; isolation active"));
 }else{
  cr01_release_log_event("URT: renderer not active yet; preferences apply at the first renderer start\r\n");
 }
 HANDLE watch=FindFirstChangeNotificationW(g_userdir,0,
                   FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_LAST_WRITE|FILE_NOTIFY_CHANGE_SIZE);
 if(watch==INVALID_HANDLE||!watch){
  cr01_release_log_event("URT: settings change notifications unavailable (refused); URT stays inactive this session\r\n");
  (void)cr01_native_shutdown(&g_menu.controller);return 0;
 }
 /* FIX5-proven ordering: arm the file watcher first, then perform one bounded
  * startup import/repair pass so any concurrent Witcher save is observable. */
 /* V5 recovery: V4.3.1 stopped the runtime for the whole session when the
  * game's settings file was missing/empty/invalid at launch. V5 keeps every
  * URT feature native (nothing applied, nothing written) and retries the same
  * read-only startup import on each change of the settings directory, so the
  * session recovers as soon as the game saves a valid file. No timer reads. */
 {
  unsigned waiting=0;
  while(!startup_sync_menu_once()){
   if(!waiting++)cr01_release_log_event("URT: waiting for a valid dx12user.settings; URT stays native until the game saves one (no URT writes)\r\n");
   for(;;){
    DWORD w=WaitForSingleObject(watch,1000ul);
    if(__atomic_load_n(&g_running,__ATOMIC_ACQUIRE)!=1u){
     (void)FindCloseChangeNotification(watch);(void)cr01_native_shutdown(&g_menu.controller);return 0;}
    if(w==WAIT_OBJECT_0){
     if(!FindNextChangeNotification(watch)){
      cr01_release_log_event("URT: settings change notifications lost while waiting; URT stays inactive this session\r\n");
      (void)FindCloseChangeNotification(watch);(void)cr01_native_shutdown(&g_menu.controller);return 0;}
     break;
    }
   }
  }
  if(waiting)cr01_release_log_event("URT: valid dx12user.settings detected; startup synchronization completed (recovered)\r\n");
 }
 /* Seed exact-file metadata after startup repair/import so broad directory
  * activity cannot masquerade as a CR01 settings event. */
 (void)settings_stamp_sample(0);
 cr01_release_log_event("URT: saved preferences synchronized; watching for settings changes\r\n");
 cr01_release_log_event(g_menu.controller.saved.master?
   "CR01: Master state at startup: On\r\n":
   "CR01: Master state at startup: Off\r\n");
 if(g_mode_initialized){
  int exclusive=cr01_indirect_shader_exclusive(&g_indirect_owner,g_indirect_slot,&g_menu.controller.saved,last);
  if(!exclusive&&g_menu.controller.saved.master&&last==CR01_RENDER_PT&&g_menu.controller.saved.v[F_INDIRECT]!=100u)
    cr01_release_log_event("URT: Indirect-Light Compensation paused: the native indirect-light setting was changed by something else (refused to override)\r\n");
  publish_now(last,exclusive);
  (void)cr01_shadow501_update(&g_menu.controller.saved,last==CR01_RENDER_PT);
 }
 g_ready=1;
 /* Private legacy Normal RT engine CVar adapter. No changes on install or
  when advanced controls remain at their native Auto defaults. */
 if(!cr01_rt_legacy501_init(base,image))
  cr01_release_log_event("URT: Normal RT: RTGI settings binding rejected; native Normal RT values kept\r\n");
 else {
  (void)cr01_rt_legacy501_reload(g_userfile);
  cr01_release_log_event("URT: Normal RT settings verified; ready\r\n");
 }
 /* V5.1: Normal RT performance controls (all Native by default: no writes). */
 if(urt_rtperf501_init(base,image))(void)urt_rtperf501_reload();
 cpu_bone_refresh();
 /* Pin exact native backing and registered default for this single EXE.
  * This intentionally rejects a different game binary/layout. */
 {
  uint32_t *budget=find_cvar(base,image,"LODBudget");
  uint32_t *distance=find_cvar(base,image,"LODDistanceLimit");
  const uint32_t budget_rva=0x512EE58u;
  /* Known code: exact V4.x RVA pin. Adaptive: same descriptor shape (the
   * registered 25.0f default 8 bytes before the live value), no RVA pin. */
  if(!budget || (g_tier!=URT_TIER_ADAPTIVE && budget!=(uint32_t*)(base+budget_rva)) ||
     !distance ||
     __atomic_load_n(budget-2,__ATOMIC_ACQUIRE)!=0x41c80000u ||
     !cr01_physics501_init(budget,distance))
   cr01_release_log_event("URT: CPU Physics Level of Detail: engine setting rejected; native behaviour kept\r\n");
  else{
   physics_pref=cpu_physics_preference();
   cpu_physics_refresh();
  }
 }
 dt_current_pref=cpu_dt_preference();
 if(!cr01_dt501_init(find_cvar(base,image,"RTSTBillboardsProximity"),
                      find_cvar(base,image,"RTSTBillboards")))
  cr01_release_log_event("URT: CPU Distant Trees: engine setting binding refused; native behaviour kept\r\n");
 if(g_mode_initialized)cpu_dt_refresh((unsigned)last);
 cr01_release_log_event("CR01: runtime ready\r\n");
 for(;;){
  DWORD waited=WaitForSingleObject(watch,CR01_MODE_FALLBACK_MS);
  unsigned config_event=(waited==WAIT_OBJECT_0);
  unsigned publish=0,shadow=0,mode_changed=0;
  CR01RenderMode mode;

  if(config_event){
   if(!FindNextChangeNotification(watch)){
    cr01_release_log_event("URT: settings change notifications lost; URT-owned values released\r\n");break;
   }
   /* FindFirstChangeNotification is directory-scoped. Ignore every wake that
    * did not alter the exact dx12user.settings file. */
   if(!settings_stamp_sample(1))continue;
   /* Genuine CR01 menu events explicitly SaveUserSettings in the compiled
    * descriptor. Consume one complete validated generation for this exact edge. */
   if(detect_mode(&mode)&&(!g_mode_initialized||mode!=last)){
    last=mode;g_mode_initialized=1u;mode_changed=1u;
    (void)cr01_dt501_release();
    (void)cr01_menu_bridge_mode(&g_menu,mode);publish=1u;shadow=1u;
    cr01_release_log_event(mode==CR01_RENDER_PT ? "CR01: renderer transition -> PT" : (mode==CR01_RENDER_NORMAL_RT ? "CR01: renderer transition -> Normal RT" : "CR01: renderer transition -> raster"));
   }
   (void)cr01_dt501_release(); /* Avoid sharing PT Billboards slot during policy updates. */
   if(consume_exact_settings_edge()){publish=1u;shadow=1u;}
   (void)cr01_rt_legacy501_reload(g_userfile);
   (void)urt_rtperf501_reload();
   cpu_bone_refresh();
   physics_pref=cpu_physics_preference();
   cpu_physics_refresh();
   dt_current_pref=cpu_dt_preference();
   /* Native RT-shadows changes are also explicitly saved by the descriptor.
    * This updates only the isolated mirror and never the game's shadow CVar. */
   shadow=1u;
  }else if(waited==WAIT_TIMEOUT){
   /* No verified engine callback exists for arbitrary foreign in-memory mode
    * changes. This bounded READ-ONLY safety check has exactly one job:
    * renderer-mode transition detection. It never scans preferences, checks
    * shader readiness, persists data, updates shadows without a transition,
    * publishes unchanged settings or runs any optimization policy. */
   if(detect_mode(&mode)&&(!g_mode_initialized||mode!=last)){
    last=mode;g_mode_initialized=1u;mode_changed=1u;
    (void)cr01_dt501_release();
    (void)cr01_menu_bridge_mode(&g_menu,mode);publish=1u;shadow=1u;
    cr01_release_log_event(mode==CR01_RENDER_PT ? "CR01: renderer transition -> PT" : (mode==CR01_RENDER_NORMAL_RT ? "CR01: renderer transition -> Normal RT" : "CR01: renderer transition -> raster"));
   }
  }
  if(g_mode_initialized&&publish){
    static unsigned indirect_warn_once;
    int exclusive=cr01_indirect_shader_exclusive(&g_indirect_owner,g_indirect_slot,&g_menu.controller.saved,last);
    if(!exclusive&&g_menu.controller.saved.master&&last==CR01_RENDER_PT&&g_menu.controller.saved.v[F_INDIRECT]!=100u){
      if(!indirect_warn_once++)
        cr01_release_log_event("URT: Indirect-Light Compensation paused: the native indirect-light setting was changed by something else (refused to override)\r\n");
    }else indirect_warn_once=0;
    publish_now(last,exclusive);
  }
  if(g_mode_initialized&&(shadow||mode_changed))
    (void)cr01_shadow501_update(&g_menu.controller.saved,last==CR01_RENDER_PT);
  /* Mirrors original 07_normal_rt.inc: settings and mode are live on the
   existing worker, with CAS restoration, never a renderer synchronization claim. */
  if(detect_mode(&mode))cpu_dt_refresh((unsigned)mode);
  else cpu_dt_refresh(0u);
  if(detect_mode(&mode))cr01_rt_legacy501_tick((unsigned)mode,g_menu.controller.saved.master);
  else cr01_rt_legacy501_tick(0u,0u);
  if(detect_mode(&mode))urt_rtperf501_tick((unsigned)mode,g_menu.controller.saved.master);
  else urt_rtperf501_tick(0u,0u);
  if(__atomic_load_n(&g_running,__ATOMIC_ACQUIRE)!=1u)break;
 }
 (void)cr01_physics501_shutdown();
 (void)cr01_dt501_shutdown();
 (void)cr01_bone501_shutdown();
 (void)cr01_rt_legacy501_shutdown();
 urt_rtperf501_shutdown();
 (void)FindCloseChangeNotification(watch);
 (void)cr01_shadow501_update(&g_menu.controller.saved,0u);
 (void)cr01_release_owner(&g_indirect_owner);
 (void)cr01_native_shutdown(&g_menu.controller);
 cr01_release_log_event("URT: runtime stopped; URT-owned values released\r\n");return 0;
}
void urt_runtime_note_shader(int st){g_shader_state=st;}
int urt_runtime_shader_state(void){return g_shader_state;}
int urt_runtime_tier(void){return g_tier;}
int cr01_prod_runtime501_start_tier(int tier){
 if(tier<URT_TIER_ADAPTIVE||tier>URT_TIER_FULL)return 0;
 if(!__sync_bool_compare_and_swap(&g_running,0ul,1ul))return 0;
 g_tier=tier;
 HANDLE thread=CreateThread(0,0,worker,0,0,0);
 if(!thread){g_running=0;return 0;}
 (void)CloseHandle(thread);
 return 1;
}
