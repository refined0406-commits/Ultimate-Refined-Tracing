/* V5.3.0 R7 EXPERIMENTAL Path Tracing code limits (Patch 5.01). See urt_ptcode501.h.
 * Portable C: the host tests run this unit against the mapped executable with a plain-store writer. */
#include "urt_ptcode501.h"
#include "cr01_release_log.h"
#include <stddef.h>

/* 0x1c20011: mov rax,[r15+0x698] / mov edi,0x100 / test rax,rax / je / mov eax,[rax+0x434] / mov ebx,edi /
 *            cmp eax,edi / cmovbe ebx,eax  (the PT light-count clamp in 0x1c1fdd0) */
static const uint8_t k_cap[30]={0x49,0x8b,0x87,0x98,0x06,0x00,0x00,0xbf,0x00,0x01,0x00,0x00,0x48,0x85,0xc0,0x74,
                                0x0f,0x8b,0x80,0x34,0x04,0x00,0x00,0x8b,0xdf,0x3b,0xc7,0x0f,0x46,0xd8};
#define CAP_SIG_RVA 0x1c20011u
/* 0x1cf2a5d..0x1cf2ad7: the two PT/RT selections of the proximity-TLAS filter (angular size, big object) */
static const uint8_t k_cull[]={
 0x45,0x84,0xc0,0x74,0x0e,0x84,0xc0,0x74,0x0a,0x84,0xd2,0x75,0x06,0xc5,0xf8,0x28,0xd1,0xeb,0x04,0xc5,0xf8,0x28,0xd0,
 0x45,0x84,0xc0,0x74,0x0c,0x84,0xc0,0x74,0x08,0x84,0xd2,0x75,0x04,0xc5,0xf8,0x28,0xc1,0xc5,0xfa,0x59,0xc2,0xc5,0xfa,
 0x11,0x41,0x10,0x4c,0x8b,0x05,0x5b,0xcb,0x45,0x03,0x48,0x8b,0x15,0x04,0xe6,0x45,0x03,0x48,0x8b,0x05,0x35,0xca,0x45,
 0x03,0xc5,0xfa,0x10,0x15,0x5d,0x77,0x45,0x03,0x45,0x84,0xc0,0x74,0x0e,0x84,0xc0,0x74,0x0a,0x84,0xd2,0x75,0x06,0xc5,
 0xf8,0x28,0xc1,0xeb,0x04,0xc5,0xf8,0x28,0xc2,0x45,0x84,0xc0,0x74,0x08,0x84,0xc0,0x74,0x04,0x84,0xd2,0x74,0x04,0xc5,
 0xf8,0x28,0xca,0xc5,0xf2,0x59,0xc0,0xc5,0xfa,0x11,0x41,0x14};
#define CULL_SIG_RVA 0x1cf2a5du
/* the function's own head (0x1cf2a20: loads 1e-4, PTEnable, DisableEnableRayTracing, EnableRT, AngleSize) */
static const uint8_t k_head[0x3d]={
 0x8b,0x02,0x4c,0x8b,0xca,0xc5,0xfa,0x10,0x0d,0xc3,0x4d,0xdc,0x01,0x89,0x01,0x8b,0x42,0x04,0x89,0x41,0x04,0x8b,0x42,
 0x08,0xc5,0xfa,0x11,0x51,0x0c,0x89,0x41,0x08,0x4c,0x8b,0x05,0xa9,0xcb,0x45,0x03,0x48,0x8b,0x15,0x52,0xe6,0x45,0x03,
 0x48,0x8b,0x05,0x83,0xca,0x45,0x03,0xc5,0xfa,0x10,0x05,0xcb,0xcb,0x45,0x03};
#define HEAD_RVA 0x1cf2a20u
static const uint32_t k_cull_rva[4]={URT_PTC_CULL_RVA0,URT_PTC_CULL_RVA1,URT_PTC_CULL_RVA2,URT_PTC_CULL_RVA3};
static const uint8_t k_cull_native[4]={0xd1,0xc1,0xc1,0x04};
static const uint8_t k_cull_on[4]={0xd0,0xc0,0xc2,0x00};

static UrtCodeWriter g_w;
static uint8_t *g_base;
static unsigned g_avail,g_cap,g_cull,g_faults,g_writes,g_logged,g_refused;

void urt_ptcode501_set_writer(UrtCodeWriter w){g_w=w;}
static int same(const uint8_t*p,const uint8_t*q,unsigned n){for(unsigned i=0;i<n;i++)if(p[i]!=q[i])return 0;return 1;}
unsigned urt_ptcode501_init(uint8_t *base, uint32_t image, int known_exe){
 g_avail=0;g_cap=0;g_cull=0;g_base=0;g_refused=0;g_faults=0;g_logged=0;
 if(!base||!known_exe||!g_w||image<0x1cf2b00u)return 0;
 if(same(base+CAP_SIG_RVA,k_cap,sizeof k_cap))g_avail|=1u;
 if(same(base+HEAD_RVA,k_head,sizeof k_head)&&same(base+CULL_SIG_RVA,k_cull,sizeof k_cull))g_avail|=2u;
 g_base=base;
 return g_avail;
}
static int put(uint32_t rva,const uint8_t*b,unsigned n){
 if(!g_w(g_base+rva,b,n)||!same(g_base+rva,b,n)){g_faults++;return 0;}
 g_writes++;return 1;
}
/* Write the cap immediate. Only the low two bytes ever differ from 256 (caps are 1..255). */
static int set_cap(unsigned cap){
 const unsigned v=cap?cap:256u;
 uint8_t now[4],want[4]={(uint8_t)v,(uint8_t)(v>>8),0,0};
 for(unsigned i=0;i<4;i++)now[i]=g_base[URT_PTC_CAP_RVA+i];
 const uint32_t cur=(uint32_t)now[0]|(uint32_t)now[1]<<8|(uint32_t)now[2]<<16|(uint32_t)now[3]<<24;
 /* ownership: the immediate must be the native 256 or the value URT wrote */
 if(cur!=256u&&cur!=(g_cap?g_cap:256u))return 0;
 if(cur==v)return 1;
 if(now[2]||now[3])return 0;
 /* byte order keeps every transient value >= the smaller of the old and new cap (never 0) */
 if(v>cur)return put(URT_PTC_CAP_RVA+1u,&want[1],1)&&put(URT_PTC_CAP_RVA,&want[0],1);
 return put(URT_PTC_CAP_RVA,&want[0],1)&&put(URT_PTC_CAP_RVA+1u,&want[1],1);
}
static int set_cull(int on){
 const uint8_t*from=on?k_cull_native:k_cull_on,*to=on?k_cull_on:k_cull_native;
 for(unsigned i=0;i<4;i++){const uint8_t c=g_base[k_cull_rva[i]];if(c!=from[i]&&c!=to[i])return 0;}
 for(unsigned i=0;i<4;i++)if(g_base[k_cull_rva[i]]!=to[i]&&!put(k_cull_rva[i],&to[i],1))return 0;
 return 1;
}
void urt_ptcode501_tick(unsigned cap, int cull){
 if(!g_base||!g_w||g_faults)return;
 if(cap>255u)cap=0;
 if((g_avail&1u)&&cap!=g_cap){
  if(set_cap(cap)){g_cap=cap;
   if(g_logged<16u){g_logged++;cr01_release_log_event(cap?"URT: PT light limit active (Path Tracing lights per frame capped)\r\n":
                                                          "URT: PT light limit off (native 256)\r\n");}}
  else if(!(g_refused&1u)){g_refused|=1u;cr01_release_log_event("URT: PT light limit: the game code is not in the expected state (another mod?); left alone\r\n");}
 }
 cull=cull?1:0;
 if((g_avail&2u)&&(unsigned)cull!=g_cull){
  if(set_cull(cull)){g_cull=(unsigned)cull;
   if(g_logged<16u){g_logged++;cr01_release_log_event(cull?"URT: PT small-object culling active\r\n":"URT: PT small-object culling off (native)\r\n");}}
  else if(!(g_refused&2u)){g_refused|=2u;cr01_release_log_event("URT: PT small-object culling: the game code is not in the expected state (another mod?); left alone\r\n");}
 }
 if(g_faults){(void)set_cap(0);(void)set_cull(0);g_cap=0;g_cull=0;
  cr01_release_log_event("URT: PT code limits: a code write failed; both limits restored to native for this session\r\n");}
}
void urt_ptcode501_shutdown(void){
 if(!g_base||!g_w)return;
 if(g_avail&1u)(void)set_cap(0);
 if(g_avail&2u)(void)set_cull(0);
 g_cap=0;g_cull=0;
}
void urt_ptcode501_stats(UrtPtCodeStats *s){
 if(!s)return;
 s->available=g_avail;s->cap=g_cap;s->cull=g_cull;s->faults=g_faults;s->writes=g_writes;
}
