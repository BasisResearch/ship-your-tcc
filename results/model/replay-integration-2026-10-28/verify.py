#!/usr/bin/env python3
"""Verify the combined local theorem checkpoint, not whole-compiler correctness."""
from pathlib import Path
import argparse,hashlib,json,tarfile
here=Path(__file__).resolve().parent;root=here.parents[2]
a=argparse.ArgumentParser();a.add_argument('--sources',action='store_true');args=a.parse_args()
H=lambda b:hashlib.sha256(b).hexdigest()
m=json.loads((here/'pins.json').read_text());p=here/'captures.tar.xz';assert H(p.read_bytes())==m['archive_sha256']
with tarfile.open(p) as t:
 data={x.name:t.extractfile(x).read() for x in t if x.isfile()}
assert {p:H(b) for p,b in data.items()}==m['members']
q=json.loads(data['gate/request.json']);r=json.loads(data['gate/result.json']);old=json.loads(data['array/request.json'])
assert q['request_id']==r['request_id']=='replay-integrated-1' and r['exit']==0 and r['source_hashes_verified']
changed=sorted(p for p,h in q['sha256'].items() if old['sha256'].get(p)!=h)
assert changed==m['changed_source_inputs']==['Tcc/Model/GhostAudit.lean','Tcc/Model/GhostDriver.lean']
assert q['sha256'].keys()==old['sha256'].keys() and len(q['sha256'])==m['source_inputs']
for p in changed:assert H(data['source/'+p])==q['sha256'][p]
for name in ['erase_syntaxFields','associationReplayErasure']:
 assert ("'Tcc.Model.Ghost."+name+"' depends on axioms: [propext, Classical.choice, Quot.sound]").encode() in data['gate/full.log']
assert data['gate/full.log'].rstrip().endswith(b'check: OK')
refresh=json.loads(data['main-array-refresh/result.json']);assert refresh['exit']==0 and refresh['source_hashes_verified']
assert json.loads(data['main-array-refresh/request.json'])['sha256']==old['sha256']
if args.sources:
 for p,h in q['sha256'].items():assert H((root/p).read_bytes())==h,p
print(f"Verified {len(data)} captures, {len(q['sha256'])} combined source pins, standard axioms; only two proof/audit inputs changed")
