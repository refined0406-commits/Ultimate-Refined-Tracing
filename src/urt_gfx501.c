/* V5.2.0 curated controls (16 engine rows + FG); no native texture-quality preset CVars.
 * RC6: rows whose variable the game saves are detached from the settings save while owned. */
#include "urt_gfx501.h"
#include "urt_gfx_core.h"
#include "urt_fg501.h"
#include "urt_cvar_index.h"
#include "cr01_release_log.h"
#include "urt_compat53.h"
#include <stddef.h>
typedef unsigned short WCHAR;
typedef unsigned long DWORD;
extern DWORD urt_cfg_get_ext(const WCHAR*,WCHAR*,DWORD);
extern const UrtCvarIndex *urt_runtime_cvar_index(void);
extern int cr01_gate_verify_exe_file(void);
static UrtGxOwner g_owner;
static UrtGxPrefs g_prefs;
static unsigned g_ready,g_valid,g_known,g_bound,g_conflicts,g_yields,g_sbc,g_det,g_detlog;
/* V5.3: optional W3LoadFix [Override] text (set by the runtime before init; empty when W3LoadFix is absent),
 * rows left to W3LoadFix, the image base for the read-only RT acceleration-structure diagnostic. */
static const char *g_ext_text;static unsigned g_ext_len,g_deferred,g_asdiag;static uint8_t *g_base;
void urt_gfx501_set_external_overrides(const char*text,unsigned len){g_ext_text=text;g_ext_len=text?len:0u;}
static void as_diagnostic(void);
/* RC6: for a row whose variable the game saves (flag 0x100), *savebyte receives the address of
 * the descriptor's flag byte (+9; bit 0 = save). Bound only when the value sits at the fixed
 * descriptor offset +0x20 and the bit is set now; otherwise the row is not bound at all. */
static uint8_t *bind_row(uint8_t*base,uint32_t image,const UrtGxDef*d,uint8_t**savebyte){
 const UrtCvarIndex*ix=urt_runtime_cvar_index();uint32_t desc=0;
 *savebyte=0;
 if(!ix||ix->base!=base||ix->image_size!=image||!d->cvar[0])return 0;
 uint32_t*slot=d->category[0] ? urt_cvar_lookup_cat(ix,d->cvar,d->category,d->flags,d->def,&desc) : 0;
 if(!slot)return 0;
 if(g_known&&(uint8_t*)slot!=base+d->rva)return 0;
 if(d->flags&0x100u){
  uint8_t*fb=base+desc+9u;
  if((uint8_t*)slot!=base+desc+0x20u||desc+0x20u>=image)return 0;
  if(!(__atomic_load_n(fb,__ATOMIC_ACQUIRE)&1u))return 0;
  *savebyte=fb;
 }
 return (uint8_t*)slot;
}
int urt_gfx501_init(uint8_t*base,uint32_t image){
 g_known=(unsigned)cr01_gate_verify_exe_file();urt_gx_init(&g_owner);
 for(unsigned i=0;i<URT_GX_COUNT;i++)g_prefs.v[i]=0;
 g_ready=g_valid=0;g_bound=g_conflicts=g_yields=g_sbc=g_det=g_detlog=0;g_deferred=0;
 for(unsigned i=0;i<URT_GX_COUNT;i++){
   if(urt_gx_defs[i].special!=URT_GX_SPECIAL_NONE)g_owner.slot[i]=0;
   else {g_owner.slot[i]=bind_row(base,image,&urt_gx_defs[i],&g_owner.savebyte[i]);if(g_owner.slot[i])g_bound++;}
   /* V5.3: a variable set by W3LoadFix's [Override] section stays W3LoadFix's (never bound, never written). */
   if(g_owner.slot[i]&&g_ext_len&&urt_w3lf_override_mentions(g_ext_text,g_ext_len,urt_gx_defs[i].category,urt_gx_defs[i].cvar)){
     g_owner.slot[i]=0;g_owner.savebyte[i]=0;g_bound--;g_deferred++;
   }
 }
 g_base=base;g_asdiag=0;
 (void)urt_fg501_init(base,image,(int)g_known);
 g_ready=1;
 cr01_release_log_event(g_bound==URT_GX_COUNT-1u ?
  "URT: selected graphics controls verified (all engine variables plus FG)\r\n" :
  "URT: selected graphics controls partially unavailable (unverified rows stay native)\r\n");
 if(g_deferred)cr01_release_log_event("URT: W3LoadFix [Override] sets a variable URT could control; that URT row stays native and W3LoadFix keeps it this session\r\n");
 return 1;
}
static int read_row(unsigned i,uint8_t*out){
 const char*name=urt_gx_defs[i].key;WCHAR k[64],s[8];char a[8];unsigned n=0;
 while(name[n]&&n<63u){k[n]=(WCHAR)(unsigned char)name[n];n++;}
 if(name[n])return 0;k[n]=0;s[0]=0;
 DWORD c=urt_cfg_get_ext(k,s,8);if(c>4)return 0;
 for(DWORD j=0;j<c;j++){if(s[j]>127)return 0;a[j]=(char)s[j];}a[c]=0;
 return urt_gx_parse_index(a,i,out);
}
int urt_gfx501_reload(void){
 if(!g_ready)return 0;
 /* Called on every exact dx12user.settings change (and once at start): the game has saved, so a
  * saved row that the game or another mod changed may be owned again (see urt_gx_settings_saved). */
 urt_gx_settings_saved(&g_owner);
 UrtGxPrefs p;
 for(unsigned i=0;i<URT_GX_COUNT;i++)if(!read_row(i,&p.v[i]))goto invalid;
 g_prefs=p;g_valid=1;return 1;
invalid:
 g_valid=0;urt_gx_release_all(&g_owner);
 cr01_release_log_event("URT: invalid selected graphics setting; all variables released to native\r\n");return 0;
}
void urt_gfx501_tick(unsigned mode_valid,unsigned mode,unsigned master,uint32_t now_ms){
 if(!g_ready)return;
 urt_gx_update(&g_owner,&g_prefs,master&&g_valid,mode_valid,mode,now_ms);
 if(mode_valid&&mode)as_diagnostic();
 urt_fg501_tick(g_valid&&master&&g_prefs.v[URT_GX_IX_FGMENURESOURCES]==1u);
 if(g_owner.conflicts!=g_conflicts){g_conflicts=g_owner.conflicts;
  cr01_release_log_event("URT: foreign graphics value detected; did not overwrite\r\n");}
 if(g_owner.yields!=g_yields){g_yields=g_owner.yields;
  cr01_release_log_event("URT: conflicting graphics variable yielded to the game or another mod\r\n");}
 if(g_owner.sb_conflicts!=g_sbc){g_sbc=g_owner.sb_conflicts;
  cr01_release_log_event("URT: a saved graphics variable was not in the expected state; it stays native\r\n");}
 if(g_owner.detaches!=g_det&&g_detlog<8u){g_det=g_owner.detaches;g_detlog++;
  cr01_release_log_event("URT: graphics value applied for this session only (not written to dx12user.settings)\r\n");}
}
/* V5.3: the RT acceleration-structure rows are read by the engine once, when ray tracing initialises
 * (render-device start; Patch 5.01 copies them into its AS configuration at RVA 0x405af10/0x405af14).
 * Apply them as early as the worker can, before a renderer mode exists. Live rows are not touched here. */
void urt_gfx501_boot(unsigned master,uint32_t now_ms){
 if(!g_ready||!g_valid)return;
 unsigned want=0;
 for(unsigned i=0;i<URT_GX_COUNT;i++)if(urt_gx_is_startup(i)&&g_prefs.v[i])want++;
 if(!want)return;
 const int early=g_known&&g_base&&__atomic_load_n((uint32_t*)(void*)(g_base+0x405af14u),__ATOMIC_ACQUIRE)==0u;
 urt_gx_update(&g_owner,&g_prefs,master,0u,0u,now_ms);
 cr01_release_log_event(!g_known?"URT: RT memory settings applied at startup (unknown executable: capture point not verified)\r\n":
  early?"URT: RT memory settings applied before ray tracing initialised\r\n":
        "URT: RT memory settings applied after ray tracing initialised; they take effect from the next game start\r\n");
}
/* Read-only diagnostic, once per session after the first renderer tick: does the engine's AS configuration
 * hold the values URT applied? (Patch 5.01 only; never written.) */
static void as_diagnostic(void){
 if(g_asdiag||!g_known||!g_base)return;
 unsigned d=URT_GX_IX_RTASDEFRAG,b=URT_GX_IX_RTASBATCH;
 if(!g_owner.owned[d]&&!g_owner.owned[b])return;
 const uint32_t cd=__atomic_load_n((uint32_t*)(void*)(g_base+0x405af10u),__ATOMIC_ACQUIRE);
 const uint32_t cb=__atomic_load_n((uint32_t*)(void*)(g_base+0x405af14u),__ATOMIC_ACQUIRE);
 if(!cb)return;                                   /* ray tracing not initialised yet: check again later */
 g_asdiag=1;
 const int ok=(!g_owner.owned[d]||cd==g_owner.applied[d])&&(!g_owner.owned[b]||cb==g_owner.applied[b]);
 cr01_release_log_event(ok?"URT: RT acceleration-structure manager is using URT's RT memory settings\r\n":
  "URT: RT acceleration-structure manager started with other RT memory values; URT's values apply from the next game start\r\n");
}
void urt_gfx501_shutdown(void){
 if(!g_ready)return;
 urt_gx_release_all(&g_owner);urt_fg501_shutdown();
}
