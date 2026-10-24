#include "cr01_rtao_config.h"
void cr01_rtao_defaults(CR01RtaoSettings *s){if(s){s->mode=0;s->rays=0;s->radius=0;s->strength=0;}}
int cr01_rtao_valid(const CR01RtaoSettings *s){return s&&s->mode<=2&&s->rays<=3&&s->radius<=6&&s->strength<=5;}
uint32_t cr01_rtao_count(uint32_t k){static const uint32_t a[]={1,2,4,8};return k<4?a[k]:0;}
uint32_t cr01_rtao_radius100(uint32_t k){static const uint32_t a[]={125,25,50,75,200,300,400};return k<7?a[k]:0;}
uint32_t cr01_rtao_strength100(uint32_t k){static const uint32_t a[]={65,0,25,40,80,100};return k<6?a[k]:0;}
uint32_t cr01_rtao_overlay(const CR01RtaoSettings *s){
 if(!cr01_rtao_valid(s)||s->mode!=1)return 0;
 return (1u<<24)|((s->strength&7u)<<21)|((s->radius&7u)<<18)|((s->rays&3u)<<16);
}
uint32_t cr01_rtao_apply(uint32_t c17_packed,const CR01RtaoSettings *s,
                        uint32_t master,uint32_t pt,uint32_t custom_live,
                        uint32_t gate_epoch,uint32_t rtao_epoch){
 uint32_t over=cr01_rtao_overlay(s);
 /* Strict mismatch/neutral rejection: no extra bits survive Master Off,
  * Native Auto, Off, PT exit, missing live shader or changing generations. */
 if(!over||master!=1||pt!=1||custom_live!=1||gate_epoch!=rtao_epoch||
    (c17_packed&0xffff0000u)!=0)return c17_packed;
 return c17_packed|over;
}
