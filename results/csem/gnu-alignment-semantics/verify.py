#!/usr/bin/env python3
"""Verify portable alignment evidence and unchanged historical source controls."""
import hashlib
import json
from pathlib import Path
import tarfile

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]

def sha(data):return hashlib.sha256(data).hexdigest()

def archive(path, expected):
    assert sha(path.read_bytes())==expected['archive_sha256'],path
    with tarfile.open(path) as t:
        actual={f.name:sha(t.extractfile(f).read()) for f in t.getmembers() if f.isfile()}
    assert actual==expected['members_sha256'],path
    return actual

def main():
    m=json.loads((HERE/'manifest.json').read_text())
    members=archive(HERE/'observations.tar.gz',m)
    for name,h in m['artifacts'].items():assert sha((HERE/name).read_bytes())==h,name
    old=ROOT/'results/model/typedef-alignment'
    original=json.loads((old/'manifest.json').read_text())
    archive(old/'observations.tar.gz',original)
    srcs={n.removeprefix('semantic-inputs/'):h for n,h in original['members_sha256'].items() if n.startswith('semantic-inputs/') and n.endswith('.c')}
    assert len(srcs)==26
    for n,h in srcs.items():assert sha((ROOT/'c/tests/csem-alignment'/n).read_bytes())==h,n
    arrays=ROOT/'results/csem/array-constraints-2026-10-28'
    ar=json.loads((arrays/'pins.json').read_text())
    for name,h in ar['artifacts'].items():assert sha((arrays/name).read_bytes())==h,name
    with tarfile.open(arrays/'captures.tar.gz') as t:
        actual={f.name:sha(t.extractfile(f).read()) for f in t.getmembers() if f.isfile()}
    assert actual==ar['archive_members'] and len(actual)==173
    control={n.split('/')[-1]:h for n,h in actual.items() if n.startswith('array-constraint-review-final/') and n.endswith('.c')}
    assert len(control)==10
    for n,h in control.items():assert sha((ROOT/'c/tests/csem-alignment'/('array-'+n)).read_bytes())==h,n
    r=json.loads((HERE/'report.json').read_text());rows={x['case']:x for x in r['rows']}
    assert len(rows)==71
    assert r['counts']=={'COMPILER_ARRAY_DECAY_DISCREPANCY':1,'CONSTRAINT_DIAGNOSTIC':12,'DEFINED_ZERO':53,'FUNCTION_TYPE_POLICY_OBSTRUCTION':2,'OCAML_PLACEMENT_LIMITATION':3}
    for name in ['deref','unary-types','common-types','expression-local','expression-global','expression-parent','expression-array','expression-address']:
        assert rows[name]['status']=='DEFINED_ZERO',name
    assert all(x['full_text_cabs'] for x in rows.values())
    for x in rows.values():
        if x['status'] in ['DEFINED_ZERO','CONSTRAINT_DIAGNOSTIC','FUNCTION_TYPE_POLICY_OBSTRUCTION','COMPILER_ARRAY_DECAY_DISCREPANCY']:assert x['full_ocaml_cabs'],x['case']
        if x['invalid']:assert x['status']=='CONSTRAINT_DIAGNOSTIC' and x['ocaml_constraint_diagnostic'],x['case']
    with tarfile.open(HERE/'observations.tar.gz') as t:
        def read(n):return json.loads(t.extractfile(n).read())
        req=read('receipts/worker-build.alignsem-full-1.request')
        res=read('receipts/worker-build.alignsem-full-1.result')
        assert req['request_id']==res['request_id']=='alignsem-full-1'
        assert res['exit']==0 and res['source_hashes_verified']
        bad=[p for p,h in req['sha256'].items() if sha((ROOT/p).read_bytes())!=h]
        assert not bad,bad
        for n in ['deref','unary-types','common-types']:
            for e in ['tcc','gcc']:
                rec=read(f'sail/{n}-{e}/record.json');assert rec['exit']==0
        assert read('array-decay-sail/tcc/record.json')['exit']==32
        assert read('array-decay-sail/gcc/record.json')['exit']==8
    print(f'Verified {len(members)} archive members, {len(req["sha256"])} final source pins, 26 original inputs and unchanged ten-case historical matrix')
if __name__=='__main__':main()
