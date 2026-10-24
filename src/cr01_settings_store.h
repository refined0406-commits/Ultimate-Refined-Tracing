#ifndef CR01_SETTINGS_STORE_H
#define CR01_SETTINGS_STORE_H
#include "cr01_menu_bridge.h"
/* Host supplies namespace-scoped file operations. No Witcher/other-mod file
 * is read, truncated, or rewritten by the CR01 policy layer. */
typedef struct {
 /* Returns 1 for loaded data, 0 for missing file, -1 for I/O failure.
  * Length must include every byte, including any malformed trailing data. */
 int (*read_private)(void*,char*,size_t,size_t*);
 /* Transactional: return 1 ONLY after a durable atomic commit. */
 int (*commit_private)(void*,const char*,size_t);
 void*ctx;
 /* V5 optional recovery hooks (may be NULL). read_backup reads the last
  * known-good copy; quarantine moves a primary file aside ONLY when its bytes
  * were read successfully but are invalid (never on transient I/O errors). */
 int (*read_backup)(void*,char*,size_t,size_t*);
 int (*quarantine)(void*);
} CR01PrivateStore;
/* 1=success, 0=bad/corrupt/unavailable. Defaults remain Master Off. */
int cr01_store_load(CR01MenuBridge*,const CR01PrivateStore*);
int cr01_store_flush(CR01MenuBridge*,const CR01PrivateStore*);
/* V5: never leaves the runtime passive because of the private file.
 * Returns the source used: 0 primary (or new install), 1 backup, 2 defaults.
 * Sources 1 and 2 mark the bridge dirty so the next flush rewrites the file. */
int cr01_store_load_resilient(CR01MenuBridge*,const CR01PrivateStore*);
#endif
