/* Windows half of urt_exe_identity: ONE pass over witcher3.exe computing the
 * whole-file SHA-256 and the code-identity SHA-256 together, cached for the
 * process lifetime. V4.3.1 recomputed the whole-file hash at every call site
 * (three times on the game's main thread before startup continued, again on
 * the worker and on every Fast Bone Reads enable). A running image file cannot
 * be rewritten by Windows while mapped, so one result is valid for the run. */
#include "urt_exe_identity.h"
#define WIN __stdcall
#define IMP __declspec(dllimport)
typedef void *HANDLE; typedef void *HMODULE; typedef unsigned long DWORD; typedef int BOOL;
typedef unsigned short WCHAR; typedef uintptr_t HCRYPTPROV; typedef uintptr_t HCRYPTHASH;
typedef struct { void *p; } URT_ONCE;
IMP DWORD WIN GetModuleFileNameW(HMODULE,WCHAR*,DWORD);
IMP HANDLE WIN CreateFileW(const WCHAR*,DWORD,DWORD,void*,DWORD,DWORD,HANDLE);
IMP BOOL WIN ReadFile(HANDLE,void*,DWORD,DWORD*,void*);
IMP BOOL WIN CloseHandle(HANDLE);
IMP BOOL WIN GetFileSizeEx(HANDLE,int64_t*);
IMP BOOL WIN InitOnceExecuteOnce(URT_ONCE*,BOOL(WIN*)(URT_ONCE*,void*,void**),void*,void**);
IMP BOOL WIN CryptAcquireContextW(HCRYPTPROV*,const WCHAR*,const WCHAR*,DWORD,DWORD);
IMP BOOL WIN CryptCreateHash(HCRYPTPROV,DWORD,HCRYPTPROV,DWORD,HCRYPTHASH*);
IMP BOOL WIN CryptHashData(HCRYPTHASH,const unsigned char*,DWORD,DWORD);
IMP BOOL WIN CryptGetHashParam(HCRYPTHASH,DWORD,unsigned char*,DWORD*,DWORD);
IMP BOOL WIN CryptDestroyHash(HCRYPTHASH);
IMP BOOL WIN CryptReleaseContext(HCRYPTPROV,DWORD);

/* Protected Patch 5.01 witcher3.exe (2026-10 store build) */
static const uint8_t kFileSha[32]={
 0x2d,0xf7,0x65,0x39,0x8a,0x3d,0x1f,0x16,0x5d,0xb4,0x18,0x27,0x1b,0xe7,0x20,0x01,
 0x78,0x32,0x0a,0x44,0x66,0x81,0x73,0x79,0xf2,0xfa,0x0d,0xc0,0x00,0x1f,0xb1,0x01};
/* tools/exe_codeid.py over the same file */
static const uint8_t kCodeIdSha[32]={
 0xea,0x05,0x68,0xe9,0xa5,0xce,0xc8,0x89,0xa2,0x7b,0xea,0x86,0xee,0x59,0xf4,0xa9,
 0x98,0x8a,0x77,0xe6,0x1b,0x3e,0x61,0x36,0xd6,0xe3,0x89,0x10,0x7f,0x68,0x50,0x87};

static URT_ONCE g_once;
static int g_tier=URT_EXE_UNKNOWN;
static uint8_t g_chunk[65536];
static UrtCodeIdPlan g_plan;

static int feed(void *ctx, const uint8_t *p, uint32_t n){
  return CryptHashData(*(HCRYPTHASH*)ctx,p,n,0)!=0;
}
static int finish(HCRYPTHASH h, const uint8_t want[32]){
  uint8_t got[32];DWORD len=32;
  if(!CryptGetHashParam(h,2,got,&len,0)||len!=32)return 0;
  for(unsigned i=0;i<32;i++)if(got[i]!=want[i])return 0;
  return 1;
}
static BOOL WIN compute(URT_ONCE *o, void *a, void **c){
  WCHAR path[1024];DWORD n,got=0;HANDLE f;int64_t size=0;
  HCRYPTPROV prov=0;HCRYPTHASH whole=0,code=0;uint64_t off=0;int planned=0,ok=0;
  (void)o;(void)a;(void)c;
  n=GetModuleFileNameW(0,path,1024);
  if(!n||n>=1024)return 1;
  f=CreateFileW(path,0x80000000ul,1|2|4,0,3,0x80,0);
  if(f==(HANDLE)(intptr_t)-1)return 1;
  if(!GetFileSizeEx(f,&size)||size<=0)goto done;
  if(!CryptAcquireContextW(&prov,0,0,24,0xf0000000ul))goto done;
  if(!CryptCreateHash(prov,0x800c,0,0,&whole)||!CryptCreateHash(prov,0x800c,0,0,&code))goto done;
  for(;;){
    if(!ReadFile(f,g_chunk,sizeof(g_chunk),&got,0))goto done;
    if(!got)break;
    if(!CryptHashData(whole,g_chunk,got,0))goto done;
    if(off==0){
      planned=urt_codeid_plan(g_chunk,got,(uint64_t)size,&g_plan);
      if(planned&&!CryptHashData(code,g_plan.layout,g_plan.layout_len,0))planned=0;
    }
    if(planned&&!urt_codeid_feed_chunk(&g_plan,off,g_chunk,got,feed,&code))planned=0;
    off+=got;
  }
  ok=1;
 done:
  if(ok){
    if(finish(whole,kFileSha))g_tier=URT_EXE_EXACT;
    else if(planned&&finish(code,kCodeIdSha))g_tier=URT_EXE_CODE_IDENTICAL;
  }
  if(whole)CryptDestroyHash(whole);
  if(code)CryptDestroyHash(code);
  if(prov)CryptReleaseContext(prov,0);
  CloseHandle(f);
  return 1;
}
int urt_exe_identity(void){
  (void)InitOnceExecuteOnce(&g_once,compute,0,0);
  return g_tier;
}
/* V4.x contract kept for every existing call site: "is this the protected
 * Patch 5.01 code?" Both EXACT and CODE_IDENTICAL qualify. */
int cr01_gate_verify_exe_file(void){
  return urt_exe_identity()>=URT_EXE_CODE_IDENTICAL;
}
