#!/usr/bin/env python3
"""Verify retained observations; finite equality is not compiler refinement."""
import argparse
import hashlib
import json
from pathlib import Path
import tarfile
import tempfile
import sys

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
def sha(data):return hashlib.sha256(data).hexdigest()
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--live',action='store_true');a=ap.parse_args()
    manifest=json.loads((HERE/'manifest.json').read_text())
    path=HERE/'observations.tar.xz'
    assert sha(path.read_bytes())==manifest['archive_sha256']
    with tarfile.open(path) as t:
        actual={f.name:sha(t.extractfile(f).read()) for f in t.getmembers() if f.isfile()}
        assert actual==manifest['members_sha256']
        def data(n):return t.extractfile(n).read()
        def read(n):return json.loads(data(n))
        for req_id in ['arraydecay-lean-1','arraydecay-full-1']:
            req=read('receipts/worker-build.'+req_id+'.request');res=read('receipts/worker-build.'+req_id+'.result')
            assert req['request_id']==res['request_id']==req_id
            assert res['exit']==0 and res['source_hashes_verified']
            for name,h in req['sha256'].items():
                assert sha(data('source/'+req_id+'/'+name))==h,name
                if a.live and req_id=='arraydecay-full-1':assert sha((ROOT/name).read_bytes())==h,name
        alignment=read('alignment/report.json')
        assert alignment['counts']=={'CONSTRAINT_DIAGNOSTIC':12,'DEFINED_ZERO':54,'FUNCTION_TYPE_POLICY_OBSTRUCTION':2,'OCAML_PLACEMENT_LIMITATION':3}
        repro=next(row for row in alignment['rows'] if row['case']=='array-alias-decay')
        assert repro['status']=='DEFINED_ZERO' and repro['gcc_exit']==repro['tcc_exit']==0
        assert data('alignment/array-alias-decay/input.c')==data('before/array-alias-decay.c')
        assert read('before/array-decay-sail/tcc/record.json')['exit']==32
        assert read('before/array-decay-sail/gcc/record.json')['exit']==8
        final=read('gate-decay/report.json');fast=read('gate-decay/fast.json')
        assert final['fast_totals']['same']==29
        rows=[r for group in fast['results'].values() for r in group]
        assert len(rows)==34 and sum(r['result']=='same-refusal' for r in rows)==5
        assert all(r['result'] in ['same','same-refusal'] for r in rows)
        for row in rows:
            stem=Path(row['name']).stem
            folder='fast-arenas/'+stem+'/'
            if row['result']=='same':
                ref=data(folder+'reference.arena');model=data(folder+'model.arena')
                assert ref==model
                header,body=ref.split(b'\n',5)[:5],ref.split(b'\n',5)[5]
                assert header[0]==b'TCCARENA 1'
                assert int(header[4].split()[1],16)==len(body)<=4194304
                assert data(folder+'reference.status')==b'ran\n'
            else:assert data(folder+'reference.status').startswith(b'exit ')
        assert all(r['accepted'] for r in final['gcc']) and len(final['gcc'])==34
        assert b'C11: 210 positive cases, 27 negative cases; all passed' in data('logs/arraydecay-c11-2.log')
        assert len(final['native'])==3 and all(row['exit']==0 for row in final['native'])
        sys.path.insert(0,str(ROOT/'scripts'))
        from compare_sail_arena import compare
        native=json.loads((HERE/'native-inventory.json').read_text())
        assert len(native)==3
        with tempfile.TemporaryDirectory() as tmp:
            tmp=Path(tmp)
            for name in actual:
                if name.startswith('native/'):
                    dst=tmp/name;dst.parent.mkdir(parents=True,exist_ok=True);dst.write_bytes(data(name))
            for n in native:
                capture=tmp/'native'/n['case']/n['directory']
                import gzip
                model=capture/'model.arena';model.write_bytes(gzip.decompress((capture/'model.arena.gz').read_bytes()))
                observed=compare(capture,model)
                assert observed['compared_bytes']==4194304
                assert observed['source_sha256']==sha((capture/'source.c').read_bytes())
                assert observed['source_sha256']==sha(data('gate-decay/inputs/'+n['case']+'.c'))
                assert observed['arena_sha256']==n['comparison']['arena_sha256']
                execution=read('gate-decay/'+n['case']+'-native/comparison.json')
                assert execution['execution']['exit']==0 and execution['erasure']
                assert data('gate-decay/'+n['case']+'-native/execution.stdout')==b''
    for name,h in manifest['artifacts'].items():assert sha((HERE/name).read_bytes())==h,name
    print(f'Verified {len(actual)} members, 29 fast arenas, 5 separate refusals, 3 complete native arenas/executions and unchanged wrong32/independent8 captures')
if __name__=='__main__':main()
