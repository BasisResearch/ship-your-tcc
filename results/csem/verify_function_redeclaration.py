#!/usr/bin/env python3
"""Verify retained CSEM-10 evidence and its matching final build receipt.
Run from the worktree root. Old binary pins remain historical; current-source
and current-binary pins are checked only against the final observations.
"""
import hashlib
import json
import re
import tarfile
from pathlib import Path

ROOT=Path.cwd()
BASE=ROOT/'results/csem/function-redeclaration'

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_text())

request_id='redecl-gate-43da13328e6a43ac3576'
r=read(BASE/'receipts'/f'{request_id}-request.json')
d=read(BASE/'receipts'/f'{request_id}-result.json')
assert d['request_id']==r['request_id']==request_id and d['exit']==0 and d['source_hashes_verified']
assert all(sha(p)==h for p,h in r['sha256'].items()), 'final frozen source drift'
print('final receipt and',len(r['sha256']),'source hashes verified')

for archive in BASE.rglob('streams.tar.xz'):
    manifest=read(archive.parent/'streams.manifest.json')
    with tarfile.open(archive) as tar:
        assert set(tar.getnames())==set(manifest)
        for name,m in manifest.items():
            b=tar.extractfile(name).read()
            assert len(b)==m['size'] and hashlib.sha256(b).hexdigest()==m['sha256']
print('all archived streams and original member hashes verified')

for path in ['after/results.json','library-recheck/results.json','sail/results.json','native/result.json']:
    data=read(BASE/path)
    assert all(sha(p)==h for p,h in data['pins'].items()), f'{path}: input/binary drift'

after=read(BASE/'after/results.json')['rows']
assert len(after)==32 and all(x['same_cabs'] for x in after)
assert sum(x['status']=='PASS' for x in after)==30
assert all(x['pass'] or x.get('expected_gap',False) for x in after)
assert sum(not x['invalid_constraint'] for x in after)==28
lib=read(BASE/'library-recheck/results.json')['rows']
assert len(lib)==24 and all(x['status']=='PASS' for x in lib)
assert sum(x.get('semantic_consumer_checks',0) for x in lib)==5580
sail=read(BASE/'sail/results.json')['rows']
assert len(sail)==32
for row in sail:
    if row.get('invalid_constraint'):
        assert row['tcc_compile_exit']!=0
    else:
        for engine in ['tcc','gcc']:
            v=row[engine];assert v['exit']==0 and v['steps']>0
            assert sha(v['image'])==v['image_sha256']
native=read(BASE/'native/result.json')
assert native['exit']==0 and native['steps']==[3382570,3382595,3382698] and native['isa_exit']==0
trace=read(BASE/'trace.json')
assert trace['ocaml_mine_id']==483
for row in trace['symbols']:
    assert sha(row['quotation'])==row['quotation_sha256']
    ids=sorted(set(map(int,re.findall(r'_root_\.sym.Symbol "[^"]*" (\d+) \(_root_\.symbol_description.SD_Id "mine"\)',Path(row['quotation']).read_text()))))
    assert ids==row['canonical_mine_ids']==([0] if row['phase']=='before' else [1])
assert 'Ran 11 tests' in (BASE/'checks/redecl-raw-tests.log').read_text()
print('32 positioned Cabs, focused outcomes/gaps, 24 library cases/5580 consumers, 28 cross-program Sail pairs and one full-compiler Sail run verified')
manifest=BASE/'evidence.sha256.json'
if manifest.exists():
    entries=read(manifest)
    assert all(sha(p)==h for p,h in entries.items()),'evidence manifest drift'
    print(len(entries),'final evidence hashes verified')
