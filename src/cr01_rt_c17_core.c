/* C17 PRIVATE extra grid caps. Separate CAS ownership from immutable C16 six fields.
 * Updates/restorations occur on the EXISTING asynchronous C16 worker.
 * Not a synchronized renderer precommit barrier or proof of GPU consumption. */
#include "cr01_rt_c17_core.h"
#include <stddef.h>
static uint32_t read_slot(uint32_t*p){return __atomic_load_n(p,__ATOMIC_ACQUIRE);}
static int write_if_equal(uint32_t*p,uint32_t old,uint32_t value){
 return __atomic_compare_exchange_n(p,&old,value,0,__ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE);
}
void cr01_c17_caps_init(CR01C17CapsOwner*s){
 if(!s)return;
 s->mask=s->active=s->conflict=s->preset=s->parent_revision=s->local_revision=0;
 for(unsigned i=0;i<CR01_C17_CAP_FIELDS;i++){
  s->slot[i]=NULL;s->original[i]=s->applied[i]=0;
 }
}
static int in_choices(uint32_t v,const uint32_t*allowed,unsigned n){
 for(unsigned i=0;i<n;i++)if(v==allowed[i])return 1;
 return 0;
}
int cr01_c17_caps_profile_valid(const CR01C17CapsProfile*p){
 static const uint32_t global[]={0u,4096u,8192u,12288u,16384u,24576u,32768u};
 static const uint32_t grid1[]={0u,16384u,32768u,49152u,65536u,131072u,262144u};
 return p&&in_choices(p->values[CR01_C17_GLOBAL],global,7u)&&
  in_choices(p->values[CR01_C17_GRID1],grid1,7u);
}
void cr01_c17_caps_release(CR01C17CapsOwner*s){
 if(!s)return;
 for(unsigned i=0;i<CR01_C17_CAP_FIELDS;i++)if(s->mask&(1u<<i)){
  if(!s->slot[i]||!write_if_equal(s->slot[i],s->applied[i],s->original[i]))s->conflict=1;
 }
 s->mask=0;s->active=0;
}
int cr01_c17_caps_update(CR01C17CapsOwner*s,const CR01C17CapsProfile*p,
 unsigned eligible,unsigned preset,uint32_t parent_revision,uint32_t local_revision){
 if(!s||!p)return 0;
 if(!eligible||preset>1||!cr01_c17_caps_profile_valid(p)){
  cr01_c17_caps_release(s);return 0;
 }
 for(unsigned i=0;i<CR01_C17_CAP_FIELDS;i++)if(!s->slot[i]){
  cr01_c17_caps_release(s);return 0;
 }
 if(s->active){
  if(s->preset!=preset||s->parent_revision!=parent_revision||s->local_revision!=local_revision)
   cr01_c17_caps_release(s);
  else {
   for(unsigned i=0;i<CR01_C17_CAP_FIELDS;i++)if(s->mask&(1u<<i)){
    if(read_slot(s->slot[i])!=s->applied[i]){
     cr01_c17_caps_release(s);return 0; /* foreign writer, never clobber */
    }
   }
   return 1;
  }
 }
 for(unsigned i=0;i<CR01_C17_CAP_FIELDS;i++){
  s->original[i]=read_slot(s->slot[i]);s->applied[i]=s->original[i];
  /* No ownership of uninitialized, implausible or compromised slots. */
  if(s->original[i]<128u||s->original[i]>67108864u)return 0;
 }
 for(unsigned i=0;i<CR01_C17_CAP_FIELDS;i++){
  const uint32_t target=p->values[i];
  if(target==0u||target==s->original[i])continue; /* Native Auto never writes */
  if(!write_if_equal(s->slot[i],s->original[i],target)){
   cr01_c17_caps_release(s);return 0;
  }
  s->applied[i]=target;s->mask|=1u<<i;
 }
 s->active=1;s->preset=preset;s->parent_revision=parent_revision;s->local_revision=local_revision;
 return 1;
}
