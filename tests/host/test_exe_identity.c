/* Host test: the C code-identity planner + streaming feed (exactly the code
 * linked into the ASI) reproduces tools/exe_codeid.py on the real exe, with
 * the same 64 KiB chunking the ASI uses, and rejects tampered variants. */
#include "../../src/urt_exe_identity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* --- minimal SHA-256 (FIPS 180-4) --- */
typedef struct{uint32_t h[8];uint64_t n;uint8_t b[64];uint32_t l;}S;
static const uint32_t K[64]={0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};
#define R(x,n) (((x)>>(n))|((x)<<(32-(n))))
static void blk(S*s,const uint8_t*p){uint32_t w[64],a[8];for(int i=0;i<16;i++)w[i]=(uint32_t)p[4*i]<<24|p[4*i+1]<<16|p[4*i+2]<<8|p[4*i+3];
 for(int i=16;i<64;i++)w[i]=w[i-16]+(R(w[i-15],7)^R(w[i-15],18)^(w[i-15]>>3))+w[i-7]+(R(w[i-2],17)^R(w[i-2],19)^(w[i-2]>>10));
 memcpy(a,s->h,32);for(int i=0;i<64;i++){uint32_t t1=a[7]+(R(a[4],6)^R(a[4],11)^R(a[4],25))+((a[4]&a[5])^(~a[4]&a[6]))+K[i]+w[i];
 uint32_t t2=(R(a[0],2)^R(a[0],13)^R(a[0],22))+((a[0]&a[1])^(a[0]&a[2])^(a[1]&a[2]));memmove(a+1,a,28);a[4]+=t1;a[0]=t1+t2;}
 for(int i=0;i<8;i++)s->h[i]+=a[i];}
static void sinit(S*s){static const uint32_t i0[8]={0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};memcpy(s->h,i0,32);s->n=0;s->l=0;}
static int supd(void*c,const uint8_t*p,uint32_t n){S*s=c;s->n+=n;while(n){uint32_t k=64-s->l;if(k>n)k=n;memcpy(s->b+s->l,p,k);s->l+=k;p+=k;n-=k;if(s->l==64){blk(s,s->b);s->l=0;}}return 1;}
static void sfin(S*s,char hex[65]){uint64_t bits=s->n*8;uint8_t pad=0x80,z=0;supd(s,&pad,1);while(s->l!=56)supd(s,&z,1);uint8_t L[8];for(int i=0;i<8;i++)L[i]=(uint8_t)(bits>>(56-8*i));supd(s,L,8);
 for(int i=0;i<8;i++)sprintf(hex+8*i,"%08x",s->h[i]);}
static int codeid(const uint8_t*d,size_t n,char hex[65]){
  UrtCodeIdPlan pl; S s; sinit(&s);
  if(!urt_codeid_plan(d,n<65536?n:65536,n,&pl))return 0;
  supd(&s,pl.layout,pl.layout_len);
  for(size_t off=0;off<n;off+=65536){uint32_t len=(uint32_t)(n-off<65536?n-off:65536);
    if(!urt_codeid_feed_chunk(&pl,off,d+off,len,supd,&s))return 0;}
  sfin(&s,hex);return 1;
}
int main(int c,char**v){
  FILE*f=fopen(v[1],"rb");fseek(f,0,SEEK_END);size_t n=(size_t)ftell(f);fseek(f,0,SEEK_SET);
  uint8_t*d=malloc(n);if(fread(d,1,n,f)!=n)return 1;fclose(f);
  char h[65];const char*want="ea0568e9a5cec889a27bea86ee59f4a9988a77e61b3e6136d6e389107f685087";
  if(!codeid(d,n,h)||strcmp(h,want)){printf("FAIL codeid %s\n",h);return 1;}
  printf("  real exe code-id %s (matches Python reference)\n",h);
  int fails=0;
  /* 1. resource-only change (inside .rsrc raw data) keeps identity */
  uint8_t*m=malloc(n);memcpy(m,d,n);m[0x551c800+0x1000]^=0xff;
  if(!codeid(m,n,h)||strcmp(h,want)){puts("FAIL: .rsrc edit changed code-id");fails++;}else puts("  .rsrc byte edit: identity kept (CODE_IDENTICAL tier)");
  /* 2. one .text byte change breaks identity */
  memcpy(m,d,n);m[0x600+0x123456]^=1;
  if(codeid(m,n,h)&&!strcmp(h,want)){puts("FAIL: .text edit kept code-id");fails++;}else puts("  .text byte edit: identity rejected");
  /* 3. .data change breaks identity */
  memcpy(m,d,n);m[0x3ff0600+0x10]^=1;
  if(codeid(m,n,h)&&!strcmp(h,want)){puts("FAIL: .data edit kept code-id");fails++;}else puts("  .data byte edit: identity rejected");
  /* 4. entry point change breaks identity */
  memcpy(m,d,n);{uint32_t pe;memcpy(&pe,m+0x3c,4);m[pe+24+16]^=1;}
  if(codeid(m,n,h)&&!strcmp(h,want)){puts("FAIL: entry edit kept code-id");fails++;}else puts("  entry-point edit: identity rejected");
  /* 5. appended overlay / signature (file grows) keeps identity */
  uint8_t*g=malloc(n+4096);memcpy(g,d,n);memset(g+n,0x5a,4096);
  if(!codeid(g,n+4096,h)||strcmp(h,want)){puts("FAIL: overlay changed code-id");fails++;}else puts("  appended overlay/signature: identity kept");
  /* 6. truncated file fails closed */
  if(codeid(d,0x400000,h)&&!strcmp(h,want)){puts("FAIL truncated");fails++;}else puts("  truncated file: rejected");
  /* 7. malformed headers */
  memcpy(m,d,n);m[0]='X';if(codeid(m,n,h)){puts("FAIL bad MZ accepted");fails++;}else puts("  bad MZ header: rejected");
  if(fails){printf("FAIL %d\n",fails);return 1;}
  puts("PASS exe identity tiers");return 0;
}
