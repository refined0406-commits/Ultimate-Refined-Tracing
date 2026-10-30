/* Startup guard pure logic (src/urt_startup_guard.c). R12 FIX2: session-state design - a clean exit is never a crash;
 * an interrupted session (no clean exit, no proof) skips one start, retries once, then turns the value off. */
#include "../../src/urt_startup_guard.h"
#include <stdio.h>
#include <string.h>
static int fails;
#define CHECK(c,m) do{ if(c) printf("PASS %s\n",m); else {printf("FAIL %s\n",m); fails++;} }while(0)
static void cycle(UrtSgFile*f){char t[1024];unsigned n=urt_sg_format(f,t,sizeof t);urt_sg_parse(t,n,f);}
static const char*const FT[]={"URTX_RTASFastTrace"};
int main(void){
  UrtSgFile f;int ev;
  urt_sg_parse("",0,&f);
  CHECK(urt_sg_decide_ex(&f,"URTX_RTASFastTrace",1,&ev)&&ev==URT_SG_EV_NEW&&f.n==1&&f.e[0].state==URT_SG_PENDING,"new value: applied and recorded PENDING before it takes effect");
  cycle(&f);
  CHECK(f.n==1&&f.e[0].value==1&&f.e[0].state==URT_SG_PENDING,"file round trip keeps the record");
  /* 1. clean exit after a few seconds (quit / restart): never a crash */
  CHECK(urt_sg_mark_clean(&f,FT,1)==1&&f.e[0].state==URT_SG_CLEAN,"clean exit: this session's PENDING record becomes CLEAN");
  cycle(&f);
  CHECK(urt_sg_decide_ex(&f,"URTX_RTASFastTrace",1,&ev)&&ev==URT_SG_EV_CLEAN&&f.e[0].state==URT_SG_PENDING,"next start after a clean exit: applied immediately (however short the session)");
  /* 2. a session that ends without a clean exit and before the proof: suspected crash */
  cycle(&f);
  CHECK(!urt_sg_decide_ex(&f,"URTX_RTASFastTrace",1,&ev)&&ev==URT_SG_EV_SUSPECTED&&f.e[0].state==URT_SG_RETRY,"interrupted session: this start does NOT apply it (normal behaviour), record RETRY");
  CHECK(urt_sg_mark_clean(&f,FT,1)==0&&f.e[0].state==URT_SG_RETRY,"the skipped start's own clean exit does not clear the suspicion");
  cycle(&f);
  CHECK(urt_sg_decide_ex(&f,"URTX_RTASFastTrace",1,&ev)&&ev==URT_SG_EV_RETRYING&&f.e[0].state==URT_SG_RETRYING,"next start retries it once (RETRYING)");
  /* 2a. the retry ends cleanly: back to normal */
  {UrtSgFile g=f;CHECK(urt_sg_mark_clean(&g,FT,1)==1&&urt_sg_decide_ex(&g,"URTX_RTASFastTrace",1,&ev)&&ev==URT_SG_EV_CLEAN,"a retry that ends cleanly clears the suspicion");}
  /* 2b. the retry is interrupted too: off, no loop */
  cycle(&f);
  CHECK(!urt_sg_decide_ex(&f,"URTX_RTASFastTrace",1,&ev)&&ev==URT_SG_EV_FAILED_NOW&&f.e[0].state==URT_SG_FAILED,"retry interrupted as well: turned OFF (at most two interrupted sessions)");
  cycle(&f);
  CHECK(!urt_sg_decide_ex(&f,"URTX_RTASFastTrace",1,&ev)&&ev==URT_SG_EV_BLOCKED,"FAILED stays off while the same value is kept (no crash loop)");
  CHECK(urt_sg_mark_clean(&f,FT,1)==0&&f.e[0].state==URT_SG_FAILED,"a clean exit never re-enables a FAILED value");
  CHECK(!urt_sg_decide_ex(&f,"URTX_RTASFastTrace",0,&ev)&&f.n==0,"Native / Auto for one start clears the record (retry method)");
  CHECK(urt_sg_decide_ex(&f,"URTX_RTASFastTrace",1,&ev)&&ev==URT_SG_EV_NEW,"choosing it again starts fresh");
  /* 3. proof */
  CHECK(urt_sg_prove_all(&f)==1&&f.e[0].state==URT_SG_PROVEN,"1 minute of ray-traced rendering: PROVEN");
  cycle(&f);
  CHECK(urt_sg_decide_ex(&f,"URTX_RTASFastTrace",1,&ev)&&ev==URT_SG_EV_PROVEN&&f.e[0].state==URT_SG_PROVEN,"PROVEN is never blocked by a later interruption");
  CHECK(URT_SG_STABLE_MS==60000u,"proof after 1 minute of RT/PT rendering (was 10 minutes)");
  /* 4. independence and session scoping */
  CHECK(urt_sg_decide_ex(&f,"URTX_RTAsyncGIClassify",1,&ev)&&f.n==2,"rows are independent");
  CHECK(urt_sg_mark_clean(&f,FT,1)==0&&f.e[1].state==URT_SG_PENDING,"a clean exit only marks the keys this session applied");
  {UrtSgFile g=f;CHECK(!urt_sg_decide_ex(&g,"URTX_RTAsyncGIClassify",1,&ev)&&urt_sg_decide_ex(&g,"URTX_RTASFastTrace",1,&ev),"suspecting one row never blocks another");}
  {const char*const*k;urt_sg_session_clear();urt_sg_session_note("URTX_A");urt_sg_session_note("URTX_A");urt_sg_session_note("URTX_B");
   CHECK(urt_sg_session_keys(&k)==2&&!strcmp(k[0],"URTX_A")&&!strcmp(k[1],"URTX_B"),"session key list: each applied key once");urt_sg_session_clear();}
  /* 5. file format: every state survives; malformed lines ignored */
  {UrtSgFile g;const char*t="garbage\r\nURTX_X=1 bogus\r\n=1 pending\r\nURTX_Y=0 pending\r\nURTX_Z=2 failed\r\nURTX_C=2 clean\r\nURTX_R=2 retry\r\nURTX_G=2 retrying\r\n";
   urt_sg_parse(t,(unsigned)strlen(t),&g);
   CHECK(g.n==4&&!strcmp(g.e[0].key,"URTX_Z")&&g.e[1].state==URT_SG_CLEAN&&g.e[2].state==URT_SG_RETRY&&g.e[3].state==URT_SG_RETRYING,"malformed lines ignored; every state parsed");
   cycle(&g);CHECK(g.n==4&&g.e[3].state==URT_SG_RETRYING,"every state round-trips");
   CHECK(urt_sg_decide_ex(&g,"URTX_Z",1,&ev)&&ev==URT_SG_EV_NEW&&urt_sg_state(&g,"URTX_Z")==URT_SG_PENDING,"a different value replaces a FAILED record");}
  {UrtSgFile g;const char*t="URTX_OLD=1 pending\r\n";urt_sg_parse(t,(unsigned)strlen(t),&g);
   CHECK(!urt_sg_decide_ex(&g,"URTX_OLD",1,&ev)&&ev==URT_SG_EV_SUSPECTED,"an R12 file's PENDING record (written under the old rule) gets the new suspected-crash handling");}
  for(int e=URT_SG_EV_NEW;e<=URT_SG_EV_FULL;e++)if(!urt_sg_event_text(e)[0]){CHECK(0,"every event has a log sentence");break;}
  printf("TOTAL %s %d\n",fails?"FAIL":"PASS",fails);return fails?1:0;
}
