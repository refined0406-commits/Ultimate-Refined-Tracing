/* V5.2.0 curated controls (16 engine rows + FG); no native texture-quality preset CVars.
 * RC6: rows whose variable the game saves are detached from the settings save while owned. */
#include "urt_build.h"
#include "urt_gfx501.h"
#include "urt_gfx_core.h"
#include "urt_fg501.h"
#include "urt_cvar_index.h"
#include "cr01_release_log.h"
#include "urt_compat53.h"
#include "urt_startup_guard.h"
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
static unsigned g_ptdiag;
/* V5.3.0 rev4: startup guard (see urt_startup_guard.h) for the restart-dependent rows still under test.
 * The runtime registers the guard-file I/O; without it the guarded rows are not applied (fail safe). */
static int (*g_sg_read)(char*,unsigned);static int (*g_sg_write)(const char*,unsigned);
static UrtSgFile g_sg;static unsigned g_sg_live;static uint32_t g_sg_rt_ms,g_sg_last;
/* R5: async AS compaction removed (crashed in the owner's test); only the two remaining rows under test are guarded */
/* R8: + RT/PT: Solid Distant Foliage. R9: foliage retired; + PT: Radiance Cache (SHaRC). R11: SHaRC row retired;
 * + PT: Second Bounce Coverage, decided by the PT shader preparation at ASI load (urt_bounce_boot501.c), which writes
 * its record before this boot step: the boot step keeps that record (never decides it again) and proves it. */
#define URT_SG_ROWS 5u
static const unsigned kGuarded[URT_SG_ROWS]={URT_GX_IX_RTASFASTTRACE,URT_GX_IX_RTASYNCGICLASSIFY,URT_GX_IX_PTDIFFUSECOVERAGE,URT_GX_IX_EXPPTSUNSHADOWCOVERAGE,URT_GX_IX_EXPPTLOCALSHADOWCOVERAGE};
/* R11/R12: rows of URT's PT library variant, decided and recorded at ASI load (urt_bounce_boot501.c) */
static int asi_row(unsigned i){return i==URT_GX_IX_EXPPTLOCALSHADOWCOVERAGE||i==URT_GX_IX_PTDIFFUSECOVERAGE||i==URT_GX_IX_EXPPTSUNSHADOWCOVERAGE;}
void urt_gfx501_set_guard_io(int(*rd)(char*,unsigned),int(*wr)(const char*,unsigned)){g_sg_read=rd;g_sg_write=wr;}
static unsigned g_vframes,g_cov_live;               /* R12 FIX4: PT frames rendered with URT's variant (render thread count) */
void urt_gfx501_note_variant_frames(unsigned frames){g_vframes=frames;}
static unsigned g_bc_decided;                      /* R11: the ASI-load decision ran this session (PT shader prepared) */
void urt_gfx501_set_bounce_decided(unsigned decided){g_bc_decided=decided?1u:0u;}
unsigned urt_gfx501_owned(void){return urt_gx_owned_count(&g_owner);}   /* R12 FIX3: diagnostics (engine values URT holds now) */
unsigned urt_gfx501_writes(void){return g_owner.writes;}               /* R12 FIX3: engine-variable writes since start (diagnostics, tests) */
/* R9: runtime proof for PT: Radiance Cache (SHaRC). Patch 5.01 render interface (vtable 0x382a950) at [0x5c92af0];
 * its SHaRC path (vt+0x160 = 0x1eefcd0) allocates the cache in 0x1ef06e0 and stores the entry count at +0x220
 * (0x1ef0870 'mov [rdi+0x220],r13d'; 0 when not allocated or released after a failed allocation). Read-only. */
#define URT_SHARC_RI_PTR   urt_rva(0x5c92af0u)
#define URT_SHARC_RI_VT    urt_rva(0x382a950u)
#define URT_SHARC_RI_COUNT 0x220u
static unsigned g_shdiag;static uint32_t g_sh_ms,g_sh_last;
static uint32_t sharc_entries(void){
 static const uint8_t store[7]={0x44,0x89,0xaf,0x20,0x02,0x00,0x00},gate[3]={0x44,0x38,0x0d};
 if(!g_known||!g_base)return 0;
 /* R11: 'cmp [rip+d],r9b' is checked by opcode and by its target (PTUseSHARC) so it holds on every known build */
 const uint32_t st=urt_rva(0x1ef0870u),gt=urt_rva(0x1c1e97eu),flag=urt_rva(0x5149b40u);
 if(!st||!gt||!flag||!URT_SHARC_RI_PTR||!URT_SHARC_RI_VT)return 0;
 for(unsigned i=0;i<7u;i++)if(g_base[st+i]!=store[i])return 0;
 for(unsigned i=0;i<3u;i++)if(g_base[gt+i]!=gate[i])return 0;
 {const uint8_t*d=g_base+gt+3u;const int32_t disp=(int32_t)((uint32_t)d[0]|(uint32_t)d[1]<<8|(uint32_t)d[2]<<16|(uint32_t)d[3]<<24);
  if((uint32_t)((int64_t)gt+7+disp)!=flag)return 0;}
 const uint8_t*ri=(const uint8_t*)(uintptr_t)__atomic_load_n((const uint64_t*)(const void*)(g_base+URT_SHARC_RI_PTR),__ATOMIC_ACQUIRE);
 /* the engine also stores small sentinels (0x10 when the holder is empty, 0x1ef37d4) and clears it at shutdown */
 if((uintptr_t)ri<0x10000u||((uintptr_t)ri&7u)||*(const uint64_t*)(const void*)ri!=(uint64_t)(uintptr_t)(g_base+URT_SHARC_RI_VT))return 0;
 return __atomic_load_n((const uint32_t*)(const void*)(ri+URT_SHARC_RI_COUNT),__ATOMIC_ACQUIRE);
}
static void log_kv(const char*pre,const char*key,unsigned v,const char*post){
 char b[560];unsigned n=0;
 for(const char*q=pre;*q&&n<300;q++)b[n++]=*q;
 for(const char*q=key;*q&&n<300;q++)b[n++]=*q;
 if(n<300){b[n++]='=';b[n++]=(char)('0'+(v%10u));}
 for(const char*q=post;*q&&n<556;q++)b[n++]=*q;
 b[n]=0;cr01_release_log_event(b);
}
static int sg_save(void){
 char t[1024];unsigned n=urt_sg_format(&g_sg,t,sizeof t);
 return n&&g_sg_write&&g_sg_write(t,n)==(int)n;
}
/* RC6: for a row whose variable the game saves (flag 0x100), *savebyte receives the address of
 * the descriptor's flag byte (+9; bit 0 = save). Bound only when the value sits at the fixed
 * descriptor offset +0x20 and the bit is set now; otherwise the row is not bound at all. */
static uint8_t *bind_row(uint8_t*base,uint32_t image,const UrtGxDef*d,uint8_t**savebyte){
 const UrtCvarIndex*ix=urt_runtime_cvar_index();uint32_t desc=0;
 *savebyte=0;
 if(!ix||ix->base!=base||ix->image_size!=image||!d->cvar[0])return 0;
 uint32_t*slot=d->category[0] ? urt_cvar_lookup_cat(ix,d->cvar,d->category,d->flags,d->def,&desc) : 0;
 if(!slot)return 0;
 if(g_known&&(uint8_t*)slot!=base+urt_rva(d->rva))return 0;
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
 g_base=base;g_asdiag=0;g_ptdiag=0;g_shdiag=0;g_sh_ms=0;g_sh_last=0;
 (void)urt_fg501_init(base,image,(int)g_known);
 g_ready=1;
 unsigned specials=0;for(unsigned i=0;i<URT_GX_COUNT;i++)if(urt_gx_defs[i].special!=URT_GX_SPECIAL_NONE)specials++;
 cr01_release_log_event(g_bound==URT_GX_COUNT-specials ?
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
 for(unsigned i=0;i<URT_GX_COUNT;i++)if(urt_gx_defs[i].lead==URT_GX_NO_LEAD&&!read_row(i,&p.v[i]))goto invalid;
 /* V5.3.0: hidden follower rows (no menu key) take their leader's option index */
 for(unsigned i=0;i<URT_GX_COUNT;i++)if(urt_gx_defs[i].lead!=URT_GX_NO_LEAD)p.v[i]=p.v[urt_gx_defs[i].lead];
 g_prefs=p;g_valid=1;return 1;
invalid:
 g_valid=0;urt_gx_release_all(&g_owner);
 cr01_release_log_event("URT: invalid selected graphics setting; all variables released to native\r\n");return 0;
}
void urt_gfx501_tick(unsigned mode_valid,unsigned mode,unsigned master,uint32_t now_ms){
 if(!g_ready)return;
 urt_gx_update(&g_owner,&g_prefs,master&&g_valid,mode_valid,mode,now_ms);
 if(mode_valid&&mode)as_diagnostic();
 /* R12 FIX4: the PT shader coverage rows qualify once URT's variant has rendered URT_SG_PT_FRAMES Path Tracing frames
    (counted on the render thread from frames whose CameraCB word carried the level; a hung GPU stops frames, so a hang
    cannot qualify). Their failure modes (state object, validation, first dispatches) show in the first frames. */
 if(g_cov_live&&g_vframes>=URT_SG_PT_FRAMES){unsigned c=0;g_cov_live=0;
  for(unsigned k=0;k<URT_SG_ROWS;k++)if(asi_row(kGuarded[k]))c+=(unsigned)urt_sg_prove_key(&g_sg,urt_gx_defs[kGuarded[k]].key);
  if(c)cr01_release_log_event(sg_save()?"URT: startup guard: PT shader coverage options qualified: URT's variant rendered 300 Path Tracing frames (about 5 s); recorded as safe\r\n":
                                        "URT: startup guard: safe state could not be recorded (file not writable)\r\n");
  g_sg_live=urt_sg_pending(&g_sg);}
 /* RT engine restart rows (Trace Mode, Async GI Classification) become "proven" after URT_SG_STABLE_MS (1 minute) of
    ray-traced rendering: their recorded failures happened when scenes loaded (R4: about 15 s in), so a few seconds is too short */
 if(g_sg_live){
  if(mode_valid&&mode){if(g_sg_last){uint32_t d=now_ms-g_sg_last;g_sg_rt_ms+=d>2000u?2000u:d;}g_sg_last=now_ms;}
  else g_sg_last=0;
  if(g_sg_rt_ms>=URT_SG_STABLE_MS){   /* R12 FIX4: RT engine rows only; a coverage row is proven only by frames rendered with it */
   unsigned c2=0;for(unsigned k=0;k<URT_SG_ROWS;k++)if(!asi_row(kGuarded[k]))c2+=(unsigned)urt_sg_prove_key(&g_sg,urt_gx_defs[kGuarded[k]].key);
   g_sg_live=0;
   if(c2)cr01_release_log_event(sg_save()?"URT: startup guard: the RT restart settings under test ran 1 minute of ray-traced rendering; recorded as stable\r\n":
                                    "URT: startup guard: stable state could not be recorded (file not writable)\r\n");}
 }
 urt_fg501_tick(g_valid&&master&&g_prefs.v[URT_GX_IX_FGMENURESOURCES]==1u);
 /* R11: the R7 PT code limits (urt_ptcode501.c) are retired and no longer linked */
 {
  /* read-only diagnostic, once: is the game's PT radiance cache (SHaRC, PTUseSHARC) on in this configuration? */
  if(mode_valid&&mode==2u&&!g_ptdiag&&g_known&&g_base){g_ptdiag=1;
   cr01_release_log_event(__atomic_load_n(g_base+urt_rva(0x5149b40u),__ATOMIC_ACQUIRE)?
    "URT: diagnostic: the game's PT radiance cache (SHaRC) is on\r\n":
    "URT: diagnostic: the game's PT radiance cache (SHaRC) is off in this configuration (PT: Radiance Cache Density has no effect)\r\n");}
  /* R9 runtime proof, once per session: while SHaRC is on in Path Tracing, did the game allocate the cache (= its SHaRC
   * dispatch path ran)? Checked on each PT tick for up to 60 s of PT rendering. */
  if(mode_valid&&mode==2u&&!g_shdiag&&g_known&&g_base&&__atomic_load_n(g_base+urt_rva(0x5149b40u),__ATOMIC_ACQUIRE)){
   const uint32_t ent=sharc_entries();
   if(g_sh_last){uint32_t d=now_ms-g_sh_last;g_sh_ms+=d>2000u?2000u:d;}g_sh_last=now_ms;
   if(ent){g_shdiag=1;
    char b[300];unsigned n=0,k=0;char dg[12];uint32_t e=ent;
    do{dg[k++]=(char)('0'+e%10u);e/=10u;}while(e&&k<11u);
    for(const char*q="URT: PT radiance cache (SHaRC) active: the game allocated it (";*q;q++)b[n++]=*q;
    while(k)b[n++]=dg[--k];
    for(const char*q=" entries) and renders Path Tracing with it; URT's PT: Geometry Bounces, Hair Bounces and Indirect-Light Compensation do not apply while it is on\r\n";*q&&n<298u;q++)b[n++]=*q;
    b[n]=0;cr01_release_log_event(b);}
   else if(g_sh_ms>=60000u){g_shdiag=1;
    cr01_release_log_event("URT: PT radiance cache (SHaRC) is on but the game did not allocate it after 60 s of Path Tracing (not used this session; normal Path Tracing continues)\r\n");}
  }
  if(!(mode_valid&&mode==2u))g_sh_last=0;}
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
 /* V5.3.0 rev2 fix (log accuracy only): before ray tracing initialises, the AS configuration word holds its static
  * image value 128 (Patch 5.01 .data), not 0; the AS-config routine (0x1ee97d0) is its only writer. */
 const uint32_t pre=g_known&&g_base?__atomic_load_n((uint32_t*)(void*)(g_base+urt_rva(0x405af14u)),__ATOMIC_ACQUIRE):1u;
 const int early=g_known&&g_base&&(pre==0u||pre==128u);
 /* V5.3.0 rev4: boot rows are decided once, here, before any renderer exists. RT Animated Geometry Trace Mode
  * changes the build flags of every animated BLAS; the engine recomputes those flags from its AS configuration
  * on every build AND refit (0x5106f0/0x5107f0), so the value must be in place before the first BLAS and must
  * never change during the session: it is applied only when ray tracing has not initialised yet. */
 UrtGxPrefs p=g_prefs;
 if(!g_owner.boot_done&&master&&urt_gx_prefs_valid(&p)){   /* once per session, with the boot step */
  g_sg_live=0;g_sg_rt_ms=0;g_sg_last=0;g_cov_live=0;g_vframes=0;
  if(p.v[URT_GX_IX_RTASFASTTRACE]&&!early){
   p.v[URT_GX_IX_RTASFASTTRACE]=0;
   cr01_release_log_event("URT: RT Animated Geometry Trace Mode not applied this session: ray tracing had already initialised (applies from the next start)\r\n");
  }
  char t[1024];int n=g_sg_read?g_sg_read(t,sizeof t-1u):-1;
  urt_sg_parse(t,n>0?(unsigned)n:0u,&g_sg);
  /* R5: records of rows that are no longer guarded (e.g. the removed async compaction) are dropped */
  for(unsigned j=0;j<g_sg.n;){int keep=0;for(unsigned k=0;k<URT_SG_ROWS;k++)if(urt_sg_key_is(&g_sg,j,urt_gx_defs[kGuarded[k]].key))keep=1;
   if(keep)j++;else urt_sg_drop_at(&g_sg,j);}
  /* R11/R12 FIX2: no ASI-load decision this session (late attach: no PT shader preparation). A coverage record the last
   * session left PENDING / RETRYING (no clean exit, not proven) is judged here exactly as at ASI load. */
  if(!g_bc_decided)for(unsigned j=0;j<g_sg.n;j++)
   if(urt_sg_key_is(&g_sg,j,urt_gx_defs[URT_GX_IX_PTDIFFUSECOVERAGE].key)||urt_sg_key_is(&g_sg,j,urt_gx_defs[URT_GX_IX_EXPPTLOCALSHADOWCOVERAGE].key)||
      urt_sg_key_is(&g_sg,j,urt_gx_defs[URT_GX_IX_EXPPTSUNSHADOWCOVERAGE].key)){
    if(g_sg.e[j].state==URT_SG_PENDING){g_sg.e[j].state=URT_SG_RETRY;
     log_kv("URT: startup guard: ",g_sg.e[j].key,g_sg.e[j].value,urt_sg_event_text(URT_SG_EV_SUSPECTED));}
    else if(g_sg.e[j].state==URT_SG_RETRYING){g_sg.e[j].state=URT_SG_FAILED;
     log_kv("URT: startup guard: ",g_sg.e[j].key,g_sg.e[j].value,urt_sg_event_text(URT_SG_EV_FAILED_NOW));}}
  unsigned want=0,blocked=0,had=g_sg.n;
  for(unsigned k=0;k<URT_SG_ROWS;k++){unsigned i=kGuarded[k];
   if(asi_row(i))continue;                         /* R11/R12: decided (and recorded) at ASI load, see above */
   if(p.v[i]==0u&&!(i==URT_GX_IX_RTASFASTTRACE&&g_prefs.v[i])){int nf;(void)urt_sg_decide(&g_sg,urt_gx_defs[i].key,0,&nf);continue;}
   if(!p.v[i])continue;                         /* trace mode deferred above: its record is left as it is */
   int ev=0;
   const int go=urt_sg_decide_ex(&g_sg,urt_gx_defs[i].key,p.v[i],&ev);
   log_kv("URT: startup guard: ",urt_gx_defs[i].key,p.v[i],urt_sg_event_text(ev));   /* R12 FIX2: every decision is logged */
   if(go){want++;urt_sg_session_note(urt_gx_defs[i].key);continue;}
   p.v[i]=0;blocked++;
  }
  if((want||blocked||had||g_sg.n||n>0)&&!sg_save()){
   for(unsigned k=0;k<URT_SG_ROWS;k++)p.v[kGuarded[k]]=0;want=0;
   cr01_release_log_event("URT: startup guard file could not be written; the restart settings under test stay native this session\r\n");
  }
  g_sg_live=urt_sg_pending(&g_sg);
  {g_cov_live=0;for(unsigned k=0;k<URT_SG_ROWS;k++){const int st=urt_sg_state(&g_sg,urt_gx_defs[kGuarded[k]].key);
    if(asi_row(kGuarded[k])&&(st==URT_SG_PENDING||st==URT_SG_RETRYING))g_cov_live=1u;}}
 }
 if(urt_gx_boot_apply(&g_owner,&p,master)){
  cr01_release_log_event("URT: async compute settings applied for this session (changes apply at the next game start)\r\n");
  for(unsigned i=0;i<URT_GX_COUNT;i++)if(urt_gx_is_boot(i)&&g_owner.owned[i])
   log_kv("URT: startup override applied: ",urt_gx_defs[i].key,p.v[i],"\r\n");
 }
 unsigned want=0;
 for(unsigned i=0;i<URT_GX_COUNT;i++)if(urt_gx_is_startup(i)&&g_prefs.v[i])want++;
 if(!want)return;
 urt_gx_update(&g_owner,&g_prefs,master,0u,0u,now_ms);
 cr01_release_log_event(!g_known?"URT: RT memory settings applied at startup (unknown executable: capture point not verified)\r\n":
  early?"URT: RT memory settings applied before ray tracing initialised\r\n":
        "URT: RT memory settings applied after ray tracing initialised; they take effect from the next game start\r\n");
}
/* Read-only diagnostic, once per session after the first renderer tick: does the engine's AS configuration
 * hold the values URT applied? (Patch 5.01 only; never written.) */
static void as_diagnostic(void){
 if(g_asdiag||!g_known||!g_base)return;
 unsigned d=URT_GX_IX_RTASDEFRAG,b=URT_GX_IX_RTASBATCH,ft=URT_GX_IX_RTASFASTTRACE;
 if(!g_owner.owned[d]&&!g_owner.owned[b]&&!g_owner.owned[ft])return;
 const uint32_t cd=__atomic_load_n((uint32_t*)(void*)(g_base+urt_rva(0x405af10u)),__ATOMIC_ACQUIRE);
 const uint32_t cb=__atomic_load_n((uint32_t*)(void*)(g_base+urt_rva(0x405af14u)),__ATOMIC_ACQUIRE);
 /* V5.3.0 rev2: RTConfigFastTraceDynamic is copied to the byte at 0x405af23 by the same AS-config routine */
 const uint8_t cf=__atomic_load_n((uint8_t*)(void*)(g_base+urt_rva(0x405af23u)),__ATOMIC_ACQUIRE);
 if(!cb)return;                                   /* ray tracing not initialised yet: check again later */
 g_asdiag=1;
 const int ok=(!g_owner.owned[d]||cd==g_owner.applied[d])&&(!g_owner.owned[b]||cb==g_owner.applied[b])&&
              (!g_owner.owned[ft]||cf==(uint8_t)g_owner.applied[ft]);
 cr01_release_log_event(ok?"URT: RT acceleration-structure manager is using URT's RT memory settings\r\n":
  "URT: RT acceleration-structure manager started with other RT memory values; URT's values apply from the next game start\r\n");
}
void urt_gfx501_shutdown(void){
 if(!g_ready)return;
 urt_gx_release_all(&g_owner);urt_fg501_shutdown();
}
