#!/usr/bin/env python3
"""Verify combined alignment evidence; --sources checks the frozen integration."""
from pathlib import Path
import argparse,gzip,hashlib,json,tarfile

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--sources',action='store_true');a=ap.parse_args()
 here=Path(__file__).resolve().parent;root=here.parents[2]
 H=lambda b:hashlib.sha256(b).hexdigest()
 read=lambda n:json.loads((here/(n+'.json')).read_text())
 pins=read('pins');assert all(H((here/p).read_bytes())==h for p,h in pins['artifacts'].items())
 seen={}
 with tarfile.open(here/'captures.tar.xz','r|xz') as archive:
  for item in archive:
   assert item.isfile() and item.name not in seen
   f=archive.extractfile(item);h=hashlib.sha256()
   while chunk:=f.read(1<<20):h.update(chunk)
   seen[item.name]=h.hexdigest()
 assert seen==pins['archive_members']
 q=json.loads(gzip.decompress((here/'gate.request.json.gz').read_bytes()));r=json.loads(gzip.decompress((here/'gate.result.json.gz').read_bytes()))
 assert q['request_id']==r['request_id'] and r['exit']==0 and r['source_hashes_verified']
 if a.sources:assert all(H((root/p).read_bytes())==h for p,h in q['sha256'].items())
 alignment=read('alignment');assert alignment['counts']=={'COMPILER_ARRAY_DECAY_DISCREPANCY':1,'CONSTRAINT_DIAGNOSTIC':12,'DEFINED_ZERO':53,'FUNCTION_TYPE_POLICY_OBSTRUCTION':2,'OCAML_PLACEMENT_LIMITATION':3}
 assert len(alignment['rows'])==71 and all(x['full_text_cabs'] for x in alignment['rows'])
 assert sum(x['full_ocaml_cabs'] for x in alignment['rows'])==68
 authority=read('authority');assert authority['exact']==18 and len(authority['rows'])==18 and all(r['exact'] for r in authority['rows'])
 assert len(authority['harness_checks'])==6 and all(authority['harness_checks'].values())
 for n,k in [('shared',45),('raw-component',46),('independent',45)]:
  d=read(n);assert d['summary']=={'exact':k,'explicit-nonsuccess':56-k} and d['acceptance']['accepted']
 assert read('independent')['input_mode']=='independent-Raw.checked-cfgTcc-source-preprocessing-and-parsing'
 physical=read('physical');assert physical['summary']=={'exact':58,'explicit_differences':0} and all(physical['harness_checks'].values())
 erasure=read('erasure');assert len(erasure['rows'])==15 and all(r['exact'] for r in erasure['rows'])
 for n,k in [('arenas',18),('invocation',1)]:assert read(n)['totals']=={'n':k,'same':k}
 for row in read('arena-index'):
  p=row['archive_prefix'];assert seen[p+'/model.arena']==seen[p+'/reference.arena']==row['arena_sha256']
 print(f'Verified {len(seen)} captured files, {len(q["sha256"])} gate inputs, 71 classified alignment cases and independent origin/arena regressions; universal contracts remain open')
if __name__=='__main__':main()
