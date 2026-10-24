#include "cr01_startup_barrier.h"
int cr01_bs_sync_prepare(CR01StartupBarrier*s,CR01PrepareOneContainer callback,void*opaque){
 uint32_t stage=CR01_BS_UNSTARTED;const uint8_t*p=0;size_t n=0;
 if(!s||!callback)return 0;
 if(!__atomic_compare_exchange_n(&s->stage,&stage,CR01_BS_PREPARING,0,
      __ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE))return stage==CR01_BS_PREPARED;
 /* If registration already chose native, don't attempt custom DXIL later. */
 if(__atomic_load_n(&s->registration_policy,__ATOMIC_ACQUIRE)!=CR01_REG_UNDECIDED){
  __atomic_store_n(&s->stage,CR01_BS_NATIVE_FAILED,__ATOMIC_RELEASE);return 0;
 }
 if(!callback(opaque,&p,&n)||!p||!n){
  __atomic_store_n(&s->stage,CR01_BS_NATIVE_FAILED,__ATOMIC_RELEASE);return 0;
 }
 s->custom_container=p;s->custom_size=n;
 __atomic_store_n(&s->stage,CR01_BS_PREPARED,__ATOMIC_RELEASE);
 /* Registration can race completion: its one-time policy freezes the winner.
    Even if it froze Native during preparation, NEVER promote mid-process. */
 return __atomic_load_n(&s->registration_policy,__ATOMIC_ACQUIRE)!=CR01_REG_NATIVE;
}
CR01SharedShaderPair cr01_bs_registration(CR01StartupBarrier*s,
                                           const uint8_t*native,size_t n){
 CR01SharedShaderPair result={native,n,CR01_REG_NATIVE};
 uint32_t selected,candidate,unselected;
 if(!s||!native||!n)return result;
 selected=__atomic_load_n(&s->registration_policy,__ATOMIC_ACQUIRE);
 if(selected==CR01_REG_UNDECIDED){
  candidate=__atomic_load_n(&s->stage,__ATOMIC_ACQUIRE)==CR01_BS_PREPARED&&
             s->custom_container&&s->custom_size?CR01_REG_CUSTOM:CR01_REG_NATIVE;
  unselected=CR01_REG_UNDECIDED;
  if(!__atomic_compare_exchange_n(&s->registration_policy,&unselected,candidate,0,
           __ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE))selected=unselected;
  else selected=candidate;
 }
 if(selected==CR01_REG_CUSTOM){
  result.bytes=s->custom_container;result.bytes_len=s->custom_size;result.policy=selected;
 }
 return result;
}
