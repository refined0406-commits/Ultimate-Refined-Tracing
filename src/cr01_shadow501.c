/* Isolated verified LC44 native PT shadow mirror carrier, Patch 5.01.
 * Two uniquely signature-located sites plus 11 independently fingerprinted
 * shadow reads; original game owns its native RT shadows CVar. This module
 * never writes NAPL light settings or normal RT shadow state. */
#include "urt_build.h"
#include "cr01_shadow501.h"
#include "cr01_shadow_policy.h"
#include "refined_pt_shadow_ext_501.h"
#include <stddef.h>
#define IMP __declspec(dllimport)
#define WIN __stdcall
#define R 0x1000ul
#define C 0x2000ul
#define RX 0x20ul
#define RW 0x04ul
#define RWX 0x40ul
#define REL 0x8000ul
typedef unsigned long DWORD;typedef int BOOL;typedef unsigned short WCHAR;
IMP void*WIN VirtualAlloc(void*,size_t,DWORD,DWORD);
IMP BOOL WIN VirtualFree(void*,size_t,DWORD);
IMP BOOL WIN VirtualProtect(void*,size_t,DWORD,DWORD*);
IMP BOOL WIN FlushInstructionCache(void*,const void*,size_t);
IMP void*WIN GetCurrentProcess(void);
static volatile uint32_t *g_native,*g_pt;
static volatile uint32_t *g_mirror;
static uint8_t *g_branch[3],*g_sites[2];
static uint8_t g_forced, g_ready, g_fault;
static uint32_t raw32(const uint8_t*p){return (uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;}
static void write32(uint8_t*p,uint32_t x){for(unsigned i=0;i<4;i++)p[i]=(uint8_t)(x>>(i*8));}
static int match_mask(const uint8_t*p,const uint8_t*pat,const char*mask,unsigned len){
 for(unsigned i=0;i<len;i++)if(mask[i]=='x'&&p[i]!=pat[i])return 0;
 return 1;
}
static uint8_t*unique_text(uint8_t*base,uint32_t image,const uint8_t*pat,const char*mask,unsigned len){
 if(!base||image<0x1000000||raw32(base+0x3c)>0x1000)return 0;
 uint32_t pe=raw32(base+0x3c),sec=pe+24+(uint16_t)(base[pe+20]|base[pe+21]<<8);
 unsigned count=(unsigned)(base[pe+6]|base[pe+7]<<8);uint8_t*hit=0;
 if(count>96||sec>image||count*40>image-sec)return 0;
 for(unsigned s=0;s<count;s++){
  uint8_t*h=base+sec+s*40;uint32_t sz=raw32(h+8),rva=raw32(h+12),flags=raw32(h+36);
  if((flags&0x20000000u)==0 || rva>image||sz>image-rva||sz<len)continue;
  for(uint32_t at=0;at<=sz-len;at++){
   uint8_t*pos=base+rva+at;
   if(match_mask(pos,pat,mask,len)) {if(hit)return 0;hit=pos;}
  }
 }
 return hit;
}
static int patch_bytes(uint8_t*p,const uint8_t*want,unsigned len){
 DWORD old=0,ignored=0;if(!p||!VirtualProtect(p,len,RWX,&old))return 0;
 for(unsigned i=0;i<len;i++)p[i]=want[i];
 int good=FlushInstructionCache(GetCurrentProcess(),p,len)!=0;
 if(!VirtualProtect(p,len,old,&ignored))good=0;
 for(unsigned i=0;i<len;i++)if(p[i]!=want[i])good=0;
 return good;
}
static int read_target(uint8_t*operand,uint8_t*next,uint8_t**out){
 intptr_t off=(int32_t)raw32(operand);if(off>(int64_t)0x7fffffff||off<(-INT64_C(0x80000000)))return 0;
 *out=next+off;return 1;
}
/* Two gated byte rewrites are the LC44-proven PT-only branch policy, whose
 * all-three transactional rollback is required before changing the mirror. */
static int set_off(unsigned force){
 uint8_t actual[3],wanted[3],done=0;
 if(!g_ready||g_fault||force>1)return 0;
 if(g_forced==force)return 1;
 actual[0]=force?0x74:0x71;actual[1]=force?0x84:0x81;actual[2]=force?0x74:0xeb;
 wanted[0]=force?0x71:0x74;wanted[1]=force?0x81:0x84;wanted[2]=force?0xeb:0x74;
 for(unsigned i=0;i<3;i++)if(!g_branch[i]||*g_branch[i]!=actual[i])return 0;
 for(unsigned i=0;i<3;i++){
  if(!patch_bytes(g_branch[i],&wanted[i],1)){
   if(*g_branch[i]==wanted[i])(void)patch_bytes(g_branch[i],&actual[i],1);
   while(done){--done;(void)patch_bytes(g_branch[done],&actual[done],1);}
   g_fault=1;return 0;
  }
  done++;
 }
 g_forced=(uint8_t)force;return 1;
}
int cr01_shadow501_prepare(uint8_t*base,uint32_t image){
 static const uint8_t gp[]={0x84,0xc9,0x74,0x0c,0x84,0xc0,0x75,0x08,0x39,0x3d,0,0,0,0,0x77,0x11,0x40,0x38,0x3d,0,0,0,0,0x74,0x43,0x84,0xc9,0x74,0x3f,0x84,0xc0,0x75,0x3b};
 static const char gm[]="xxxxxxxxxx????xxxxx????xxxxxxxxxx";
 static const uint8_t lp[]={0x84,0xc9,0x74,0x0d,0x84,0xc0,0x75,0x09,0x83,0x3d,0,0,0,0,0x00,0x77,0x1d,0x80,0x3d,0,0,0,0,0x00,0x0f,0x84,0,0,0,0,0x84,0xc9,0x0f,0x84};
 static const char lm[]="xxxxxxxxxx????xxxxx????xxx????xxxx";
 if(g_ready||g_fault||!base||image!=urt_image_bytes())return 0;
 uint8_t*g=unique_text(base,image,gp,gm,sizeof(gp));
 uint8_t*l=unique_text(base,image,lp,lm,sizeof(lp));
 if(!g||!l||g[23]!=0x74||l[24]!=0x0f||l[25]!=0x84)return 0;
 uint8_t *na=0,*nr=0,*ptg=0,*ptl=0;
 if(!read_target(g+10,g+14,&na)||!read_target(l+10,l+15,&nr)||
    !read_target(g+19,g+23,&ptg)||!read_target(l+19,l+24,&ptl)||
    na!=nr||ptg!=ptl||na==ptg)return 0;
 if(!urt_rva(0x228f866u)||!urt_rva(0x228f858u))return 0;
 uint8_t*p=base+urt_rva(0x228f866u),*s=base+urt_rva(0x228f858u);
 if(p[-7]!=0x80||p[-6]!=0x3d||p[-1]!=0||p[0]!=0x74||p[1]!=0x0b||
    s[-2]!=0x83||s[-1]!=0x3d||s[4]!=0||s[5]!=0x77||s[6]!=0x11)return 0;
 if(p+(int32_t)raw32(p-5)!=ptg||s+5+(int32_t)raw32(s)!=na)return 0;
 uintptr_t center=((uintptr_t)(base+0x1d00000u))&~(uintptr_t)0xffffu;
 uint8_t *mirror=0;
 for(uintptr_t step=0x10000u;step<0x70000000u&&!mirror;step+=0x10000u){
  for(unsigned dir=0;dir<2;dir++){
   uintptr_t at=dir?(center>=step?center-step:0):center+step;
   if(!at||at>INTPTR_MAX)continue;
   intptr_t gd=(intptr_t)at-(intptr_t)(g+14);
   intptr_t ld=(intptr_t)at-(intptr_t)(l+15);
   if(gd<INT32_MIN||gd>INT32_MAX||ld<INT32_MIN||ld>INT32_MAX)continue;
   mirror=(uint8_t*)VirtualAlloc((void*)at,0x1000u,R|C,RW);
   if(mirror)break;
  }
 }
 if(!mirror)return 0;
 *(volatile uint32_t*)mirror=*(volatile uint32_t*)na;
 uint8_t original[2][4],replacement[2][4];
 uint8_t *sites[2]={g+10,l+10};uint8_t*after[2]={g+14,l+15};
 for(unsigned j=0;j<2;j++){
  for(unsigned i=0;i<4;i++)original[j][i]=sites[j][i];
  intptr_t disp=(intptr_t)mirror-(intptr_t)after[j];
  if(disp<INT32_MIN||disp>INT32_MAX){(void)VirtualFree(mirror,0,REL);return 0;}
  write32(replacement[j],(uint32_t)(int32_t)disp);
 }
 if(!patch_bytes(sites[0],replacement[0],4))goto fail_first;
 if(!patch_bytes(sites[1],replacement[1],4))goto fail_both;
 if(!refined_pt_shadow_ext_install(base,(const volatile uint32_t*)na,(volatile uint32_t*)mirror))goto fail_both;
 g_native=(volatile uint32_t*)na;g_pt=(volatile uint32_t*)ptg;
 g_mirror=(volatile uint32_t*)mirror;
 g_sites[0]=sites[0];g_sites[1]=sites[1];
 g_branch[0]=g+23;g_branch[1]=l+25;g_branch[2]=p;
 g_ready=1;return 1;
fail_both:
 (void)patch_bytes(sites[1],original[1],4);
fail_first:
 (void)patch_bytes(sites[0],original[0],4);
 if(raw32(sites[0])!=raw32(original[0])||raw32(sites[1])!=raw32(original[1])||
    refined_pt_shadow_ext_redirects()){
  /* Residual redirects keep the mirror pinned forever, fail closed. */
  g_fault=1;g_mirror=(volatile uint32_t*)mirror;return 0;
 }
 (void)VirtualFree(mirror,0,REL);return 0;
}
int cr01_shadow501_ready(void){return g_ready&&!g_fault;}
int cr01_shadow501_update(const CR01Settings*settings,unsigned pt){
 unsigned force=0;uint32_t desired=0;
 if(!cr01_shadow501_ready()||!settings||pt>1)return 0;
 uint32_t native=__atomic_load_n(g_native,__ATOMIC_ACQUIRE);
 if(!cr01_shadow_policy(settings,pt,native,&force,&desired))return 0;
 if(!set_off(force))return 0;
 if(__atomic_load_n(g_mirror,__ATOMIC_ACQUIRE)!=desired)
   __atomic_store_n(g_mirror,desired,__ATOMIC_RELEASE);
 return 1;
}
