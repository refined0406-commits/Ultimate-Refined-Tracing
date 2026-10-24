#ifndef CR01_GATE_LIVE_ADAPTER_H
#define CR01_GATE_LIVE_ADAPTER_H
#include "cr01_gate_core.h"
#include "cr01_contract.h"
/* E06 original CR01Settings, one serialized state owner, zero polling. */
int cr01_gate_publish_e06(CR01GateState *out,const CR01Settings *saved,
                           uint32_t known_pt_active,
                           uint32_t native_registration_complete,
                           uint32_t custom_gpu_accepted,uint32_t epoch);
#endif
