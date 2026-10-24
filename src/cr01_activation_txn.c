#include "cr01_activation_txn.h"
int cr01_activation_run(CR01ActivationTxn*t,const CR01ActivationOps*ops,void*ctx,
                        int early,int quiescent){
 uint32_t cold=CR01_TX_COLD;
 if(!t||!ops||!ops->install_registration_neutral||
    !ops->install_serializer_neutral||!ops->arm_registration||!ops->force_native)
  return 0;
 if(!__atomic_compare_exchange_n(&t->stage,&cold,CR01_TX_RUNNING,0,
                                  __ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE))return 0;
 /* An unsupported or late entry must not even attempt executable writes. */
 if(!early||!quiescent)goto fail;
 /* IMPORTANT: all registered hooks begin native-only. The shader-container
  * decision is exposed only after BOTH hook commits and checks have passed. */
 if(!ops->install_registration_neutral(ctx))goto fail;
 t->installed_registration=1;
 if(!ops->install_serializer_neutral(ctx))goto fail;
 t->installed_serializer=1;
 if(!ops->arm_registration(ctx))goto fail;
 __atomic_store_n(&t->stage,CR01_TX_CUSTOM_ARMED,__ATOMIC_RELEASE);
 return 1;
fail:
 /* First revoke custom selection. This must never wait for asynchronous
  * shutdown and MUST be safe with either hook partially installed. */
 ops->force_native(ctx);
 /* Both revert callbacks are allowed only under the same external exclusive
  * ownership proof used for initial patching. Never erase a reachable relay. */
 if(t->installed_serializer && quiescent && ops->revert_serializer_quiescent &&
    ops->revert_serializer_quiescent(ctx))t->installed_serializer=0;
 if(t->installed_registration && quiescent && ops->revert_registration_quiescent &&
    ops->revert_registration_quiescent(ctx))t->installed_registration=0;
 /* An installer may return failure AFTER publishing code but before tracking
  * success (e.g., page-protection restore failure). Always treat such a
  * failure as potentially pinned neutral code, not clean rollback. */
 t->pinned_neutral_code_possible=1;
 __atomic_store_n(&t->stage,CR01_TX_FAILED_NATIVE,__ATOMIC_RELEASE);
 return 0;
}
