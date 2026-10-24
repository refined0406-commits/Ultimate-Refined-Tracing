/* URT V5 one-pass CVar registry index. See urt_cvar_index.h.
 * Semantics intentionally equal the V4.x find_cvar resolver; the host test
 * tests/host/test_cvar_index.c proves equality against the real Patch 5.01
 * image for every name URT uses and for a large sample of all registry names. */
#include "urt_cvar_index.h"

#define SCN_READ  0x40000000u
#define SCN_WRITE 0x80000000u

static uint32_t rd32(const uint8_t *p){return (uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;}
static uint16_t rd16(const uint8_t *p){return (uint16_t)(p[0]|p[1]<<8);}
static uint64_t rd64(const uint8_t *p){return (uint64_t)rd32(p)|((uint64_t)rd32(p+4)<<32);}

uint32_t urt_fnv1a(const char *s){
  uint32_t h=2166136261u;
  while(*s){h^=(uint8_t)*s++;h*=16777619u;}
  return h;
}

typedef struct { uint32_t rva, span, flags; } Sec;

static int sections(const uint8_t *base, uint32_t image, Sec *out, unsigned *n){
  if(!base||image<0x1000||base[0]!='M'||base[1]!='Z')return 0;
  uint32_t pe=rd32(base+0x3c);
  if(pe>image-256)return 0;
  unsigned count=rd16(base+pe+6);
  uint32_t sec=pe+24+rd16(base+pe+20);
  if(count==0||count>96||sec>image||count*40u>image-sec)return 0;
  for(unsigned i=0;i<count;i++){
    const uint8_t *sh=base+sec+i*40u;
    uint32_t span=rd32(sh+8),rva=rd32(sh+12);
    if(!span)span=rd32(sh+16);
    out[i].rva=rva;out[i].span=span;out[i].flags=rd32(sh+36);
    if(rva>image||span>image-rva){out[i].flags=0;}
  }
  *n=count;return 1;
}

/* writable_range() of V4.x: target in some writable section, 4 bytes fit. */
static int in_writable(const Sec *s, unsigned n, uint32_t rva){
  for(unsigned i=0;i<n;i++){
    if(!(s[i].flags&SCN_WRITE))continue;
    if(s[i].span<4)continue;
    if(rva>=s[i].rva&&rva<=s[i].rva+s[i].span-4)return 1;
  }
  return 0;
}

/* V4.x accepted a name occurrence at offset `at` of a readable section of
 * span S only when at+n<S (its NUL inside the section) and n<80. */
static int name_ok(const uint8_t *base, const Sec *s, unsigned n, uint32_t rva, uint32_t *len){
  for(unsigned i=0;i<n;i++){
    if(!(s[i].flags&SCN_READ))continue;
    if(rva<s[i].rva||rva>=s[i].rva+s[i].span)continue;
    uint32_t end=s[i].rva+s[i].span, k=0;
    while(k<80u&&rva+k<end){
      uint8_t c=base[rva+k];
      if(c==0)break;
      if(c<0x20||c>0x7e)return 0;
      k++;
    }
    if(k==0||k>=80u||rva+k>=end||base[rva+k]!=0)return 0;
    *len=k;return 1;
  }
  return 0;
}

int urt_cvar_index_build(UrtCvarIndex *ix, uint8_t *base, uint32_t image,
                         UrtCvarEntry *storage, uint32_t capacity){
  Sec s[96];unsigned n=0;
  if(!ix)return 0;
  ix->base=base;ix->image_size=image;ix->entries=storage;ix->count=0;ix->capacity=capacity;ix->overflow=0;
  if(!storage||!capacity||!sections(base,image,s,&n))return 0;
  uintptr_t lo=(uintptr_t)base, hi=(uintptr_t)base+image;
  for(unsigned w=0;w<n;w++){
    if(!(s[w].flags&SCN_WRITE)||s[w].span<24)continue;
    for(uint32_t off=0;off+24<=s[w].span;off+=8){
      const uint8_t *d=base+s[w].rva+off;
      uint64_t q=rd64(d);
      if(q<lo||q>=hi)continue;
      uint64_t loc=rd64(d+16);
      if(loc<lo||loc>=hi)continue;
      uint32_t name_rva=(uint32_t)(q-lo), value_rva=(uint32_t)(loc-lo), len=0;
      if(!in_writable(s,n,value_rva))continue;
      if(!name_ok(base,s,n,name_rva,&len))continue;
      if(ix->count>=capacity){ix->overflow=1;return 0;}
      UrtCvarEntry *e=&storage[ix->count++];
      e->name_rva=name_rva;e->value_rva=value_rva;e->desc_rva=s[w].rva+off;
      e->hash=urt_fnv1a((const char*)base+name_rva);
    }
  }
  return 1;
}

static int streq(const char *a, const char *b){
  while(*a&&*a==*b){a++;b++;}
  return *a==*b;
}

uint32_t *urt_cvar_lookup(const UrtCvarIndex *ix, const char *name){
  uint32_t found=0,desc=0;
  if(!ix||!ix->base||ix->overflow||!name||!name[0])return 0;
  unsigned len=0;while(name[len]&&len<80)len++;
  if(len==80)return 0;
  uint32_t h=urt_fnv1a(name);
  for(uint32_t i=0;i<ix->count;i++){
    const UrtCvarEntry *e=&ix->entries[i];
    if(e->hash!=h||!streq((const char*)ix->base+e->name_rva,name))continue;
    if(found&&found!=e->value_rva)return 0; /* ambiguous: fail closed */
    found=e->value_rva;desc=e->desc_rva;
  }
  (void)desc;
  return found?(uint32_t*)(void*)(ix->base+found):0;
}

int urt_cvar_descriptor(const UrtCvarIndex *ix, const char *name, uint32_t *desc_out){
  uint32_t found=0,desc=0;
  if(!ix||!ix->base||ix->overflow||!name||!desc_out)return 0;
  uint32_t h=urt_fnv1a(name);
  for(uint32_t i=0;i<ix->count;i++){
    const UrtCvarEntry *e=&ix->entries[i];
    if(e->hash!=h||!streq((const char*)ix->base+e->name_rva,name))continue;
    if(found&&(found!=e->value_rva||desc!=e->desc_rva))return 0; /* need one unique descriptor */
    found=e->value_rva;desc=e->desc_rva;
  }
  if(!found)return 0;
  *desc_out=desc;return 1;
}

uint32_t *urt_cvar_lookup_scoped(const UrtCvarIndex *ix, const char *name,
                                 const char *category, uint64_t kind, uint32_t *desc_out){
  uint32_t found=0,desc=0;
  if(!ix||!ix->base||ix->overflow||!name||!category)return 0;
  uint32_t h=urt_fnv1a(name);
  for(uint32_t i=0;i<ix->count;i++){
    const UrtCvarEntry *e=&ix->entries[i];
    if(e->hash!=h||!streq((const char*)ix->base+e->name_rva,name))continue;
    if(kind&&rd64(ix->base+e->desc_rva+8)!=kind)continue;
    /* Category string: the next string after the name and its NUL padding. */
    uint32_t a=e->name_rva;
    while(a<ix->image_size&&ix->base[a])a++;
    while(a<ix->image_size&&ix->base[a]==0)a++;
    unsigned k=0;
    while(category[k]&&a+k<ix->image_size&&ix->base[a+k]==(uint8_t)category[k])k++;
    if(category[k]||a+k>=ix->image_size||ix->base[a+k]!=0)continue;
    if(found&&found!=e->value_rva)return 0;
    found=e->value_rva;desc=e->desc_rva;
  }
  if(!found)return 0;
  if(desc_out)*desc_out=desc;
  return (uint32_t*)(void*)(ix->base+found);
}

/* V5.1 (additive): identity by the descriptor's REAL category pointer (+48),
 * its flag word (+8) and the low 32 bits of the registered default (+24).
 * Patch 5.01 registry descriptor (56 bytes): +0 name*, +8 flags (low byte type:
 * 0 bool, 1 int, 2 float), +16 value*, +24 default, +40 type*, +48 category*.
 * Unique match required; anything ambiguous or malformed fails closed. */
uint32_t *urt_cvar_lookup_cat(const UrtCvarIndex *ix, const char *name, const char *category,
                              uint64_t flags, uint32_t default_lo, uint32_t *desc_out){
  uint32_t found=0,desc=0;
  if(!ix||!ix->base||ix->overflow||!name||!category||!name[0]||!category[0])return 0;
  uintptr_t lo=(uintptr_t)ix->base, hi=(uintptr_t)ix->base+ix->image_size;
  uint32_t h=urt_fnv1a(name);
  for(uint32_t i=0;i<ix->count;i++){
    const UrtCvarEntry *e=&ix->entries[i];
    if(e->hash!=h||!streq((const char*)ix->base+e->name_rva,name))continue;
    if(e->desc_rva>ix->image_size-56u)continue;
    const uint8_t *d=ix->base+e->desc_rva;
    if(rd64(d+8)!=flags||rd32(d+24)!=default_lo)continue;
    uint64_t c=rd64(d+48);
    if(c<lo||c>=hi)continue;
    uint32_t crva=(uint32_t)(c-lo),k=0;
    while(category[k]&&crva+k<ix->image_size&&ix->base[crva+k]==(uint8_t)category[k])k++;
    if(category[k]||crva+k>=ix->image_size||ix->base[crva+k]!=0)continue;
    if(found&&found!=e->value_rva)return 0;
    found=e->value_rva;desc=e->desc_rva;
  }
  if(!found)return 0;
  if(desc_out)*desc_out=desc;
  return (uint32_t*)(void*)(ix->base+found);
}
