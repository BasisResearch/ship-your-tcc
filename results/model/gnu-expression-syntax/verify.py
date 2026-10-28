#!/usr/bin/env python3
"""Offline byte checks for GNU syntax, distinct classifications and native arenas."""
import argparse,gzip,hashlib,json,re,tarfile
from pathlib import Path
HERE=Path(__file__).resolve().parent
def digest(b):return hashlib.sha256(b).hexdigest()
def sha(p):return digest(p.read_bytes())

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--source-root',type=Path);a=ap.parse_args()
 pins=json.loads((HERE/'pins.json').read_text())
 for n,h in pins['files'].items():assert sha(HERE/n)==h,n
 manifest=json.loads((HERE/'artifact-manifest.json').read_text());seen=set();data={}
 with tarfile.open(HERE/'evidence.tar.xz','r|xz') as archive:
  for member in archive:
   assert member.isfile() and member.name in manifest and member.name not in seen,member.name
   stream=archive.extractfile(member);h=hashlib.sha256();chunks=[]
   wanted=member.name.endswith(('.json','.request','.result')) or '/gnu-expr-native-exports/' in member.name
   wanted=wanted or '/reference.arena.gz' in member.name or '/model.arena.gz' in member.name or member.name=='build/worker-build-gnu-expr-full-1.log'
   while chunk:=stream.read(1<<20):
    h.update(chunk)
    if wanted:chunks.append(chunk)
   assert h.hexdigest()==manifest[member.name],member.name
   if wanted:data[member.name]=b''.join(chunks)
   seen.add(member.name)
 assert seen==manifest.keys() and len(seen)==pins['members']
 def obj(p):return json.loads(data[p])
 for rid,count in [('gnu-expr-lean-1',3380),('gnu-expr-full-1',pins['source_pins'])]:
  req=obj(f'build/worker-build.{rid}.request');res=obj(f'build/worker-build.{rid}.result')
  assert req['request_id']==res['request_id']==rid and res['exit']==0 and res['source_hashes_verified']
  assert len(req['sha256'])==count
  for name,h in req['sha256'].items():assert manifest['sources/'+h]==h,(rid,name)
 assert data['build/worker-build-gnu-expr-full-1.log'].rstrip().endswith(b'check: OK')
 opts=obj('build/gnu-expr-effective-options.json')
 assert opts['effective_compiler_argv']==['tcc','-std=c11','-w','-run','/prog.c']
 assert opts['source_sha256']==req['sha256']['Tcc/ModelMain.lean']
 image=obj('build/gnu-expr-reviewed-c-image.json')
 assert len(image['rows'])==5 and all(r['unchanged'] for r in image['rows'])
 old=obj('build/assert-messages-artifacts.json')['pins'];current=obj('build/gnu-expr-artifact-pins.json')
 assert all(old[r['path']]==current[r['path']] for r in image['rows'])
 for name,count in [('shared',50),('component',51),('independent',50)]:
  x=obj(f'build/gnu-expr-{name}-1/comparison.json')
  assert x['summary']=={'exact':count,'explicit-nonsuccess':56-count} and x['acceptance']['accepted']
  assert all(r['status']=='exact' for r in x['rows'] if r['case'] in ('gnu-conditional','typeof','choose'))
 physical=obj('build/gnu-expr-physical-final/comparison.json')
 assert physical['summary']=={'exact':58,'explicit_differences':0}
 assert len(physical['harness_checks'])==4 and all(physical['harness_checks'].values())
 erasure=obj('build/gnu-expr-onoff-final/erasure.json')
 assert len(erasure['rows'])==15 and all(r['exact'] for r in erasure['rows'])
 gnu=[d for d in pins['gates'] if '/gnu-expr-gate.' in d];assert len(gnu)==1;g=gnu[0]
 syntax=obj(g+'/syntax/comparison.json')
 assert syntax['exact_valid']==50 and syntax['diagnostics']==10 and len(syntax['compiler_constraints'])==2
 assert all(r['accepted'] for r in syntax['rows'])
 assert all(all(v['difference'] is None for v in r['comparisons'].values()) for r in syntax['rows'] if r['valid'])
 arenas=obj(g+'/arenas/report.json')
 assert len(arenas['exact'])==50 and len(arenas['diagnostics'])==10 and len(arenas['compiler_constraints'])==2
 assert len(arenas['erasure'])==62 and all(r['equal'] for r in arenas['erasure'])
 for r in arenas['compiler_rows']:
  accepted=r['category']=='valid' or (r['engine']=='cross-tcc' and r['category']=='compiler-constraint-gap')
  assert (r['exit']==0)==accepted,r
 pairs=[p for p in data if p.startswith(g+'/arenas/') and p.endswith('/reference.arena.gz')]
 assert len(pairs)==52
 for p in pairs:
  ref=gzip.decompress(data[p]);model=gzip.decompress(data[p.removesuffix('reference.arena.gz')+'model.arena.gz'])
  assert model==ref,p
 assert arenas['native']['exit']==0
 native=obj(g+'/arenas/native/comparison.json')
 assert native['erasure'] and native['execution']['exit']==native['native_arena']['exit']==0
 ghosts=[obj(p) for p in data if p==g+'/arenas/native/on.ghost.json']
 assert len(ghosts)==1 and ghosts[0]['status']=='ok' and not ghosts[0]['token_errors']
 extra=obj('build/gnu-expr-extra-syntax-1/report.json')
 assert len(extra['rows'])==6
 assert all(all(x is None for x in r['comparisons'].values()) and all(e['exit']==0 for e in r['engines']) for r in extra['rows'])
 authority=[obj(p) for p in data if '/injected-binding-gate.' in p and p.endswith('/model-origins/comparison.json')]
 assert len(authority)==1 and authority[0]['exact']==18 and len(authority[0]['harness_checks'])==6 and all(authority[0]['harness_checks'].values())
 # Superseded failed expectations remain distinguishable from final results.
 first=obj('build/gnu-expr-syntax-1/comparison.json')
 assert first['exact_valid']==50 and [r['case'] for r in first['rows'] if not r['accepted']]==['typeof-sideeffect-const','choose-condition-comma']
 initial=obj('build/gnu-expr-arenas-1/report.json');assert len(initial['diagnostics'])==12
 exports=[(p,obj(p)) for p in data if '/gnu-expr-native-exports/' in p and p.endswith('/inventory.json')]
 assert len(exports)==6==len(pins['native'])
 for path,inv in exports:
  assert inv['successes']==1;prefix=path.removesuffix('inventory.json')
  for row in inv['rows']:
   folder=prefix+row['directory']+'/'
   cut=obj(folder+'cut.json')
   for n,info in cut['files'].items():
    raw=data.get(folder+n)
    if raw is None:raw=gzip.decompress(data[folder+n+'.gz'])
    assert len(raw)==info['bytes'] and digest(raw)==info['sha256']
   arena=gzip.decompress(data[folder+'arena.bin.gz']);model=gzip.decompress(data[folder+'model.arena.gz'])
   payload=model.split(b'\n',5)[5];assert len(arena)==4194304 and arena==payload.ljust(4194304,b'\0')
   assert row['comparison']['status']=='equal' and row['comparison']['compared_bytes']==4194304
   regs={n:int(v,16) for n,v in re.findall(r'⟨Register\.(\w+), 0x([0-9a-fA-F]+)#64⟩',data[folder+'registers.txt'].decode())}
   assert regs['PC']==cut['pc']
   assert all(regs[n]==v for n,v in row['comparison']['handoff'].items())
 if a.source_root:
  root=a.source_root.resolve();req=obj('build/worker-build.gnu-expr-full-1.request')
  assert all(sha(root/p)==h for p,h in req['sha256'].items()),'frozen source changed'
  assert all(sha(root/p)==h for p,h in obj('build/gnu-expr-artifact-pins.json').items()),'compiler/runtime/tool changed'
  assert all(sha(root/p)==h for p,h in pins['prior_evidence'].items()),'reviewed prior evidence changed'
 print('verified',len(seen),'members; 50 exact GNU controls per scope, 10 refusals/2 policy differences, 62 new/15 legacy ghost pairs, 58 physical controls, 52 separated fast pairs, 6 supplemental VLA controls and 6 complete native exports')
if __name__=='__main__':main()
