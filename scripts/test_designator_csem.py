#!/usr/bin/env python3
"""Compare generated continuation probes with both repaired CSem engines.

Pass --engine-root for the reviewed union-repair checkout. All artifacts are
written in this worktree. Engine refusals and wrong semantic answers remain
separate gaps; they never count as successful compiler or model validation.
"""
import argparse
from concurrent.futures import ThreadPoolExecutor
import hashlib
import json
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parent.parent

def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def run(cmd):
    cmd = list(map(str, cmd))
    p = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, timeout=120,
                       env={**os.environ, 'LEAN_ABORT_ON_PANIC': '1'})
    return dict(command=cmd, exit=p.returncode, stdout=p.stdout, stderr=p.stderr)

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--engine-root', required=True, type=Path)
    ap.add_argument('--fixtures', type=Path, default=ROOT/'build/designator-focused')
    ap.add_argument('--out', type=Path, default=ROOT/'results/model/designator/csem.json')
    args = ap.parse_args()
    engine = args.engine_root.resolve()
    fork = Path(subprocess.check_output([engine/'scripts/csem_fork_oracle.sh', '--path'], text=True).strip())
    oracle = fork/'_build/default/backend/driver/main.exe'
    lean = engine/'.lake/build/bin/csem_run'
    runtime = '--runtime='+str(fork/'_build/install/default')
    def one(f):
        export = run([oracle, runtime, '--nolibc', '--cabs-json', f])
        j = f.with_suffix('.cabs.json'); j.write_text(export['stdout'])
        o = run([oracle, runtime, '--nolibc', '--exec', '--batch', '--mode=exhaustive', f])
        l = run([lean, '--batch', runtime, j])
        expected = 'Defined {value: "Specified(0)", stdout: "", stderr: "", blocked: "false"}'
        ok = lambda r: r['exit'] == 0 and r['stdout'].strip() == expected
        export['stdout_sha256'] = hashlib.sha256(export.pop('stdout').encode()).hexdigest()
        result = 'same' if export['exit'] == 0 and ok(o) and ok(l) else 'semantic-gap'
        row = dict(name=str(f.relative_to(ROOT)), source_sha256=digest(f), export=export,
                   ocaml=o, lean=l, result=result)
        print(result, row['name'], flush=True)
        return row
    files = sorted(args.fixtures.resolve().glob('designator_*.c'))
    if not files:
        raise SystemExit('no generated probes')
    with ThreadPoolExecutor(max_workers=3) as pool:
        rows = list(pool.map(one, files))
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(dict(rows=rows, binaries={str(p):digest(p) for p in [oracle, lean]},
        shared_source_sha256=digest(engine/'csem/frontend/model/desugaring_init.lem')), indent=2)+'\n')
    return int(any(r['result'] != 'same' for r in rows))

if __name__ == '__main__':
    raise SystemExit(main())
