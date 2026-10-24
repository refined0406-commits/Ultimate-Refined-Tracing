#include "../../src/urt_ini.h"
#include <stdio.h>
#include <string.h>
static int fails;
static void expect(const UrtIniSection*s,const char*k,const char*want,unsigned cap){
 char v[256];unsigned n=urt_ini_get(s,k,v,cap);
 if(strcmp(v,want)||n!=strlen(want)){printf("  FAIL key %s -> '%s' (%u) want '%s'\n",k,v,n,want);fails++;}
}
int main(void){
 UrtIniSection s;
 const char*t1="[Other]\r\nMasterEnabled=0\r\n[ refinedtracingcr01 ]\r\nMasterEnabled=1\r\n  GeometryBounces =  4  \r\nIndirectLight=\"1.25\"\r\nSunImpact='0.80'\r\nOdd=\"x'\r\nnoequals\r\nMasterEnabled=0\r\n=novalue\r\nEmpty=\r\n[RefinedTracingCR01]\r\nHairBounces=9\r\n[Next]\r\nPTShadows=2\r\n";
 if(!urt_ini_parse((const uint8_t*)t1,strlen(t1),"RefinedTracingCR01",&s)){puts("FAIL parse1");return 1;}
 expect(&s,"MasterEnabled","1",80);          /* first key in first matching section */
 expect(&s,"masterenabled","1",80);          /* case-insensitive key */
 expect(&s,"GeometryBounces","4",80);        /* trimmed key and value */
 expect(&s,"IndirectLight","1.25",80);       /* double quotes removed */
 expect(&s,"SunImpact","0.80",80);           /* single quotes removed */
 expect(&s,"Odd","\"x'",80);                 /* mismatched quotes kept */
 expect(&s,"Empty","",80);
 expect(&s,"HairBounces","",80);             /* duplicate later section ignored */
 expect(&s,"PTShadows","",80);               /* other section */
 expect(&s,"GeometryBounces","4",1+1);       /* truncation to cap-1 */
 expect(&s,"IndirectLight","1.",3);
 /* UTF-8 BOM + LF only */
 const char t2[]="\xEF\xBB\xBF[RefinedTracingCR01]\nMasterEnabled=true\n";
 urt_ini_parse((const uint8_t*)t2,sizeof(t2)-1,"RefinedTracingCR01",&s);expect(&s,"MasterEnabled","true",80);
 /* UTF-16LE with BOM */
 uint8_t u[256];unsigned n=0;u[n++]=0xff;u[n++]=0xfe;const char*a="[RefinedTracingCR01]\r\nCPUPhysicsLOD=1\r\n";
 for(;*a;a++){u[n++]=(uint8_t)*a;u[n++]=0;}
 if(!urt_ini_parse(u,n,"RefinedTracingCR01",&s)){puts("  FAIL utf16");fails++;}else expect(&s,"CPUPhysicsLOD","1",80);
 /* non-ASCII UTF-16 inside our section -> unusable (fall back to API) */
 n=2;a="[RefinedTracingCR01]\r\nX=";for(;*a;a++){u[n++]=(uint8_t)*a;u[n++]=0;}u[n++]=0xe9;u[n++]=0x00;u[n++]=0x10;u[n++]=0x20;
 if(urt_ini_parse(u,n,"RefinedTracingCR01",&s)){puts("  FAIL non-ascii utf16 accepted");fails++;}
 /* missing section: parsed, every key absent */
 const char*t3="[Rendering]\nPTEnable=1\n";
 if(!urt_ini_parse((const uint8_t*)t3,strlen(t3),"RefinedTracingCR01",&s)){puts("  FAIL t3");fails++;}expect(&s,"PTEnable","",80);
 /* empty / zero-byte file */
 if(!urt_ini_parse((const uint8_t*)"",0,"RefinedTracingCR01",&s)){puts("  FAIL empty");fails++;} expect(&s,"MasterEnabled","",80);
 /* no trailing newline */
 const char*t4="[RefinedTracingCR01]\nHairBounces=10";
 urt_ini_parse((const uint8_t*)t4,strlen(t4),"RefinedTracingCR01",&s);expect(&s,"HairBounces","10",80);
 if(fails){printf("FAIL %d\n",fails);return 1;}
 puts("PASS ini snapshot semantics (16 cases)");return 0;
}
