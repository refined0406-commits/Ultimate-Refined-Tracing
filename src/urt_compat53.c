/* V5.3 W3LoadFix coexistence helpers. See urt_compat53.h. */
#include "urt_compat53.h"
static char lo(char c){return (c>='A'&&c<='Z')?(char)(c-'A'+'a'):c;}
static int blank(char c){return c==' '||c=='\t'||c=='\r'||c=='\n';}
static unsigned slen(const char*s){unsigned n=0;while(s&&s[n])n++;return n;}
/* key[a,b) equals text t (case-insensitive) */
static int eq(const char*k,unsigned n,const char*t){
 unsigned m=slen(t);if(n!=m)return 0;
 for(unsigned i=0;i<n;i++)if(lo(k[i])!=lo(t[i]))return 0;
 return 1;
}
int urt_w3lf_override_mentions(const char *sec, unsigned len, const char *category, const char *cvar){
 if(!sec||!cvar||!cvar[0])return 0;
 unsigned cl=slen(category),vl=slen(cvar);
 unsigned i=0;
 while(i<len){
  unsigned s=i;while(i<len&&sec[i])i++;            /* one line [s,i) */
  unsigned e=i;i++;
  while(s<e&&blank(sec[s]))s++;
  if(s>=e||sec[s]==';'||sec[s]=='#')continue;
  unsigned q=s;while(q<e&&sec[q]!='=')q++;
  if(q>=e)continue;                                  /* not key=value */
  unsigned ke=q;while(ke>s&&blank(sec[ke-1]))ke--;
  unsigned n=ke-s;const char*k=sec+s;
  if(eq(k,n,cvar))return 1;
  if(n==cl+1u+vl&&cl&&k[cl]=='/'){
   unsigned ok=1;for(unsigned j=0;j<cl;j++)if(lo(k[j])!=lo(category[j])){ok=0;break;}
   if(ok&&eq(k+cl+1,vl,cvar))return 1;
  }
  if(n>vl&&k[n-vl-1]=='/'&&eq(k+n-vl,vl,cvar))return 1;
 }
 return 0;
}
