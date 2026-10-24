/* DT39 REV2: Patch 5.01 native SpeedTree billboard-proximity policy.
 * RTSTBillboardsProximity is a separate boolean from PT Billboards'
 * RTSTBillboards. Patch 5.01 code at RVA 0x1CE3D43, 0x1CE3DD1,
 * 0x1CE3E54 and 0x1CEA547 gates billboard-instance inclusion on this
 * proximity switch and the native per-instance mode. Tree geometry uses
 * a separate RTSTTrees bit; never modify it or global RTSTBillboards.
 *
 * A live CVar write affects subsequent native instance decisions, but does
 * NOT prove existing RT scene instances have been rebuilt. A normal area
 * transition/reload is the fallback for full A/B parity. No fabricated
 * GPU synchronization or engine refresh is attempted.
 */
#include "cr01_distant_trees501.h"
#include "cr01_contract.h"
#include <stdint.h>
static CR01Owner g_owner;
static uint32_t *g_proximity;
static unsigned g_started,g_fault;
static unsigned sane(uint32_t v){return v<=1u;}

int cr01_dt501_init(uint32_t *proximity_slot,uint32_t *global_billboard_slot){
 if(g_started||!proximity_slot||!global_billboard_slot||
    proximity_slot==global_billboard_slot ||
    ((uintptr_t)proximity_slot&3u)||((uintptr_t)global_billboard_slot&3u)||
    !sane(__atomic_load_n(proximity_slot,__ATOMIC_ACQUIRE))||
    !sane(__atomic_load_n(global_billboard_slot,__ATOMIC_ACQUIRE)))return 0;
 g_proximity=proximity_slot;g_fault=0;g_started=1;
 return 1;
}
int cr01_dt501_release(void){
 if(!g_started)return 1;
 if(!cr01_release_owner(&g_owner)){g_fault=1;return 0;}
 if(g_owner.conflicted){g_fault=1;return 0;}
 return 1;
}
/* 0=Off/native, 1=On/proximity gated, 2=already gated natively,
 * 3=DT preference temporarily suspended for PT Billboards On;
 * -1=fail-closed. Never change the separate global RTSTBillboards CVar.
 * Saved CPU preference is untouched and resumes when PT Billboards leaves On.
 * A live CVar release does not itself guarantee scene-instance rebuilding. */
int cr01_dt501_refresh(unsigned mode,unsigned master,unsigned pref_on,unsigned pt_billboards){
 if(!g_started||!g_proximity||mode>2u||master>1u||pref_on>1u||pt_billboards>2u){
  (void)cr01_dt501_release();return -1;
 }
 if(g_fault)return -1;
 /* PT explicitly On owns visual priority.  Do NOT alter either saved menu
  * value or the native PT Billboards switch. Release only this module's
  * proximity owner, with its existing compare-and-swap guard. Crucially,
  * this is renderer-specific: Normal RT retains normal DT policy. */
 if(mode==2u && master && pt_billboards==CR01_ON && pref_on){
  if(!cr01_dt501_release())return -1;
  return 3;
 }
 if(mode==0u||!master||!pref_on){
  if(!cr01_dt501_release())return -1;
  return 0;
 }
 unsigned current=__atomic_load_n(g_proximity,__ATOMIC_ACQUIRE);
 if(!sane(current)){
  (void)cr01_dt501_release();g_fault=1;return -1;
 }
 if(g_owner.owned && current!=g_owner.installed){
  g_fault=1; (void)cr01_dt501_release();return -1;
 }
 if(current==0u && !g_owner.owned)return 2;
 if(!cr01_write_owner(&g_owner,g_proximity,0u)){
  g_fault=1;(void)cr01_dt501_release();return -1;
 }
 return g_owner.owned?1:2;
}
int cr01_dt501_shutdown(void){
 int ok=cr01_dt501_release();
 if(ok){
  g_started=0;g_proximity=0;g_fault=0;
  g_owner=(CR01Owner){0};
 }
 return ok;
}
