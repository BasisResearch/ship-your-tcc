#!/usr/bin/env python3
"""Verify the frozen ghost evidence; optionally compare the exact source tree."""
import argparse, gzip, hashlib, json
from pathlib import Path

def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--sources',action='store_true');a=ap.parse_args()
    dest=Path(__file__).resolve().parent;root=dest.parents[2]
    pins=json.loads((dest/'pins.json').read_text())
    bad=[p for p,h in pins['artifacts'].items() if sha(dest/p)!=h]
    assert not bad,('artifact mismatch',bad)
    for prefix in ['explicit-build','full-gate']:
        request=json.loads(gzip.decompress((dest/(prefix+'.request.json.gz')).read_bytes()))
        receipt=json.loads(gzip.decompress((dest/(prefix+'.result.json.gz')).read_bytes()))
        assert request['request_id']==receipt['request_id'] and receipt['exit']==0
        assert receipt['source_hashes_verified']
        if a.sources:
            bad=[p for p,h in request['sha256'].items() if not (root/p).is_file() or sha(root/p)!=h]
            assert not bad,('frozen sources changed',bad)
        print(request['request_id'],len(request['sha256']),'frozen inputs; passing receipt')
    fixture=json.loads((dest/'fixtures.json').read_text())
    assert fixture['acceptance']['accepted'] and fixture['summary']=={'exact':44,'explicit-nonsuccess':12}
    origins=json.loads((dest/'origins.json').read_text())
    assert origins['acceptance']['accepted'] and origins['summary']=={'exact':16,'explicit_differences':3}
    raw=json.loads((dest/'raw-strict.json').read_text())
    assert not raw['acceptance']['accepted'] and raw['summary']=={'explicit-nonsuccess':3}
    erasure=json.loads((dest/'erasure.json').read_text())
    assert len(erasure['rows'])==7 and all(r['equal'] and r['ghost_status']=='ok' and not r['token_errors'] for r in erasure['rows'])
    exits=json.loads((dest/'after-gate-exit-statuses.json').read_text())
    assert len(exits)==2 and all(r['exit']==0 for r in exits)
    print('artifacts verified; explicit raw/coverage failures remain failures')
if __name__=='__main__':main()
