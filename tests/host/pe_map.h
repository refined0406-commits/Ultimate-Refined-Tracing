/* Host-only (Linux) test helper: map a Windows x64 PE image into memory the
 * way the Windows loader would (headers + sections at their RVAs, base
 * relocations applied for the actual mapping address). Used to run URT's pure
 * resolver code against the real witcher3.exe without Windows. */
#ifndef URT_PE_MAP_H
#define URT_PE_MAP_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

typedef struct { uint8_t *base; uint32_t size; uint64_t preferred; } PeImage;

static uint32_t pm_u32(const uint8_t *p){uint32_t v;memcpy(&v,p,4);return v;}
static uint16_t pm_u16(const uint8_t *p){uint16_t v;memcpy(&v,p,2);return v;}

static int pe_map_file(const char *path, PeImage *img){
  FILE *f=fopen(path,"rb"); if(!f) return 0;
  fseek(f,0,SEEK_END); long n=ftell(f); fseek(f,0,SEEK_SET);
  uint8_t *file=malloc((size_t)n); if(!file||fread(file,1,(size_t)n,f)!=(size_t)n){fclose(f);return 0;}
  fclose(f);
  uint32_t pe=pm_u32(file+0x3c); const uint8_t *opt=file+pe+24;
  uint16_t nsec=pm_u16(file+pe+6), optsz=pm_u16(file+pe+20);
  uint64_t pref; memcpy(&pref,opt+24,8);
  uint32_t image=pm_u32(opt+56), hdr=pm_u32(opt+60);
  uint8_t *base=mmap(0,image,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
  if(base==MAP_FAILED) return 0;
  memcpy(base,file,hdr);
  const uint8_t *sh=file+pe+24+optsz;
  for(unsigned i=0;i<nsec;i++,sh+=40){
    uint32_t vs=pm_u32(sh+8),va=pm_u32(sh+12),rs=pm_u32(sh+16),ro=pm_u32(sh+20);
    uint32_t c=rs<vs||!vs?rs:vs; if(c) memcpy(base+va,file+ro,c);
  }
  /* base relocations: data directory 5 */
  uint32_t rrva=pm_u32(opt+112+5*8), rsz=pm_u32(opt+112+5*8+4);
  int64_t delta=(int64_t)((uintptr_t)base-pref);
  for(uint32_t o=0;o<rsz;){
    uint32_t page=pm_u32(base+rrva+o), bs=pm_u32(base+rrva+o+4); if(!bs) break;
    for(uint32_t e=8;e<bs;e+=2){
      uint16_t t=pm_u16(base+rrva+o+e);
      if((t>>12)==10){uint64_t v;memcpy(&v,base+page+(t&0xfff),8);v+=(uint64_t)delta;memcpy(base+page+(t&0xfff),&v,8);}
    }
    o+=bs;
  }
  free(file);
  img->base=base; img->size=image; img->preferred=pref; return 1;
}
#endif
