#!/usr/bin/env python3
"""Verify portable captures; --sources additionally requires the frozen tree."""
import argparse,hashlib,json,tarfile
from pathlib import Path
ap=argparse.ArgumentParser();ap.add_argument('--sources',action='store_true');a=ap.parse_args()
base=Path(__file__).resolve().parent;root=base.parents[2]
def sha(b):return hashlib.sha256(b).hexdigest()
pins=json.loads((base/'pins.json').read_text());archive=base/'captures.tar.gz'
assert sha(archive.read_bytes())==pins['archive_sha256']
with tarfile.open(archive,'r:gz') as t:
 members={m.name:m for m in t if m.isfile()};assert set(members)==set(pins['members'])
 for name,m in members.items():assert sha(t.extractfile(m).read())==pins['members'][name],name
 def data(n):return json.loads(t.extractfile(members[n]).read())
 for n in ['physical-1','physical-2','physical-3','physical-4','physical-5','physical-gate-1']:
  req=data(f'build/worker-build.{n}.request');res=data(f'build/worker-build.{n}.result')
  assert req['request_id']==res['request_id']==n and res['source_hashes_verified']
  assert res['exit']==(1 if n=='physical-1' else 0),(n,res['exit'])
 origin=data('build/physical-final/origins/comparison.json')
 assert origin['summary']=={'exact':58,'explicit_differences':0} and origin['acceptance']['accepted']
 assert len(origin['acceptance']['required_exact'])==58 and len(origin['harness_checks'])==4 and all(origin['harness_checks'].values())
 for n,count in [('shared-cabs',45),('raw-cabs',46)]:
  r=data(f'build/physical-final/{n}/comparison.json');assert r['summary']=={'exact':count,'explicit-nonsuccess':56-count} and r['acceptance']['accepted']
 onoff=data('build/physical-final/onoff/erasure.json');assert len(onoff['rows'])==15 and all(r['exact'] for r in onoff['rows'])
 fast=data('build/physical-final/fast-arenas.json');assert fast['totals']=={'n':15,'same':15} and all(r['result']=='same' for rs in fast['results'].values() for r in rs)
 assert onoff['binary_sha256']==fast['pins']['.lake/build/bin/tcc_model']==pins['binary_hashes']['.lake/build/bin/tcc_model']
 assert origin['binary_sha256']==pins['binary_hashes']['.lake/build/bin/tcc_ghost']
 # Complete retained arena files, independent reference and on/off, must agree.
 captures=data('build/physical-final/fast-captures/captures.json');assert len(captures)==15
 for r in captures:
  stem=Path(r['source']).stem;prefix=f'build/physical-final/fast-captures/{stem}'
  assert t.extractfile(members[prefix+'.model.arena']).read()==t.extractfile(members[prefix+'.reference.arena']).read(),stem
 if a.sources:
  for mapping in [pins['source_hashes'],pins['supplementary_source_hashes']]:
   for p,h in mapping.items():assert sha((root/p).read_bytes())==h,p
  for p,h in pins['binary_hashes'].items():assert sha((root/p).read_bytes())==h,p
print('verified',len(pins['members']),'artifacts; gate, 58 origins, 4 omission guards, 45/46 Cabs components, 15 on/off and 15 independent arenas')
