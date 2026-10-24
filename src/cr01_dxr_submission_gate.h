#ifndef CR01_DXR_SUBMISSION_GATE_H
#define CR01_DXR_SUBMISSION_GATE_H
#include <stdint.h>
#include <stddef.h>
/* Verified Windows x64 SDK ABI (not historical LC81 mock enums).
 * This component reads only game-owned D3D12 descriptor metadata. It does not
 * replace any COM methods, write a GPU resource, or claim shader execution. */
enum { CR01_D3D12_SUBOBJECT_DXIL_LIBRARY=5 };
enum { CR01_DXR_CREATE_STATE_OBJECT=1, CR01_DXR_ADD_TO_STATE_OBJECT=2 };
typedef struct { const void *code; size_t bytes; } CR01DxrBytecode;
typedef struct {
 CR01DxrBytecode dxil; uint32_t number_of_exports; uint32_t reserved;
 const void *exports;
} CR01DxrLibraryDesc;
typedef struct {
 const uint16_t *name; const uint16_t *export_to_rename;
 uint32_t flags; uint32_t reserved;
} CR01DxrExportDesc;
typedef struct { uint32_t type; uint32_t reserved; const void *desc; } CR01DxrSubobject;
typedef struct { uint32_t type; uint32_t count; const CR01DxrSubobject *subobjects; } CR01DxrStateDesc;
typedef struct {
 const uint8_t *approved; size_t approved_size;
 uint32_t accepted_method_mask;
 uint32_t rejected;
 const void *accepted_state_object;
} CR01DxrSubmissionGate;
/* Startup-only; exact signed container identity checked elsewhere. */
int cr01_dxr_gate_init(CR01DxrSubmissionGate*,const uint8_t*,size_t);
/* Called only after real ID3D12Device5::CreateStateObject or
 * ID3D12Device7::AddToStateObject returns. A successful state creation is
 * NOT equivalent to binding, dispatch or visible GPU execution. */
int cr01_dxr_gate_note_submit(CR01DxrSubmissionGate*,unsigned method,
 const CR01DxrStateDesc*,int32_t hresult,const void *state_object);
/* Explicitly never authorizes live parameter writes on its own. */
int cr01_dxr_gate_state_created(const CR01DxrSubmissionGate*);
/* Process-lifetime witness: only the real, linked renderer callback may record
 * a COM submission. No independent Boolean permits custom GPU output. */
int cr01_dxr_runtime_init(const uint8_t*,size_t);
int cr01_dxr_runtime_note_submit(unsigned,const CR01DxrStateDesc*,int32_t,const void*);
int cr01_dxr_runtime_state_created(void);
#endif
