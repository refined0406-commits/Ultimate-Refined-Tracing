#!/usr/bin/env python3
"""Release guard (RC3, extended in RC6): native graphics-settings safety, static.
 - the native Texture Quality CVars removed in RC3 can never be owned again;
 - PT: Scene Range's two TLAS variables are owned ONLY by its PT-only module (urt_ptperf501.c);
 - every selected row whose variable the game saves (flag 0x100) goes through the save-bit
   detach (urt_gfx_core) and no row writes a save-flagged variable through another path;
 - no INI writer, read-only access to dx12user.settings, 3 script saves (unchanged since RC3);
 - the WCC receipt, when present, matches source, menu (or a documented menu delta) and blob."""
from pathlib import Path
import json,hashlib,re,sys
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/v52_selected'))
import spec as S
text=(ROOT/'src/urt_gfx_table.c').read_text()
raw=(ROOT/'game/wcc/RefinedTracingCR01_RTGI3.ws').read_text('utf-8-sig')
menu=(ROOT/'build/menu/RefinedTracing.xml').read_text()
names=['src/cr01_prod_runtime501.c','src/urt_gfx501.c','src/urt_gfx_table.c','src/urt_gfx_core.c','src/urt_ptperf501.c','src/urt_rtperf501.c','src/cr01_rt_legacy501.c']
all_source='\n'.join((ROOT/p).read_text() for p in names)
for key in ('TextureMemoryBudget','TextureMipBias','PTBaseAngleForGradient','MaxTextureSize','TextureInFlightBudget"'):
 assert key not in all_source, f'forbidden native variable still owned: {key}'
for key in ('URTX_PTTextureDetail','URTX_TextureBudget','URTX_TextureSharpness','RTQualityShadingBudget','RTPerfShadingBudget'):
 assert key not in menu and key not in raw and key not in text, f'withdrawn control still active: {key}'
for key in S.RETIRED_AFTER_COMPILE+('Caustics',):
 assert key not in menu and key not in text and key not in all_source, f'RC7 caustics still active: {key}'
for p in names:
 t=(ROOT/p).read_text()
 if 'ProximityTLASSize' in t or 'ProximityExtendedTLASSize' in t:
  assert p=='src/urt_ptperf501.c', f'TLAS variables owned outside PT: Scene Range: {p}'
pt=(ROOT/'src/urt_ptperf501.c').read_text()
assert '0x202u' in pt and 'mode==(unsigned)CR01_RENDER_PT' in pt, 'PT: Scene Range: session-only variables, PT only'
# saved rows: flag byte detach path present and used
core=(ROOT/'src/urt_gfx_core.c').read_text(); ad=(ROOT/'src/urt_gfx501.c').read_text()
assert 'await_save' in core and 'urt_gx_settings_saved(&g_owner)' in ad and 'URT_GX_DETACH_SETTLE_MS' in core and 'reattach(o,i);' in core and 'detach(o,i,now_ms)' in core
assert 'base+desc+9u' in ad and 'base+desc+0x20u' in ad
saved=[r['id'] for r in S.ROWS if r['flags']&0x100]
assert saved==['URTX_RTAOScreenSpace','URTX_RTHBIL','URTX_RTAOPretrace','URTX_RTReflScreenSpace','URTX_TextureStreams','URTX_TerrainPreload'],saved
assert set(S.NATIVE_MENU_HOLD)=={'URTX_TerrainPreload'}
assert 'FoliageDistanceScale' not in all_source and 'URTX_TreeDistance' not in menu, 'RC8: Foliage Visibility Range belongs to the game'
# Source-only checkpoints must not ship a stale blob; after compilation a genuine
# source/menu/blob-bound receipt is mandatory. Do not weaken this release gate.
wcc_receipt=ROOT/'game/wcc/V52_VERIFIED_WCC.json'
wcc_blob=ROOT/'game/wcc/precompiled.rsblob'
assert wcc_receipt.exists()==wcc_blob.exists(),'unpaired WCC blob/receipt'
sha=lambda b:hashlib.sha256(b).hexdigest()
r=json.loads(wcc_receipt.read_text())
if r['candidate_source_sha256']==sha(raw.encode('utf-8')):
 assert r['wcc_exit_code']==0 and r['workspace_restored'] is True and r['static_checks_passed'] is True
 import menu_delta; menu_delta.verify()
 assert r['blob_sha256']==sha(wcc_blob.read_bytes()),'WCC blob hash mismatch'
 assert r['blob_bytes']==wcc_blob.stat().st_size,'WCC blob size mismatch'
 state='receipt matches the RC6 source'
else:
 state='RC6 source NOT compiled yet (receipt is for an earlier source: packaging is blocked by build.py)'
for key in ('URTX_TextureStreams','URTX_FGMenuResources','URTX_PTAOQuality','URTX_RTReflFog','URTX_TerrainPreload'):
 assert key in menu and key in text, f'selected setting lost: {key}'
assert 'WritePrivateProfileStringW(' not in all_source, 'native settings writer forbidden'
assert 'GENERIC_READ' in (ROOT/'src/cr01_prod_runtime501.c').read_text()
assert raw.count('theGame.SaveUserSettings();')==3,'unexpected WCC save change'
print('PASS native graphics safety static: texture-quality CVars absent; TLAS only in PT Scene Range; 6 saved rows detached while owned; '+state)
