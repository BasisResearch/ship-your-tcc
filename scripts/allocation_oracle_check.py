#!/usr/bin/env python3
"""Focused allocation replay evidence; samples exercise, never define, CSem outcomes."""
import argparse, hashlib, json, os, re, subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'results/csem/allocation-oracle'

def run(args, timeout=120):
    p = subprocess.run(args, cwd=ROOT, text=True, capture_output=True, timeout=timeout)
    return {'command': [str(x) for x in args], 'exit': p.returncode,
            'stdout': p.stdout, 'stderr': p.stderr}

def verdicts(r):
    return [s for s in r['stdout'].splitlines() if s.startswith(('Defined {', 'Undefined {', 'Error {', 'ModelFailure {'))]

def main():
    ap = argparse.ArgumentParser(); ap.add_argument('--sail', action='store_true')
    ap.add_argument('--out', type=Path, default=OUT); args = ap.parse_args()
    args.out.mkdir(parents=True, exist_ok=True)
    rows = []; failed = []
    def check(name, result, pred):
        result['name'] = name; result['pass'] = bool(pred(result)); rows.append(result)
        if not result['pass']: failed.append(name)
        print(('PASS ' if result['pass'] else 'FAIL ') + name, flush=True)
    base = ['scripts/csem_run.sh', '--impl', 'tcc-rv64']
    low = [65536 + 4096*i for i in range(128)]
    high = list(reversed(low))
    def replay(file, addresses):
        return run(base + ['--alloc-addresses', ','.join(map(str, addresses)), 'csem/tests/allocation/'+file+'.c'])
    for file in ['global-address', 'order', 'reuse', 'evaluation-order']:
        default = run(base + ['csem/tests/allocation/'+file+'.c'])
        check(file+'/default', default, lambda r: r['exit']==0 and bool(verdicts(r)) and all(v.startswith('Defined {') for v in verdicts(r)))
        for name, addresses in [('ascending',low), ('descending',high)]:
            check(file+'/'+name, replay(file, addresses), lambda r: r['exit']==0 and bool(verdicts(r)) and all(v.startswith('Defined {') for v in verdicts(r)))
    reused = low.copy(); reused[8] = reused[5]
    check('reuse/same-address-new-lifetime', replay('reuse',reused),
          lambda r: r['exit']==0 and 'reuse=1 value=19' in r['stdout'])
    for policy, addresses in [('default',None), ('ascending',low), ('descending',high)]:
        r = run(base + ['csem/tests/allocation/undefined.c']) if addresses is None else replay('undefined',addresses)
        check('undefined/'+policy, r, lambda r: bool(verdicts(r)) and all(v.startswith('Undefined {') for v in verdicts(r)))
    order = [r for r in rows if r['name'] in ['order/ascending', 'order/descending']]
    if {re.search(r'Specified\((\d+)\)', r['stdout']).group(1) for r in order} != {'11','22'}: failed.append('order/both-observations')
    evals = [r for r in rows if r['name'].startswith('evaluation-order/')]
    if any(set(re.findall(r'Specified\((\d+)\)', r['stdout'])) != {'12','21'} for r in evals): failed.append('evaluation-order/independence')
    # Bad placements must terminate diagnostically; no Defined or Undefined leaf.
    for name, addresses in [('overlapping',[65536]*128), ('unaligned',[65537]*128),
                            ('out-of-width',[2**64]*128), ('missing',[65536])]:
        check('rejected/'+name, replay('order',addresses), lambda r: r['exit']!=0 and 'invalid allocation oracle resolution' in r['stdout']+r['stderr'] and not any(v.startswith(('Defined {','Undefined {')) for v in verdicts(r)))
    if args.sail:
        text=(OUT/'global-address.sail.stdout').read_text()
        assert (OUT/'global-address.sail.exit').read_text().strip() == '0'
        m=re.fullmatch(r'object=(\d+) value=7\n',text); assert m, text
        target=int(m.group(1))
        # Replay only placement data. The C bytes and printf execution are untouched.
        found=False
        for i in range(16):
            addresses=low.copy();addresses[i]=target
            r=replay('global-address',addresses)
            if r['exit']==0 and f'object={target} value=7\\n' in r['stdout']:
                check('native-Sail/placement',r,lambda _: True)
                found=True;break
        if not found: failed.append('native-Sail/placement')
    report={'source_sha256':{str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (ROOT/'csem/tests/allocation').glob('*.c')},
            'csem_binary_sha256':hashlib.sha256((ROOT/'.lake/build/bin/csem_run').read_bytes()).hexdigest(),
            'rows':rows,'failed':failed,'samples_are_semantics':False}
    (args.out/'focused.json').write_text(json.dumps(report,indent=2)+'\n')
    return bool(failed)
if __name__ == '__main__': raise SystemExit(main())
