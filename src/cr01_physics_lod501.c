/* Refined Tracing PRIVATE PLOD40. Patch 5.01 native Physics/LODBudget.
 * Competitor PT Optimization 2.2: writes 5.0f while enabled, restores the
 * game's own registered default while disabled. EXACT FNV1a hash
 * Physics/LODBudget = 0xBAB96F8B matches the competitor's setting descriptor.
 * Game .data registration: current pointer RVA 0x512EE58, default 25.0f.
 * No scene/ragdoll/body hooks, no code patch, no physics-thread callback.
 */
#include "cr01_physics_lod501.h"
#include "cr01_contract.h"
#include <stdint.h>
static CR01Owner g_owner;
static uint32_t *g_slot;
static unsigned g_ready, g_fault;
#define NATIVE_25 0x41c80000u
#define OPT_5 0x40a00000u
static int finite_positive_budget(uint32_t u){
 unsigned exp=(u>>23)&255u;
 return !(u>>31) && exp>0u && exp<255u && u>=0x3dcccccdU && u<=0x447a0000U;
}
int cr01_physics501_init(uint32_t *slot,uint32_t *distance_slot){
 if(g_ready || !slot || !distance_slot || slot==distance_slot ||
    ((uintptr_t)slot&3u) || ((uintptr_t)distance_slot&3u)) return 0;
 if(!finite_positive_budget(__atomic_load_n(slot,__ATOMIC_ACQUIRE))) return 0;
 /* Do not touch Physics/LODDistanceLimit. Its presence confirms that we
  * resolved both distinct native physics registration names. */
 if(!finite_positive_budget(__atomic_load_n(distance_slot,__ATOMIC_ACQUIRE)))return 0;
 g_slot=slot;g_ready=1;g_fault=0;g_owner=(CR01Owner){0};return 1;
}
int cr01_physics501_refresh(unsigned master,unsigned pref_on){
 if(!g_ready||!g_slot||master>1u||pref_on>1u)return -1;
 if(g_fault)return -1;
 uint32_t now=__atomic_load_n(g_slot,__ATOMIC_ACQUIRE);
 if(g_owner.owned && now!=g_owner.installed){
  g_fault=1; (void)cr01_release_owner(&g_owner); return -1;
 }
 if(!master||!pref_on){
  if(!cr01_release_owner(&g_owner)){g_fault=1;return -1;}
  return 0;
 }
 if(!finite_positive_budget(now)){g_fault=1;return -1;}
 if(now==OPT_5 && !g_owner.owned)return 2; /* already game/other owner */
 if(!cr01_write_owner(&g_owner,g_slot,OPT_5)){g_fault=1;return -1;}
 return g_owner.owned?1:2;
}
int cr01_physics501_shutdown(void){
 int ok=cr01_release_owner(&g_owner);
 if(!ok||g_owner.conflicted){g_fault=1;return 0;}
 g_ready=0;g_slot=0;g_fault=0;g_owner=(CR01Owner){0};return 1;
}
