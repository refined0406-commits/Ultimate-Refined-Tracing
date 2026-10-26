/* V5.2 selected controls: pure ownership logic. See urt_gfx_core.h. */
#include "urt_gfx_core.h"

int urt_gx_prefs_valid(const UrtGxPrefs *p){
  if(!p)return 0;
  for(unsigned i=0;i<URT_GX_COUNT;i++)if(p->v[i]>urt_gx_defs[i].nvals)return 0;
  return 1;
}
int urt_gx_parse_index(const char *s, unsigned row, uint8_t *out){
  if(!s||!out||row>=URT_GX_COUNT)return 0;
  if(!s[0]){*out=0;return 1;}                       /* game prunes defaults: missing = Native */
  unsigned v=0,n=0;
  for(;s[n];n++){if(n>=3||s[n]<'0'||s[n]>'9')return 0;v=v*10u+(unsigned)(s[n]-'0');}
  if(v>urt_gx_defs[row].nvals)return 0;
  *out=(uint8_t)v;return 1;
}
int urt_gx_row_saved(unsigned row){
  return row<URT_GX_COUNT&&urt_gx_defs[row].special==URT_GX_SPECIAL_NONE&&(urt_gx_defs[row].flags&0x100u)!=0;
}
int urt_gx_in_scope(unsigned row, unsigned mode){
  if(row>=URT_GX_COUNT||mode>2u)return 0;
  switch(urt_gx_defs[row].scope){
  case URT_GX_ALL: return 1;
  case URT_GX_RT: return mode==1u;
  case URT_GX_PT: return mode==2u;
  case URT_GX_RTPT: return mode==1u||mode==2u;
  default: return 0;
  }
}
static int plausible(unsigned row, uint32_t native){
  if(urt_gx_defs[row].kind==URT_GX_FLOAT)return (native&0x7f800000u)!=0x7f800000u;   /* finite */
  if(urt_gx_defs[row].kind==URT_GX_BOOL)return native<=1u;
  return 1;
}
int urt_gx_target(const UrtGxPrefs *p, unsigned row, uint32_t native, uint32_t *out){
  if(!p||!out||row>=URT_GX_COUNT)return 0;
  const UrtGxDef *d=&urt_gx_defs[row];
  if(d->special!=URT_GX_SPECIAL_NONE)return 0;
  unsigned i=p->v[row];
  if(!i||i>d->nvals)return 0;
  if(!plausible(row,native))return 0;
  *out=d->vals[i-1];
  return 1;
}
static uint32_t load(const UrtGxOwner *o, unsigned i){
  if(urt_gx_defs[i].kind==URT_GX_BOOL)return __atomic_load_n(o->slot[i],__ATOMIC_ACQUIRE);
  return __atomic_load_n((uint32_t*)(void*)o->slot[i],__ATOMIC_ACQUIRE);
}
static int cas(UrtGxOwner *o, unsigned i, uint32_t expect, uint32_t want){
  if(urt_gx_defs[i].kind==URT_GX_BOOL){
    uint8_t e=(uint8_t)expect;
    return __atomic_compare_exchange_n(o->slot[i],&e,(uint8_t)want,0,__ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE);
  }
  return __atomic_compare_exchange_n((uint32_t*)(void*)o->slot[i],&expect,want,0,__ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE);
}
void urt_gx_init(UrtGxOwner *o){
  if(!o)return;
  volatile uint8_t *b=(volatile uint8_t*)o;for(unsigned long k=0;k<sizeof *o;k++)b[k]=0;
}
/* ---- save-bit detach / reattach (saved rows only) ---- */
static int detach(UrtGxOwner *o, unsigned i, uint32_t now){
  if(o->sb_state[i]==URT_GX_SB_DETACHED)return 1;
  if(!o->savebyte[i])return 0;
  uint8_t cur=__atomic_load_n(o->savebyte[i],__ATOMIC_ACQUIRE);
  if(!(cur&1u)){o->sb_conflicts++;return 0;}          /* not in the expected state: never own */
  uint8_t e=cur;
  if(!__atomic_compare_exchange_n(o->savebyte[i],&e,(uint8_t)(cur&~1u),0,__ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE)){
    o->sb_conflicts++;return 0;
  }
  o->sb_orig[i]=cur;o->sb_state[i]=URT_GX_SB_DETACHED;o->sb_t0[i]=now;o->detaches++;
  return 1;
}
static void reattach(UrtGxOwner *o, unsigned i){
  if(o->sb_state[i]!=URT_GX_SB_DETACHED)return;
  uint8_t e=(uint8_t)(o->sb_orig[i]&~1u);
  if(!__atomic_compare_exchange_n(o->savebyte[i],&e,o->sb_orig[i],0,__ATOMIC_ACQ_REL,__ATOMIC_ACQUIRE))
    o->sb_conflicts++;                                  /* someone else changed the flags: leave them */
  else o->reattaches++;
  o->sb_state[i]=URT_GX_SB_ATTACHED;
}
static void release_one(UrtGxOwner *o, unsigned i){
  if(o->owned[i]){
    if(!o->slot[i]||!cas(o,i,o->applied[i],o->original[i]))o->conflicts++;
    o->owned[i]=0;
  }
  reattach(o,i);                                        /* value first, then the save bit */
}
void urt_gx_release_all(UrtGxOwner *o){
  if(!o)return;
  for(unsigned i=0;i<URT_GX_COUNT;i++)release_one(o,i);
}
void urt_gx_settings_saved(UrtGxOwner *o){
  if(!o)return;
  for(unsigned i=0;i<URT_GX_COUNT;i++)o->await_save[i]=0;
}
unsigned urt_gx_owned_count(const UrtGxOwner *o){
  unsigned n=0;if(!o)return 0;
  for(unsigned i=0;i<URT_GX_COUNT;i++)n+=o->owned[i]?1u:0u;
  return n;
}
unsigned urt_gx_detached_count(const UrtGxOwner *o){
  unsigned n=0;if(!o)return 0;
  for(unsigned i=0;i<URT_GX_COUNT;i++)n+=o->sb_state[i]==URT_GX_SB_DETACHED?1u:0u;
  return n;
}
static void foreign_write(UrtGxOwner *o, unsigned i, const UrtGxPrefs *p, uint32_t now){
  o->owned[i]=0;o->conflicts++;
  reattach(o,i);                     /* the game's (or another mod's) value may be saved again */
  if(urt_gx_row_saved(i))o->await_save[i]=1;   /* RC7: let that value reach the file before URT takes the row again */
  if(!o->fights[i]||now-o->fight_t0[i]>URT_GX_FIGHT_WINDOW_MS){o->fights[i]=0;o->fight_t0[i]=now;}
  if(++o->fights[i]>=URT_GX_FIGHT_LIMIT){
    o->yielded[i]=1;o->yield_choice[i]=p->v[i];o->fights[i]=0;o->yields++;
  }
}
int urt_gx_is_startup(unsigned row){
  return row<URT_GX_COUNT&&urt_gx_defs[row].special==URT_GX_SPECIAL_NONE&&urt_gx_defs[row].stage==URT_GX_STAGE_STARTUP;
}
void urt_gx_update(UrtGxOwner *o, const UrtGxPrefs *p, unsigned master, unsigned mode_valid,
                   unsigned mode, uint32_t now_ms){
  if(!o)return;
  if(!p||!master||!urt_gx_prefs_valid(p)){urt_gx_release_all(o);return;}
  /* V5.2 rows: no valid renderer mode = everything released (unchanged). V5.3 startup rows are read by the
   * engine once, when ray tracing initialises, i.e. before a renderer mode exists; they stay owned while
   * Master is On and are released by Master Off / Native exactly like every other row. */
  for(unsigned i=0;i<URT_GX_COUNT;i++){
    if(!o->slot[i]||urt_gx_defs[i].special!=URT_GX_SPECIAL_NONE)continue;
    if(!mode_valid&&!urt_gx_is_startup(i)){release_one(o,i);continue;}   /* exactly the V5.2 release_all path */
    const int saved=urt_gx_row_saved(i);
    /* a yielded field comes back only when the user picks a different value */
    if(o->yielded[i]){if(p->v[i]==o->yield_choice[i]){reattach(o,i);continue;}o->yielded[i]=0;o->fights[i]=0;}
    if(o->await_save[i]&&!o->owned[i]){reattach(o,i);continue;}
    const int want=urt_gx_is_startup(i)?1:urt_gx_in_scope(i,mode);
    if(o->owned[i]){
      if(load(o,i)!=o->applied[i]){foreign_write(o,i,p,now_ms);continue;}
      uint32_t t;
      if(!want||!urt_gx_target(p,i,o->original[i],&t)){release_one(o,i);continue;}
      if(t==o->applied[i])continue;
      if(cas(o,i,o->applied[i],t)){o->applied[i]=t;o->writes++;}
      else foreign_write(o,i,p,now_ms);
      continue;
    }
    uint32_t native=load(o,i),t;
    if(!want||!urt_gx_target(p,i,native,&t)){reattach(o,i);continue;}
    if(saved){
      /* never write a saved variable before its save bit has been cleared for a while */
      if(o->sb_state[i]!=URT_GX_SB_DETACHED){(void)detach(o,i,now_ms);continue;}
      if(now_ms-o->sb_t0[i]<URT_GX_DETACH_SETTLE_MS)continue;
    }
    if(t==native){o->original[i]=native;o->applied[i]=t;o->owned[i]=1;continue;}  /* same value: own without writing */
    if(cas(o,i,native,t)){o->original[i]=native;o->applied[i]=t;o->owned[i]=1;o->writes++;}
    else reattach(o,i);
  }
}
