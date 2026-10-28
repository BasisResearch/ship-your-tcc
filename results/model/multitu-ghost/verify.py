#!/usr/bin/env python3
"""Verify every retained member, frozen receipt, ordered TU and full arena pair."""
from pathlib import Path
import argparse,hashlib,json,tarfile
H=lambda b:hashlib.sha256(b).hexdigest()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--sources',action='store_true');a=ap.parse_args()
 here=Path(__file__).resolve().parent;root=here.parents[2]
 manifest=json.loads((here/'manifest.json').read_text());archive=here/'evidence.tar.xz'
 assert H(archive.read_bytes())==manifest['archive_sha256']
 with tarfile.open(archive) as t:
  entries=t.getmembers();assert all(x.isfile() for x in entries)
  files={x.name:t.extractfile(x).read() for x in entries}
 assert len(files)==len(entries) and {p:H(b) for p,b in files.items()}==manifest['members_sha256']
 read=lambda p:json.loads(files[p])
 req=read('receipts/worker-build.multitu-full-1.request');res=read('receipts/worker-build.multitu-full-1.result')
 assert req['request_id']==res['request_id']=='multitu-full-1' and res['exit']==0 and res['source_hashes_verified']
 assert files['receipts/worker-build-multitu-full-1.log'].rstrip().endswith(b'check: OK')
 requests=[p for p in files if p.startswith('receipts/worker-build.') and p.endswith('.request')]
 for p in requests:
  for name,h in read(p)['sha256'].items():assert H(files['sources/'+h])==h,(p,name)
 if a.sources:
  assert all(H((root/p).read_bytes())==h for p,h in req['sha256'].items())
 report=read('captures/multitu-final/comparison.json');arena_report=read('captures/multitu-fast/report.json')
 if a.sources:assert all(H((root/p).read_bytes())==h for p,h in report['source_hashes'].items())
 assert report['source_hashes']==arena_report['source_hashes']
 for p,h in report['source_hashes'].items():
  assert H(files['sources/'+h])==h
  if p in req['sha256']:assert req['sha256'][p]==h
 assert report['runtime_key']==arena_report['runtime_key'] and report['image_symbols_sha256']==H(files['runtime/image.syms'])
 assert all(report['guards'].values()) and len(report['guards'])>=6
 total=0;checked=0;arena_pairs=0
 for row in report['rows']:
  case=row['case'];prefix='captures/multitu-final/'+case+'/'
  bundle=read(prefix+'on.ghost.json');assert len(bundle['units'])==row['unit_count']
  assert row['erasure']
  for suffix in ['stdout','stderr','console']:
   assert files[prefix+'on.'+suffix]==files[prefix+'off.'+suffix]
  expected=row['expected_units']
  assert len(bundle['units'])==(1 if case=='earlier-error' else 2)
  for i,unit in enumerate(bundle['units']):
   total+=1
   assert unit['ordinal']==i and bytes(unit['filename']).decode()==expected[i]['filename']
   assert H(bytes(unit['source']['contents']))==expected[i]['sha256']
   assert unit['source']['opened_as']==unit['filename']
  if row['category']=='diagnostic':
   assert bundle['units'][-1]['compiler_result']==-1 and files[prefix+'on.stdout']==b'exit 1\n'
  else:
   assert files[prefix+'on.stdout']==b'ran\n'
   assert files[prefix+'on.arena']==files[prefix+'off.arena']
   raw=files[prefix+'on.arena'];parts=raw.split(b'\n',5);assert len(parts)==6 and len(parts[5])==int(parts[4].split()[1],16)
   full=parts[5].ljust(4194304,b'\0');assert len(full)==4194304
   assert all(run['full_arena_sha256']==H(full) for run in row['runs'])
   fast='captures/multitu-fast/'+case+'/'
   assert raw==files[fast+'model.arena']==files[fast+'reference.arena'];arena_pairs+=1
  if row['exact']:
   checked+=1
   ids=[]
   for i,u in enumerate(bundle['units']):
    spec=read(prefix+f'raw-{i}.stdout');assert spec['status']==u['status']=='ok'
    for key in ['cabs','bindings','excluded','tokens','origins']:assert u[key]==spec[key],(case,i,key)
    assert u['opening_files']==spec['sources'] and not u['token_errors']
    event=u['compiler_event'];assert event['authenticated'] and event['directive']==spec['compiler_opening']
    ids.extend([event['root_id'],event['header_id']])
    roots=[s for s in u['sources'] if s['id']==event['root_id']];headers=[s for s in u['sources'] if s['id']==event['header_id']]
    assert len(roots)==len(headers)==1
    assert roots[0]['contents']==event['predefined'] and headers[0]['contents']==event['header_contents']
    assert headers[0]['included_at']==event['directive']
   assert len(ids)==len(set(ids))
  elif row['category']=='raw-layout-gap':
   assert read(prefix+'raw-0.stdout')=={'status':'raw-checked-error','error':'unimplemented-pack-layout'}
   component=read(prefix+'raw-0-parse-component.stdout')
   for key in ['cabs','bindings','excluded','origins']:assert bundle['units'][0][key]==component[key]
   assert bundle['units'][0]['opening_files']==component['sources']
  elif row['category']=='raw-opening-gap':
   for i,u in enumerate(bundle['units']):
    spec=read(prefix+f'raw-{i}.stdout')
    for key in ['cabs','bindings','excluded','tokens','origins']:assert u[key]==spec[key]
    assert len(u['opening_files'])+1==len(spec['sources'])
 independent=read('captures/multitu-independent-final/comparison.json')
 assert independent['acceptance']['accepted'] and independent['summary']=={'exact':51,'explicit-nonsuccess':5}
 assert all(row['result']==('same-refusal' if row['category']=='diagnostic' else 'same') for row in arena_report['rows'])
 print(f'Verified {len(files)} members, {len(req["sha256"])} final gate pins, {total} retained TU records, {checked} exact checked bundles and {arena_pairs} complete 4 MiB arena pairs')
if __name__=='__main__':main()
