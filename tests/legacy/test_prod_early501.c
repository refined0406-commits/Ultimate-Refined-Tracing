#include <assert.h>
#include <stdio.h>
#include <stdint.h>
int cr01_prod_early501_eligibility(int,uint64_t,uint64_t,int,int,int);
int main(void){
 assert(cr01_prod_early501_eligibility(1,0,0,1,1,1));
 assert(!cr01_prod_early501_eligibility(0,0,0,1,1,1));
 assert(!cr01_prod_early501_eligibility(1,0x1200,0,1,1,1));
 assert(!cr01_prod_early501_eligibility(1,0,0x4200,1,1,1));
 assert(!cr01_prod_early501_eligibility(1,0,0,0,1,1));
 assert(!cr01_prod_early501_eligibility(1,0,0,1,0,1));
 assert(!cr01_prod_early501_eligibility(1,0,0,1,1,0));
 puts("PASS: bounded first-DI Patch 5.01 native owner readiness rejects every late/foreign signature case");
}
