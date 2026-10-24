/* CR01 CPU PHASE2, PATCH 5.01, QUARANTINED, NOT-LINKED. Not a game hook.
 * Pure Win64-call-compatible candidate for the *copy* portion only.
 * A safe lifetime/synchronization owner and CPU hotness have NOT been proven.
 */
#include <stdint.h>
#include <stddef.h>

typedef unsigned char cr01_v16 __attribute__((vector_size(16)));
typedef uint8_t (*CR01BoneNative)(const uint8_t *, int32_t, uint8_t *);

/* Caller supplies the original native function to preserve its precise
 * forward-scalar semantics when the input and output regions alias.
 * Bounds/lifetime are the engine caller's obligations, as in native 5.01.
 * Never install or publish on the strength of this routine alone. */
uint8_t cr01_bone_fast_safe501(const uint8_t *pose, int32_t idx, uint8_t *out,
                              CR01BoneNative original) {
    int16_t bone;
    uint16_t count, base;
    uint8_t stride;
    uintptr_t matrices;
    uint64_t displacement;
    uintptr_t src_address, dst_address;
    cr01_v16 x0, x1, x2;
    uint64_t final8;
    uint32_t final4;
    const uint8_t *src;
    if (!original) return 0; /* absent original is an integration failure */

    __builtin_memcpy(&bone, pose + 0x26 + (int64_t)idx * 2, sizeof(bone));
    if (bone == -1) return 0;
    __builtin_memcpy(&count, pose + 0x20, sizeof(count));
    if (bone >= (int32_t)count) return 0; /* signed: other negative values MAY be valid */
    stride = pose[0x23];
    __builtin_memcpy(&base, pose + 0x1C, sizeof(base));
    __builtin_memcpy(&matrices, pose + 0x10, sizeof(matrices));

    /* Match native 64-bit signed-index addition BEFORE multiplying by 64. */
    displacement = ((uint64_t)((int64_t)base * stride + (int64_t)bone)) << 6;
    src_address = matrices + (uintptr_t)displacement;
    dst_address = (uintptr_t)out;
    src = (const uint8_t *)src_address;

    /* Exact native semantics in overlap cases, including forward-copy aliasing.
     * Native reads 60 bytes and writes 64 (last 4 bytes are 1.0f). */
    if ((dst_address >= src_address && dst_address - src_address < 60u) ||
        (src_address > dst_address && src_address - dst_address < 64u))
        return original(pose, idx, out);

    /* Only read 60 bytes. The competitor's 64-byte load can over-read near
     * a mapped page boundary and carries no additional useful matrix data. */
    __builtin_memcpy(&x0, src + 0, 16);
    __builtin_memcpy(&x1, src + 16, 16);
    __builtin_memcpy(&x2, src + 32, 16);
    __builtin_memcpy(&final8, src + 48, 8);
    __builtin_memcpy(&final4, src + 56, 4);
    __builtin_memcpy(out + 0, &x0, 16);
    __builtin_memcpy(out + 16, &x1, 16);
    __builtin_memcpy(out + 32, &x2, 16);
    __builtin_memcpy(out + 48, &final8, 8);
    __builtin_memcpy(out + 56, &final4, 4);
    final4 = 0x3f800000u;
    __builtin_memcpy(out + 60, &final4, 4);
    return 1;
}
