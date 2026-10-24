#ifndef CR01_PT_SHARED_DXIL_H
#define CR01_PT_SHARED_DXIL_H
#include <stdint.h>
#include <stddef.h>
/* Host provides a trusted, version-fingerprinted DxcCreateInstance proc.
 * Output is a single immutable original-metadata-preserving DXBC library,
 * with BOTH registered PT exports transformed within that one container.
 * Returns 0 on *any* failure; output pointers stay null. This is not GPU
 * CreateStateObject acceptance. */
typedef long (__stdcall *CR01DxcCreate)(const void*,const void*,void**);
int cr01_build_shared_dxil(const uint8_t *original,size_t size,
                           CR01DxcCreate create,
                           const uint8_t **out,size_t *out_size);
#endif
