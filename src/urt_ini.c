#include "urt_ini.h"

static int lower(int c){return c>='A'&&c<='Z'?c+32:c;}
static int ieq(const char*a,unsigned an,const char*b,unsigned bn){
  if(an!=bn)return 0;
  for(unsigned i=0;i<an;i++)if(lower((unsigned char)a[i])!=lower((unsigned char)b[i]))return 0;
  return 1;
}
static int ws(char c){return c==' '||c=='\t';}

/* Read one logical character stream as bytes; UTF-16LE is narrowed only when
 * every code unit is ASCII (anything else makes the snapshot unusable). */
typedef struct { const uint8_t*d; size_t n,pos; int utf16; int bad; } Rd;
static int next(Rd*r,char*c){
  if(r->utf16){
    if(r->pos+1>=r->n)return 0;
    unsigned u=(unsigned)r->d[r->pos]|((unsigned)r->d[r->pos+1]<<8);r->pos+=2;
    if(u>0x7f){*c='?';r->bad=1;return 1;}
    *c=(char)u;return 1;
  }
  if(r->pos>=r->n)return 0;
  *c=(char)r->d[r->pos++];return 1;
}

int urt_ini_parse(const uint8_t*d,size_t n,const char*section,UrtIniSection*s){
  Rd r={d,n,0,0,0};char line[1024];
  unsigned sn=0;while(section[sn])sn++;
  int in=0,done=0,bad_in_section=0;
  s->count=0;s->valid=0;s->text_used=0;
  if(!d)return 0;
  if(n>=2&&d[0]==0xff&&d[1]==0xfe){r.utf16=1;r.pos=2;}
  else if(n>=3&&d[0]==0xef&&d[1]==0xbb&&d[2]==0xbf)r.pos=3;
  for(;;){
    unsigned len=0;char c;int got=0,overlong=0;
    r.bad=0;
    while((got=next(&r,&c))&&c!='\n'){if(len<sizeof(line)-1)line[len++]=c;else overlong=1;}
    if(!got&&len==0)break;
    while(len&&(line[len-1]=='\r'))len--;
    unsigned a=0;while(a<len&&ws(line[a]))a++;
    if(a<len&&line[a]=='['){
      unsigned b=a+1;while(b<len&&line[b]!=']')b++;
      unsigned x=a+1,y=b;while(x<y&&ws(line[x]))x++;while(y>x&&ws(line[y-1]))y--;
      if(in)done=1;                      /* first matching section only */
      in=!done&&ieq(line+x,y-x,section,sn);
      continue;
    }
    if(!in)continue;
    if(r.bad||overlong)bad_in_section=1;
    unsigned eq=a;while(eq<len&&line[eq]!='=')eq++;
    if(eq>=len)continue;
    unsigned ks=a,ke=eq;while(ke>ks&&ws(line[ke-1]))ke--;
    unsigned vs=eq+1,ve=len;while(vs<ve&&ws(line[vs]))vs++;while(ve>vs&&ws(line[ve-1]))ve--;
    if(ve-vs>=2&&(line[vs]=='"'||line[vs]=='\'')&&line[ve-1]==line[vs]){vs++;ve--;}
    if(ke==ks)continue;
    int dup=0;
    for(unsigned i=0;i<s->count;i++)if(ieq(s->text+s->key_off[i],s->key_len[i],line+ks,ke-ks)){dup=1;break;}
    if(dup)continue;                     /* first matching key wins */
    if(s->count>=URT_INI_MAX_KEYS||s->text_used+(ke-ks)+(ve-vs)>URT_INI_MAX_TEXT)return 0;
    s->key_off[s->count]=(uint16_t)s->text_used;s->key_len[s->count]=(uint16_t)(ke-ks);
    for(unsigned i=ks;i<ke;i++)s->text[s->text_used++]=line[i];
    s->val_off[s->count]=(uint16_t)s->text_used;s->val_len[s->count]=(uint16_t)(ve-vs);
    for(unsigned i=vs;i<ve;i++)s->text[s->text_used++]=line[i];
    s->count++;
  }
  if(bad_in_section)return 0;
  s->valid=1;return 1;
}

unsigned urt_ini_get(const UrtIniSection*s,const char*key,char*out,unsigned cap){
  unsigned kn=0;while(key[kn])kn++;
  if(!out||!cap)return 0;
  out[0]=0;
  if(!s||!s->valid)return 0;
  for(unsigned i=0;i<s->count;i++){
    if(!ieq(s->text+s->key_off[i],s->key_len[i],key,kn))continue;
    unsigned n=s->val_len[i];if(n>cap-1)n=cap-1;
    for(unsigned j=0;j<n;j++)out[j]=s->text[s->val_off[i]+j];
    out[n]=0;return n;
  }
  return 0;
}
