#!/usr/bin/env python3
"""Verify combined unevaluated-syntax evidence; --sources checks the exact frozen integration."""
from pathlib import Path
import argparse,gzip,hashlib,json,tarfile

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--sources',action='store_true');a=ap.parse_args()
 here=Path(__file__).resolve().parent;root=here.parents[2]
 H=lambda b:hashlib.sha256(b).hexdigest()
 read=lambda n:json.loads((here/(n+'.json')).read_text())
 pins=read('pins');assert all(H((here/p).read_bytes())==h for p,h in pins['artifacts'].items())
 index=read('arena-index');native=[r for r in index if r['kind']=='native'];assert len(native)==1
 prefix=native[0]['archive_prefix']+'/';seen={};data={}
 with tarfile.open(here/'captures.tar.xz','r|xz') as archive:
  for item in archive:
   assert item.isfile() and item.name not in seen
   f=archive.extractfile(item);h=hashlib.sha256();parts=[]
   while chunk:=f.read(1<<20):
    h.update(chunk)
    if item.name.startswith(prefix):parts.append(chunk)
   seen[item.name]=h.hexdigest()
   if item.name.startswith(prefix):data[item.name]=b''.join(parts)
 assert seen==pins['archive_members']
 q=json.loads(gzip.decompress((here/'gate.request.json.gz').read_bytes()));r=json.loads(gzip.decompress((here/'gate.result.json.gz').read_bytes()))
 assert q['request_id']==r['request_id'] and r['exit']==0 and r['source_hashes_verified']
 if a.sources:assert all(H((root/p).read_bytes())==h for p,h in q['sha256'].items())
 focused=read('focused');assert focused['exact_valid']==21 and focused['diagnostics']==12 and focused['residuals']==['assert-wide'] and all(r['accepted'] for r in focused['rows'])
 execution=read('native-execution');assert execution['erasure'] and execution['native_arena']['exit']==0 and execution['execution']['exit']==0
 authority=read('authority');assert authority['exact']==18 and len(authority['rows'])==18 and all(r['exact'] for r in authority['rows'])
 assert len(authority['harness_checks'])==6 and all(authority['harness_checks'].values())
 for n,k in [('shared',47),('raw-component',48),('independent',47)]:
  d=read(n);assert d['summary']=={'exact':k,'explicit-nonsuccess':56-k} and d['acceptance']['accepted']
 assert read('independent')['input_mode']=='independent-Raw.checked-cfgTcc-source-preprocessing-and-parsing'
 physical=read('physical');assert physical['summary']=={'exact':58,'explicit_differences':0} and all(physical['harness_checks'].values())
 erasure=read('erasure');assert len(erasure['rows'])==15 and all(r['exact'] for r in erasure['rows'])
 for n,k in [('arenas',18),('native',1),('invocation',1)]:assert read(n)['totals']=={'n':k,'same':k}
 for row in index:
  p=row['archive_prefix'];assert seen[p+'/model.arena']==seen[p+'/reference.arena']==row['arena_sha256']
 report=next(iter(read('native')['results'].values()))[0]['sail']
 assert report['status']=='equal' and report['compared_bytes']==4194304 and report['steps']==3487048
 model=data[prefix+'model.arena'];assert H(model)==report['model_arena_sha256']
 head=model.split(b'\n',5);assert len(head)==6 and head[0]==b'TCCARENA 1' and int(head[4].split()[1],16)==len(head[5])
 arena=gzip.decompress(data[prefix+'capture/arena.bin.gz']);assert arena==head[5].ljust(4194304,b'\0') and H(arena)==report['arena_sha256']
 cut=json.loads(data[prefix+'capture/cut.json']);assert H(data[prefix+'capture/cut.json'])==report['capture_manifest_sha256']
 for name,info in cut['files'].items():
  key=prefix+'capture/'+name;b=data[key] if key in data else gzip.decompress(data[key+'.gz'])
  assert len(b)==info['bytes'] and H(b)==info['sha256']
 print(f'Verified {len(seen)} captured files, {len(q["sha256"])} gate inputs and the complete native arena; universal contracts remain open')
if __name__=='__main__':main()
