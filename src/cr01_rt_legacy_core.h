#ifndef CR01_RT_LEGACY_CORE_H
#define CR01_RT_LEGACY_CORE_H
#include <stdint.h>
#define CR01_RT_LEGACY_FIELDS 6u
#define CR01_RT_L_PRIMARY 0u
#define CR01_RT_L_SECONDARY 1u
#define CR01_RT_L_CAP 2u
#define CR01_RT_L_AO 3u
#define CR01_RT_L_BUDGET 4u
#define CR01_RT_L_SHADING 5u
/* Zero is Native Auto for EVERY user option, never an optimization. */
typedef struct { uint32_t values[CR01_RT_LEGACY_FIELDS]; } CR01RtLegacyProfile;
typedef struct {
 /* 0=Off, 1=legacy On (native-preset-selected; migration until user chooses),
  * 2=explicit Quality, 3=explicit Performance. Never reinterpret old saved 1. */
 uint32_t enabled; CR01RtLegacyProfile profile[2]; uint32_t revision;
} CR01RtLegacyPreferences;
typedef struct {
 uint32_t*slot[CR01_RT_LEGACY_FIELDS];
 uint32_t original[CR01_RT_LEGACY_FIELDS];
 uint32_t applied[CR01_RT_LEGACY_FIELDS];
 unsigned mask,profile,native_preset; uint32_t revision,generation;
 unsigned active,conflict;
} CR01RtLegacy;
void cr01_rt_legacy_core_init(CR01RtLegacy*);
int cr01_rt_legacy_profile_valid(const CR01RtLegacyProfile*);
/* Always clear owned mask, including foreign-write CAS conflicts. Never force-clobber. */
void cr01_rt_legacy_release(CR01RtLegacy*);
/* All slots must come from verified Patch 5.01 descriptors, and shading rays
 * must be verified native 32 before this function is called. */
int cr01_rt_legacy_update(CR01RtLegacy*,const CR01RtLegacyPreferences*,
 unsigned normal_rt,unsigned master,unsigned preset,unsigned native_shading32);
#endif
