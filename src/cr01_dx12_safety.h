/* Private settings integrity predicate. No filesystem side effects. */
#ifndef CR01_DX12_SAFETY_H
#define CR01_DX12_SAFETY_H
#include <stdint.h>
static int cr01_dx12_preflight_safe(int metadata_ok, uint32_t attributes,
                                   uint32_t size_high, uint32_t size_low){
 /* A missing, unreadable, zero-byte, or directory path is NOT writable. */
 return metadata_ok && !(attributes & 0x10u) && (size_high || size_low);
}
#endif
