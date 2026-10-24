#include "cr01_register_sites.h"
static uint32_t le32(const uint8_t*p){return (uint32_t)p[0]|((uint32_t)p[1]<<8)|
                        ((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
int cr01_register_sites_preflight(const uint8_t *image,size_t size){
 const uint32_t sites[]={CR01_PT_FINAL_CALL_RVA,CR01_PT_SER_CALL_RVA};
 if(!image||size<=CR01_PT_SER_CALL_RVA+5u)return 0;
 for(unsigned i=0;i<2;i++){
  uint32_t rv=sites[i];
  if(image[rv]!=0xe8||rv+5u+(int32_t)le32(image+rv+1)!=
       CR01_NATIVE_REGISTER_HELPER_RVA)return 0;
 }
 if(((CR01_PT_FINAL_CALL_RVA+1u)&3u)!=3u ||
    ((CR01_PT_SER_CALL_RVA+1u)&3u)!=0u)return 0;
 return 1;
}
