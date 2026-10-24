#include "cr01_contract.h"
void cr01_defaults(CR01Settings *s){unsigned i;s->master=0;for(i=0;i<CR01_FEATURE_COUNT;i++)s->v[i]=0;
  s->v[F_INDIRECT]=s->v[F_SUN]=s->v[F_SKY]=100;cr01_rtao_defaults(&s->rtao);}
int cr01_wants_write(const CR01Settings *s,unsigned f){
  if(!s||!s->master||f>=CR01_FEATURE_COUNT)return 0;
  /* 1.00 is a deliberately neutral, no-write compensation sentinel. */
  if(f==F_INDIRECT||f==F_SUN||f==F_SKY)return s->v[f]!=100;
  return s->v[f]!=0;
}
/* A foreign write cancels ownership; never overwrite another mod's value. */
int cr01_write_owner(CR01Owner *o,uint32_t *dst,uint32_t desired){
  uint32_t current,old;
  if(!o||!dst||o->conflicted)return 0;
  current=*(volatile uint32_t *)dst;
  if(o->owned){
    if(o->ptr!=(uintptr_t)dst){o->conflicted=1;return 0;}
    if(current!=o->installed){o->owned=0;o->conflicted=1;return 0;}
    if(current!=desired){
      old=__sync_val_compare_and_swap((volatile uint32_t*)dst,current,desired);
      if(old!=current){o->owned=0;o->conflicted=1;return 0;}
    }
    if(*(volatile uint32_t *)dst!=desired)return 0;
    o->installed=desired;return 1;
  }
  /* Do not own already-native state or recapture a foreign edit as native. */
  if(current==desired)return 1;
  old=__sync_val_compare_and_swap((volatile uint32_t*)dst,current,desired);
  if(old!=current){o->conflicted=1;return 0;}
  o->ptr=(uintptr_t)dst;o->native=current;o->installed=desired;o->owned=1;
  return *(volatile uint32_t *)dst==desired;
}
int cr01_release_owner(CR01Owner *o){
  uint32_t *dst,old;
  if(!o)return 0;
  if(!o->owned){o->conflicted=0;return 1;}
  dst=(uint32_t *)o->ptr;
  if(!dst)return 0;
  old=__sync_val_compare_and_swap((volatile uint32_t *)dst,o->installed,o->native);
  if(old!=o->installed&&old!=o->native){o->conflicted=1;}
  if(old==o->installed&&*(volatile uint32_t *)dst!=o->native)return 0;
  o->owned=0;
  return 1;
}
void cr01_release_all(CR01Owner o[CR01_FEATURE_COUNT]){unsigned i;for(i=0;i<CR01_FEATURE_COUNT;i++)(void)cr01_release_owner(o+i);}
