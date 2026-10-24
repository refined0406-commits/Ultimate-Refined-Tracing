#include "cr01_distant_trees501.h"
#include <assert.h>
#include <stdio.h>
int main(void){
 unsigned prox=1,global=1;
 assert(!cr01_dt501_init(0,&global));
 assert(!cr01_dt501_init(&prox,0));
 assert(!cr01_dt501_init(&prox,&prox));
 prox=3;assert(!cr01_dt501_init(&prox,&global));
 prox=1;global=3;assert(!cr01_dt501_init(&prox,&global));
 global=1;assert(cr01_dt501_init(&prox,&global));
 for(int i=0;i<12000;i++){
  assert(cr01_dt501_refresh(1,1,1,0)==1&&prox==0&&global==1);
  assert(cr01_dt501_refresh(2,1,1,1)==3&&prox==1&&global==1);
  assert(cr01_dt501_refresh(2,1,1,2)==1&&prox==0&&global==1);
  assert(cr01_dt501_refresh(0,1,1,0)==0&&prox==1);
  assert(cr01_dt501_refresh(1,1,0,0)==0&&prox==1);
 }
 assert(cr01_dt501_refresh(3,1,1,0)==-1&&prox==1);
 assert(cr01_dt501_refresh(2,1,1,3)==-1&&prox==1); /* invalid PT control */
 assert(cr01_dt501_refresh(1,1,1,0)==1&&prox==0);
 prox=5; /* another owner changes the slot during ownership */
 assert(cr01_dt501_refresh(1,1,1,0)==-1&&prox==5);
 assert(cr01_dt501_refresh(1,1,1,0)==-1&&prox==5);
 assert(cr01_dt501_refresh(0,1,0,0)==-1&&prox==5);
 assert(cr01_dt501_shutdown()==1&&prox==5); /* foreign value retained; no writer owned at shutdown */
 puts("PASS: 12k transitions, invalid bindings, strict foreign-owner fail-close");
}
