#ifndef CR01_REGISTER_SITES_H
#define CR01_REGISTER_SITES_H
#include <stdint.h>
#include <stddef.h>
#define CR01_PT_FINAL_CALL_RVA 0x1EE6DC6u
#define CR01_PT_SER_CALL_RVA 0x1EE6FF3u
#define CR01_NATIVE_REGISTER_HELPER_RVA 0x4FE390u
#define CR01_NATIVE_BLOB_RVA 0x32E7C28u
#define CR01_NATIVE_BLOB_LENGTH 231144u
/* READ-ONLY preflight; does not install or modify either registration callsite.
 * First call operand is UNALIGNED; a four-byte atomic CAS is not permissible. */
int cr01_register_sites_preflight(const uint8_t *image,size_t image_size);
#endif
