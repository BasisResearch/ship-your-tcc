#!/usr/bin/env python3
"""Verify retained invocation evidence and all four MiB of the final native arena."""
from pathlib import Path
import argparse,gzip,hashlib,json,tarfile

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--sources',action='store_true');a=ap.parse_args()
 here=Path(__file__).resolve().parent;root=here.parents[2]
 H=lambda b:hashlib.sha256(b).hexdigest()
 read=lambda n:json.loads((here/(n+'.json')).read_text())
 pins=read('pins');assert all(H((here/p).read_bytes())==h for p,h in pins['artifacts'].items())
 with tarfile.open(here/'captures.tar.xz') as t:
  captures={m.name:t.extractfile(m).read() for m in t.getmembers() if m.isfile()}
 assert {n:H(b) for n,b in captures.items()}==pins['archive_members']
 q=json.loads(gzip.decompress((here/'gate.request.json.gz').read_bytes()));r=json.loads(gzip.decompress((here/'gate.result.json.gz').read_bytes()))
 assert q['request_id']==r['request_id'] and r['exit']==0 and r['source_hashes_verified']
 if a.sources:assert all(H((root/p).read_bytes())==h for p,h in q['sha256'].items())
 d=read('native-final');assert d['totals']=={'n':1,'same':1}
 row=d['results']['invocations'][0];sail=row['sail'];assert row['result']=='same' and sail['status']=='equal' and sail['compared_bytes']==4194304
 model=captures['native-final/model.arena'];assert H(model)==sail['model_arena_sha256']
 header=model.split(b'\n',5);assert len(header)==6 and header[0]==b'TCCARENA 1'
 assert len(header[5])==int(header[4].split()[1],16)
 arena=gzip.decompress(captures['native-final/capture/arena.bin.gz'])
 assert len(arena)==4194304 and arena==header[5].ljust(4194304,b'\0') and H(arena)==sail['arena_sha256']
 cut=json.loads(captures['native-final/capture/cut.json'])
 for name,info in cut['files'].items():
  key='native-final/capture/'+name
  data=captures[key] if key in captures else gzip.decompress(captures[key+'.gz'])
  assert len(data)==info['bytes'] and H(data)==info['sha256']
 assert cut['steps']==sail['steps']==11459979
 assert H(captures['native-final/capture/cut.json'])==sail['capture_manifest_sha256']
 old=read('standalone');assert old['totals']=={'n':2,'same':0}
 assert all(r['result']=='same-refusal' for rows in old['results'].values() for r in rows)
 assert len(read('historical-special-invocations'))==12
 print(f'Verified {len(captures)} captures, {len(q["sha256"])} gate inputs and all 4 MiB of the final native arena')
if __name__=='__main__':main()
