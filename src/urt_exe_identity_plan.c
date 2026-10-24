/* Pure planning half of urt_exe_identity (host-testable, no Win32). */
#include "urt_exe_identity.h"

static uint32_t r32(const uint8_t *p){return (uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;}
static uint16_t r16(const uint8_t *p){return (uint16_t)(p[0]|p[1]<<8);}
static void put(UrtCodeIdPlan *pl, const uint8_t *p, unsigned n){
  for(unsigned i=0;i<n;i++)pl->layout[pl->layout_len++]=p[i];
}
static void put32(UrtCodeIdPlan *pl, uint32_t v){uint8_t b[4]={(uint8_t)v,(uint8_t)(v>>8),(uint8_t)(v>>16),(uint8_t)(v>>24)};put(pl,b,4);}

int urt_codeid_plan(const uint8_t *h, size_t n, uint64_t file_size, UrtCodeIdPlan *pl){
  static const uint8_t tag[12]={'U','R','T','5','C','O','D','E','I','D','1',0};
  if(!h||!pl||n<0x200||h[0]!='M'||h[1]!='Z')return 0;
  uint32_t pe=r32(h+0x3c);
  if(pe+24+240>n||h[pe]!='P'||h[pe+1]!='E'||h[pe+2]||h[pe+3])return 0;
  if(r16(h+pe+4)!=0x8664)return 0;                 /* x64 only */
  uint16_t nsec=r16(h+pe+6),optsz=r16(h+pe+20);
  const uint8_t *opt=h+pe+24;
  if(r16(opt)!=0x20b||nsec==0||nsec>URT_CODEID_MAX_RANGES)return 0; /* PE32+ */
  uint32_t st=pe+24+optsz;
  if(st+(uint32_t)nsec*40u>n)return 0;
  pl->count=0;pl->layout_len=0;
  put(pl,tag,12);
  put32(pl,r32(opt+16));            /* AddressOfEntryPoint */
  put32(pl,r32(opt+56));            /* SizeOfImage */
  put(pl,opt+24,8);                 /* ImageBase */
  uint64_t last_end=0;
  for(unsigned i=0;i<nsec;i++){
    const uint8_t *sh=h+st+i*40u;
    int rsrc=sh[0]=='.'&&sh[1]=='r'&&sh[2]=='s'&&sh[3]=='r'&&sh[4]=='c'&&sh[5]==0;
    uint32_t vs=r32(sh+8),va=r32(sh+12),rs=r32(sh+16),ro=r32(sh+20),ch=r32(sh+36);
    if(rsrc)continue;
    put(pl,sh,8);put32(pl,va);put32(pl,vs);put32(pl,rs);put32(pl,ch);
    if(!rs)continue;
    if((uint64_t)ro+rs>file_size||ro<last_end)return 0; /* must be ascending, in-file */
    pl->start[pl->count]=ro;pl->length[pl->count]=rs;pl->count++;
    last_end=(uint64_t)ro+rs;
  }
  return pl->count>0;
}

int urt_codeid_feed_chunk(const UrtCodeIdPlan *pl, uint64_t off, const uint8_t *c,
                          uint32_t len, UrtFeed feed, void *ctx){
  uint64_t end=off+len;
  for(uint32_t i=0;i<pl->count;i++){
    uint64_t a=pl->start[i],b=a+pl->length[i];
    if(b<=off||a>=end)continue;
    uint64_t s=a>off?a:off,e=b<end?b:end;
    if(!feed(ctx,c+(s-off),(uint32_t)(e-s)))return 0;
  }
  return 1;
}

const char *urt_exe_identity_name(int t){
  return t==URT_EXE_EXACT?"exact Patch 5.01 executable":
         t==URT_EXE_CODE_IDENTICAL?"code-identical Patch 5.01 executable (resources/signature differ)":
         "unknown executable (adaptive CVar-only mode)";
}
