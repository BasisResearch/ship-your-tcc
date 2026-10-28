#!/usr/bin/env python3
"""Verify combined GNU evidence; --sources requires its exact source snapshot."""
import argparse,collections,gzip,hashlib,json,tarfile
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--sources',action='store_true');args=ap.parse_args()
    here=Path(__file__).resolve().parent;root=here.parents[2]
    def read(name):return json.loads((here/(name+'.json')).read_text())
    pins=read('pins');assert all(sha(here/p)==h for p,h in pins['artifacts'].items())
    with tarfile.open(here/'captures.tar.gz') as t:
        actual={m.name:hashlib.sha256(t.extractfile(m).read()).hexdigest() for m in t.getmembers() if m.isfile()}
    assert actual==pins['archive_members']
    q=json.loads(gzip.decompress((here/'gate2.request.json.gz').read_bytes()))
    r=json.loads(gzip.decompress((here/'gate2.result.json.gz').read_bytes()))
    assert q['request_id']==r['request_id'] and r['exit']==0 and r['source_hashes_verified']
    if args.sources:assert all(sha(root/p)==h for p,h in q['sha256'].items())
    assert collections.Counter(x['status'] for x in read('semantics')['rows'])=={'PASS':23,'DIAGNOSTIC':8,'SEMANTIC_DIAGNOSTIC':3}
    assert len(read('headers')['rows'])==6 and all(x['status']=='PARSE_PASS' and x['asm_bindings']==21 for x in read('headers')['rows'])
    assert read('fixtures')['summary']=={'exact':45,'explicit-nonsuccess':11}
    assert read('raw-ghost')['summary']=={'exact':3}
    assert read('attribute')['acceptance']['accepted'] and read('attribute')['summary']=={'exact':1}
    assert read('origins')['summary']=={'exact':17,'explicit_differences':2}
    assert all(read('origins')['harness_checks'].values())
    assert read('arenas')['totals']=={'n':7,'same':7}
    rows=read('erasure')['rows'];assert len(rows)==7 and all(x['equal'] and x['runs'][1]['ghost_status']=='ok' for x in rows)
    for name in ['fixtures','origins','raw-ghost','attribute']:
        assert read(name)['binary_sha256']==pins['binary_pins']['.lake/build/bin/tcc_ghost']
    print(f'Verified {len(actual)} capture members and {len(q["sha256"])} final gate inputs; remaining gaps explicit')
if __name__=='__main__':main()
