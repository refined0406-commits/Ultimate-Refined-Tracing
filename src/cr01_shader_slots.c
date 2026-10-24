/* R12: pre-renderer installation of the already validated CR01 shared PT DXIL
 * into the two exact pristine Patch 5.01 native shader slots. Witcher remains
 * sole owner of DXR state objects/SBTs/queues/submission. This only restores
 * the LC12-proven shader-consumption mechanism and is transactional until the
 * CameraCB hook has also committed. */
#include "cr01_shader_slots.h"
#include "cr01_register_sites.h"
#include <stdint.h>
#include <stddef.h>
#if !defined(_WIN64) && !defined(_M_X64)
#error Windows x64 only
#endif
#define WIN __stdcall
#define IMP __declspec(dllimport)
#define PAGE_RW 0x04ul
#define MEM_COMMIT 0x1000ul
#define MEM_RESERVE 0x2000ul
#define MEM_RELEASE 0x8000ul
#define IMAGE_SCN_MEM_READ 0x40000000ul
typedef unsigned long DWORD;typedef int BOOL;typedef void* HANDLE;
IMP void* WIN VirtualAlloc(void*,size_t,DWORD,DWORD);
IMP BOOL WIN VirtualFree(void*,size_t,DWORD);
IMP BOOL WIN VirtualProtect(void*,size_t,DWORD,DWORD*);
IMP BOOL WIN FlushInstructionCache(HANDLE,const void*,size_t);
IMP HANDLE WIN GetCurrentProcess(void);
extern int cr01_gate_verify_native_blob(const uint8_t*,size_t);
extern int cr01_custom_shader_identity(const uint8_t*,size_t);

typedef struct CR01Slot {uint8_t*p,*section_end;uint32_t size;} CR01Slot;
typedef struct CR01Backup {uint8_t*bytes;uint32_t total;} CR01Backup;
static CR01Slot g_a,g_b;
static CR01Backup g_ba,g_bb;
static volatile uint32_t g_active,g_pending,g_attempted;

static uint16_t rd16(const uint8_t*p){return (uint16_t)p[0]|(uint16_t)p[1]<<8;}
static uint32_t rd32(const uint8_t*p){return (uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;}
static void wr32(uint8_t*p,uint32_t v){for(unsigned i=0;i<4;i++)p[i]=(uint8_t)(v>>(8u*i));}
static int has_ascii(const uint8_t*p,uint32_t n,const char*s){
 size_t m=0;while(s[m])m++;if(!m||m>n)return 0;
 for(uint32_t i=0;i+m<=n;i++){size_t j=0;while(j<m&&p[i+j]==(uint8_t)s[j])j++;if(j==m)return 1;}return 0;
}
static int slot_room(const CR01Slot*s,uint32_t target){
 if(!s||!s->p||target<65536u||target>1048576u)return 0;
 if(target<=s->size)return 1;
 size_t need=(size_t)(target-s->size);
 if((size_t)(s->section_end-(s->p+s->size))<need)return 0;
 for(size_t i=0;i<need;i++)if(s->p[s->size+i]!=0)return 0;
 return 1;
}
static int locate(uint8_t*base,CR01Slot*a,CR01Slot*b){
 if(!base||!a||!b||base[0]!='M'||base[1]!='Z')return 0;
 uint32_t pe=rd32(base+0x3c);uint8_t*nt=base+pe;
 if(nt[0]!='P'||nt[1]!='E'||nt[2]||nt[3])return 0;
 uint16_t sections=rd16(nt+6),opt=rd16(nt+20);if(!sections||sections>96)return 0;
 uint32_t st=pe+24u+(uint32_t)opt,count=0;CR01Slot f[2]={{0}};
 for(uint32_t i=0;i<sections;i++){
  uint8_t*sh=base+st+i*40u;uint32_t vs=rd32(sh+8),va=rd32(sh+12),rs=rd32(sh+16),ch=rd32(sh+36);
  if(!(ch&IMAGE_SCN_MEM_READ)||!va)continue;uint32_t span=vs?vs:rs;if(span<65536u)continue;uint8_t*start=base+va;
  for(uint32_t j=8;j+32u<=span;j++){
   uint8_t*p=start+j;if(p[0]!='D'||p[1]!='X'||p[2]!='B'||p[3]!='C')continue;
   uint32_t sz=rd32(p+24);if(sz!=CR01_NATIVE_BLOB_LENGTH||j+sz>span)continue;
   if(rd32(p-8)!=sz||!has_ascii(p,sz,"CameraCB")||!has_ascii(p,sz,"SceneBVH")||
      !has_ascii(p,sz,"PTRGSFinal")||!has_ascii(p,sz,"PTRGSWithSERFinal"))continue;
   if(!cr01_gate_verify_native_blob(p,sz))continue;
   if(count<2){f[count].p=p;f[count].section_end=start+span;f[count].size=sz;}count++;j+=sz-1u;
  }
 }
 if(count!=2u||f[0].size!=f[1].size)return 0;
 /* Anchor the scan to the already signed/verified native RVA used by CR01. */
 uint8_t*anchor=base+CR01_NATIVE_BLOB_RVA;
 if(f[0].p!=anchor&&f[1].p!=anchor)return 0;
 *a=f[0];*b=f[1];return 1;
}
static int backup_one(const CR01Slot*s,uint32_t target,CR01Backup*out){
 uint32_t clear=s->size>target?s->size:target,total=clear+8u;
 uint8_t*p=(uint8_t*)VirtualAlloc(0,total,MEM_RESERVE|MEM_COMMIT,PAGE_RW);if(!p)return 0;
 for(uint32_t i=0;i<total;i++)p[i]=(s->p-8)[i];out->bytes=p;out->total=total;return 1;
}
static void free_backup(CR01Backup*b){if(b->bytes)VirtualFree(b->bytes,0,MEM_RELEASE);b->bytes=0;b->total=0;}
static int write_one(const CR01Slot*s,const uint8_t*custom,uint32_t n){
 uint32_t clear=s->size>n?s->size:n,total=clear+8u;DWORD old=0,tmp=0;uint8_t*r=s->p-8;
 if(!VirtualProtect(r,total,PAGE_RW,&old))return 0;
 wr32(s->p-8,n);for(uint32_t i=0;i<n;i++)s->p[i]=custom[i];for(uint32_t i=n;i<clear;i++)s->p[i]=0;
 int ok=FlushInstructionCache(GetCurrentProcess(),r,total)!=0;
 if(!VirtualProtect(r,total,old,&tmp))ok=0;
 if(rd32(s->p-8)!=n||rd32(s->p+24)!=n||!cr01_custom_shader_identity(s->p,n))ok=0;
 return ok;
}
static int restore_one(const CR01Slot*s,const CR01Backup*b){
 if(!s||!s->p||!b||!b->bytes||b->total<8u)return 0;uint8_t*r=s->p-8;DWORD old=0,tmp=0;
 if(!VirtualProtect(r,b->total,PAGE_RW,&old))return 0;for(uint32_t i=0;i<b->total;i++)r[i]=b->bytes[i];
 int ok=FlushInstructionCache(GetCurrentProcess(),r,b->total)!=0;if(!VirtualProtect(r,b->total,old,&tmp))ok=0;
 if(rd32(s->p-8)!=CR01_NATIVE_BLOB_LENGTH||!cr01_gate_verify_native_blob(s->p,CR01_NATIVE_BLOB_LENGTH))ok=0;return ok;
}
int cr01_shader_slots_install(uint8_t*exe,const uint8_t*custom,size_t custom_size){
 if(__sync_val_compare_and_swap(&g_attempted,0u,1u)!=0u)return 0;
 if(!exe||!custom||custom_size>0xffffffffu||!cr01_custom_shader_identity(custom,custom_size))return 0;
 uint32_t n=(uint32_t)custom_size;if(!locate(exe,&g_a,&g_b)||!slot_room(&g_a,n)||!slot_room(&g_b,n))return 0;
 if(!backup_one(&g_a,n,&g_ba)||!backup_one(&g_b,n,&g_bb))goto fail;
 if(!write_one(&g_a,custom,n)||!write_one(&g_b,custom,n))goto fail_restore;
 __atomic_store_n(&g_pending,1u,__ATOMIC_RELEASE);__atomic_store_n(&g_active,1u,__ATOMIC_RELEASE);return 1;
fail_restore:
 (void)restore_one(&g_a,&g_ba);(void)restore_one(&g_b,&g_bb);
fail:
 free_backup(&g_ba);free_backup(&g_bb);return 0;
}
int cr01_shader_slots_commit(void){
 if(!__atomic_load_n(&g_pending,__ATOMIC_ACQUIRE)||!__atomic_load_n(&g_active,__ATOMIC_ACQUIRE))return 0;
 free_backup(&g_ba);free_backup(&g_bb);__atomic_store_n(&g_pending,0u,__ATOMIC_RELEASE);return 1;
}
int cr01_shader_slots_rollback(void){
 int ok=1;if(!__atomic_load_n(&g_pending,__ATOMIC_ACQUIRE))return 0;
 if(!restore_one(&g_a,&g_ba))ok=0;if(!restore_one(&g_b,&g_bb))ok=0;
 free_backup(&g_ba);free_backup(&g_bb);__atomic_store_n(&g_active,0u,__ATOMIC_RELEASE);__atomic_store_n(&g_pending,0u,__ATOMIC_RELEASE);return ok;
}
int cr01_shader_slots_active(void){return __atomic_load_n(&g_active,__ATOMIC_ACQUIRE)==1u;}
