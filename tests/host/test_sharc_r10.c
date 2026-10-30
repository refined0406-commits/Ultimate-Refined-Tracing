/* V5.3.0 R10: SHaRC crash root cause and repair, verified against the REAL Patch 5.01 witcher3.exe.
 * 1. The game's SHaRC dispatch (0x1eefcd0 -> 0x1ef0010) picks its ray-generation shaders from the per-frame words
 *    written by the native serializer 0x1ef2340 into ri + frame*0x2200 + 0x300 (call site 0x1ef0fbb..0x1ef0fcf):
 *    Final (0x13/0x14) only when +0x210 == 2, +0x214 == 1, +0x380 == 4, +0x384 == 1, +0x410 == 0x3efff, +0x118 == 0,
 *    +0x4e4 == 0; otherwise 0x11/0x12.
 * 2. Raygen index = descriptor slot (0x3227270 + 0x50*i): 0x11/0x12 = PTRGS_SharcUpdate / PTRGS_SharcQuery,
 *    0x13/0x14 = the Final variants. Only the Final descriptors are referenced by code (state-object builder);
 *    the non-Final ones are referenced nowhere, so they are never in any state object.
 * 3. URT's serializer gate writes +0x210 (geometry | compensation << 8; 0x6402 at all-native settings) and +0x380
 *    whenever its PT shader is live -> with SHaRC on, the game dispatched unregistered shaders (the R9 crash).
 * 4. The R10 gate core: no custom write while SHaRC is on; the Final-word check accepts exactly the native words.
 * Usage: test_sharc_r10 <witcher3.exe> */
#include "urt_test_build.h"
#include "pe_map.h"
#include "../../src/cr01_gate_core.h"
#include <stdio.h>
#include <string.h>
static int fails;
#define CHECK(c,m) do{ if(c) printf("PASS %s\n",m); else {printf("FAIL %s\n",m); fails++;} }while(0)
static int bytes(const uint8_t*B,uint32_t rva,const char*hex){
  size_t n=strlen(hex)/2;for(size_t i=0;i<n;i++){unsigned v;sscanf(hex+2*i,"%2x",&v);if(B[rva+i]!=(uint8_t)v)return 0;}return 1;}
static uint64_t q(const uint8_t*B,uint32_t rva){uint64_t v;memcpy(&v,B+rva,8);return v;}
static int wname(const uint8_t*B,uint64_t va,const char*s){
  /* pe_map applies the base relocations: descriptor pointers are live addresses inside the mapped image */
  if(va<(uint64_t)(uintptr_t)B)return 0;const uint8_t*p=(const uint8_t*)(uintptr_t)va;
  for(size_t i=0;;i++){if(p[2*i]!=(uint8_t)s[i]||p[2*i+1])return 0;if(!s[i])return 1;}}
int main(int argc,char**argv){
  PeImage img;if(argc<2||!pe_map_file(argv[1],&img)){puts("FAIL map");return 1;}
  if(!urt_test_build_init(img.size))return 1;
  const uint8_t*B=img.base;
  /* 1 */
  CHECK(bytes(B,RV(0x1ef0010),"83bc321804000000754b83bc321405000001754183bc321005000002753783bc328006000004752d83bc32e407000000752383bc328406000001751981bc3210070000ffef0300750cba13000000bb14000000eb0aba11000000bb12000000"),
        "SHaRC dispatch selects raygen 0x13/0x14 only for frame words 0x418=0,0x514=1,0x510=2,0x680=4,0x7e4=0,0x684=1,0x710=0x3efff; else 0x11/0x12");
  CHECK(bytes(B,RV(0x1ef0fbb),"4869f8002200004881c7000300004803fe488bcfe86c130000"),
        "serializer 0x1ef2340 writes ri + frame*0x2200 + 0x300 (so frame word 0x510 = serialized +0x210, 0x680 = +0x380)");
  CHECK(bytes(B,RV(0x1ef29e1),"c783100200000200000044898314020000")&&bytes(B,RV(0x1ef26c2),"41b801000000"),
        "native serializer stores +0x210 = 2 (URT's geometry site) and +0x214 = r8d = 1");
  CHECK(bytes(B,RV(0x1ef2f5d),"44898384030000c5fa118360030000c7838003000004000000")&&bytes(B,RV(0x1ef2de4),"c78310040000ffef0300"),
        "native serializer stores +0x384 = 1, +0x380 = 4 (URT's hair site) and +0x410 = 0x3efff");
  CHECK(bytes(B,RV(0x1ef32bd),"8983180400008bc23897b40400000f95c08983e4040000")&&bytes(B,RV(0x1ef341e),"898318010000"),
        "+0x118 and +0x4e4 come from PT parameters (0 in normal play)");
  /* 2 */
  const struct {uint32_t d;const char*n;} rg[]={{RV(0x3227720),"PTRGSFinal"},{RV(0x32277c0),"PTRGS_SharcUpdate"},{RV(0x3227810),"PTRGS_SharcQuery"},
    {RV(0x3227860),"PTRGS_SharcUpdateFinal"},{RV(0x32278b0),"PTRGS_SharcQueryFinal"}};
  CHECK((RV(0x3227720u)-RV(0x3227270u))/0x50u==0xfu&&(RV(0x32277c0u)-RV(0x3227270u))/0x50u==0x11u&&(RV(0x3227810u)-RV(0x3227270u))/0x50u==0x12u&&
        (RV(0x3227860u)-RV(0x3227270u))/0x50u==0x13u&&(RV(0x32278b0u)-RV(0x3227270u))/0x50u==0x14u,"raygen index = descriptor slot (0xf = PTRGSFinal as the normal PT dispatch uses)");
  {int ok=1;for(unsigned i=0;i<5;i++)ok&=wname(B,q(B,rg[i].d+0x10),rg[i].n);CHECK(ok,"descriptor export names: 0x11/0x12 non-Final SHaRC, 0x13/0x14 Final SHaRC");}
  /* xrefs: rip-relative disp32 to the descriptor (and its second table copy) anywhere in .text */
  {const uint32_t T[8]={RV(0x32277c0),RV(0x3227810),RV(0x33e9ef0),RV(0x33e9f40), RV(0x3227860),RV(0x32278b0),RV(0x33e9f90),RV(0x33e9fe0)};unsigned cnt[8]={0};
   uint32_t pe=pm_u32(B+0x3c);const uint8_t*sh=B+pe+24+pm_u16(B+pe+20);uint32_t tva=pm_u32(sh+12),tvs=pm_u32(sh+8);
   for(uint32_t p=tva;p+4<tva+tvs;p++){int32_t d;memcpy(&d,B+p,4);int64_t e=(int64_t)p+4+d;
     for(unsigned k=0;k<8;k++)if(e==T[k]&&(B[p-2]==0x8b||B[p-2]==0x8d)&&B[p-3]>=0x48&&B[p-3]<=0x4f)cnt[k]++;}
   CHECK(cnt[0]==0&&cnt[1]==0&&cnt[2]==0&&cnt[3]==0,"non-Final PTRGS_SharcUpdate / PTRGS_SharcQuery: referenced by no code (never registered)");
   CHECK(cnt[4]==1&&cnt[5]==1,"Final SHaRC descriptors: referenced once each (state-object builder 0x1ee6df0 / 0x1ee6e61)");}
  /* 3 + 4: gate behaviour on a serialized buffer */
  {static uint8_t dst[0x600];CR01GateState g={0};CR01GateFields f={2,4,100,1,1,1,1};
   memset(dst,0,sizeof dst);uint32_t v=2;memcpy(dst+0x210,&v,4);v=1;memcpy(dst+0x214,&v,4);v=4;memcpy(dst+0x380,&v,4);v=1;memcpy(dst+0x384,&v,4);v=0x3efff;memcpy(dst+0x410,&v,4);
   CHECK(cr01_gate_sharc_final_words(dst),"native serialized words select the registered Final SHaRC shaders");
   f.comp=105;CHECK(cr01_gate_publish(&g,f,1),"gate staged: geometry 2, hair 4, Indirect-Light Compensation 105");
   CHECK(cr01_gate_post_serialize(&g,dst,1)==1&&!cr01_gate_sharc_final_words(dst),"R9 behaviour reproduced: URT's write makes the frame words select unregistered 0x11/0x12");
   v=2;memcpy(dst+0x210,&v,4);v=4;memcpy(dst+0x380,&v,4);
   CHECK(cr01_gate_effective_custom(1,1)==0&&cr01_gate_post_serialize(&g,dst,cr01_gate_effective_custom(1,1))==0&&cr01_gate_sharc_final_words(dst),
         "R10: SHaRC on -> no URT write, words stay native, registered shaders selected");
   CHECK(cr01_gate_effective_custom(1,0)==1&&cr01_gate_effective_custom(0,0)==0,"R10: SHaRC off -> URT's PT shader controls unchanged");
   v=1;memcpy(dst+0x118,&v,4);CHECK(!cr01_gate_sharc_final_words(dst),"safety net: any non-native word (e.g. +0x118) is detected");}
  printf("TOTAL %s %d\n",fails?"FAIL":"PASS",fails);return fails?1:0;
}
