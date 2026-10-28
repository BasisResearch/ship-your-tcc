#!/usr/bin/env python3
"""Verify every evidence member and the separately labeled successful checks."""
import argparse,hashlib,json,tarfile
from pathlib import Path
HERE=Path(__file__).resolve().parent
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--source-root',type=Path);a=ap.parse_args()
 pins=json.loads((HERE/'pins.json').read_text())
 for p,h in pins['files'].items():assert sha(HERE/p)==h,p
 manifest=json.loads((HERE/'artifact-manifest.json').read_text());seen=set();data={}
 # Retain only small report/receipt objects in memory; hash every large capture.
 with tarfile.open(HERE/'evidence.tar.xz','r|xz') as archive:
  for item in archive:
   assert item.isfile() and item.name in manifest and item.name not in seen,item.name
   stream=archive.extractfile(item);h=hashlib.sha256();buf=[]
   wanted=item.name.endswith(('/comparison.json','/erasure.json','/arenas.json','.request','.result','/native/on.ghost.json')) or item.name=='build/unevaluated-fast-5.json'
   while chunk:=stream.read(1<<20):
    h.update(chunk)
    if wanted:buf.append(chunk)
   assert h.hexdigest()==manifest[item.name],item.name
   if wanted:data[item.name]=json.loads(b''.join(buf))
   seen.add(item.name)
 assert seen==manifest.keys() and len(seen)==pins['artifacts']
 def report(n):return data[f'build/unevaluated-{n}-5/comparison.json']
 for n,count in [('shared',47),('component',48),('independent',47)]:
  x=report(n);assert len(x['rows'])==56 and x['summary']=={'exact':count,'explicit-nonsuccess':56-count} and x['acceptance']['accepted']
 assert report('independent')['input_mode']=='independent-Raw.checked-cfgTcc-source-preprocessing-and-parsing'
 batch=report('batch-cabs');assert all(batch['checks'].values())
 x=report('focused');assert x['exact_valid']==21 and x['diagnostics']==12 and x['residuals']==['assert-wide'] and all(r['accepted'] for r in x['rows'])
 for r in x['rows']:
  if r['valid']:assert all(v['difference'] is None for v in r['comparisons'].values())
 x=report('authority');assert x['exact']==18 and len(x['harness_checks'])==6 and all(x['harness_checks'].values())
 x=report('physical');assert x['summary']=={'exact':58,'explicit_differences':0} and len(x['harness_checks'])==4 and all(x['harness_checks'].values())
 x=data['build/unevaluated-onoff-5/erasure.json'];assert len(x['rows'])==15 and all(r['exact'] for r in x['rows'])
 assert data['build/unevaluated-fast-5.json']['totals']=={'n':18,'same':18}
 for rid in [pins['explicit_build'],pins['full_gate']]:
  req=data[f'build/worker-build.{rid}.request'];res=data[f'build/worker-build.{rid}.result']
  assert req['request_id']==res['request_id']==rid and res['exit']==0 and res['source_hashes_verified'] and len(req['sha256'])==pins['source_pins']
 assert data['build/worker-build.unevaluated-lean-4.result']['exit']==1
 native=[x for p,x in data.items() if '/unevaluated-syntax-gate.' in p and p.endswith('/native/comparison.json')]
 ghost=[x for p,x in data.items() if '/unevaluated-syntax-gate.' in p and p.endswith('/native/on.ghost.json')]
 assert len(ghost)==1 and ghost[0]['status']=='ok' and ghost[0]['token_errors']==[]
 assert len(native)==1 and native[0]['erasure'] and native[0]['native_arena']['exit']==0 and native[0]['execution']['exit']==0
 arenas=[x for p,x in data.items() if '/unevaluated-syntax-gate.' in p and p.endswith('/native/arenas.json')]
 assert len(arenas)==1 and arenas[0]['totals']=={'n':1,'same':1}
 assert arenas[0]['sail_settings'] is not None
 observations=[r for rs in arenas[0]['results'].values() for r in rs]
 assert len(observations)==1 and observations[0]['sail']['compared_bytes']==4194304 and observations[0]['sail']['steps']==3487048 and observations[0]['sail']['status']=='equal'
 if a.source_root:
  req=data[f"build/worker-build.{pins['full_gate']}.request"]
  assert all(sha(a.source_root/p)==h for p,h in req['sha256'].items())
 print(f'verified {len(seen)} members; 21 complete focused positives, 12 invalid diagnostics, 1 valid residual; 47 shared/48 component/47 independent, 18 authority/6 guards, 58 origins/4 guards, 15 erasures/18 fast arenas; native arena/execution and frozen gate exit0')
if __name__=='__main__':main()
