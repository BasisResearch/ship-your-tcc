#!/usr/bin/env python3
"""Verify portable injected-origin evidence; optionally check the frozen tree."""
import argparse,hashlib,json,tarfile
from pathlib import Path
HERE=Path(__file__).resolve().parent

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source-root',type=Path);a=ap.parse_args()
 pins=json.loads((HERE/'pins.json').read_text())
 for p,h in pins['files'].items():assert sha(HERE/p)==h,p
 manifest=json.loads((HERE/'artifact-manifest.json').read_text());seen=set();data={}
 wanted={f'build/binding-origin-{n}-6/comparison.json' for n in ['controls','shared','raw-component','independent-raw','physical']}
 wanted|={'build/binding-origin-onoff-6/erasure.json','build/binding-origin-fast-6.json',
          'build/worker-build.binding-origin-lean-6.request','build/worker-build.binding-origin-full-1.request',
          'build/worker-build.binding-origin-lean-6.result','build/worker-build.binding-origin-full-1.result',
          'build/worker-build.binding-origin-lean-2.result'}
 with tarfile.open(HERE/'evidence.tar.xz','r|xz') as archive:
  for item in archive:
   assert item.isfile() and item.name in manifest and item.name not in seen,item.name
   f=archive.extractfile(item);h=hashlib.sha256();chunks=[]
   while chunk:=f.read(1<<20):
    h.update(chunk)
    if item.name in wanted:chunks.append(chunk)
   assert h.hexdigest()==manifest[item.name],item.name
   if chunks:data[item.name]=json.loads(b''.join(chunks))
   seen.add(item.name)
 assert seen==manifest.keys() and wanted<=data.keys()
 def result(n):return data[f'build/binding-origin-{n}-6/comparison.json']
 c=result('controls');assert c['exact']==18 and len(c['rows'])==18 and all(r['exact'] for r in c['rows'])
 assert len(c['harness_checks'])==6 and all(c['harness_checks'].values())
 for n,count,other in [('shared',45,11),('raw-component',46,10),('independent-raw',45,11)]:
  r=result(n);assert len(r['rows'])==56 and r['summary']=={'exact':count,'explicit-nonsuccess':other} and r['acceptance']['accepted']
 assert result('independent-raw')['input_mode']=='independent-Raw.checked-cfgTcc-source-preprocessing-and-parsing'
 p=result('physical');assert len(p['rows'])==58 and p['summary']=={'exact':58,'explicit_differences':0}
 assert len(p['harness_checks'])==4 and all(p['harness_checks'].values())
 erasure=data['build/binding-origin-onoff-6/erasure.json'];assert len(erasure['rows'])==15 and all(r['exact'] for r in erasure['rows'])
 assert data['build/binding-origin-fast-6.json']['totals']=={'n':18,'same':18}
 for rid in ['binding-origin-lean-6','binding-origin-full-1']:
  req=data[f'build/worker-build.{rid}.request'];receipt=data[f'build/worker-build.{rid}.result']
  assert req['request_id']==receipt['request_id']==rid and receipt['exit']==0 and receipt['source_hashes_verified']
  assert len(req['sha256'])==3327
 assert data['build/worker-build.binding-origin-lean-2.result']['exit']==1
 if a.source_root:
  req=data['build/worker-build.binding-origin-full-1.request']
  assert all(sha(a.source_root/p)==h for p,h in req['sha256'].items())
 print(f"verified {len(seen)} artifacts; 18 opening controls/6 guards, 45 independent raw, 45 shared, 46 component, 58 origins/4 guards, 15 erasures, 18 arenas; frozen gate exit0")
if __name__=='__main__':main()
