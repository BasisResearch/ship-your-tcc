#!/usr/bin/env python3
"""C11 union initialization: specified results, not agreement between two engines.
Both engines consume the same patched shared Lem source. The oracle must be an
isolated patched build; a pristine oracle is accepted only as a separate census.
Run with --sail from the project root for the patched tcc/Sail comparison.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
FIXTURES = ROOT / 'tests/union-init'


def check_result(rc, out, value):
    lines = [s for s in out.splitlines() if not s.startswith('EXECUTION ')]
    want = f'Defined {{value: "Specified({value})", stdout: "", stderr: "", blocked: "false"}}'
    return rc == 0 and bool(lines) and all(s == want for s in lines)


def run(cmd, cwd, env):
    p = subprocess.run(list(map(str, cmd)), cwd=cwd, env=env, capture_output=True,
                       text=True, timeout=180)
    return dict(command=list(map(str, cmd)), exit=p.returncode, stdout=p.stdout, stderr=p.stderr)


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--selftest', action='store_true')
    ap.add_argument('--sail', action='store_true')
    ap.add_argument('--pristine', help='separate pristine upstream oracle census')
    ap.add_argument('--report', type=Path)
    a = ap.parse_args()
    if a.selftest:
        # Detect old first-member/all-zero answers, refusal/crash, nonzero exit,
        # mixed good/UB branches, missing output, and a correct-valued error.
        valid = 'Defined {value: "Specified(7)", stdout: "", stderr: "", blocked: "false"}\n'
        assert check_result(0, valid, 7)
        for rc, out in [(0, valid.replace('7', '0')), (125, ''), (134, valid),
                        (0, valid+'Undefined {}\n'), (0, ''),
                        (0, 'Error {msg: "Specified(7)"}')]:
            assert not check_result(rc, out, 7), (rc, out)
        print('union-init: result-checker plants passed', flush=True)
    env = dict(os.environ, LEAN_ABORT_ON_PANIC='1', NO_COLOR='1', TERM='dumb')
    oracle = Path(env.get('CSEM_ORACLE', env.get('CERB_ORACLE_BIN_OVERRIDE',
                       str(ROOT/'_build/default/backend/driver/main.exe'))))
    runtime = Path(env.get('CSEM_RUNTIME', str(ROOT/'_build/install/default')))
    lean = Path(env.get('CSEM_LEAN_BIN', env.get('CERB_LEAN_BIN_OVERRIDE',
                     str(ROOT/'lean_frontend/.lake/build/bin/cerberus-lean'))))
    rows = []
    expected = json.loads((FIXTURES/'expected.json').read_text())
    with tempfile.TemporaryDirectory(prefix='union-init-') as tmp:
        tmp = Path(tmp)
        for name, value in expected.items():
            src = FIXTURES/(name+'.c')
            row = dict(name=name, expected=value, source_sha256=hashlib.sha256(src.read_bytes()).hexdigest())
            export = run([oracle, f'--runtime={runtime}', '--cabs-json', src], tmp, env)
            j = tmp/(name+'.json')
            j.write_text(export['stdout'])
            row['cabs_export_exit'] = export['exit']
            row['ocaml'] = run([oracle, f'--runtime={runtime}', '--nolibc', '--exec', '--batch',
                                '--mode=exhaustive', src], tmp, env)
            row['lean'] = run([lean, '--batch', f'--runtime={runtime}', j], tmp, env)
            row['gcc_compile'] = run(['gcc', '-std=c11', '-O0', '-funsigned-char', '-w', src,
                                      '-o', tmp/'prog'], tmp, env)
            if row['gcc_compile']['exit'] == 0:
                row['gcc'] = run([tmp/'prog'], tmp, env)
            row['ok'] = (export['exit'] == 0 and row['gcc_compile']['exit'] == 0 and
                         row.get('gcc', {}).get('exit') == value % 256 and
                         row['gcc']['stdout'] == '' and
                         all(check_result(row[k]['exit'], row[k]['stdout'], value)
                             for k in ('ocaml','lean')))
            if a.sail:
                row['sail'] = run([ROOT.parent/'scripts/run_tcc_sail.sh', src], ROOT.parent,
                                 dict(env, TCC_ARGS='"-w","-std=c11",'))
                row['ok'] &= row['sail']['exit'] == value % 256 and row['sail']['stdout'] == ''
            if a.pristine:
                row['pristine_upstream_only'] = run([a.pristine, f'--runtime={runtime}', '--nolibc',
                                                    '--exec', '--batch', src], tmp, env)
            rows.append(row)
            print(name, 'PASS' if row['ok'] else 'FAIL', flush=True)
    report = dict(rows=rows, engines={str(p): hashlib.sha256(p.read_bytes()).hexdigest()
                                      for p in (oracle, lean)})
    if a.report:
        a.report.parent.mkdir(parents=True, exist_ok=True)
        a.report.write_text(json.dumps(report, indent=2)+'\n')
    failed = [r for r in rows if not r['ok']]
    if failed:
        print(json.dumps(failed, indent=2))
    print(f'union-init: {len(rows)-len(failed)}/{len(rows)} specified outcomes agree')
    return bool(failed)


if __name__ == '__main__':
    raise SystemExit(main())
