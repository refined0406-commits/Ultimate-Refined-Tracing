#ifndef CR01_GATE_CORE_H
#define CR01_GATE_CORE_H
#include <stdint.h>
#include <stddef.h>
/* The callback is invoked immediately after the original native serializer
   on the SAME engine-owned CPU destination, before the wrapper resumes. */
#define CR01_GEOM_OFFSET 0x210u
#define CR01_HAIR_OFFSET 0x380u
#define CR01_NATIVE_GEOM 2u
#define CR01_NATIVE_HAIR 4u
#define CR01_NATIVE_COMP 100u
#define CR01_CALL_RVA 0x1EF0FCFu
#define CR01_WRITER_RVA 0x1EF2340u
#define CR01_GEOM_RVA 0x1EF29E1u
#define CR01_HAIR_RVA 0x1EF2F6Cu
/* Verified file digest of protected Patch 5.01 EXE, used by both native integrations. */
int cr01_gate_verify_exe_file(void);
int cr01_gate_verify_native_blob(const uint8_t*,size_t);
/* One atomic 64-bit publication on settings events; no frame polling.
   Low 32 bits: geometry8, hair8, indirect8, master1, PT1, GPU-ready1,
   shader-ready1. High 32 bits: epoch. */
typedef struct CR01GateState { uint64_t word; } CR01GateState;
typedef struct CR01GateFields {
  uint8_t geometry,hair,comp;
  uint8_t master,pt,renderer_ready,shader_ready;
} CR01GateFields;
/* Returns zero on out-of-contract request. The caller owns the settings
   snapshot and publishes only after validation; never called from render. */
int cr01_gate_publish(CR01GateState *s, CR01GateFields f, uint32_t epoch);
uint64_t cr01_gate_snapshot(const CR01GateState *s);
/* Safe defaults: no owner write unless Master+PT publication is active AND
   the render-side caller proves the protected custom PT registration pair is
   currently live. Values may be staged before registration; native output is
   preserved byte-for-byte until custom_shader_live is true. */
int cr01_gate_post_serialize(const CR01GateState *s,void *native_dest,
                             int custom_shader_live);
/* PE call operand preflight. Does NOT install or alter anything. */
int cr01_gate_exact_call_preflight(const uint8_t *call_instruction,
                                  const uint8_t *geometry_instruction,
                                  const uint8_t *hair_instruction,
                                  uintptr_t call_va,uintptr_t native_va,
                                  uintptr_t replacement_va,
                                  int32_t *old_disp,int32_t *new_disp);
/* Portable installation primitive; caller supplies a pre-authorized safe
   page-protection transaction AND ensures execution cannot race installation.
   Compare-exchange is 4-byte aligned, opcode E8 unchanged, no trampoline copy.
   Rolls back own operand on protection/flush failure if possible. */
typedef struct CR01CallPlatform {
  int (*writable)(void *address,size_t length,void *ctx);
  int (*restore_protection)(void *address,size_t length,void *ctx);
  int (*flush_icache)(void *address,size_t length,void *ctx);
  void *ctx;
} CR01CallPlatform;
int cr01_gate_commit_operand(uint8_t *call_site,int32_t expected,int32_t next,
                             const CR01CallPlatform *platform);
#endif
