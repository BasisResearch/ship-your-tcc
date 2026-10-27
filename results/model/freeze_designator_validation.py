#!/usr/bin/env python3
"""Freeze the final initializer-continuation evidence; fail on stale sources.

Run after all three run_tcc_sail.sh processes have completed successfully and
model_diff has written its final broad JSON. Native observations remain tests,
not kernel reachability proofs or reference input to the compiler model.
"""
from pathlib import Path
from collections import Counter
import hashlib
import json
import shutil

ROOT=Path(__file__).resolve().parents[2]
OUT=ROOT/'results/model/designator'

def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def read(path):
    return json.loads(Path(path).read_text())

def check_manifest(request, result):
    req=read(request);res=read(result)
    assert req['request_id']==res['request_id'] and res['exit']==0
    assert res['source_hashes_verified']
    for p,h in req['sha256'].items():
        assert digest(ROOT/p)==h, f'stale source: {p}'
    return req,res

def counts(data):
    return dict(Counter(r['result'] for rows in data['results'].values() for r in rows))

def main():
    req,res=check_manifest(OUT/'build-02.request.json',OUT/'build-02.result.json')
    gate,gate_res=check_manifest(OUT/'full-gate.request.json',OUT/'full-gate.result.json')
    focused=read(OUT/'focused.json');broad=read(OUT/'broad.json')
    for data in [focused,broad]:
        for p in ['patches/0003-tcc-c11.patch','c/build/base.elf','.lake/build/bin/tcc_model']:
            assert data['pins'][p]==digest(ROOT/p), f'stale validation: {p}'
        for rows in data['results'].values():
            for row in rows:
                assert row['result'] in ['same','same-refusal','unsupported-asm']
        assert data['compiler_args']==['-std=c11','-w']
    assert counts(focused)=={'same':62,'same-refusal':8}
    assert broad['totals']['same']>=1000
    assert '144 positive cases, 14 negative cases; all passed' in (OUT/'c11.log').read_text()
    assert 'pass=58 fail=0' in (OUT/'cross-sail.log').read_text()
    assert 'isa-bad objects=0 elfs=0' in (OUT/'cross-sail.log').read_text()
    csem=read(OUT/'csem.json');cc=Counter(r['result'] for r in csem['rows'])
    assert cc=={'same':52,'semantic-gap':6}
    sail=[]
    sources={'minimal':'results/repro/tcc_designator_continuation.c',
             'nested':'build/designator-focused/designator_nested_union_static.c',
             'suite':'build/designator-union-src/csem/tests/union-init-batch.c'}
    for name,source in sources.items():
        base=ROOT/f'build/designator-{name}-final'
        meta=dict(line.split('=',1) for line in base.with_suffix('.meta').read_text().splitlines())
        assert all(int(meta[k])>0 for k in ['compile_steps','program_steps','steps'])
        stdout=Path(str(base)+'.sail.stdout').read_bytes();stderr=Path(str(base)+'.sail.stderr').read_text()
        assert stdout==b''
        work=Path(next(l[6:] for l in stderr.splitlines() if l.startswith('work: ')))
        raw=(work/'raw').read_bytes();trace=(work/'trace').read_bytes()
        assert b'TRACE-FUEL-OUT' not in trace and b'FAILURE' not in raw
        assert sum(l.startswith(b'T\t') for l in trace.splitlines())==3
        record=dict(name=name,source=source,source_sha256=digest(ROOT/source),
                    elf_sha256=digest(work/'prog.elf'),meta=meta,
                    stdout_hex=stdout.hex(),stderr=stderr,
                    command=['timeout','900','scripts/run_tcc_sail.sh',source],
                    environment={'KEEP_WORK':'1','ORACLE_META':str(base.relative_to(ROOT))+'.meta'},
                    emulator_sha256=digest(Path.home()/'Documents/code/ship-your-lua/riscv-lean/lean_emulator/.lake/build/bin/lean_riscv_emulator'))
        for f in ['raw','trace','pcs']:
            shutil.copyfile(work/f,OUT/f'sail-{name}.{f}')
        sail.append(record)
    summary=dict(build_request=req['request_id'],full_gate_request=gate['request_id'],
        focused=counts(focused),broad=counts(broad),csem=dict(cc),sail=sail,
        pins={p:digest(ROOT/p) for p in ['Tcc/Model/Gen7.lean','patches/0003-tcc-c11.patch',
            'scripts/gen_designator_fixtures.py','scripts/test_tcc_c11.py','scripts/test_designator_csem.py',
            'scripts/model_diff.py','scripts/ref_arena.sh','scripts/gen_model_corpus.py',
            '.lake/build/bin/tcc_model','c/build/base.elf']},
        fixtures={str(p.relative_to(ROOT)):digest(p) for p in sorted((ROOT/'build/designator-focused').glob('*.c'))},
        inventory_sha256=digest(ROOT/'build/designator-inventory.txt'))
    (OUT/'validation.json').write_text(json.dumps(summary,indent=2)+'\n')
    print(json.dumps({k:summary[k] for k in ['focused','broad','csem']},indent=2))

if __name__=='__main__':main()
