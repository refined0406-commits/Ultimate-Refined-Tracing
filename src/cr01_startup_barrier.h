#ifndef CR01_STARTUP_BARRIER_H
#define CR01_STARTUP_BARRIER_H
#include <stdint.h>
#include <stddef.h>
enum {CR01_BS_UNSTARTED=0,CR01_BS_PREPARING=1,CR01_BS_PREPARED=2,
      CR01_BS_NATIVE_FAILED=3};
enum {CR01_REG_UNDECIDED=0,CR01_REG_NATIVE=1,CR01_REG_CUSTOM=2};
typedef struct CR01StartupBarrier {
 uint32_t stage;
 uint32_t registration_policy;
 const uint8_t* custom_container;
 size_t custom_size;
} CR01StartupBarrier;
typedef struct CR01SharedShaderPair {
 const uint8_t *bytes;size_t bytes_len;uint32_t policy;
} CR01SharedShaderPair;
typedef int (*CR01PrepareOneContainer)(void *opaque,const uint8_t**out,size_t*size);
/* Must be called synchronously OUTSIDE DllMain loader lock, e.g. verified
   DirectInput8Create entry. A prepare callback must fully validate and pin one
   immutable shared DXIL library with both exports; no worker ACK is enough. */
int cr01_bs_sync_prepare(CR01StartupBarrier*,CR01PrepareOneContainer,void*);
/* Both protected registration sites call this same policy, not separate
   decisions. If DXIL isn't fully prepared before the first call, BOTH native. */
CR01SharedShaderPair cr01_bs_registration(CR01StartupBarrier*,
                                           const uint8_t*native,size_t native_len);
#endif
