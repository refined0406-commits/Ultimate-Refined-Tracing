#ifndef URT_BUILD_H
#define URT_BUILD_H
#include <stdint.h>
/* V5.3.0 R11: game builds URT knows instruction by instruction.
 *   URT_BUILD_1048522  Patch 5.01 as protected since V4.x (every pinned RVA in the sources is this build's).
 *   URT_BUILD_1090142  Steam update of 29 October 2026 (FileVersion 5.0.0.1090142). Same shaders (all 735 DXBC
 *                      containers byte-identical), same CameraCB writer and PT registration code; code and data
 *                      moved. Every pinned address URT uses is mapped in urt_build_table.h (tools/port/).
 * The build is set once by the executable identity (urt_exe_identity_win.c) and never changes afterwards.
 * Every pinned address is used as urt_rva(<Patch 5.01 RVA>): the same RVA on build 1048522, the mapped RVA on
 * build 1090142, and 0 for an unmapped address (callers verify bytes before any write, so 0 fails closed). */
enum { URT_BUILD_1048522 = 0, URT_BUILD_1090142 = 1 };
void urt_build_set(int build);
int urt_build(void);
uint32_t urt_rva(uint32_t rva_1048522);
uint32_t urt_image_bytes(void);            /* SizeOfImage of the known build */
const char *urt_build_name(void);
#endif
