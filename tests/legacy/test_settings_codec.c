#include "cr01_settings_codec.h"
#include "cr01_native_execution.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
int main(void){
 CR01Settings x,y,hold;char output[500];size_t n;
 cr01_defaults(&x);x.master=1;x.v[F_GEOM]=4;x.v[F_INDIRECT]=135;
 x.v[F_SUN]=250;x.v[F_HAIR]=5;x.v[F_AO]=CR01_ON;
 assert(cr01_settings_serialize(&x,output,sizeof(output),&n));
 assert(n==strlen(output));assert(cr01_settings_parse(output,n,&y));
 assert(!memcmp(&x,&y,sizeof(x)));
 hold=y;assert(!cr01_settings_parse(output,n-1,&y));
 assert(!memcmp(&hold,&y,sizeof(hold)));
 output[n]='Z';assert(!cr01_settings_parse(output,n+1,&y));
 output[0]='Q';assert(!cr01_settings_parse(output,n,&y));
 output[0]='C';assert(cr01_settings_parse(output,n,&y));
 assert(!cr01_settings_serialize(&x,output,15,&n));
 x.v[F_GEOM]=70;assert(!cr01_settings_serialize(&x,output,sizeof(output),&n));
 puts("PASS settings roundtrip, malformed/extra/partial refusal and atomic parse");
 return 0;
}
