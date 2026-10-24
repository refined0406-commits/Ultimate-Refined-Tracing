#include "cr01_shadow_policy.h"
int cr01_shadow_policy(const CR01Settings*settings,unsigned pt,uint32_t native,
                       unsigned*force_off,uint32_t*effective){
 if(!settings||pt>1||native>3u||!force_off||!effective||
    settings->master>1||settings->v[F_SHADOWS]>2u)return 0;
 unsigned override=(unsigned)(settings->master&&pt&&settings->v[F_SHADOWS]);
 *force_off=(override&&settings->v[F_SHADOWS]==2u)?1u:0u;
 *effective=override?(settings->v[F_SHADOWS]==1u?2u:0u):native;
 return 1;
}
