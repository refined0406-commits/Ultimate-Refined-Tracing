static int writable_range(uint8_t*base,uint32_t limit,uint8_t*target){
 if(!base||!target||target<base||target>base+limit-4)return 0;
 unsigned pe=u32(base+0x3c),sec=pe+24+u16(base+pe+20),count=u16(base+pe+6);
 if(count>96||sec>limit||count*40>limit-sec)return 0;
 for(unsigned i=0;i<count;i++){
  uint8_t*sh=base+sec+i*40;uint32_t span=u32(sh+8),rva=u32(sh+12);
  if(!span)span=u32(sh+16);
  if(!(u32(sh+36)&IMAGE_SCN_MEM_WRITE)||rva>limit||span>limit-rva)continue;
  if(target>=base+rva&&target<=base+rva+span-4)return 1;
 }return 0;
}
static uint32_t *find_cvar(uint8_t*base,uint32_t image,const char*name){
 if(!base||!name||image<0x1000||base[0]!='M'||base[1]!='Z')return 0;
 unsigned pe=u32(base+0x3c),sec=pe+24+u16(base+pe+20),count=u16(base+pe+6),n=0;
 if(pe>image-256||count==0||count>96||sec>image||count*40>image-sec)return 0;
 while(name[n]&&n<80)n++;if(!n||n==80)return 0;
 uint32_t*unique=0;
 for(unsigned i=0;i<count;i++){
  uint8_t*sh=base+sec+i*40;uint32_t span=u32(sh+8),rva=u32(sh+12);
  if(!span)span=u32(sh+16);
  if(!(u32(sh+36)&IMAGE_SCN_MEM_READ)||rva>image||span>image-rva||span<n+1)continue;
  for(uint32_t at=0;at+n<span;at++){
   uint8_t*ns=base+rva+at;unsigned k=0;
   while(k<n&&ns[k]==(uint8_t)name[k])k++;
   if(k!=n||ns[n])continue;
   for(unsigned wi=0;wi<count;wi++){
    uint8_t*wsh=base+sec+wi*40;uint32_t wspan=u32(wsh+8),wrva=u32(wsh+12);
    if(!wspan)wspan=u32(wsh+16);
    if(!(u32(wsh+36)&IMAGE_SCN_MEM_WRITE)||wrva>image||wspan>image-wrva)continue;
    for(uint32_t off=0;off+24<=wspan;off+=8){
     uint64_t ptr=*(const uint64_t*)(base+wrva+off);
     if(ptr!=(uintptr_t)ns)continue;
     uint8_t*location=*(uint8_t**)(base+wrva+off+16);
     if(!writable_range(base,image,location))continue;
     if(unique&&unique!=(uint32_t*)location)return 0;
     unique=(uint32_t*)location;
    }
   }
  }
 }
 return unique;
}
/* True Witcher renderer state comes only from validated CDPR engine CVars.
 * A missing/unknown value keeps all CR01 writers passive. */
