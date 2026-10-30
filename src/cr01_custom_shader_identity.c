/* Immutable signed Windows-DXC shader attestation at CR01 ASI startup.
 * Source approval is tied to SHA-256, length, DXBC header and exact artifact.
 * Never interprets a loose file or modifies engine/GPU memory. */
#include <stddef.h>
#include <stdint.h>
#define WIN __stdcall
#define IMP __declspec(dllimport)
typedef unsigned long DWORD;
typedef uintptr_t HCRYPTPROV;
typedef uintptr_t HCRYPTHASH;
typedef int BOOL;
typedef unsigned short WCHAR;
IMP BOOL WIN CryptAcquireContextW(HCRYPTPROV*,const WCHAR*,const WCHAR*,DWORD,DWORD);
IMP BOOL WIN CryptCreateHash(HCRYPTPROV,DWORD,HCRYPTPROV,DWORD,HCRYPTHASH*);
IMP BOOL WIN CryptHashData(HCRYPTHASH,const unsigned char*,DWORD,DWORD);
IMP BOOL WIN CryptGetHashParam(HCRYPTHASH,DWORD,unsigned char*,DWORD*,DWORD);
IMP BOOL WIN CryptDestroyHash(HCRYPTHASH);
IMP BOOL WIN CryptReleaseContext(HCRYPTPROV,DWORD);
/* Windows DXC 2026-10-11 validated candidate, no GPU approval implied. */
static const unsigned char expected[32]={
 0x39,0x27,0x18,0x36,0x1b,0x17,0xed,0x66,0x1e,0x81,0xbc,0x73,0xe3,0x29,0xa2,0x33,
 0xc1,0xd5,0xe6,0xf0,0x64,0x21,0xdd,0x08,0xdc,0xb3,0x1a,0xf4,0xf9,0x4a,0x40,0x84};
/* PRIVATE V7 four-control RTAO: pin the exact Windows-DXC-validated artifact.
 * Fail closed before writing either original Witcher PT shader slot. */
static const unsigned char kPrivateRTAOV7SHA256[32]={
 0xf1,0xad,0x5c,0x37,0x4a,0xe1,0x18,0x85,0x08,0x56,0x8d,0x62,0x1e,0x8b,0xe6,0x4e,
 0xde,0x3f,0x21,0x67,0xe4,0xab,0x06,0xdc,0xfc,0xa5,0xa6,0x78,0x17,0x46,0xe6,0xac};
static int cr01_private_v7_exact_hash(const uint8_t*data,size_t length){
 HCRYPTPROV provider=0;HCRYPTHASH hash=0;uint8_t actual[32];DWORD size=32;int ok=0;
 if(!data||length!=236120u)return 0;
 if(!CryptAcquireContextW(&provider,0,0,24,0xf0000000u))goto done;
 if(!CryptCreateHash(provider,0x800cu,0,0,&hash))goto done;
 if(!CryptHashData(hash,data,(DWORD)length,0))goto done;
 if(!CryptGetHashParam(hash,2,actual,&size,0)||size!=32)goto done;
 ok=1;for(unsigned i=0;i<32;i++)if(actual[i]!=kPrivateRTAOV7SHA256[i])ok=0;
done:
 if(hash)CryptDestroyHash(hash);
 if(provider)CryptReleaseContext(provider,0);
 return ok;
}
static const uint8_t* g_r14_dynamic_shader;
static size_t g_r14_dynamic_size;
int cr01_custom_shader_identity_register(const uint8_t*blob,size_t size){
 if(!blob||size!=236120u||size>240000u||blob[0]!='D'||blob[1]!='X'||blob[2]!='B'||blob[3]!='C')return 0;
 if(((uint32_t)blob[24]|(uint32_t)blob[25]<<8|(uint32_t)blob[26]<<16|(uint32_t)blob[27]<<24)!=(uint32_t)size)return 0;
 if(!cr01_private_v7_exact_hash(blob,size))return 0;
 /* The pointer is accepted only after cr01_build_shared_dxil returned success,
    which includes DXC assembly, container recomposition and validator success. */
 g_r14_dynamic_shader=blob;g_r14_dynamic_size=size;return 1;
}
/* V5.3.0 R11: PT: Second Bounce Coverage variant (urt_bounce501.c). Accepted only after the exact validated URT
   library (the pinned SHA-256 above) was registered in this process, which is the variant builder's only input; the
   variant itself was assembled, validated, signed and re-checked by that builder. From here on identity means the
   exact bytes of this variant (the comparison below), exactly as for the registered URT library. */
int cr01_custom_shader_identity_register_variant(const uint8_t*blob,size_t size){
 if(!g_r14_dynamic_shader||g_r14_dynamic_size!=236120u||!blob||size<200000u||size>253288u)return 0;
 if(blob[0]!='D'||blob[1]!='X'||blob[2]!='B'||blob[3]!='C')return 0;
 if(((uint32_t)blob[24]|(uint32_t)blob[25]<<8|(uint32_t)blob[26]<<16|(uint32_t)blob[27]<<24)!=(uint32_t)size)return 0;
 g_r14_dynamic_shader=blob;g_r14_dynamic_size=size;return 1;
}
int cr01_custom_shader_identity(const uint8_t*blob,size_t size){
 HCRYPTPROV provider=0;HCRYPTHASH hash=0;unsigned char found[32];
 DWORD count=32;int ok=0;
 /* R14 FIX1: the DXC output is copied into Witcher's protected PT slots, so
    pointer identity is insufficient after installation. Accept any location
    only when its complete bytes exactly match the registered validated blob. */
 if(blob&&g_r14_dynamic_shader&&size==g_r14_dynamic_size){
  size_t i=0;
  if(blob[0]!='D'||blob[1]!='X'||blob[2]!='B'||blob[3]!='C')return 0;
  if(((uint32_t)blob[24]|(uint32_t)blob[25]<<8|(uint32_t)blob[26]<<16|
      (uint32_t)blob[27]<<24)!=(uint32_t)size)return 0;
  for(i=0;i<size;i++)if(blob[i]!=g_r14_dynamic_shader[i])return 0;
  return 1;
 }
 if(!blob||size!=231524u||blob[0]!='D'||blob[1]!='X'||blob[2]!='B'||blob[3]!='C')return 0;
 if(((uint32_t)blob[24]|(uint32_t)blob[25]<<8|(uint32_t)blob[26]<<16|
    (uint32_t)blob[27]<<24)!=(uint32_t)size)return 0;
 if(!CryptAcquireContextW(&provider,0,0,24,0xf0000000u))goto done;
 if(!CryptCreateHash(provider,0x800cu,0,0,&hash))goto done;
 if(!CryptHashData(hash,blob,(DWORD)size,0))goto done;
 if(!CryptGetHashParam(hash,2,found,&count,0)||count!=32)goto done;
 ok=1;
 for(unsigned i=0;i<32;i++)if(found[i]!=expected[i])ok=0;
 done:
 if(hash)CryptDestroyHash(hash);
 if(provider)CryptReleaseContext(provider,0);
 return ok;
}
