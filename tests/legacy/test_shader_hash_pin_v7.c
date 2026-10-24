#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
typedef unsigned long DWORD;
typedef uintptr_t HCRYPTPROV;
typedef uintptr_t HCRYPTHASH;
typedef int BOOL;
typedef unsigned short WCHAR;
static unsigned char sum[32];
BOOL CryptAcquireContextW(HCRYPTPROV*p,const WCHAR*a,const WCHAR*b,DWORD type,DWORD flags){(void)a;(void)b;if(type!=24||flags!=0xf0000000u)return 0;*p=1;return 1;}
BOOL CryptCreateHash(HCRYPTPROV p,DWORD alg,HCRYPTPROV k,DWORD f,HCRYPTHASH*h){(void)k;(void)f;if(p!=1||alg!=0x800c)return 0;*h=1;return 1;}
BOOL CryptHashData(HCRYPTHASH h,const unsigned char*b,DWORD n,DWORD f){(void)f;if(h!=1)return 0;SHA256(b,n,sum);return 1;}
BOOL CryptGetHashParam(HCRYPTHASH h,DWORD key,unsigned char*b,DWORD*n,DWORD f){(void)f;if(h!=1||key!=2||*n<32)return 0;memcpy(b,sum,32);*n=32;return 1;}
BOOL CryptDestroyHash(HCRYPTHASH h){return h==1;}
BOOL CryptReleaseContext(HCRYPTPROV p,DWORD f){(void)f;return p==1;}
int cr01_custom_shader_identity_register(const uint8_t*,size_t);
int cr01_custom_shader_identity(const uint8_t*,size_t);
int main(int argc,char**argv){
 if(argc!=2)return 2;FILE*fp=fopen(argv[1],"rb");if(!fp)return 3;
 uint8_t*blob=(uint8_t*)malloc(240000);if(!blob)return 4;
 size_t n=fread(blob,1,240000,fp);fclose(fp);
 if(n!=236120||!cr01_custom_shader_identity_register(blob,n)||!cr01_custom_shader_identity(blob,n)){puts("FAIL exact valid shader pin");return 5;}
 uint8_t*copy=malloc(n);memcpy(copy,blob,n);
 if(!cr01_custom_shader_identity(copy,n)){puts("FAIL identical copied bytes rejected");return 6;}
 copy[12345]^=0x01;
 if(cr01_custom_shader_identity(copy,n)){puts("FAIL tampered copied bytes accepted");return 7;}
 blob[12345]^=0x01;
 if(cr01_custom_shader_identity_register(blob,n)){puts("FAIL tampered shader registered");return 8;}
 if(cr01_custom_shader_identity_register(copy,n-1)){puts("FAIL truncated shader registered");return 9;}
 puts("PASS exact SHA256 pin; byte-identity; corrupt-byte rejection; length rejection");free(copy);free(blob);return 0;
}
