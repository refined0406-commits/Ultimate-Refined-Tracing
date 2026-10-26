#!/usr/bin/env python3
"""Ultimate Refined Tracing V5 - reproducible native build.

Toolchain: clang-cl + lld-link (LLVM). No Windows SDK, no CRT: the runtime
declares the few Win32 APIs it uses and links against import libraries that
are generated here from the .def files. Builds twice into separate folders
and fails unless both outputs are byte-identical.

Usage:  python3 build.py [--out DIR] [--no-tests] [--no-package]
"""
from pathlib import Path
import argparse, hashlib, json, os, shutil, subprocess, sys, time, zipfile

ROOT = Path(__file__).resolve().parent
SRC = ROOT / 'src'
VERSION = '5.2.0'   # public release identity (the expanded V5.2.0: former private V5.3.0 DEV1/DEV2 checkpoints + final presets)
ASI_VERSION = VERSION

# Translation units linked into RefinedTracing.asi (order is stable for reproducibility).
UNITS = '''cr01_rtao_config cr01_dxr_submission_gate cr01_activation_txn cr01_gate_core
cr01_gate_windows cr01_gate_live_adapter cr01_startup_barrier cr01_bootstrap_owner
cr01_dinput_sync_gate cr01_registration_pair cr01_register_sites cr01_reg_hotpatch_core
cr01_reg_hotpatch_windows cr01_contract cr01_controls cr01_validation cr01_indirect_live
cr01_custom_shader_identity cr01_shader_slots cr01_fltused cr01_asi_entry cr01_release_log
cr01_pt_shared_dxil cr01_prod_early501 cr01_prod_runtime501 cr01_rt_legacy_core
cr01_rt_c17_core cr01_rt_legacy501 cr01_menu_bridge cr01_settings_codec cr01_settings_store
cr01_settings_store_windows cr01_native_execution cr01_shadow_policy refined_pt_shadow_ext_501
cr01_shadow501 cr01_bone_hook501 cr01_bone_fast_safe501 cr01_distant_trees501
cr01_physics_lod501'''.split()
EXTRA_UNITS_FILE = SRC / 'UNITS_V5.txt'   # new V5 modules, one per line

CFLAGS = ['/nologo', '/c', '/O2', '/Gy', '/GS-', '/GR-', '/Zl', '/W4', '/WX', '/Brepro']
LFLAGS = ['/dll', '/machine:x64', '/nodefaultlib', '/brepro', '/opt:ref', '/opt:icf']


def find_tool(names):
    for n in names:
        p = shutil.which(n)
        if p:
            return p
    raise SystemExit('missing tool: ' + ' / '.join(names))


CLANG_CL = None
LLD_LINK = None
LOG = []


def run(args):
    args = [str(a) for a in args]
    LOG.append('$ ' + ' '.join(args))
    p = subprocess.run(args, text=True, capture_output=True)
    if p.stdout.strip():
        LOG.append(p.stdout.rstrip())
    if p.stderr.strip():
        LOG.append(p.stderr.rstrip())
    if p.returncode:
        raise SystemExit('COMMAND FAILED: ' + ' '.join(args) + '\n' + p.stdout + p.stderr)


def sha(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def units():
    u = list(UNITS)
    if EXTRA_UNITS_FILE.exists():
        u += [x.strip() for x in EXTRA_UNITS_FILE.read_text().split() if x.strip()]
    return u


def build_once(out: Path, legacy_proxy: bool):
    if out.exists():
        shutil.rmtree(out)
    out.mkdir(parents=True)
    for lib in ('kernel32', 'advapi32'):
        run([LLD_LINK, '/dll', '/noentry', '/machine:x64', '/nodefaultlib', '/brepro',
             '/def:' + str(SRC / (lib + '_gate.def')), '/out:' + str(out / ('gen_' + lib + '.dll')),
             '/implib:' + str(out / (lib + '.lib'))])
        (out / ('gen_' + lib + '.dll')).unlink(missing_ok=True)
    objs = []
    for f in units():
        o = out / (f + '.obj')
        run([CLANG_CL, *CFLAGS, '/I' + str(SRC), '/DURT_VERSION="' + ASI_VERSION + '"', '/Fo' + str(o), SRC / (f + '.c')])
        objs.append(o)
    run([LLD_LINK, *LFLAGS, '/entry:DllMain', '/def:' + str(SRC / 'cr01_asi_exports.def'),
         '/out:' + str(out / 'UltimateRefinedTracing.asi'), *objs, out / 'kernel32.lib', out / 'advapi32.lib'])
    result = {'UltimateRefinedTracing.asi': sha(out / 'UltimateRefinedTracing.asi')}
    if legacy_proxy:
        L = SRC / 'legacy_dinput8'
        run([CLANG_CL, *CFLAGS, '/Fo' + str(out / 'cr01_dinput_proxy.obj'), L / 'cr01_dinput_proxy.c'])
        run([LLD_LINK, *LFLAGS, '/entry:DllMain', '/def:' + str(L / 'cr01_dinput_proxy.def'),
             '/out:' + str(out / 'dinput8.dll'), out / 'cr01_dinput_proxy.obj', out / 'kernel32.lib'])
        result['dinput8.dll'] = sha(out / 'dinput8.dll')
    for p in out.glob('*.obj'):
        p.unlink()
    for p in out.glob('*.lib'):
        p.unlink()
    return result


ZIP_TIME = (2026, 10, 21, 0, 0, 0)
UAL = ROOT / 'third_party' / 'ultimate_asi_loader_9.7.4'
UAL_SHA = '031a3e5576d91dce1e438d36b9a3d462c7334ab4791990a8ff1e3ddc0e132daf'


def package(out: Path, asi: Path):
    """Deterministic MAIN and ASI-ONLY archives (sorted entries, fixed time)."""
    # Never emit an installable V5.2 archive with the old V5.1 precompiled.rsblob.
    receipt = ROOT / 'game' / 'wcc' / 'V52_VERIFIED_WCC.json'
    if not receipt.is_file():
        raise SystemExit('BLOCKED: V5.2 genuine WCC compilation receipt not adopted; no installable packages produced.')
    evidence = json.loads(receipt.read_text())
    run([sys.executable, '-I', ROOT / 'tools' / 'build_v5_locales.py'])
    run([sys.executable, '-I', ROOT / 'tools' / 'build_v52_locales.py'])
    run([sys.executable, '-I', ROOT / 'tools' / 'build_v53_locales.py'])
    run([sys.executable, '-I', ROOT / 'tools' / 'build_v53_dev2_strings.py'])
    run([sys.executable, '-I', ROOT / 'tools' / 'build_v5_menu.py'])
    run([sys.executable, '-I', ROOT / 'tools' / 'build_v52_menu.py'])
    required = {
        'candidate_source_sha256': sha(ROOT / 'game' / 'wcc' / 'RefinedTracingCR01_RTGI3.ws'),
        'blob_sha256': sha(ROOT / 'game' / 'wcc' / 'precompiled.rsblob'),
    }
    if any(evidence.get(k) != v for k,v in required.items()) or evidence.get('wcc_exit_code') != 0 or evidence.get('workspace_restored') is not True:
        raise SystemExit('BLOCKED: WCC receipt/source/blob inconsistent; no installable packages produced.')
    # RC5: the menu is the receipt menu minus exactly the two SUBTLE_SEPARATOR rows (proved, not re-pinned).
    sys.path.insert(0, str(ROOT / 'tools' / 'v52_selected'))
    import menu_delta
    try:
        menu_delta.verify()
    except AssertionError as e:
        raise SystemExit('BLOCKED: menu is not the receipt menu minus the documented separator rows: %s' % e)
    # RC7: the menu movie shipped is the original V5.1.0 asset with the separator right-line repair
    # (tools/v52_selected/separator_swf_fix.py; byte-size neutral, both movie copies).
    sys.path.insert(0, str(ROOT / 'tools' / 'v52_selected'))
    import separator_swf_fix
    swf = ROOT / 'build' / 'swf' / 'panel_ingamemenu.redswf'
    swf.parent.mkdir(parents=True, exist_ok=True)
    swf.write_bytes(separator_swf_fix.patch((ROOT / 'game' / 'wcc' / 'panel_ingamemenu.redswf').read_bytes()))
    if sha(UAL / 'winmm.dll') != UAL_SHA:
        raise SystemExit('third-party loader hash mismatch')
    common = {
        'IMPORTANT README!.txt': ROOT / 'package' / 'IMPORTANT README!.txt',
        'bin/x64_dx12/UltimateRefinedTracing.asi': asi,
        'bin/config/r4game/user_config_matrix/pc/RefinedTracing.xml': ROOT / 'build' / 'menu' / 'RefinedTracing.xml',
        'mods/modRefinedTracing/content/precompiled.rsblob': ROOT / 'game' / 'wcc' / 'precompiled.rsblob',
        'mods/modRefinedTracing/content/gameplay/gui_new/swf/mainmenu/panel_ingamemenu.redswf': swf,
    }
    for p in sorted((ROOT / 'build' / 'loc').glob('*.w3strings')):
        common['mods/modRefinedTracing/content/' + p.name] = p
    main = dict(common)
    main['bin/x64_dx12/winmm.dll'] = UAL / 'winmm.dll'
    main['LICENSES/Ultimate-ASI-Loader-LICENSE.txt'] = UAL / 'LICENSE-Ultimate-ASI-Loader.txt'
    res = {}
    for kind, files in (('MAIN', main), ('ASI_ONLY', common)):
        z = out / f'Ultimate_Refined_Tracing_V{VERSION}_{kind}.zip'
        with zipfile.ZipFile(z, 'w', zipfile.ZIP_DEFLATED, compresslevel=9) as zf:
            for name in sorted(files):
                info = zipfile.ZipInfo(name, ZIP_TIME)
                info.compress_type = zipfile.ZIP_DEFLATED
                info.external_attr = 0o644 << 16
                zf.writestr(info, Path(files[name]).read_bytes())
        with zipfile.ZipFile(z) as zf:
            assert zf.testzip() is None
        res[z.name] = {'sha256': sha(z), 'entries': len(files),
                       'files': {n: sha(files[n]) for n in sorted(files)}}
    return res


def main():
    global CLANG_CL, LLD_LINK
    ap = argparse.ArgumentParser()
    ap.add_argument('--out', default=str(ROOT / 'out'))
    ap.add_argument('--legacy-proxy', action='store_true', help='also build the optional legacy dinput8 bootstrap')
    ap.add_argument('--no-package', action='store_true')
    ap.add_argument('--use-asi', help='package this already-tested ASI instead of the fresh build; its sha256 must equal BUILD_EVIDENCE/NATIVE_BUILD_MANIFEST.json')
    a = ap.parse_args()
    CLANG_CL = find_tool(['clang-cl', 'clang-cl-18'])
    LLD_LINK = find_tool(['lld-link', 'lld-link-18'])
    out = Path(a.out)
    t0 = time.time()
    ver = subprocess.run([CLANG_CL, '--version'], text=True, capture_output=True).stdout.splitlines()[0]
    LOG.append('toolchain: ' + ver)
    r1 = build_once(out / 'pass_a', a.legacy_proxy)
    r2 = build_once(out / 'pass_b', a.legacy_proxy)
    if r1 != r2:
        raise SystemExit('NOT REPRODUCIBLE: %r vs %r' % (r1, r2))
    final = out / 'bin'
    if final.exists():
        shutil.rmtree(final)
    shutil.copytree(out / 'pass_a', final)
    shutil.rmtree(out / 'pass_b')
    shutil.rmtree(out / 'pass_a')
    manifest = {'version': VERSION, 'toolchain': ver, 'units': units(), 'outputs': r1,
                'reproducible_two_pass': True}
    if not a.no_package:
        pkg_asi = final / 'UltimateRefinedTracing.asi'
        if a.use_asi:
            pinned = json.loads((ROOT / 'BUILD_EVIDENCE' / 'NATIVE_BUILD_MANIFEST.json').read_text())['outputs']['UltimateRefinedTracing.asi']
            if sha(Path(a.use_asi)) != pinned:
                raise SystemExit('BLOCKED: --use-asi is not the pinned, gameplay-tested ASI')
            pkg_asi = Path(a.use_asi)
            manifest['packaged_asi'] = {'source': 'pinned prebuilt', 'sha256': pinned}
        p1 = package(out, pkg_asi)
        p2 = package(out, pkg_asi)
        if {k: v['sha256'] for k, v in p1.items()} != {k: v['sha256'] for k, v in p2.items()}:
            raise SystemExit('packages not reproducible')
        manifest['packages'] = p1
    manifest['seconds'] = round(time.time() - t0, 1)
    (out / 'build_manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    (out / 'build.log').write_text('\n'.join(LOG) + '\n')
    print(json.dumps(manifest, indent=2))


if __name__ == '__main__':
    main()
