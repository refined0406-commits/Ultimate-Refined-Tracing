#!/usr/bin/env python3
"""V5.3 dev: every protected V5.2.0 source and asset is byte-identical (tests/fixtures/v520_protected_sources.json):
PT shader carrier/identity/slots, PT shadow extension, CPU (bones, distant trees, physics), Normal RT, PT Scene Range,
Frame Generation, settings store/codec, loader gate, menu movie (separator repair input), UAL, V5.2.0 string and
preset generators and fixtures. Only the documented V5.3 edit set may differ."""
import hashlib, json
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
pins = json.loads((ROOT / 'tests/fixtures/v520_protected_sources.json').read_text())['files']
# V5.3.0 R10/R11: deliberate, documented edits of protected V5.2.0 sources; only these exact bytes are accepted.
#   R10 (docs/v530/V5.3.0_R10_SHARC_REPAIR.md): the PT serializer gate keeps URT's bounce words native while the game's SHaRC is on.
#   R11 (docs/v530/V5.3.0_R11_BOUNCE_LAYER.md): the gate carries the PT: Second Bounce Coverage level (bits 16-17); the PT shader
#   preparation installs URT's variant library when that option is set; the identity check accepts that variant.
R10_EDITS = {'src/cr01_gate_core.c': '69d759f7b0764dacc79314b9cedcc4a89743e00e6ca57e9a77e7e37348d0cde9', 'src/cr01_gate_core.h': 'c7007380e9a7e3e60546588be39494a5c52edd59e89a594ce6ae00b06a6ced76', 'src/cr01_gate_windows.c': '04e965916142b8f255b6e39fc7117c444c7b8e3097a193406b1b7dfc93dd60ef', 'src/cr01_asi_entry.c': '2ad892c17d582a177f9e93149f22efddaa9838c0e3a74e033affbefb77043468', 'src/cr01_custom_shader_identity.c': '1a77240393376dd98bce4ff14749b3348e0f330984e12b9259f6633bc4ace875'}
# R12 (docs/v530/V5.3.0_R12_FIRST_BOUNCE.md): the gate carries a second 2-bit coverage level (bits 18-19) and the ASI entry builds the
#   variant when either coverage option is set; R12 FIX1: it logs the exact stage of a variant failure (same entries, newest bytes).
#   R12 FIX2 (docs/v530/V5.3.0_R12_FIX2.md): a third 2-bit level (bits 20-21, sun shadow coverage), a frame counter proving the
#   level reached PT frames, and the ASI entry records a clean process exit for the startup guard (same entries, newest bytes).
#   R12 FIX3 (docs/v530/V5.3.0_R12_FIX3.md): the in-function bounce word is computed by a pure, host-tested core
#   (cr01_gate_infunc_word), retired coverage levels are refused, per-reason frame counters; the shader slots gain a
#   read-only re-check (cr01_shader_slots_verify); levels may carry module 4 (bits 22-23) and the ASI entry builds it
#   only when set. Nothing else changes.
# V5.3.0 R11, game build 1090142 (docs/v530/V5.3.0_R11_GAME_BUILD_1090142.md): every pinned Patch 5.01 address goes through
#   urt_rva() (same address on build 1048522, the mapped one on build 1090142), the executable identity knows both builds, and the
#   two rip-relative fingerprints (Frame Generation, SHaRC diagnostic) are checked by opcode + computed target. Nothing else changes.
GAME_BUILD_EDITS = {"src/cr01_shader_slots.h": "2d9357bbdfa2e86ac480fc47afe2b68b6da0ee21ce2472450b651ea7e098c203", "src/cr01_shader_slots.c": "a41b0489f97c7895cb77ff2a9421e5056e78b2884b66a739b43cf064c26f3194", "src/cr01_asi_entry.c": "b74dfeaa60d4a0906b66a04949608a8a585d3e7bd2be8eea4837f3be289ac8d1", "src/cr01_bone_hook501.c": "70a7e93fa43620698c0b28d5983c9ca78787319dc923be14fa6f9ed1b1344bf8", "src/cr01_gate_core.h": "4d2b73f574753326b52a4822939e2ebc1809037107213171ced75c36761aaae2", "src/cr01_gate_windows.c": "e66e4e4ebdb800a0afa36d4c4878be9a02e847975ba935d6a7800c7a0e1c8a8a", "src/cr01_prod_early501.c": "25e93782f24a8016c0bc7716f788ea9a6993b063725db0543911855a091bb7b6", "src/cr01_reg_hotpatch_core.h": "16864ffee3a5a4fb40d76fcd18bdadc673ba984b7fd5122f44d1ea0d4243783b", "src/cr01_register_sites.h": "d5976205518b278100c8f6fe1c46355d3a2a3c9f1bac784ac87f22840ee1b75e", "src/cr01_rt_legacy501.c": "6cf61a0f84d2e57f10dff300010d2e9103cf53a944ca40a0df7b40dd700da117", "src/cr01_shadow501.c": "b74721c6e6b811ddad28551b46ee2e16e33933c7c59205fcd6806ef433c42347", "src/refined_pt_shadow_ext_501.c": "b7e07b83bd14a5c558a55590d0fffc4ab8d3297341e3740bac9030c3116bb8fc", "src/urt_exe_identity_plan.c": "12b3846506ee41b900a4ecaee55bf6b6d9f29a4a6a6997dac8bd0e3af354bdd7", "src/urt_exe_identity_win.c": "7ecd1e76079957923479ea2acb70c688a6f353bcde27238ca1725c9778a5a7d8", "src/urt_fg501.c": "1d09318537ca0381cb15877b9f3f8c0e2c5ce88489c9ecbcd576cd105ac58b78", "src/urt_ptperf501.c": "3c80f5b2533a937ad642df9f66634bc05c52d8a0ca255bd2b13c1dcb8ee4edf3", "src/urt_rtperf501.c": "fcbe6017ba53e708adb5fb9ab145d19749085de01722d38274ee450a61d3c980", "src/cr01_gate_core.c": "4953c4bbaae855dc9d8b6a68521847c41843689a407ade7e8c8829b60b3fd335"}
# V5.3.0 release: the game's menu movie is not redistributed; the public source does not contain it (checked when present)
NOT_REDISTRIBUTED = {'game/wcc/panel_ingamemenu.redswf'}
bad = [k for k, v in pins.items() if not (k in NOT_REDISTRIBUTED and not (ROOT / k).exists()) and
       hashlib.sha256((ROOT / k).read_bytes()).hexdigest() not in (v, R10_EDITS.get(k), GAME_BUILD_EDITS.get(k))]
assert not bad, bad
src = (ROOT / 'src/UNITS_V5.txt').read_text().split()
assert src[:14][-1] == 'urt_compat53' and src[14:] == ['urt_startup_guard', 'urt_bounce501', 'urt_bounce_boot501', 'urt_build'], src   # V5.3.0 rev4: + startup guard; R7: + PT code limits; R8: + foliage shader; R9: foliage unlinked; R11: PT code limits unlinked (sources kept), + PT bounce layer; game build 1090142: + urt_build
print('PASS %d protected V5.2.0 files byte-identical except the pinned R10/R11 edits and the game-build-1090142 address layer (PT shader identity, CPU/RT/PT modules, store, menu movie, loader, V5.2.0 generators)' % len(pins))
