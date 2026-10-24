#include "cr01_registration_pair.h"
int cr01_pair_init(CR01RegistrationPair *p,const uint8_t *native,size_t len,
                   uintptr_t a,uintptr_t b){
 if(!p||!native||len!=231144u||!a||!b||a==b||a==UINTPTR_MAX||b==UINTPTR_MAX)return 0;
 p->startup.stage=CR01_BS_UNSTARTED;p->startup.registration_policy=CR01_REG_UNDECIDED;
 p->startup.custom_container=0;p->startup.custom_size=0;
 p->original=native;p->original_size=len;p->expected_final_return=a;
 p->expected_ser_return=b;p->seen_mask=0;p->fault=0;return 1;
}
CR01PairSelection cr01_pair_select(CR01RegistrationPair *p,uintptr_t ret,
                    const uint8_t *blob,size_t size){
 CR01PairSelection result={blob,size,CR01_PAIR_WRONG_OWNER,CR01_REG_NATIVE};
 uint32_t site;
 if(!p)return result;
 site=ret==p->expected_final_return?CR01_PT_FINAL:
      ret==p->expected_ser_return?CR01_PT_SER:0;
 if(!site)return result; /* other native registrations pass completely untouched */
 if(__atomic_load_n(&p->fault,__ATOMIC_ACQUIRE)){
  result.classification=CR01_PAIR_INVALID_NATIVE;return result;
 }
 /* A late hook that misses the first protected registration must never
  * substitute the SECOND export and create a mixed native/custom pipeline.
  * Patch 5.01's protected registration ordering is Final then SER; a SER-
  * first observation is therefore an unconditional native-only fault.
  * This detects one important late-attachment class but does NOT establish
  * exclusive pre-registration quiescence for production installation. */
 if(site==CR01_PT_SER &&
    !(__atomic_load_n(&p->seen_mask,__ATOMIC_ACQUIRE)&CR01_PT_FINAL)){
  __atomic_store_n(&p->fault,CR01_PAIR_LATE,__ATOMIC_RELEASE);
  uint32_t undecided=CR01_REG_UNDECIDED;
  (void)__atomic_compare_exchange_n(&p->startup.registration_policy,&undecided,
              CR01_REG_NATIVE,0,__ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE);
  result.classification=CR01_PAIR_LATE;
  return result;
 }
 if(blob!=p->original||size!=p->original_size){
  __atomic_store_n(&p->fault,CR01_PAIR_INVALID_NATIVE,__ATOMIC_RELEASE);
  /* Any unexpected protected input before selection freezes NATIVE. */
  uint32_t undecided=CR01_REG_UNDECIDED;
  (void)__atomic_compare_exchange_n(&p->startup.registration_policy,
                 &undecided,CR01_REG_NATIVE,0,__ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE);
  result.classification=CR01_PAIR_INVALID_NATIVE;
  /* A mismatched original registration MUST be detected before committing
   * any hook. Runtime detection is emergency containment only: never claim
   * success or silently mix partial native/custom registration. */
  return result;
 }
 CR01SharedShaderPair choice=cr01_bs_registration(&p->startup,blob,size);
 result.data=choice.bytes;result.size=choice.bytes_len;
 result.chosen_policy=choice.policy;
 result.classification=choice.policy==CR01_REG_CUSTOM?CR01_PAIR_CUSTOM:CR01_PAIR_ORIGINAL;
 __atomic_fetch_or(&p->seen_mask,(uint32_t)site,__ATOMIC_ACQ_REL);
 return result;
}
