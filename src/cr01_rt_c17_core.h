#ifndef CR01_RT_C17_CORE_H
#define CR01_RT_C17_CORE_H
#include <stdint.h>
/* Independently owned C17 GlobalGrid / Grid1 shading caps.
 * Never expand or reindex the six accepted C16 fields. */
#define CR01_C17_CAP_FIELDS 2u
#define CR01_C17_GLOBAL 0u
#define CR01_C17_GRID1 1u
typedef struct {uint32_t values[CR01_C17_CAP_FIELDS];} CR01C17CapsProfile;
typedef struct {
 uint32_t *slot[CR01_C17_CAP_FIELDS];
 uint32_t original[CR01_C17_CAP_FIELDS],applied[CR01_C17_CAP_FIELDS];
 unsigned mask,active,conflict,preset;
 uint32_t parent_revision,local_revision;
} CR01C17CapsOwner;
void cr01_c17_caps_init(CR01C17CapsOwner*);
int cr01_c17_caps_profile_valid(const CR01C17CapsProfile*);
void cr01_c17_caps_release(CR01C17CapsOwner*);
/* eligibility is delegated to the accepted C16 owner result: it must be
 * actively in Normal RT + Master On + parent On + verified preset. */
int cr01_c17_caps_update(CR01C17CapsOwner*,const CR01C17CapsProfile*,
 unsigned eligible,unsigned preset,uint32_t parent_revision,uint32_t local_revision);
#endif
