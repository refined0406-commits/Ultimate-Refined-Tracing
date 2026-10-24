#include "cr01_settings_codec.h"
#include "cr01_native_execution.h"
static const char header1[]="CR01_SETTINGS_V1\n";
static const char header2[]="CR01_SETTINGS_V2\n";
static int putc1(char **p,size_t *left,char c){
 if(*left<=1)return 0;
 *(*p)++=c;--*left;return 1;
}
static int putstr(char **p,size_t *left,const char*s){
 while(*s)if(!putc1(p,left,*s++))return 0;
 return 1;
}
static int putnum(char **p,size_t *left,uint32_t x){
 char d[10];unsigned n=0;
 do{d[n++]=(char)('0'+x%10);x/=10;}while(x&&n<10);
 if(x)return 0;
 while(n)if(!putc1(p,left,d[--n]))return 0;
 return 1;
}
int cr01_settings_serialize(const CR01Settings*s,char*dst,size_t cap,size_t *written){
 char *p=dst;size_t n=cap;
 if(written)*written=0;
 if(!s||!dst||!written||!cr01_native_validate(s)||cap<2)return 0;
 if(!putstr(&p,&n,header2)||!putstr(&p,&n,"Master=")||
    !putnum(&p,&n,s->master)||!putc1(&p,&n,'\n'))return 0;
 for(unsigned i=0;i<CR01_FEATURE_COUNT;i++){
  if(!putc1(&p,&n,'F')||!putc1(&p,&n,(char)('0'+i/10))||
     !putc1(&p,&n,(char)('0'+i%10))||!putc1(&p,&n,'=')||
     !putnum(&p,&n,s->v[i])||!putc1(&p,&n,'\n'))return 0;
 }
 for(unsigned i=0;i<4;i++){
  const uint32_t k[4]={s->rtao.mode,s->rtao.rays,s->rtao.radius,s->rtao.strength};
  if(!putstr(&p,&n,"RTAO")||!putc1(&p,&n,(char)('0'+i))||
     !putc1(&p,&n,'=')||!putnum(&p,&n,k[i])||!putc1(&p,&n,'\n'))return 0;
 }
 if(!putstr(&p,&n,"END\n"))return 0;
 *p=0;*written=(size_t)(p-dst);return 1;
}
static int expected(const char**p,const char*e,const char*end){
 while(*e){if(*p==end||**p!=*e)return 0;(*p)++;e++;}
 return 1;
}
static int getnum(const char**p,const char*end,uint32_t*out){
 uint32_t n=0;unsigned count=0;
 while(*p!=end&&**p>='0'&&**p<='9'){
  unsigned d=(unsigned)(**p-'0');
  if(count==10||n>(UINT32_MAX-d)/10)return 0;
  n=n*10+d;count++;(*p)++;
 }
 if(!count)return 0;
 *out=n;return 1;
}
int cr01_settings_parse_migrated(const char*buf,size_t len,CR01Settings*out,int *migrated){
 CR01Settings tmp;const char*p=buf,*end;int v2=0;
 if(!buf||!out||len>4096)return 0;
 end=buf+len;
 if(expected(&p,header2,end)){v2=1;}
 else {p=buf;if(!expected(&p,header1,end))return 0;}
 if(!expected(&p,"Master=",end)||
    !getnum(&p,end,&tmp.master)||!expected(&p,"\n",end))return 0;
 for(unsigned i=0;i<CR01_FEATURE_COUNT;i++){
  char line[5]={'F',(char)('0'+i/10),(char)('0'+i%10),'=',0};
  if(!expected(&p,line,end)||!getnum(&p,end,&tmp.v[i])||
     !expected(&p,"\n",end))return 0;
 }
 cr01_rtao_defaults(&tmp.rtao);
 if(v2){
  uint32_t *fields[4]={&tmp.rtao.mode,&tmp.rtao.rays,&tmp.rtao.radius,&tmp.rtao.strength};
  for(unsigned i=0;i<4;i++){
   const char key[7]={'R','T','A','O',(char)('0'+i),'=',0};
   if(!expected(&p,key,end)||!getnum(&p,end,fields[i])||!expected(&p,"\n",end))return 0;
  }
 }
 if(!expected(&p,"END\n",end)||p!=end)return 0;
 /* Historical private-test range accepted up to 2.55. Never turn invalid
    low values or other invalid controls into silent valid preferences. */
 int changed=!v2;
 if(tmp.v[F_INDIRECT]>150 && tmp.v[F_INDIRECT]<=255){
  tmp.v[F_INDIRECT]=150;changed=1;
 }
 if(!cr01_native_validate(&tmp))return 0;
 *out=tmp;
 if(migrated)*migrated=changed;
 return 1;
}
int cr01_settings_parse(const char*buf,size_t len,CR01Settings*out){
 return cr01_settings_parse_migrated(buf,len,out,0);
}
