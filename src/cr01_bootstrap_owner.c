#include "cr01_bootstrap_owner.h"
int cr01_owner_prepare(CR01BootstrapOwner *o,CR01PrepareOneContainer cb,void*opaque){
 uint32_t expected=CR01_OWNER_COLD;
 if(!o||!cb)return 0;
 if(!__atomic_compare_exchange_n(&o->event_state,&expected,
      CR01_OWNER_PREPARING,0,__ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE))
  return expected==CR01_OWNER_CUSTOM_READY;
 int ready=cr01_bs_sync_prepare(&o->pair.startup,cb,opaque);
 __atomic_store_n(&o->event_state,ready?CR01_OWNER_CUSTOM_READY:
      CR01_OWNER_NATIVE_LOCKED,__ATOMIC_RELEASE);
 return ready;
}
int cr01_owner_is_early_custom(const CR01BootstrapOwner*o){
 return o&&__atomic_load_n(&o->event_state,__ATOMIC_ACQUIRE)==CR01_OWNER_CUSTOM_READY &&
 __atomic_load_n(&o->pair.startup.registration_policy,__ATOMIC_ACQUIRE)!=CR01_REG_NATIVE;
}
