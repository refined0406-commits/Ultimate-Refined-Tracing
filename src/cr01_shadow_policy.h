#ifndef CR01_SHADOW_POLICY_H
#define CR01_SHADOW_POLICY_H
#include <stdint.h>
#include "cr01_contract.h"
/* Pure lifecycle policy for the isolated PT-shadow mirror. This does not
 * locate, patch or write any game state. */
int cr01_shadow_policy(const CR01Settings*,unsigned pt_active,uint32_t native_value,
                       unsigned*force_off,uint32_t*effective_value);
#endif
