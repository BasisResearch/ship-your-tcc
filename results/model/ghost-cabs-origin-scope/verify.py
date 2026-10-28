#!/usr/bin/env python3
"""Check the model-selected comparison scope, captures and frozen source pins."""
import argparse,gzip,hashlib,json
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--sources',action='store_true');a=ap.parse_args()
    dest=Path(__file__).resolve().parent;root=dest.parents[2]
    pins=json.loads((dest/'pins.json').read_text())
    assert all(sha(dest/p)==h for p,h in pins['artifacts'].items()),'artifact changed'
    for name in ['explicit-build','full-gate']:
        req=json.loads(gzip.decompress((dest/(name+'.request.json.gz')).read_bytes()))
        res=json.loads(gzip.decompress((dest/(name+'.result.json.gz')).read_bytes()))
        assert req['request_id']==res['request_id'] and res['exit']==0 and res['source_hashes_verified']
        if a.sources:assert all((root/p).is_file() and sha(root/p)==h for p,h in req['sha256'].items()),'source changed'
        print(req['request_id'],len(req['sha256']),'verified source pins')
    origin=json.loads((dest/'origins.json').read_text())
    assert origin['acceptance']['accepted'] and origin['summary']==dict(exact=16,explicit_differences=3)
    assert len(origin['harness_checks'])==3 and all(origin['harness_checks'].values())
    fixture=json.loads((dest/'fixtures.json').read_text());assert fixture['acceptance']['accepted']
    raw=json.loads((dest/'raw-strict.json').read_text());assert not raw['acceptance']['accepted']
    erasure=json.loads((dest/'erasure.json').read_text())
    assert len(erasure['rows'])==7 and all(r['equal'] and r['ghost_status']=='ok' and not r['token_errors'] for r in erasure['rows'])
    exits=json.loads((dest/'after-gate-exit-statuses.json').read_text());assert len(exits)==2 and all(r['exit']==0 for r in exits)
    print('scope controls and artifacts verified; three origin/raw agreement gaps remain explicit')
if __name__=='__main__':main()
