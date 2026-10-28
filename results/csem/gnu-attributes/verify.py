#!/usr/bin/env python3
"""Verify GNU checkpoint artifacts and, optionally, its exact source snapshot."""
import argparse,collections,gzip,hashlib,json,tarfile
from pathlib import Path

def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--sources',action='store_true');args=ap.parse_args()
    here=Path(__file__).resolve().parent;root=here.parents[2]
    pins=json.loads((here/'pins.json').read_text())
    assert all(sha(here/p)==h for p,h in pins['artifacts'].items())
    with tarfile.open(here/'checkpoint/captures.tar.gz') as t:
        actual={m.name:hashlib.sha256(t.extractfile(m).read()).hexdigest() for m in t.getmembers() if m.isfile()}
    assert actual==pins['archive_members']
    req=json.loads(gzip.decompress((here/'checkpoint/full-gate.request.json.gz').read_bytes()))
    res=json.loads(gzip.decompress((here/'checkpoint/full-gate.result.json.gz').read_bytes()))
    assert req['request_id']==res['request_id'] and res['exit']==0 and res['source_hashes_verified']
    if args.sources:assert all(sha(root/p)==h for p,h in req['sha256'].items())
    rows=json.loads((here/'checkpoint/semantics.json').read_text())['rows']
    assert collections.Counter(r['status'] for r in rows)=={'PASS':23,'DIAGNOSTIC':8,'SEMANTIC_DIAGNOSTIC':3}
    headers=json.loads((here/'checkpoint/headers.json').read_text())['rows']
    assert len(headers)==6 and all(r['status']=='PARSE_PASS' and r['asm_bindings']==21 for r in headers)
    print(f'Verified {len(actual)} capture members and {len(req["sha256"])} final gate inputs; semantic/coverage gaps retained')
if __name__=='__main__':main()
