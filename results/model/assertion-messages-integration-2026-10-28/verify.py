#!/usr/bin/env python3
"""Check source-equivalent full-gate reuse and the fresh combined default gate."""
from pathlib import Path
import argparse,json,hashlib,tarfile,gzip
p=argparse.ArgumentParser();p.add_argument('--sources',action='store_true');args=p.parse_args();here=Path(__file__).resolve().parent;root=here.parents[2];H=lambda b:hashlib.sha256(b).hexdigest()
m=json.loads((here/'pins.json').read_text());archive=here/'captures.tar.xz';assert H(archive.read_bytes())==m['archive_sha256']
with tarfile.open(archive) as t:data={f.name:t.extractfile(f).read() for f in t if f.isfile()}
assert {p:H(b) for p,b in data.items()}==m['members']
q=json.loads(data['gate/request.json']);r=json.loads(data['gate/result.json']);wq=json.loads(data['worker/full.request.json']);wr=json.loads(data['worker/full.result.json']);prior=json.loads(data['prior/request.json']);e=json.loads(data['combined/equivalence.json'])
assert q['request_id']==r['request_id']=='assert-integrated-1' and r['exit']==0 and r['source_hashes_verified'] and r['command']==['scripts/check.sh']
assert wq['request_id']==wr['request_id']=='assert-messages-full-1' and wr['exit']==0 and wr['source_hashes_verified'] and wr['command']==['scripts/check.sh','--csem']
assert data['gate/log'].rstrip().endswith(b'check: OK')
assert all(q['sha256'].get(p)==h for p,h in wq['sha256'].items())
extra={p:h for p,h in prior['sha256'].items() if p not in wq['sha256']}
assert q['sha256']=={**wq['sha256'],**extra}
assert len(wq['sha256'])==e['runtime_inputs']==3379 and len(extra)==e['extra_inputs']==269 and len(q['sha256'])==m['source_inputs']==3648
assert all(p.endswith('.md') or p.startswith(('results/','tasks/')) for p in e['nonruntime_tree_changes'])
assert H((root/'results/model/static-assert-messages/evidence.tar.xz').read_bytes())==m['worker_archive_sha256']
if args.sources:
 for p,h in q['sha256'].items():assert H((root/p).read_bytes())==h,p
nd=json.loads(data['native/execution/arenas.json']);assert nd['totals']=={'n':1,'same':1}
report=next(iter(nd['results'].values()))[0]['sail'];assert report['status']=='equal' and report['compared_bytes']==4194304
execution=json.loads(data['native/execution/comparison.json']);assert execution['erasure'] and execution['native_arena']['exit']==execution['execution']['exit']==0
assert data['native/execution/execution.stdout']==b''
model=data['native/model.arena'];assert model==data['native/reference.arena'] and H(model)==report['model_arena_sha256']
head=model.split(b'\n',5);assert len(head)==6 and head[0]==b'TCCARENA 1' and int(head[4].split()[1],16)==len(head[5])
arena=gzip.decompress(data['native/capture/arena.bin.gz']);assert arena==head[5].ljust(4194304,b'\0') and H(arena)==report['arena_sha256']
cut=json.loads(data['native/capture/cut.json']);assert H(data['native/capture/cut.json'])==report['capture_manifest_sha256']
for name,info in cut['files'].items():
 key='native/capture/'+name;b=data[key] if key in data else gzip.decompress(data[key+'.gz'])
 assert len(b)==info['bytes'] and H(b)==info['sha256']
sections=json.loads(data['combined/archive-sections.json']);symbols=json.loads(data['combined/archive-symbols.json'])
assert len(sections['rows'])==len(symbols['rows'])==4
for row in sections['rows']:assert {d['section'] for d in row['differences']}<={'.symtab','.strtab'}
for row in symbols['rows']:
 assert len(row['file_symbol_differences'])==1
 for diff in row['file_symbol_differences']:
  a,b=diff['combined'],diff['worker'];assert a[1:]==b[1:] and a[1]&15==4 and Path(a[0]).name==Path(b[0]).name
print(f"Verified {len(data)} combined captures, 3648 source pins; 3379 full-gated worker inputs identical and 269 prior inputs unchanged; fresh default gate and one fresh complete native arena/program run")
