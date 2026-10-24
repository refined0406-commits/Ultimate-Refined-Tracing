#include "cr01_distant_trees501.h"
#include <assert.h>
#include <stdio.h>
int main(void){
 unsigned prox=1,global=1;
 assert(cr01_dt501_init(&prox,&global));
 assert(cr01_dt501_refresh(0,1,1,0)==0&&prox==1);
 assert(cr01_dt501_refresh(1,1,1,0)==1&&prox==0&&global==1);
 assert(cr01_dt501_refresh(2,1,1,1)==3&&prox==1&&global==1);
 assert(cr01_dt501_refresh(2,1,1,1)==3&&prox==1&&global==1); /* repeat safe */
 assert(cr01_dt501_refresh(2,1,1,2)==1&&prox==0&&global==1);
 assert(cr01_dt501_refresh(0,1,1,0)==0&&prox==1&&global==1);
 assert(cr01_dt501_refresh(1,1,1,0)==1&&prox==0);
 assert(cr01_dt501_refresh(1,1,0,0)==0&&prox==1);
 assert(cr01_dt501_refresh(1,1,1,0)==1&&prox==0); /* late-On is allowed */
 assert(cr01_dt501_refresh(2,0,1,0)==0&&prox==1);
 assert(cr01_dt501_shutdown()==1&&prox==1);
 assert(cr01_dt501_init(&prox,&global));
 assert(cr01_dt501_refresh(1,1,1,0)==1&&prox==0);
 assert(cr01_dt501_shutdown()&&prox==1);
 printf("PASS: PT Billboards priority, DT restoration/resumption, RT/PT/raster, native release\n");
}
