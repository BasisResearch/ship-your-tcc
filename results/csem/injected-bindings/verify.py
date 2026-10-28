#!/usr/bin/env python3
"""Verify the focused frozen evidence without printing source hash manifests."""
import argparse,hashlib,json,tarfile
from pathlib import Path
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
def sha(data):return hashlib.sha256(data).hexdigest()
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--sources',action='store_true');a=ap.parse_args()
 manifest=json.loads((HERE/'evidence.manifest.json').read_text());archive=HERE/'evidence.tar.gz'
 assert sha(archive.read_bytes())==manifest['archive_sha256']
 for name,digest in manifest['companions'].items():assert sha((HERE/name).read_bytes())==digest,name
 with tarfile.open(archive) as tar:
  members={m.name:m for m in tar.getmembers()};assert len(members)==len(tar.getmembers())
  assert set(members)==set(manifest['members'])
  for name,m in members.items():
   assert m.isfile() and sha(tar.extractfile(m).read())==manifest['members'][name],name
  def read(name):return tar.extractfile(name).read()
  def data(name):return json.loads(read(name))
  final=manifest['final_gate'];req=data('build/worker-build.'+final+'.request');res=data('build/worker-build.'+final+'.result')
  assert req['request_id']==res['request_id']==final and res['exit']==0 and res['source_hashes_verified']
  matrix=data(manifest['matrix']+'/report.json');rows=matrix['rows']
  assert len(rows)==40 and all(x['cabs_equal'] and x['full_binding_records'] for x in rows)
  expected={'defined':('PASS',13),'constraint':('PASS',3),'excluded':('PASS',6),'no-entry':('NO_ENTRY',1),'runtime-gap':('RUNTIME_GAP',17)}
  for kind,(status,count) in expected.items():
   rs=[x for x in rows if x['expected']==kind];assert len(rs)==count and all(x['status']==status for x in rs)
  for suite in [manifest['openings'],manifest['library']]:
   r=data(suite+('/results.json' if suite==manifest['library'] else '/report.json'))
   assert all(x['status']=='PASS' for x in r['rows'])
  r=data(manifest['redeclaration']+'/results.json')
  assert len(r['rows'])==32 and sum(x['status']=='PASS' for x in r['rows'])==31
  assert next(x for x in r['rows'] if x['case']=='invalid-object')['expected_gap']
  native=data(manifest['native']+'/report.json');assert len(native)==3
  for case in native:
   stem=manifest['native']+'/'+case['case']
   assert sha(read(stem+'/prog.c'))==case['source_sha256']
   sail=next(x for x in case['observations'] if x['engine']=='tcc-sail')
   assert sail['exit']==0 and sha(read(stem+'/prog.elf'))==sail['elf_sha256']
  if a.sources:
   assert all(sha((ROOT/p).read_bytes())==h for p,h in req['sha256'].items()),'final gate source drift'
   print('verified',len(req['sha256']),'final frozen source pins')
 print('verified',len(manifest['members']),'evidence members; full gate and focused observations pass; recorded gaps remain separate')
if __name__=='__main__':main()
