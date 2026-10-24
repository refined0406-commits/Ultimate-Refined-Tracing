#ifndef CR01_PROD_EARLY501_H
#define CR01_PROD_EARLY501_H
#include <stdint.h>
/* Read-only production compatibility and native-owner preflight. The caller is
 * the synchronous first DirectInput8Create bootstrap, never DllMain. */
int cr01_prod_early501_enter(void);
int cr01_pre_registration_quiescence_approved(void);
void cr01_prod_early501_exit(void);
/* Pure negative fixture: if either protected native DXR owner has appeared,
 * or a protected code signature has changed, authorization is impossible. */
int cr01_prod_early501_eligibility(int first_directinput,uint64_t create_owner,
 uint64_t add_owner,int registration_sites_exact,int native_blob_exact,
 int serializer_site_exact);
#endif
