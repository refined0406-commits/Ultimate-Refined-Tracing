#ifndef CR01_SETTINGS_CODEC_H
#define CR01_SETTINGS_CODEC_H
#include "cr01_contract.h"
/* Deterministic private settings wire format; codec is pure and never reads
 * game memory, disk, or another mod's configuration. All-or-nothing decode. */
int cr01_settings_serialize(const CR01Settings *s,char *dst,size_t cap,size_t *written);
int cr01_settings_parse(const char *buf,size_t len,CR01Settings *out);
/* Old CR01 private-test 1.51..2.55 is clamped ONCE to 1.50.
 * No other malformed input is repaired or partially applied. */
int cr01_settings_parse_migrated(const char *buf,size_t len,CR01Settings *out,int *migrated);
#endif
