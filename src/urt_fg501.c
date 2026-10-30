/* V5.2.0 "Frame Generation: Keep Resources in Menus" (Patch 5.01). See urt_fg501.h.
 * Portable C (no Win32): the host tests run the real wrapper against the mapped exe. */
#include "urt_fg501.h"
#include "urt_build.h"
#include "cr01_release_log.h"
#include <stddef.h>

#define FG_PTR_RVA     urt_rva(0x5ca8140u)  /* game's cached slDLSSGSetOptions pointer (.data, bss) */
#define FG_MODE_RVA    urt_rva(0x51513e8u)  /* DLSSGMode (PostProcess, int): player's FG setting  */
#define FG_OPT_SIZE    0x78u          /* sl::DLSSGOptions as built by the game (structVersion 5) */
/* setter 0x1b81530: mov rax,[ptr] / lea r8,[ptr] (lazy resolve) / mov rax,[ptr] ; call rax */
/* V5.3.0 R11: the three rip-relative instructions are checked by opcode and by their computed target (the pointer
 * slot), so the same check holds on every known build (Patch 5.01: displacements 0x04126bd3 / af / 8d). */
static const struct { uint32_t rva; uint8_t b[3]; } k_fp[3]={
  {0x1b81566u,{0x48,0x8b,0x05}},
  {0x1b8158au,{0x4c,0x8d,0x05}},
  {0x1b815acu,{0x48,0x8b,0x05}},
};
static const uint8_t k_call[10]={0x48,0x8b,0xd7,0x48,0x8d,0x4c,0x24,0x20,0xff,0xd0};
/* sl::DLSSGOptions structType GUID {FAC5F1CB-2DFD-4F36-A1E6-3A9E865256C5} as 4 dwords */
static const uint32_t k_guid[4]={0xfac5f1cbu,0x4f362dfdu,0x9e3ae6a1u,0xc5565286u};

typedef int (*SetOptions)(const void*,const void*);
static uint64_t *g_ptr;               /* the game's pointer slot */
static const int32_t *g_mode;         /* DLSSGMode */
static SetOptions volatile g_orig;    /* never cleared once set: stale callers stay valid */
static volatile int g_enabled;
static int g_available, g_installed, g_given_up;
static uint32_t g_installs, g_replaced;
static volatile uint32_t g_calls, g_off_calls, g_flagged;

static uint32_t rd32(const void*p,unsigned off){
  const volatile uint8_t*b=(const volatile uint8_t*)p+off;
  return (uint32_t)b[0]|((uint32_t)b[1]<<8)|((uint32_t)b[2]<<16)|((uint32_t)b[3]<<24);
}
int urt_fg501_should_retain(const void *o, int fg_user_enabled){
  if(!o||!fg_user_enabled)return 0;
  for(unsigned i=0;i<4;i++)if(rd32(o,8u+4u*i)!=k_guid[i])return 0;   /* structType */
  if(rd32(o,0x18u)!=5u||rd32(o,0x1cu)!=0u)return 0;                  /* structVersion 5 */
  if(rd32(o,0x20u)!=0u)return 0;                                       /* mode == eOff */
  return 1;
}
int urt_fg501_wrapper(const void *viewport, const void *options){
  SetOptions orig=g_orig;
  __atomic_add_fetch(&g_calls,1u,__ATOMIC_RELAXED);
  if(options&&rd32(options,0x20u)==0u)__atomic_add_fetch(&g_off_calls,1u,__ATOMIC_RELAXED);
  const int user=g_mode?(*(const volatile int32_t*)g_mode>0):0;
  if(__atomic_load_n(&g_enabled,__ATOMIC_ACQUIRE)&&urt_fg501_should_retain(options,user)){
    uint64_t copy[FG_OPT_SIZE/8u];
    const volatile uint64_t*src=(const volatile uint64_t*)options;
    for(unsigned i=0;i<FG_OPT_SIZE/8u;i++)copy[i]=src[i];
    /* flags (+0x28) |= DLSSGFlags::eRetainResourcesWhenOff (1 << 3) */
    copy[0x28u/8u]|=(uint64_t)8u;
    __atomic_add_fetch(&g_flagged,1u,__ATOMIC_RELAXED);
    return orig(viewport,copy);
  }
  return orig(viewport,options);
}
int urt_fg501_init(uint8_t *base, uint32_t image, int known_exe){
  g_available=0;g_installed=0;g_given_up=0;g_ptr=0;g_mode=0;
  if(!base||!known_exe||image<=FG_PTR_RVA+8u)return 0;
  const uint32_t ptr_rva=FG_PTR_RVA,call_rva=urt_rva(0x1b815b3u);
  if(!ptr_rva||!FG_MODE_RVA||!call_rva||image<=call_rva+10u)return 0;
  for(unsigned k=0;k<3;k++){
    const uint32_t at=urt_rva(k_fp[k].rva);
    if(!at||image<=at+7u)return 0;
    for(unsigned j=0;j<3;j++)if(base[at+j]!=k_fp[k].b[j])return 0;
    if((uint32_t)((int64_t)at+7+(int32_t)rd32(base,at+3u))!=ptr_rva)return 0;   /* rip target == the pointer slot */
  }
  for(unsigned j=0;j<10;j++)if(base[call_rva+j]!=k_call[j])return 0;
  g_ptr=(uint64_t*)(void*)(base+FG_PTR_RVA);
  g_mode=(const int32_t*)(const void*)(base+FG_MODE_RVA);
  g_available=1;
  return 1;
}
static uint64_t wrapper_addr(void){return (uint64_t)(uintptr_t)&urt_fg501_wrapper;}
void urt_fg501_tick(int want){
  if(!g_available)return;
  __atomic_store_n(&g_enabled,want?1:0,__ATOMIC_RELEASE);
  uint64_t cur=__atomic_load_n(g_ptr,__ATOMIC_ACQUIRE);
  if(g_installed&&cur!=wrapper_addr()){
    /* the game re-resolved the function or another mod replaced the pointer */
    g_installed=0;g_replaced++;
    cr01_release_log_event("URT: Frame Generation: the game's option pointer changed; wrapper re-checked\r\n");
    if(g_replaced>3u){g_given_up=1;cr01_release_log_event("URT: Frame Generation: pointer keeps changing; Keep Resources in Menus left inactive this session\r\n");}
  }
  if(want&&!g_installed&&!g_given_up){
    if(!cur)return;                       /* Frame Generation not used yet */
    if(cur==wrapper_addr()){g_installed=1;return;}   /* already ours: never wrap ourselves */
    g_orig=(SetOptions)(uintptr_t)cur;
    uint64_t e=cur;
    if(__atomic_compare_exchange_n(g_ptr,&e,wrapper_addr(),0,__ATOMIC_SEQ_CST,__ATOMIC_SEQ_CST)){
      g_installed=1;g_installs++;
      if(g_installs==1u)cr01_release_log_event("URT: Frame Generation: Keep Resources in Menus active (resources kept while menus turn Frame Generation off)\r\n");
    }
    return;
  }
  if(!want&&g_installed){
    uint64_t e=wrapper_addr();
    (void)__atomic_compare_exchange_n(g_ptr,&e,(uint64_t)(uintptr_t)g_orig,0,__ATOMIC_SEQ_CST,__ATOMIC_SEQ_CST);
    g_installed=0;
    cr01_release_log_event("URT: Frame Generation: Keep Resources in Menus off (game pointer restored)\r\n");
  }
}
void urt_fg501_shutdown(void){
  if(!g_available)return;
  urt_fg501_tick(0);
}
void urt_fg501_stats(UrtFgStats *s){
  if(!s)return;
  s->installed=(uint32_t)g_installed;s->installs=g_installs;s->replaced=g_replaced;
  s->calls=g_calls;s->off_calls=g_off_calls;s->flagged=g_flagged;s->available=g_available;
}
