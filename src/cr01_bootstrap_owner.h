#ifndef CR01_BOOTSTRAP_OWNER_H
#define CR01_BOOTSTRAP_OWNER_H
#include "cr01_registration_pair.h"
enum {CR01_OWNER_COLD=0,CR01_OWNER_PREPARING=1,
      CR01_OWNER_CUSTOM_READY=2,CR01_OWNER_NATIVE_LOCKED=3};
typedef struct {
 CR01RegistrationPair pair;
 uint32_t event_state;
} CR01BootstrapOwner;
/* Must be invoked synchronously from a verified, non-loader-lock
 * DirectInput8Create gateway. No worker may acknowledge preparation. */
int cr01_owner_prepare(CR01BootstrapOwner*,CR01PrepareOneContainer,void*);
int cr01_owner_is_early_custom(const CR01BootstrapOwner*);
#endif
