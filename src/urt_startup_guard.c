#include "urt_startup_guard.h"
static int streq(const char*a,const char*b){while(*a&&*a==*b){a++;b++;}return *a==*b;}
static UrtSgEntry *find(UrtSgFile*f,const char*key){
  for(unsigned i=0;i<f->n;i++)if(streq(f->e[i].key,key))return &f->e[i];
  return 0;
}
static void drop(UrtSgFile*f,UrtSgEntry*e){
  unsigned i=(unsigned)(e-f->e);
  for(;i+1<f->n;i++)f->e[i]=f->e[i+1];
  f->n--;
}
void urt_sg_parse(const char*t,unsigned len,UrtSgFile*out){
  out->n=0;
  unsigned i=0;
  while(i<len&&out->n<URT_SG_MAX){
    unsigned a=i;while(i<len&&t[i]!='\n')i++;
    unsigned b=i;if(i<len)i++;
    if(b>a&&t[b-1]=='\r')b--;
    /* line: KEY=V STATE   (KEY: [A-Za-z0-9_], V: one digit, STATE: pending|proven|failed) */
    unsigned k=a;UrtSgEntry e;unsigned kn=0;
    while(k<b&&t[k]!='='){char c=t[k];
      if(!((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='_')||kn>=sizeof e.key-1){kn=0;break;}
      e.key[kn++]=c;k++;}
    if(!kn||k>=b||t[k]!='=')continue;
    e.key[kn]=0;k++;
    if(k>=b||t[k]<'1'||t[k]>'9')continue;
    e.value=(uint8_t)(t[k]-'0');k++;
    if(k>=b||t[k]!=' ')continue;k++;
    unsigned sl=b-k;const char*s=t+k;
    static const char*const names[]={"pending","proven","failed","clean","retry","retrying"};
    e.state=URT_SG_NONE;
    for(unsigned w=0;w<6;w++){unsigned j=0;while(names[w][j]&&j<sl&&s[j]==names[w][j])j++;if(!names[w][j]&&j==sl){e.state=(uint8_t)(w+1);break;}}
    if(e.state==URT_SG_NONE)continue;
    if(find(out,e.key))continue;          /* first record per key wins */
    out->e[out->n++]=e;
  }
}
unsigned urt_sg_format(const UrtSgFile*f,char*o,unsigned cap){
  static const char hdr[]="; Ultimate Refined Tracing startup guard (restart-dependent rows under test). Safe to delete.\r\n";
  unsigned n=0;
  for(unsigned i=0;hdr[i];i++){if(n+1>=cap)return 0;o[n++]=hdr[i];}
  for(unsigned i=0;i<f->n;i++){
    const UrtSgEntry*e=&f->e[i];
    static const char*const names[]={"failed","pending","proven","failed","clean","retry","retrying"};
    const char*st=names[e->state<=URT_SG_RETRYING?e->state:0];
    for(unsigned j=0;e->key[j];j++){if(n+1>=cap)return 0;o[n++]=e->key[j];}
    if(n+4>=cap)return 0;o[n++]='=';o[n++]=(char)('0'+e->value);o[n++]=' ';
    for(unsigned j=0;st[j];j++){if(n+1>=cap)return 0;o[n++]=st[j];}
    if(n+3>=cap)return 0;o[n++]='\r';o[n++]='\n';
  }
  o[n]=0;return n;
}
int urt_sg_decide_ex(UrtSgFile*f,const char*key,uint8_t wish,int*ev){
  UrtSgEntry*e=find(f,key);
  *ev=URT_SG_EV_NATIVE;
  if(!wish||wish>9){if(e)drop(f,e);return 0;}            /* Native: forget; a later choice starts fresh */
  if(e&&e->value!=wish){drop(f,e);e=0;}                 /* another value: fresh record */
  if(!e){
    if(f->n>=URT_SG_MAX){*ev=URT_SG_EV_FULL;return 0;}  /* cannot record: do not apply (fail safe) */
    e=&f->e[f->n++];unsigned j=0;while(key[j]&&j<sizeof e->key-1){e->key[j]=key[j];j++;}e->key[j]=0;
    e->value=wish;e->state=URT_SG_PENDING;*ev=URT_SG_EV_NEW;return 1;
  }
  switch(e->state){
    case URT_SG_PROVEN:*ev=URT_SG_EV_PROVEN;return 1;
    case URT_SG_CLEAN:e->state=URT_SG_PENDING;*ev=URT_SG_EV_CLEAN;return 1;            /* last session quit normally */
    case URT_SG_PENDING:e->state=URT_SG_RETRY;*ev=URT_SG_EV_SUSPECTED;return 0;      /* interrupted: skip this start */
    case URT_SG_RETRY:e->state=URT_SG_RETRYING;*ev=URT_SG_EV_RETRYING;return 1;      /* one more try */
    case URT_SG_RETRYING:e->state=URT_SG_FAILED;*ev=URT_SG_EV_FAILED_NOW;return 0;   /* interrupted twice: off */
    default:*ev=URT_SG_EV_BLOCKED;return 0;                                          /* FAILED stays off */
  }
}
int urt_sg_decide(UrtSgFile*f,const char*key,uint8_t wish,int*newly_failed){
  int ev=0,r=urt_sg_decide_ex(f,key,wish,&ev);
  *newly_failed=ev==URT_SG_EV_SUSPECTED||ev==URT_SG_EV_FAILED_NOW;
  return r;
}
unsigned urt_sg_mark_clean(UrtSgFile*f,const char*const*keys,unsigned nk){
  unsigned c=0;
  for(unsigned k=0;k<nk;k++){UrtSgEntry*e=keys[k]?find(f,keys[k]):0;
    if(e&&(e->state==URT_SG_PENDING||e->state==URT_SG_RETRYING)){e->state=URT_SG_CLEAN;c++;}}
  return c;
}
int urt_sg_state(const UrtSgFile*f,const char*key){
  for(unsigned i=0;i<f->n;i++)if(streq(f->e[i].key,key))return f->e[i].state;
  return URT_SG_NONE;
}
unsigned urt_sg_prove_all(UrtSgFile*f){
  unsigned c=0;for(unsigned i=0;i<f->n;i++)if(f->e[i].state==URT_SG_PENDING||f->e[i].state==URT_SG_RETRYING){f->e[i].state=URT_SG_PROVEN;c++;}
  return c;
}
int urt_sg_prove_key(UrtSgFile*f,const char*key){
  for(unsigned i=0;i<f->n;i++)if(streq(f->e[i].key,key)&&(f->e[i].state==URT_SG_PENDING||f->e[i].state==URT_SG_RETRYING)){f->e[i].state=URT_SG_PROVEN;return 1;}
  return 0;
}
unsigned urt_sg_pending(const UrtSgFile*f){
  unsigned c=0;for(unsigned i=0;i<f->n;i++)c+=f->e[i].state==URT_SG_PENDING||f->e[i].state==URT_SG_RETRYING;return c;
}
int urt_sg_key_is(const UrtSgFile*f,unsigned i,const char*key){return i<f->n&&streq(f->e[i].key,key);}
void urt_sg_drop_at(UrtSgFile*f,unsigned i){if(i<f->n)drop(f,&f->e[i]);}

static const char *g_session[URT_SG_MAX];static unsigned g_nsession;
void urt_sg_session_note(const char*key){
  for(unsigned i=0;i<g_nsession;i++)if(streq(g_session[i],key))return;
  if(g_nsession<URT_SG_MAX)g_session[g_nsession++]=key;   /* keys are static strings (row keys) */
}
void urt_sg_session_clear(void){g_nsession=0;}
unsigned urt_sg_session_keys(const char*const**keys){*keys=g_session;return g_nsession;}
const char *urt_sg_event_text(int ev){
  switch(ev){
    case URT_SG_EV_NEW:return " applied (new choice; safe after a clean game exit, or once it qualifies in play: PT coverage 300 frames, RT rows 1 minute)";
    case URT_SG_EV_CLEAN:return " applied (the last session that used it ended with a clean game exit)";
    case URT_SG_EV_PROVEN:return " applied (proven by an earlier session)";
    case URT_SG_EV_RETRYING:return " applied once more after a suspected crash (an interruption now turns it off until Native / Auto is chosen for one start)";
    case URT_SG_EV_SUSPECTED:return " NOT applied this start: the last session that used it ended without a clean game exit before it qualified (suspected start-up failure). It is retried automatically at the next start";
    case URT_SG_EV_FAILED_NOW:return " turned OFF: the retry after a suspected crash was interrupted as well. Choose Native / Auto for one start to clear it, then choose it again";
    case URT_SG_EV_BLOCKED:return " still off after two interrupted sessions (choose Native / Auto for one start to clear it)";
    case URT_SG_EV_FULL:return " NOT applied: the startup guard file is full";
    default:return "";
  }
}
