/* R11 helper: run URT's Orange Hair Fix port (urt_ohf_bake) on a text file (for the equivalence check) */
#include "urt_bounce501.h"
#include <stdio.h>
#include <stdlib.h>
int main(int c,char**v){if(c<3)return 2;FILE*f=fopen(v[1],"rb");if(!f)return 2;fseek(f,0,2);long n=ftell(f);fseek(f,0,0);
 char*b=malloc((size_t)n+1);if(fread(b,1,(size_t)n,f)!=(size_t)n)return 2;fclose(f);
 uint32_t cap=(uint32_t)n+262144u,o=0;unsigned s=0;char*out=malloc(cap);if(!urt_ohf_bake(b,(uint32_t)n,out,cap,&o,&s))return 1;
 f=fopen(v[2],"wb");if(!f)return 2;fwrite(out,1,o,f);fclose(f);printf("sites %u\n",s);return 0;}
