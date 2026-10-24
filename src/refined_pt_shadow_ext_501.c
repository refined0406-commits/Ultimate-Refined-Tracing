/* Native Refined Tracing PT-only shadow-read redirection, Witcher 3 v5.01.
   Patch 5.01 source-owned reconstruction: eleven additional game reads, supplementing
   the two guarded PT-shadow reads from RT shadow guard. Does not change the CDPR Shadow CVar.
   The mirror is set to native outside PT and to the PT-shadow choice only during PT.
   All target instructions are byte-checked, with fail-closed transactional rollback.
   All instruction sites are checked before temporary installation. */
#include "refined_pt_shadow_ext_501.h"
#include <stdint.h>
#include <stddef.h>
#define WINAPI __stdcall
extern __declspec(dllimport) int WINAPI VirtualProtect(void*,size_t,unsigned long,unsigned long*);
extern __declspec(dllimport) int WINAPI FlushInstructionCache(void*,const void*,size_t);
extern __declspec(dllimport) void* WINAPI GetCurrentProcess(void);
#define PAGE_EXECUTE_READWRITE 0x40u
#define SITE_COUNT 11u

typedef struct ShadowReference {
   uint32_t disp_rva;
   uint8_t trailing_immediate;
   uint8_t opcode[3];
   uint8_t opcode_len;
   uint8_t last_byte;
   uint8_t check_trailer;
} ShadowReference;
/* All sites independently verified against the user-supplied 5.01 EXE.
   They reference the exact same engine Shadow storage as RT shadow guard's two PT reads. */
static const ShadowReference g_refs[SITE_COUNT]={
 {0x1c2ae35u,0,{0x44,0x39,0x2d},3,0,0},
 {0x1c2e7f1u,0,{0x39,0x35,0},2,0,0},
 {0x1c2f5cbu,0,{0x48,0x8b,0x15},3,0,0},
 {0x1d60c78u,0,{0x39,0x2d,0},2,0,0},
 {0x228f8f1u,0,{0x4c,0x8b,0x05},3,0,0},
 {0x1c345c3u,1,{0x83,0x3d,0},2,0,1},
 {0x1c3ae28u,1,{0x83,0x3d,0},2,0,1},
 {0x1c6d7a8u,1,{0x83,0x3d,0},2,0,1},
 {0x227351cu,1,{0x83,0x3d,0},2,0,1},
 {0x228f858u,1,{0x83,0x3d,0},2,0,1},
 {0x229025fu,1,{0x83,0x3d,0},2,0,1}
};
static uint8_t *g_sites[SITE_COUNT];
static uint8_t g_original[SITE_COUNT][4];
static uint8_t g_ours[SITE_COUNT][4];
static int g_active=0; /* 0 pristine, 1 fully installed, 2 incomplete rollback */
static unsigned g_count=0;
static int32_t r32(const uint8_t *p){
  uint32_t n=(uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
  return (int32_t)n;
}
static void w32(uint8_t *p,int32_t d){uint32_t u=(uint32_t)d;unsigned j;for(j=0;j<4;j++)p[j]=(uint8_t)(u>>(8*j));}
static int same4(const uint8_t*a,const uint8_t*b){unsigned k;for(k=0;k<4;k++)if(a[k]!=b[k])return 0;return 1;}
static unsigned remaining_refs(void){
  unsigned i,n=0;
  for(i=0;i<SITE_COUNT;i++)if(g_sites[i]&&!same4(g_sites[i],g_original[i]))n++;
  return n;
}
static int patch4(uint8_t *addr,const uint8_t data[4]){
  unsigned long prior=0,unused=0;unsigned i;
  if(!VirtualProtect(addr,4,PAGE_EXECUTE_READWRITE,&prior))return 0;
  for(i=0;i<4;i++)addr[i]=data[i];
  FlushInstructionCache(GetCurrentProcess(),addr,4);
  if(!VirtualProtect(addr,4,prior,&unused))return 0;
  return same4(addr,data);
}
int refined_pt_shadow_ext_install(uint8_t*base,const volatile uint32_t*native,volatile uint32_t*mirror){
   unsigned i,j,committed=0; int64_t diff;
   uint8_t* p;
   if(g_active)return g_active==1;
   if(!base||!native||!mirror || (uintptr_t)native==(uintptr_t)mirror)return 0;
   /* No patches at all unless every original instruction and RIP destination matches. */
   for(i=0;i<SITE_COUNT;i++){
     const ShadowReference *ref=&g_refs[i];
     p=base+ref->disp_rva;
     for(j=0;j<ref->opcode_len;j++)if(p[-(ptrdiff_t)ref->opcode_len+j]!=ref->opcode[j])return 0;
     if(ref->check_trailer && p[4]!=ref->last_byte)return 0;
     diff=(int64_t)(intptr_t)mirror-(int64_t)(intptr_t)(p+4+ref->trailing_immediate);
     if(diff<-2147483648LL||diff>2147483647LL)return 0;
     if((uintptr_t)(p+4+ref->trailing_immediate+r32(p))!=(uintptr_t)native)return 0;
     g_sites[i]=p;
     for(j=0;j<4;j++)g_original[i][j]=p[j];
     w32(g_ours[i],(int32_t)diff);
   }
   for(i=0;i<SITE_COUNT;i++){
      if(!patch4(g_sites[i],g_ours[i])){
         /* Restore a potentially partial failing write plus all completed writes. */
         if(g_sites[i] && same4(g_sites[i],g_ours[i]))(void)patch4(g_sites[i],g_original[i]);
         while(committed){--committed;
             if(same4(g_sites[committed],g_ours[committed]))
                 (void)patch4(g_sites[committed],g_original[committed]);
         }
         /* Never report pristine when any read may still target the mirror.
            Its owner must keep that allocation alive until restoration succeeds. */
         g_count=remaining_refs();g_active=g_count?2:0;return 0;
      }
      ++committed;
   }
   g_count=SITE_COUNT;g_active=1;return 1;
}
int refined_pt_shadow_ext_restore(void){
   unsigned i;int ok=1;
   if(!g_active)return 1;
   for(i=0;i<SITE_COUNT;i++){
     if(!same4(g_sites[i],g_original[i])&&!same4(g_sites[i],g_ours[i]))return 0;
   }
   for(i=0;i<SITE_COUNT;i++)if(!same4(g_sites[i],g_original[i]))
     if(!patch4(g_sites[i],g_original[i]))ok=0;
   g_count=remaining_refs();g_active=g_count?2:0;return ok&&!g_count;
}
unsigned refined_pt_shadow_ext_redirects(void){return g_count;}
