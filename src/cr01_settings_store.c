#include "cr01_settings_store.h"
#include "cr01_settings_codec.h"
int cr01_store_load(CR01MenuBridge*b,const CR01PrivateStore*io){
 static char buf[4097];size_t got=0;int result;
 if(!b||!io||!io->read_private||!b->controller.initialized)return 0;
 result=io->read_private(io->ctx,buf,sizeof(buf),&got);
 if(result==0)return 1; /* New install: leave Master OFF and defaults. */
 if(result!=1||!got||got>4096)return 0;
 CR01Settings parsed;int migrated=0;
 if(!cr01_settings_parse_migrated(buf,got,&parsed,&migrated))return 0; /* No partial apply. */
 if(!cr01_native_set_saved(&b->controller,&parsed))return 0;
 /* Pending shader carriers may remain inactive; loading preferences is not
  * proof of renderer activation. */
 if(!cr01_native_event(&b->controller,b->controller.mode) &&
    !b->controller.pending_mask)return 0;
 /* Migration must be durably saved by the normal atomic flush. If the
    commit fails, the dirty generation remains pending for a later retry. */
 if(migrated)b->dirty_generation++;
 else b->save_ack=b->dirty_generation;
 return 1;
}
int cr01_store_flush(CR01MenuBridge*b,const CR01PrivateStore*io){
 char buf[512];size_t used=0;
 if(!b||!io||!io->commit_private||!b->controller.initialized)return 0;
 if(b->dirty_generation==b->save_ack)return 1;
 if(!cr01_menu_bridge_save(b,buf,sizeof(buf),&used)||!used)return 0;
 if(!io->commit_private(io->ctx,buf,used))return 0;
 b->save_ack=b->dirty_generation;
 return 1;
}

/* A missing backup is "unavailable", never a fresh-install success. */
static const CR01PrivateStore*g_backup_io;
static int read_backup_strict(void*ctx,char*buf,size_t cap,size_t*out){
 int r=g_backup_io->read_backup(ctx,buf,cap,out);
 return r==0?-1:r;
}
int cr01_store_load_resilient(CR01MenuBridge*b,const CR01PrivateStore*io){
 if(!b||!io)return 2;
 if(cr01_store_load(b,io))return 0;
 if(io->read_backup){
  CR01PrivateStore bak={read_backup_strict,io->commit_private,io->ctx,0,0};
  g_backup_io=io;
  cr01_menu_bridge_init(b);
  if(cr01_store_load(b,&bak)){
   if(io->quarantine)(void)io->quarantine(io->ctx);
   b->dirty_generation=b->save_ack+1u;
   return 1;
  }
 }
 cr01_menu_bridge_init(b);  /* defaults: Master Off until dx12user.settings overlays */
 if(io->quarantine)(void)io->quarantine(io->ctx);
 b->dirty_generation=b->save_ack+1u;
 return 2;
}
