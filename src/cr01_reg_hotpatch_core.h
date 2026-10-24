#ifndef CR01_REG_HOTPATCH_CORE_H
#define CR01_REG_HOTPATCH_CORE_H
#include <stdint.h>
#include <stddef.h>
/* Patch 5.01 native register-helper entrance, preverified unchanged:
 * 8 CC padding preceding TEST RDX,RDX ; JE +0x8E1.
 * We install the 5-byte pad jump before publishing the atomic two-byte short
 * jump at entry. We do not write either original protected registration E8.
 */
#define CR01_HELPER_RVA 0x4fe390u
#define CR01_HELPER_PAD_LENGTH 8u
#define CR01_REG_TRAMPOLINE_OFFSET 0x40u
#define CR01_REG_RELAY_LENGTH 0x80u
#define CR01_NATIVE_BRANCH_RVA 0x4fec7au
#define CR01_NATIVE_CONTINUE_RVA 0x4fe399u
#define CR01_ATOMIC_OLD 0x8548u
#define CR01_ATOMIC_NEW 0xf6ebu
/* A preflight rejection makes no game-code changes. A failed protection
 * restoration attempts atomic entry rollback while the caller maintains
 * quiescence. If rollback cannot establish ownership, the Windows owner
 * must remain pinned and force native pass-through for process life.
 * This does NOT itself prove safe concurrency or authorize installation. */
int cr01_hotpatch_build(const uint8_t* helper,uintptr_t helper_va,
                        uintptr_t relay_va,uintptr_t hook_va,
                        uint8_t pad[8],uint8_t relay[CR01_REG_RELAY_LENGTH]);
/* All mutations MUST be externally authorized as an early (or fully
   quiesced), single-owner transaction by the Windows bootstrap. */
typedef struct CR01HotpatchOps {
 int (*make_writable)(void*,size_t,void*);
 int (*restore_protection)(void*,size_t,void*);
 int (*flush)(void*,size_t,void*);
 void* ctx;
} CR01HotpatchOps;
int cr01_hotpatch_commit(uint8_t* helper,const uint8_t pad[8],
                         const CR01HotpatchOps* ops);
/* Reverse only the exact owned entry and pad inside the same externally
 * quiescent interval; never restore code belonging to another hook owner. */
int cr01_hotpatch_uncommit(uint8_t* helper,const uint8_t expected_pad[8],
                           const CR01HotpatchOps* ops);
#endif
