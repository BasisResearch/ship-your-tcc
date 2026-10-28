#!/usr/bin/env python3
"""Verify the frozen library-identity candidate without rewriting evidence."""
from pathlib import Path
import hashlib,json
ROOT=Path(__file__).resolve().parents[3]
D=ROOT/'results/csem/library-identity'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def read(p):return json.loads(p.read_text())
q=read(D/'gate-request.json');r=read(D/'gate-result.json')
assert q['request_id']==r['request_id'] and r['exit']==0 and r['source_hashes_verified']
assert 'check: OK' in (D/'gate.log').read_text()
assert all(sha(ROOT/p)==h for p,h in q['sha256'].items())
assert all(sha(D/p)==h for p,h in read(D/'manifest.json').items())
pins=read(D/'pins.json');assert all(sha(ROOT/p)==h for p,h in pins['binaries'].items())
rows=read(D/'semantic/results.json')['rows']
assert sum(r['status']=='PASS' for r in rows)==22
assert sum(r['status']=='INHERITED_FUNCTION_POINTER_GAP' for r in rows)==2
assert sum(r['semantic_consumer_checks'] for r in rows)==5580
cabs=read(D/'cabs/results.json')['rows'];assert len(cabs)==24 and all(r['status']=='AGREE' for r in cabs)
p=read(D/'pointer-adopted/pins.json');prior=read(D/'pointer-adopted/existing-integration-pin.json')
assert p['sha256'] in json.dumps(prior['binaries']) and p['exit']==1
assert 'null function pointer' in (D/'pointer-adopted/neutral.stdout').read_text()
neutral=next(r for r in read(D/'pointer-pristine/results.json')['rows'] if r['case']=='redecl-pointer-neutral')
assert neutral['oracle_exit']==neutral['gcc_exit']==0
assert sha(D/'pointer-adopted/neutral.c')==neutral['source_sha256']
assert 'Ran 11 tests' in (D/'library-existing-final.log').read_text()
assert 'Core quotation visitor matches generator' in (D/'library-visitor-check.log').read_text()
print('library identity: source/evidence/binary pins verified; full gate; 22 checks + 2 local-fork gaps; 5580 consumer probes; 24 positioned Cabs agreements')
