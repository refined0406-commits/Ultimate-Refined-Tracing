#ifndef CR01_GATE_CORE_H
#define CR01_GATE_CORE_H
#include "urt_build.h"
#include <stdint.h>
#include <stddef.h>
/* The callback is invoked immediately after the original native serializer
   on the SAME engine-owned CPU destination, before the wrapper resumes. */
#define CR01_GEOM_OFFSET 0x210u
#define CR01_HAIR_OFFSET 0x380u
#define CR01_NATIVE_GEOM 2u
#define CR01_NATIVE_HAIR 4u
#define CR01_NATIVE_COMP 100u
#define CR01_CALL_RVA urt_rva(0x1EF0FCFu)
#define CR01_WRITER_RVA urt_rva(0x1EF2340u)
#define CR01_GEOM_RVA urt_rva(0x1EF29E1u)
#define CR01_HAIR_RVA urt_rva(0x1EF2F6Cu)
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
/* V5.3.0 R11: same, with the PT: Second Bounce Coverage level (0..2, only while URT's variant library is installed)
   carried in bits 16-17 of the +0x210 word; URT's normal library reads bits 0-15 only. */
int cr01_gate_post_serialize_level(const CR01GateState *s,void *native_dest,
                             int custom_shader_live,unsigned bounce_level);
/* V5.3.0 R10: SHaRC compatibility (docs/v530/V5.3.0_R10_SHARC_REPAIR.md).
   With PTUseSHARC on, the game picks its SHaRC ray-generation shaders from the words this serializer writes
   (0x1ef0010): only when +0x210 == 2, +0x214 == 1, +0x380 == 4, +0x384 == 1, +0x410 == 0x3efff, +0x118 == 0 and
   +0x4e4 == 0 does it dispatch the registered PTRGS_SharcUpdateFinal / PTRGS_SharcQueryFinal (raygen 0x13/0x14);
   otherwise it dispatches raygen 0x11/0x12 (PTRGS_SharcUpdate / PTRGS_SharcQuery), which Patch 5.01 never registers
   in any state object. URT's bounce words (+0x210, +0x380) must therefore stay native while SHaRC is on. */
#define CR01_SHARC_RVA urt_rva(0x5149B40u) /* Rendering/RT/PathTracer PTUseSHARC (byte) */
/* custom shader edits are allowed only while the game's SHaRC path is off */
int cr01_gate_effective_custom(int custom_shader_live,unsigned sharc_on);
/* R12 FIX3: pure core of the in-function bounce store (render thread): the word URT's PT library reads, or 2 (native) when
   Master+PT are not staged, the custom library is not live, SHaRC is on or the staged values are out of range. The
   coverage level travels whenever the word is URT's, whatever Geometry Bounces / Hair / Indirect-Light are set to. */
uint32_t cr01_gate_infunc_word(uint64_t gate_word,int custom_shader_live,unsigned sharc_on,unsigned level);
/* R12 FIX3: accepted coverage levels: bits 0-1 retired (0), 2-3 diffuse 0 / 2 (50 %), 4-5 sun shadow 0 / 1 (50 %),
   6-7 local-light shadow 0 / 1 (50 %) */
int cr01_gate_level_valid(unsigned level);
/* 1 when the serialized words select the registered (Final) SHaRC ray-generation shaders */
int cr01_gate_sharc_final_words(const uint8_t *native_dest);
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
