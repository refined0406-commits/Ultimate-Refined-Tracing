/* Research tool (host only): dump every engine CVar registry descriptor of a
 * witcher3.exe image as TSV: desc_rva value_rva kind default name category raw[0..63]. */
#include "../../tests/host/pe_map.h"
#include "../../src/urt_cvar_index.h"
#include <stdio.h>
static uint64_t u64(const uint8_t*p){uint64_t v=0;for(int i=7;i>=0;i--)v=v<<8|p[i];return v;}
static uint32_t u32(const uint8_t*p){return (uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;}
int main(int argc,char**argv){
  PeImage img; if(argc<2||!pe_map_file(argv[1],&img)){fprintf(stderr,"map fail\n");return 1;}
  static UrtCvarEntry st[200000]; UrtCvarIndex ix;
  if(!urt_cvar_index_build(&ix,img.base,img.size,st,200000)){fprintf(stderr,"build fail\n");return 1;}
  for(uint32_t i=0;i<ix.count;i++){
    const UrtCvarEntry*e=&st[i]; const uint8_t*d=img.base+e->desc_rva;
    const char*nm=(const char*)img.base+e->name_rva; size_t n=0; while(nm[n])n++;
    const char*cat=nm+n; while(!*cat && cat<nm+n+16)cat++;
    printf("%07x\t%07x\t%llx\t%u\t%s\t%s\t",e->desc_rva,e->value_rva,(unsigned long long)u64(d+8),u32(d+24),nm,cat);
    for(int k=0;k<64;k+=4)printf("%08x ",u32(d+k)); printf("\n");
  }
  fprintf(stderr,"entries=%u\n",ix.count); return 0;
}
