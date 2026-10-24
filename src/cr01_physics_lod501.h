#ifndef CR01_PHYSICS_LOD501_H
#define CR01_PHYSICS_LOD501_H
#include <stdint.h>
/* Existing engine Physics/LODBudget; Patch 5.01 exact native registration.
 * 0=Off/native, 1=On (budget 5), 2=On/no change already 5, -1=refused. */
int cr01_physics501_init(uint32_t *slot, uint32_t *distance_slot);
int cr01_physics501_refresh(unsigned master,unsigned pref_on);
int cr01_physics501_shutdown(void);
#endif
