#!/usr/bin/env python3
"""Compare one CSem branch with the image on Sail; not a refinement proof.

All eight linkable probes must have identical bytes and exit status. mkdir is
an explicit missing-library classification and never counts as an agreement.
Outputs and source/runtime pins are retained under results/csem/fs-probes/.
"""
import argparse
import concurrent.futures
import hashlib
import json
import os
from pathlib import Path
import re
import signal
import subprocess
import time

ROOT = Path(__file__).resolve().parents[1]
DEFINED = re.compile(r'^Defined \{value: "Specified\((-?\d+)\)", stdout: "((?:[^"\\]|\\.)*)", stderr: "((?:[^"\\]|\\.)*)", blocked: "false"\}$')


def unescape(s):
    out = bytearray()
    escapes = {'n': 10, 'r': 13, 't': 9, 'b': 8, '\\': 92, '"': 34, "'": 39, ' ': 32}
    i = 0
    while i < len(s):
        if s[i] != '\\':
            out.extend(s[i].encode()); i += 1
        elif len(s[i+1:i+4]) == 3 and s[i+1:i+4].isdigit():
            out.append(int(s[i+1:i+4])); i += 4
        else:
            out.append(escapes[s[i+1]]); i += 2
    return bytes(out)


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def run(cmd, env, timeout):
    p = subprocess.Popen(cmd, cwd=ROOT, env=env, stdout=subprocess.PIPE,
                         stderr=subprocess.PIPE, start_new_session=True)
    try:
        out, err = p.communicate(timeout=timeout)
        return p.returncode, out, err
    except subprocess.TimeoutExpired:
        os.killpg(p.pid, signal.SIGKILL)
        out, err = p.communicate()
        return 124, out, err + b'\nHARNESS TIMEOUT\n'


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--jobs', type=int, default=3)
    ap.add_argument('--timeout', type=int, default=1800)
    args = ap.parse_args()
    if not 1 <= args.jobs <= 4:
        ap.error('jobs must be 1..4')
    main_root = Path(subprocess.check_output(['git', 'rev-parse', '--path-format=absolute', '--git-common-dir'], cwd=ROOT, text=True).strip()).parent
    outdir = ROOT / 'results/csem/fs-probes'; outdir.mkdir(parents=True, exist_ok=True)
    # Prepare once before workers; the helper also validates its runtime cache key.
    subprocess.run([str(ROOT/'scripts/csem_image_libc_prep.sh')], cwd=ROOT, check=True, stdout=subprocess.DEVNULL)
    fslist = main_root/'c/build/fs.list'
    paths = [main_root/'c/build/base.elf', fslist, ROOT/'.lake/build/bin/csem_run', ROOT/'scripts/fs_diff.py']
    paths += [p for d in ['c/src', 'patches', 'scripts'] for p in (main_root/d).glob('*') if p.is_file()]
    paths += [p for p in (ROOT/'csem/runtime').rglob('*') if p.is_file()]
    paths += [ROOT/'build/csem-image-libc/libc.core']
    # gen_embed consumes NAME=HOSTFILE entries, one per line.
    for line in fslist.read_text().splitlines():
        if line and not line.startswith('#'):
            _, host = line.split('=', 1)
            p = Path(host)
            paths.append(p if p.is_absolute() else main_root/'c'/p)
    emu = Path(os.environ.get('EMU', Path.home()/'Documents/code/ship-your-lua/riscv-lean/lean_emulator/.lake/build/bin/lean_riscv_emulator'))
    paths.append(emu)
    probes = sorted((ROOT/'c/tests/fs').glob('*.c'))
    assert len(probes) == 9, 'missing/extra probe requires explicit review'
    paths += probes
    pins = {str(p): sha(p) for p in sorted(set(paths))}
    (outdir/'pins.json').write_text(json.dumps(pins, indent=2)+'\n')
    def measure(src):
        name = src.stem; t0 = time.monotonic()
        env = dict(os.environ, MAX_STEPS='200000000', TCC_ARGS='"-w","-std=c11",',
                   CSEM_IMAGE_FS_LIST=str(fslist), EMU=str(emu))
        lean_cmd = [str(ROOT/'scripts/csem_run_image.sh'), '--first', str(src)]
        lc, lo, le = run(lean_cmd, env, args.timeout)
        meta = outdir/(name+'.sail.meta'); meta.write_text('')
        env['ORACLE_META'] = str(meta)
        sail_cmd = [str(main_root/'scripts/run_tcc_sail.sh'), str(src)]
        sc, so, se = run(sail_cmd, env, args.timeout)
        for ext, data in [('lean.out',lo),('lean.err',le),('sail.out',so),('sail.err',se)]:
            (outdir/(name+'.'+ext)).write_bytes(data)
        lines = [l for l in lo.decode().splitlines() if not l.startswith('EXECUTION ')]
        m = DEFINED.fullmatch(lines[0]) if len(lines)==1 else None
        complete = 'program_steps=' in meta.read_text()
        status = 'FAIL'
        if lc==0 and m and complete and sc not in (124,125) and sc>=0:
            if unescape(m[2])==so and unescape(m[3])==b'' and int(m[1])%256==sc:
                status = 'AGREE_ONE_BRANCH'
        if name=='mkdir' and sc==125 and b'unresolved reference' in se and b'CerbFS refusal' in le:
            status = 'MISSING_LIBRARY'
        r = dict(probe=name, source_sha256=sha(src), lean_command=lean_cmd, sail_command=sail_cmd,
                 lean_exit=lc, sail_exit=sc, status=status, wall_s=round(time.monotonic()-t0,2))
        (outdir/(name+'.json')).write_text(json.dumps(r,indent=2)+'\n')
        print(name,status,flush=True)
        return r
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as ex:
        rows = list(ex.map(measure, probes))
    assert all(sha(Path(p))==h for p,h in pins.items()), 'input drift during run'
    (outdir/'summary.tsv').write_text('probe\tstatus\tlean_exit\tsail_exit\n'+''.join(f'{r["probe"]}\t{r["status"]}\t{r["lean_exit"]}\t{r["sail_exit"]}\n' for r in rows))
    return 0 if sum(r['status']=='AGREE_ONE_BRANCH' for r in rows)==8 and sum(r['status']=='MISSING_LIBRARY' for r in rows)==1 else 1


if __name__ == '__main__':
    raise SystemExit(main())
