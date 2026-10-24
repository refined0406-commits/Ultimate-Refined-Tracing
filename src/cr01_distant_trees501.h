#ifndef CR01_DISTANT_TREES501_H
#define CR01_DISTANT_TREES501_H
#include <stdint.h>
int cr01_dt501_init(uint32_t *proximity_slot,uint32_t *global_billboard_slot);
int cr01_dt501_release(void);
/* Returns 3 when PT Billboards On suspends DT without changing saved values.
 * Mode: 0 Raster, 1 Normal RT, 2 PT. Billboards: 0 native, 1 On, 2 Off. */
int cr01_dt501_refresh(unsigned mode,unsigned master,unsigned pref_on,unsigned pt_billboards);
int cr01_dt501_shutdown(void);
#endif
