#!/usr/bin/env python3
"""Check captured generator repair evidence; optionally verify frozen sources."""
from pathlib import Path
import argparse,collections,gzip,hashlib,json,tarfile
ap=argparse.ArgumentParser();ap.add_argument('--sources',action='store_true');a=ap.parse_args()
here=Path(__file__).resolve().parent;root=here.parents[2]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def read(n):return json.loads((here/(n+'.json')).read_text())
pins=read('pins');assert all(sha(here/p)==h for p,h in pins['artifacts'].items())
with tarfile.open(here/'captures.tar.xz') as t:members={m.name:hashlib.sha256(t.extractfile(m).read()).hexdigest() for m in t.getmembers() if m.isfile()}
assert members==pins['archive_members'] and len(members)==6133
q=json.loads(gzip.decompress((here/'gate.request.json.gz').read_bytes()));r=json.loads(gzip.decompress((here/'gate.result.json.gz').read_bytes()))
assert q['request_id']==r['request_id'] and r['exit']==0 and r['source_hashes_verified']
if a.sources:assert all(sha(root/p)==h for p,h in q['sha256'].items())
assert read('arenas')['totals']=={'n':1200,'same':1200}
assert len(read('gcc')['rows'])==1200 and all(x['exit']==0 for x in read('gcc')['rows'])
index=read('arena-index');assert len(index)==1200
for x in index:
 stem=Path(x['source']).stem
 assert members[f'arenas/{stem}/model.arena']==members[f'arenas/{stem}/reference.arena']==x['arena_sha256']
 assert members[f'corrected/cgen/{stem}.c']==x['source_sha256']
h=read('historical');assert len(h['sourcepins'])==1815 and len(h['rows'])==463
for p,digest in h['sourcepins'].items():assert members['historical/sources/'+p]==digest
old=[x for x in h['rows'] if x['corpus']=='gen-cgen'];assert len(old)==118 and all(x['gcc']['exit']==1 and 'lvalue expected' in x['console'] for x in old)
print(f'Verified 6133 captured files, 1200 arena pairs, 1815 historical sources and {len(q["sha256"])} gate inputs')
