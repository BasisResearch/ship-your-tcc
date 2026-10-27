#!/usr/bin/env python3
"""Fresh DESIGN §3.3 measurements; never count a refusal/timeout as agreement.

Run --tcc-only while the capped CSem build is pending, then --csem-only.
Opposite-side measurements are used only when their source/image pins agree.
Full batch verdicts and the untruncated C exit values are retained for CSem.
Sail's runner exposes only the host process's low eight exit bits; its traced
program exit is required, and the HTIF 32-bit convention is audited separately.
"""
import argparse
import concurrent.futures
import hashlib
import json
import os
from pathlib import Path
import re
import signal
import shutil
import subprocess
import tempfile
import time

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'results/csem/cfg-probes'


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def pins():
    files = [ROOT / 'vendor/TINYCC_REV', ROOT / 'c/Makefile']
    files += sorted((ROOT / 'patches').glob('*.patch'))
    files += sorted(p for p in (ROOT / 'c/src').rglob('*') if p.is_file())
    files += [ROOT / 'scripts' / s for s in ['cfg_probes.py', 'run_tcc_sail.sh', 'sail_console.py']]
    emu = Path(os.environ.get('EMU', Path.home() / 'Documents/code/ship-your-lua/riscv-lean/lean_emulator/.lake/build/bin/lean_riscv_emulator'))
    files.append(emu)
    # Pin the actual cross compiler, RV64I archives and image runtime objects.
    gcc = sorted((Path.home() / 'toolchains').glob('xpack-riscv-none-elf-gcc-15.2.0-*/bin/riscv-none-elf-gcc'))[0]
    files.append(gcc)
    for arg in ['-print-file-name=libc.a', '-print-file-name=libm.a', '-print-libgcc-file-name']:
        files.append(Path(subprocess.check_output([str(gcc), '-march=rv64i', '-mabi=lp64', arg], text=True).strip()))
    files += [ROOT / 'c/build' / n for n in ['crt0.o', 'htif.o', 'main.o', 'tcc.o', 'fs.list']]
    for line in (ROOT / 'c/build/fs.list').read_text().splitlines():
        if line.strip():
            files.append(Path(line.split('=', 1)[1]))
    return {str(p.relative_to(ROOT)) if p.is_relative_to(ROOT) else str(p): sha(p) for p in files}


def unescape(s):
    # OCaml String.escaped (the batch driver's protocol), including decimal bytes.
    out = bytearray()
    escapes = {'n': 10, 'r': 13, 't': 9, 'b': 8, '\\': 92, '"': 34, "'": 39, ' ': 32}
    i = 0
    while i < len(s):
        if s[i] != '\\':
            out.extend(s[i].encode()); i += 1
        elif s[i+1] in escapes:
            out.append(escapes[s[i+1]]); i += 2
        elif s[i+1:i+4].isdigit() and len(s[i+1:i+4]) == 3:
            out.append(int(s[i+1:i+4])); i += 4
        else:
            raise ValueError('unrecognized batch escape: ' + s[i:i+4])
    return out.decode('utf-8')


DEFINED = re.compile(r'^Defined \{value: "Specified\((-?\d+)\)", stdout: "((?:[^"\\]|\\.)*)", stderr: "((?:[^"\\]|\\.)*)", blocked: "false"\}$')


def measure(probe, side, image_pins, timeout):
    env = dict(os.environ, MAX_STEPS='400000000')
    # Retain each image only long enough to hash it; no shared scratch image.
    if side == 'tcc':
        env['KEEP_WORK'] = '1'
    cmd = [str(ROOT / 'scripts/run_tcc_sail.sh'), str(probe)] if side == 'tcc' else [
        str(ROOT / 'scripts/csem_run.sh'), '--impl', 'tcc-rv64', '--address-space-top',
        '18446744073709551615', str(probe)]
    # stdin needs the separately prepared C library. No filesystem changes here.
    if side == 'csem' and probe.stem == 'p09_stdin':
        cmd.insert(1, '--libc')
    with tempfile.TemporaryDirectory(prefix='cfg-probe.') as d:
        meta = Path(d) / 'meta'; meta.touch(); env['ORACLE_META'] = str(meta)
        t0 = time.monotonic()
        try:
            p = subprocess.Popen(cmd, cwd=ROOT, env=env, stdout=subprocess.PIPE, stderr=subprocess.PIPE, start_new_session=True)
            stdout, stderr = p.communicate(timeout=timeout)
            rc, raw, err = p.returncode, stdout.decode('utf-8', 'backslashreplace'), stderr.decode('utf-8', 'backslashreplace')
        except subprocess.TimeoutExpired as e:
            # Kill and reap the whole oracle process group, including Sail.
            os.killpg(p.pid, signal.SIGKILL)
            stdout, stderr = p.communicate()
            rc, raw, err = 124, stdout.decode(errors='backslashreplace'), stderr.decode(errors='backslashreplace') + '\nHARNESS TIMEOUT'
        r = dict(command=cmd, source_sha256=sha(probe), image_pins=image_pins, exit=rc,
                 raw_stdout=raw, stderr=err, meta=meta.read_text().splitlines(),
                 wall_s=round(time.monotonic()-t0, 2), measured_utc=time.strftime('%Y-%m-%dT%H:%M:%SZ',time.gmtime()))
    if side == 'tcc':
        work = re.search(r'^work: (.+)$', err, re.M)
        if work:
            scratch = Path(work[1])
            r['elf_sha256'] = sha(scratch / 'prog.elf') if (scratch / 'prog.elf').exists() else None
            shutil.rmtree(scratch)
        complete = any(m.startswith('program_steps=') for m in r['meta'])
        r['status'] = 'defined' if complete and rc >= 0 and rc not in (124,125) else 'harness-error'
        r['outcomes'] = [[raw, rc]] if r['status'] == 'defined' else []
    else:
        r['binary_sha256'] = sha(ROOT / '.lake/build/bin/csem_run')
        lines = [x for x in raw.splitlines() if not x.startswith('EXECUTION ')]
        parsed = [DEFINED.fullmatch(x) for x in lines]
        r['status'] = 'defined' if rc == 0 and parsed and all(parsed) else 'residual'
        r['outcomes'] = sorted({(unescape(m[2]), int(m[1]) % 2**32) for m in parsed if m})
    OUT.mkdir(parents=True,exist_ok=True)
    (OUT / f'{probe.stem}.{side}.json').write_text(json.dumps(r,indent=2)+'\n')
    print(f'{probe.stem} {side}: {r["status"]} ({r["wall_s"]}s)',flush=True)


def summarize(probes, image_pins):
    rows = ['probe\ttcc\tcsem\tagreement']
    for p in probes:
        sides = []
        for side in ['tcc','csem']:
            f = OUT / f'{p.stem}.{side}.json'
            r = json.loads(f.read_text()) if f.exists() else None
            if r and (r['source_sha256'] != sha(p) or r['image_pins'] != image_pins):
                r = None
            if r and side == 'csem' and r['binary_sha256'] != sha(ROOT / '.lake/build/bin/csem_run'):
                r = None
            sides.append(r)
        t,c = sides
        agree = 'pending'
        if t and c:
            agree = 'residual'
            if t['status'] == c['status'] == 'defined':
                target = t['outcomes'][0]
                agree = 'yes' if any(target[0] == o and target[1] == e % 256 for o,e in c['outcomes']) else 'NO'
        rows.append('\t'.join([p.stem, t['status'] if t else '-', c['status'] if c else '-', agree]))
    OUT.mkdir(parents=True,exist_ok=True)
    (OUT / 'summary.tsv').write_text('\n'.join(rows)+'\n')
    print('\n'.join(rows))


def main():
    a = argparse.ArgumentParser(description=__doc__)
    g=a.add_mutually_exclusive_group(); g.add_argument('--tcc-only',action='store_true'); g.add_argument('--csem-only',action='store_true')
    a.add_argument('--jobs',type=int,default=4); a.add_argument('--timeout',type=int,default=1800)
    a.add_argument('probes',nargs='*'); args=a.parse_args()
    if not 1 <= args.jobs <= 8: a.error('--jobs must be in 1..8')
    probes=[p for p in sorted((ROOT/'c/tests/cfg').glob('*.c')) if not args.probes or any(n in p.stem for n in args.probes)]
    # Ensure pinned runtime objects reflect current patches before taking pins.
    subprocess.run(['make', '-s', '-C', str(ROOT/'c'), 'BUILD=build', 'FLAGS=0', 'ELF=build/base.elf', 'build/base.elf'], check=True, stdout=subprocess.DEVNULL)
    image_pins=pins()
    sides=['tcc'] if args.tcc_only else ['csem'] if args.csem_only else ['tcc','csem']
    with concurrent.futures.ThreadPoolExecutor(args.jobs) as pool:
        futures=[pool.submit(measure,p,s,image_pins,args.timeout) for p in probes for s in sides]
        for f in concurrent.futures.as_completed(futures): f.result()
    summarize(probes,image_pins)


if __name__ == '__main__':
    main()
