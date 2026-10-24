#ifndef CR01_ACTIVATION_TXN_H
#define CR01_ACTIVATION_TXN_H
#include <stdint.h>
/* Source-level coordinator. It does not confer quiescence or GPU acceptance.
 * Both detours remain native/pass-through until arm_registration succeeds.
 * Failed patch attempts may leave neutral pinned code: see outcome. */
typedef enum {
 CR01_TX_COLD=0, CR01_TX_RUNNING=1, CR01_TX_CUSTOM_ARMED=2,
 CR01_TX_FAILED_NATIVE=3
} CR01TxStage;
typedef struct {
 uint32_t stage;
 uint32_t installed_registration;
 uint32_t installed_serializer;
 uint32_t pinned_neutral_code_possible;
} CR01ActivationTxn;
typedef struct {
 int (*install_registration_neutral)(void*);
 int (*install_serializer_neutral)(void*);
 int (*arm_registration)(void*);
 void (*force_native)(void*);
 /* Rollback hooks are optional; a false or absent callback means the ASI
  * must stay pinned and the owner cannot attest full physical rollback. */
 int (*revert_registration_quiescent)(void*);
 int (*revert_serializer_quiescent)(void*);
} CR01ActivationOps;
/* Only call after independently verified pre-registration exclusive ownership.
 * An assertion of `quiescence_proven` is a caller claim, not evidence. */
int cr01_activation_run(CR01ActivationTxn*,const CR01ActivationOps*,void*,
                        int before_registration,int quiescence_proven);
#endif
