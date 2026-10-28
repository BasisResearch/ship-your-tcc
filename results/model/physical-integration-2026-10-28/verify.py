#!/usr/bin/env python3
"""Verify retained physical-origin integration captures; --sources checks this snapshot."""
from pathlib import Path
import argparse,gzip,hashlib,json,tarfile

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--sources',action='store_true');a=ap.parse_args()
 here=Path(__file__).resolve().parent;root=here.parents[2]
 H=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 read=lambda n:json.loads((here/(n+'.json')).read_text())
 pins=read('pins');assert all(H(here/p)==h for p,h in pins['artifacts'].items())
 with tarfile.open(here/'captures.tar.xz') as t:
  actual={m.name:hashlib.sha256(t.extractfile(m).read()).hexdigest() for m in t.getmembers() if m.isfile()}
 assert actual==pins['archive_members']
 q=json.loads(gzip.decompress((here/'gate.request.json.gz').read_bytes()));r=json.loads(gzip.decompress((here/'gate.result.json.gz').read_bytes()))
 assert q['request_id']==r['request_id'] and r['exit']==0 and r['source_hashes_verified']
 if a.sources:assert all(H(root/p)==h for p,h in q['sha256'].items())
 assert read('origins')['summary']=={'exact':58,'explicit_differences':0}
 assert all(read('origins')['harness_checks'].values())
 assert read('shared')['summary']=={'exact':45,'explicit-nonsuccess':11}
 assert read('raw')['summary']=={'exact':46,'explicit-nonsuccess':10}
 for name in ['origins','shared','raw']:
  assert read(name)['acceptance']['accepted']
  assert read(name)['binary_sha256']==pins['binary_pins']['.lake/build/bin/tcc_ghost']
 assert read('arenas')['totals']=={'n':15,'same':15}
 assert len(read('erasure')['rows'])==15 and all(x['exact'] for x in read('erasure')['rows'])
 for row in read('arena-index'):
  stem=Path(row['source']).stem
  assert actual[f'arenas/{stem}/model.arena']==actual[f'arenas/{stem}/reference.arena']==row['arena_sha256']
  assert actual[f'arenas/{stem}/source.c']==row['source_sha256']
 print(f'Verified {len(actual)} captures and {len(q["sha256"])} gate inputs; proof obligations remain open')
if __name__=='__main__':main()
