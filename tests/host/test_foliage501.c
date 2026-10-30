/* V5.3.0 R8: RT/PT: Solid Distant Foliage, against the mapped Patch 5.01 witcher3.exe.
 * 1. the descriptor AHS_GBuffer_Trees, its strings and the native AnyHitTrees container (digest) verify;
 * 2. the one-comparison edit applies to the real AnyHitTrees disassembly (LLVM 18 text of the embedded DXIL, produced
 *    by the runner), only inside AnyHitTrees, and re-assembles with llvm-as; every anchor must be unique;
 * 3. the descriptor pointer swap is a compare-and-swap from the native header only (never over a foreign pointer).
 * The game-DXC build/validation/signing itself runs only on Windows (as for URT's PT library).
 * Usage: test_foliage501 <witcher3.exe> <AnyHitTrees.ll> */
#include "pe_map.h"
#include <unistd.h>
#include "../../src/urt_foliage501.h"
static int fails;
#define CHECK(c,m) do{ if(c) printf("PASS %s\n",m); else {printf("FAIL %s\n",m); fails++;} }while(0)
static int prot_calls;
static int prot(void*a,size_t n,int w){(void)a;(void)n;(void)w;prot_calls++;return 1;}
static char *slurp(const char*p,long*n){FILE*f=fopen(p,"rb");if(!f)return 0;fseek(f,0,2);*n=ftell(f);rewind(f);char*b=malloc(*n+1);if(fread(b,1,*n,f)!=(size_t)*n){fclose(f);return 0;}fclose(f);b[*n]=0;return b;}
int main(int argc,char**argv){
  PeImage img; if(argc<3||!pe_map_file(argv[1],&img)){puts("FAIL map");return 1;}
  uint8_t*B=img.base;
  CHECK(urt_fol_verify(B,img.size),"descriptor AHS_GBuffer_Trees -> AnyHitTrees, native 4520-byte container with the pinned digest");
  long n=0;char*ll=slurp(argv[2],&n);
  CHECK(ll&&n>1000,"AnyHitTrees disassembly read");
  if(!ll){printf("TOTAL FAIL\n");return 1;}
  uint32_t cap=(uint32_t)n+4096,on=0;char*out=malloc(cap);
  int ok=urt_fol_patch_ir(ll,(uint32_t)n,2,out,cap,&on);
  CHECK(ok&&strstr(out,"%urt_fol_near = fcmp fast ole float %")&&strstr(out,", 0x4034000000000000\n")&&strstr(out," = and i1 %urt_fol_a, %urt_fol_near"),
        "edit applied: transparent texel skipped only within 20 m (one comparison, same SSA result name)");
  {const char*d=strstr(out,"define void @\"\\01?AnyHitTrees@@"),*e=d?strstr(d,"\n}"):0,*x=strstr(out,"%urt_fol_near");
   CHECK(d&&e&&x>d&&x<e,"edit lies inside AnyHitTrees only");}
  CHECK((long)on-n>0&&(long)on-n<300,"nothing else in the module changed (size delta of three lines)");
  {uint32_t on2=0;CHECK(!urt_fol_patch_ir(out,on,2,malloc(cap+4096),cap+4096,&on2),"already-patched text refused (no double edit)");}
  {char*dup=malloc(n*2+16);memcpy(dup,ll,n);char*a=strstr(dup,", 0x3F1A36E2E0000000");
   /* a second transparent-texel test inside the function must make the anchor ambiguous -> refuse */
   char*ls=a;while(ls>dup&&ls[-1]!='\n')ls--;char*le=strchr(a,'\n');size_t L=(size_t)(le-ls)+1;
   memmove(le+1+L,le+1,(size_t)(dup+n-(le+1)));memcpy(le+1,ls,L);
   uint32_t o2=0;CHECK(!urt_fol_patch_ir(dup,(uint32_t)(n+L),2,malloc(cap+4096),cap+4096,&o2),"ambiguous anchor refused");}
  {FILE*f=fopen("/tmp/urt_fol_test.ll","wb");fwrite(out,1,on,f);fclose(f);
   int r=system("llvm-as /tmp/urt_fol_test.ll -o /tmp/urt_fol_test.bc 2>/dev/null");
   CHECK(r==0,"patched module re-assembles (llvm-as): valid SSA, types and control flow");}
  for(unsigned i=1;i<=4;i++){uint32_t o3=0;CHECK(urt_fol_patch_ir(ll,(uint32_t)n,i,malloc(cap),cap,&o3),"every distance option builds a text edit");}
  {uint32_t o4=0;CHECK(!urt_fol_patch_ir(ll,(uint32_t)n,0,out,cap,&o4)&&!urt_fol_patch_ir(ll,(uint32_t)n,5,out,cap,&o4),"Native (0) and out-of-range values never edit");}
  /* publish: CAS from the native header pointer only */
  uint8_t*fake=malloc(16);uint64_t*field=(uint64_t*)(void*)(B+URT_FOL_FIELD_RVA);uint64_t native=*field;
  CHECK(native==(uint64_t)(uintptr_t)(B+URT_FOL_HEADER_RVA),"descriptor holds the native header pointer");
  CHECK(urt_fol_publish(B,fake,prot)&&*field==(uint64_t)(uintptr_t)fake&&prot_calls==2,"publish swaps exactly the 8-byte pointer (protection opened and restored)");
  CHECK(!urt_fol_verify(B,img.size),"after the swap the native identity check no longer passes (no second publish)");
  uint8_t*fake2=malloc(16);
  CHECK(!urt_fol_publish(B,fake2,prot)&&*field==(uint64_t)(uintptr_t)fake,"a pointer that is not the native one is never overwritten");
  *field=native;
  CHECK(urt_fol_verify(B,img.size),"restored image verifies again");
  printf("TOTAL %s %d\n",fails?"FAIL":"PASS",fails);fflush(stdout);_exit(fails?1:0);   /* test buffers are process-lifetime */
}
