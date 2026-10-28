#!/usr/bin/env python3
"""Verify combined bitfield gate, stale-artifact diagnosis and native observations."""
from pathlib import Path
import argparse,gzip,hashlib,json,lzma,tarfile
ap=argparse.ArgumentParser();ap.add_argument('--sources',action='store_true');args=ap.parse_args();here=Path(__file__).resolve().parent;root=here.parents[2];H=lambda b:hashlib.sha256(b).hexdigest();pins=json.loads((here/'pins.json').read_text());assert H((here/'captures.tar.xz').read_bytes())==pins['archive_sha256'];assert H((here/'native-index.json').read_bytes())==pins['native_index_sha256']
with tarfile.open(here/'captures.tar.xz') as t:data={m.name:t.extractfile(m).read() for m in t if m.isfile()}
assert {n:H(b) for n,b in data.items()}==pins['members'];J=lambda n:json.loads(data[n])
q=J('gate/bitfields-integrated-2.request.json');r=J('gate/bitfields-integrated-2.result.json');old=J('gate/bitfields-integrated-1.result.json');assert q['request_id']==r['request_id']=='bitfields-integrated-2' and r['exit']==0 and r['source_hashes_verified'];assert old['exit']==1 and old['source_hashes_verified'];assert data['gate/bitfields-integrated-2.log'].rstrip().endswith(b'check: OK')
supp=J('combined/supplementary-pins.json');assert len(q['sha256'])==pins['primary_inputs']==3478;assert len(supp['sha256'])==pins['supplementary_inputs']==269
if args.sources:
 for p,h in {**q['sha256'],**supp['sha256']}.items():assert H((root/p).read_bytes())==h,p
stale=J('combined/stale-raw-diagnosis.json');assert stale['current_raw_sha256']==stale['prior_raw_sha256'] and stale['sample_semantics']=={'status':'elaboration-error'}
failed=J('focused/bitfields-integrated-1/report.json');assert sum(x['ok'] for x in failed['rows'] if x['required'])==10
focused=J('focused/bitfields-integrated-2/report.json');rows=focused['rows'];assert len(rows)==90 and sum(x['ok'] for x in rows if x['required'])==sum(x['required'] for x in rows)==82
assert sum(x['ok'] for x in rows if x['kind']=='gnu-defined')==4
assert J('combined/layout-raw-final/report.json')['ok'];assert len(J('combined/controls-final/report.json')['rows'])==11 and all(x['ok'] for x in J('combined/controls-final/report.json')['rows']);cont=J('combined/continuation-final/report.json')['rows'];assert len(cont)==58 and all(x['ok'] for x in cont)
native=J('combined/native-final/receipt.json');assert native['exit']==native['gcc_exit']==0
for n,h in native['artifacts'].items():assert H(data['combined/native-final/'+n])==h
assert H(lzma.decompress(data['combined/native-final/prog.elf.xz']))==native['elf_sha256'];stdout=data['combined/native-final/stdout'];assert stdout==data['combined/native-final/gcc.stdout'];assert len(stdout.splitlines())==80 and all(x.endswith(b'=0') for x in stdout.splitlines())
index=json.loads((here/'native-index.json').read_text());assert len(index)==5
for entry in index:
 p=entry['prefix']+'/';comparison=entry['comparison'];model=data[p+'model.arena'];assert model==data[p+'reference.arena'] and H(model)==comparison['model_arena_sha256'];assert comparison['status']=='equal' and comparison['compared_bytes']==4194304
 head=model.split(b'\n',5);assert head[0]==b'TCCARENA 1' and int(head[4].split()[1],16)==len(head[5]);arena=gzip.decompress(data[p+'capture/arena.bin.gz']);assert arena==head[5].ljust(4194304,b'\0') and H(arena)==comparison['arena_sha256'];cut=J(p+'capture/cut.json');assert H(data[p+'capture/cut.json'])==comparison['capture_manifest_sha256']
 for name,info in cut['files'].items():
  key=p+'capture/'+name;b=data[key] if key in data else gzip.decompress(data[key+'.gz']);assert len(b)==info['bytes'] and H(b)==info['sha256']
assert H((root/'results/csem/bitfields/evidence-sha256.json').read_bytes())==pins['worker_manifest_sha256']
print(f'Verified {len(data)} combined captures;3478 frozen inputs+269 unchanged supplementary pins;82 required semantic checks,80 native/GCC rows,5 complete native arenas; remaining coverage/refinement obligations stay open')
