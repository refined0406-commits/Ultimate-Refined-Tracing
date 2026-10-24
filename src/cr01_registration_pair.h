#ifndef CR01_REGISTRATION_PAIR_H
#define CR01_REGISTRATION_PAIR_H
#include "cr01_startup_barrier.h"
#include <stdint.h>
#include <stddef.h>
/* Exactly two protected native register-call owners. Other registration calls
 * must NEVER be redirected or interpreted as one of this pair. */
enum {CR01_PT_FINAL=1, CR01_PT_SER=2};
enum {CR01_PAIR_ORIGINAL=0,CR01_PAIR_CUSTOM=1,CR01_PAIR_WRONG_OWNER=2,
      CR01_PAIR_INVALID_NATIVE=3,CR01_PAIR_LATE=4};
typedef struct {
 CR01StartupBarrier startup;
 const uint8_t *original;
 size_t original_size;
 uintptr_t expected_final_return;
 uintptr_t expected_ser_return;
 uint32_t seen_mask;
 uint32_t fault;
} CR01RegistrationPair;
typedef struct {
 const uint8_t *data;
 size_t size;
 uint32_t classification;
 uint32_t chosen_policy;
} CR01PairSelection;
/* Called only after trusted full-EXE digest + exact two native target operands,
 * pointer/length, one-container digest and registration-site quiescence preflight.
 * This object neither hashes the actual file nor hooks anything itself. */
int cr01_pair_init(CR01RegistrationPair*,const uint8_t *orig,size_t n,
                   uintptr_t final_return,uintptr_t ser_return);
/* Freeze native/custom for both protected exports even across repeats/retries.
 * A suspicious input never causes mid-process promotion to custom. */
CR01PairSelection cr01_pair_select(CR01RegistrationPair *,uintptr_t caller_return,
                      const uint8_t *received,size_t received_size);
#endif
