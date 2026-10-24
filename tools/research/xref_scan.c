/* Research tool: find RIP-relative references in .text to a set of target RVAs.
 * Brute force over every byte: disp32 at p, instruction end = p+4+imm (imm in {0,1,2,4}).
 * Reports candidate (instr_end_rva, disp_pos_rva, target). Verified later with llvm-objdump.
 * Usage: xref_scan witcher3.exe targets.txt  (targets: hex RVAs, one per line) */
#include "../../tests/host/pe_map.h"
#include <stdio.h>
int main(int argc,char**argv){
  PeImage img; if(argc<3||!pe_map_file(argv[1],&img))return 1;
  static uint32_t t[4096]; unsigned nt=0; FILE*f=fopen(argv[2],"r"); char line[64];
  while(fgets(line,64,f)&&nt<4096) t[nt++]=(uint32_t)strtoul(line,0,16); fclose(f);
  uint32_t pe=pm_u32(img.base+0x3c); const uint8_t*sh=img.base+pe+24+pm_u16(img.base+pe+20);
  uint32_t tva=pm_u32(sh+12), tvs=pm_u32(sh+8);   /* first section = .text */
  static const int imms[4]={0,1,2,4};
  for(uint32_t p=tva; p+4<tva+tvs; p++){
    int32_t d; memcpy(&d,img.base+p,4);
    for(int k=0;k<4;k++){
      int64_t tgt=(int64_t)p+4+imms[k]+d;
      for(unsigned i=0;i<nt;i++) if(tgt==t[i]) printf("%07x\t%07x\t%d\n",t[i],p,imms[k]);
    }
  }
  return 0;
}
