#!/usr/bin/env python3
"""Independent complete arenas for array decay; constraints remain nonsuccesses."""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path
from gen_array_decay_fixtures import generate, REPRO
from check_gnu_alignment_semantics import fixtures as semantic_fixtures

ROOT=Path(__file__).resolve().parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def run(cmd,stem):
    p=subprocess.run(list(map(str,cmd)),cwd=ROOT,capture_output=True,timeout=7200)
    stem.with_suffix('.stdout').write_bytes(p.stdout)
    stem.with_suffix('.stderr').write_bytes(p.stderr)
    stem.with_suffix('.command.json').write_text(json.dumps(list(map(str,cmd)),indent=2)+'\n')
    return p

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--out',type=Path,required=True)
    ap.add_argument('--sail-capture',type=Path)
    a=ap.parse_args();out=a.out.resolve();out.mkdir(parents=True,exist_ok=False)
    positive,invalid=generate(out/'inputs')
    assert REPRO==semantic_fixtures()['array-alias-decay'][0]
    pins={str(p):sha(p) for p in [Path(__file__),ROOT/'scripts/gen_array_decay_fixtures.py',ROOT/'scripts/model_diff.py',ROOT/'.lake/build/bin/tcc_model',ROOT/'patches/0003-tcc-c11.patch',*map(Path,positive),*map(Path,invalid)]}
    # A strict C11 diagnostic oracle, with GNU attributes retained verbatim.
    gcc=[]
    for src in [*positive,*invalid]:
        obj=out/(Path(src).stem+'.o')
        r=run(['gcc','-std=c11','-pedantic-errors','-c',src,'-o',obj],out/(Path(src).stem+'-gcc'))
        good=(r.returncode!=0 and bool(r.stderr)) if src in invalid else r.returncode==0
        gcc.append(dict(source=src,invalid=src in invalid,exit=r.returncode,accepted=good))
    assert all(x['accepted'] for x in gcc),gcc
    cmd=['python3','scripts/model_diff.py','--no-report','--require-success',str(len(positive)),'--json-out',str(out/'fast.json'),'-j','4',*positive,*invalid]
    fast=run(cmd,out/'fast')
    data=json.loads((out/'fast.json').read_text())
    rows=[row for group in data['results'].values() for row in group]
    successes=[row for row in rows if row['result']=='same']
    nonsuccesses=[row for row in rows if row['result']!='same']
    assert fast.returncode==0 and len(successes)==len(positive),data['totals']
    assert len(nonsuccesses)==len(invalid) and all(row['result']=='same-refusal' for row in nonsuccesses),nonsuccesses
    native=[]
    if a.sail_capture:
        for name in ['array-alias-decay','calls-spill','element-referent']:
            dst=out/(name+'-native')
            r=run(['python3','scripts/unevaluated_native_check.py','--source',out/'inputs'/(name+'.c'),'--sail-capture',a.sail_capture.resolve(),'--out',dst],out/(name+'-native-command'))
            native.append(dict(case=name,exit=r.returncode,report=str(dst/'comparison.json')))
            assert r.returncode==0,native[-1]
    assert all(sha(p)==h for p,h in pins.items()),'frozen input changed'
    (out/'report.json').write_text(json.dumps(dict(pins=pins,gcc=gcc,fast_totals=data['totals'],native=native),indent=2)+'\n')
    print(f'{len(positive)} complete independent fast arenas; {len(invalid)} separate constraint refusals; {len(native)} native arena/execution controls')
if __name__=='__main__':main()
