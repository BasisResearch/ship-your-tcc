#!/usr/bin/env python3
"""Verify combined GNU/bitfield evidence without a compiler or native emulator."""
import argparse,gzip,hashlib,json,re,tarfile
from pathlib import Path
ap=argparse.ArgumentParser();ap.add_argument('--sources',action='store_true');args=ap.parse_args()
here=Path(__file__).resolve().parent;root=here.parents[2];H=lambda b:hashlib.sha256(b).hexdigest();pins=json.loads((here/'pins.json').read_text())
assert H((here/'captures.tar.xz').read_bytes())==pins['archive_sha256'];assert H((here/'native-index.json').read_bytes())==pins['native_index_sha256']
with tarfile.open(here/'captures.tar.xz') as t:data={m.name:t.extractfile(m).read() for m in t if m.isfile()}
assert {n:H(b) for n,b in data.items()}==pins['members'];J=lambda p:json.loads(data[p])
q=J('gate/request.json');v=J('gate/result.json');assert q['request_id']==v['request_id']=='gnu-expr-integrated-1' and v['exit']==0 and v['source_hashes_verified'];assert data['gate/log.txt'].rstrip().endswith(b'check: OK')
assert len(q['sha256'])==pins['primary_inputs']==3480;supp=J('post/supplementary-pins.json');assert len(supp['sha256'])==pins['supplementary_inputs']==269
for p,h in {**q['sha256'],**supp['sha256']}.items():assert H(data['sources/'+h])==h,p
if args.sources:
 for p,h in {**q['sha256'],**supp['sha256']}.items():assert H((root/p).read_bytes())==h,p
for scope,count in [('shared',50),('component',51),('independent',50)]:
 d=J('post/'+scope+'/comparison.json');assert d['summary']=={'exact':count,'explicit-nonsuccess':56-count} and d['acceptance']['accepted']
p=J('post/physical/comparison.json');assert p['summary']=={'exact':58,'explicit_differences':0};assert len(p['harness_checks'])==4 and all(p['harness_checks'].values())
e=J('post/onoff/erasure.json');assert len(e['rows'])==15 and all(x['exact'] for x in e['rows'])
e=J('post/vla/report.json');assert len(e['rows'])==6 and all(all(v is None for v in x['comparisons'].values()) and all(c['exit']==0 for c in x['engines']) for x in e['rows'])
roots={n.split('/')[1] for n in data if n.startswith('gates/gnu-expr-gate.')};assert len(roots)==1;g='gates/'+next(iter(roots))
s=J(g+'/syntax/comparison.json');assert s['exact_valid']==50 and s['diagnostics']==10 and len(s['compiler_constraints'])==2 and all(x['accepted'] for x in s['rows'])
a=J(g+'/arenas/report.json');assert len(a['exact'])==50 and len(a['diagnostics'])==10 and len(a['compiler_constraints'])==2;assert len(a['erasure'])==62 and all(x['equal'] for x in a['erasure']);assert a['native']['exit']==0
for x in a['compiler_rows']:assert (x['exit']==0)==(x['category']=='valid' or (x['engine']=='cross-tcc' and x['category']=='compiler-constraint-gap'))
pairs=[p for p in data if p.startswith(g+'/arenas/') and p.endswith('/reference.arena.gz')];assert len(pairs)==52
for p in pairs:assert gzip.decompress(data[p])==gzip.decompress(data[p.removesuffix('reference.arena.gz')+'model.arena.gz'])
b=J('bitfields/report.json');assert len(b['rows'])==90 and sum(x['ok'] for x in b['rows'] if x['required'])==sum(x['required'] for x in b['rows'])==82
native=json.loads((here/'native-index.json').read_text());assert len(native)==6
for entry in native:
 comparison=J(str(Path(entry['report']).parent/'comparison.json'));assert comparison['erasure'] and comparison['execution']['exit']==comparison['native_arena']['exit']==0
 inv=J('native/'+entry['prefix']+'/inventory.json');assert inv==entry['inventory'] and inv['successes']==1
 for row in inv['rows']:
  prefix='native/'+entry['prefix']+'/'+row['directory']+'/';cut=J(prefix+'cut.json');cmp=row['comparison']
  for n,info in cut['files'].items():
   raw=data[prefix+n] if prefix+n in data else gzip.decompress(data[prefix+n+'.gz']);assert len(raw)==info['bytes'] and H(raw)==info['sha256']
  arena=gzip.decompress(data[prefix+'arena.bin.gz']);model=gzip.decompress(data[prefix+'model.arena.gz']);payload=model.split(b'\n',5)[5]
  assert len(arena)==4194304 and arena==payload.ljust(4194304,b'\0');assert cmp['status']=='equal' and cmp['compared_bytes']==4194304
  regs={n:int(x,16) for n,x in re.findall(r'⟨Register\.(\w+), 0x([0-9a-fA-F]+)#64⟩',data[prefix+'registers.txt'].decode())};assert regs['PC']==cut['pc'] and all(regs[n]==x for n,x in cmp['handoff'].items())
probes=J('semantic-probes/summary.json');assert len(probes['rows'])==10
for row in probes['rows']:
 name=row['case'];assert H(data['semantic-probes/'+name+'/input.c'])==row['source_sha256'];assert J('semantic-probes/'+name+'/stdout')['semantics']==row['semantics'];assert all(e['compile_exit']==e['exit']==0 for e in row['engines'])
 expected=([{'status':'defined','exit':0,'stdout':''}] if name in ['choose-single-evaluation','nonvla-expression','ordinary-conditional-control'] else [{'diagnostic':"couldn't find the startup function",'status':'other-driver-error'}] if name=='conditional-single-evaluation' else {'status':'elaboration-error'})
 assert row['semantics']==expected,name
assert J('semantic-probes/upstream-source-check.json')['revision']=='704c4e697845127bced66c93fa0763eb51527ca4'
assert H((root/'results/model/gnu-expression-syntax/artifact-manifest.json').read_bytes())==pins['worker_manifest_sha256']
print('Verified',len(data),'combined members;3480primary+269supplementary inputs;50GNU positives,10refusals,2separate policies;82required bitfield checks;6complete native exports')
