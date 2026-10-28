#!/usr/bin/env python3
"""Verify pinned evidence, separated counts, source freeze and complete arenas."""
import argparse,gzip,hashlib,json,tarfile
from pathlib import Path
HERE=Path(__file__).resolve().parent

def digest(b):return hashlib.sha256(b).hexdigest()
def sha(p):return digest(p.read_bytes())
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--source-root',type=Path);a=ap.parse_args()
 pins=json.loads((HERE/'pins.json').read_text())
 for name,h in pins['files'].items():assert sha(HERE/name)==h,name
 manifest=json.loads((HERE/'artifact-manifest.json').read_text());data={};seen=set()
 with tarfile.open(HERE/'evidence.tar.xz','r|xz') as archive:
  for member in archive:
   assert member.isfile() and member.name in manifest and member.name not in seen,member.name
   stream=archive.extractfile(member);h=hashlib.sha256();chunks=[]
   wanted=member.name.endswith(('/comparison.json','/erasure.json','/inventory.json','.request','.result','/arenas.json','/report.json')) or member.name in {
    'build/assert-messages-fast-1.json','build/assert-messages-diagnostics-1.json','build/assert-messages-inherited-fast-1.json',
    'build/assert-messages-artifacts.json','build/test-c11/assert-message-diagnostics.json',
    'build/assert-messages-before-tree/tccgen.c','build/assert-messages-patch-check/tccgen.c','build/assert-messages-tree-audit.json'}
   # Native exports and fast pairs are small compressed/compact byte artifacts.
   wanted=wanted or '/assert-messages-native/' in member.name or '/assert-messages-fast-pairs/' in member.name or ('/assert-messages-gate.' in member.name and member.name.endswith('/native/on.ghost.json'))
   while chunk:=stream.read(1<<20):
    h.update(chunk)
    if wanted:chunks.append(chunk)
   assert h.hexdigest()==manifest[member.name],member.name
   if wanted:data[member.name]=b''.join(chunks)
   seen.add(member.name)
 assert seen==manifest.keys() and len(seen)==pins['members']
 def obj(path):return json.loads(data[path])
 def report(name):return obj('build/assert-messages-'+name+'-1/comparison.json')
 focused=report('focused')
 assert (focused['exact_valid'],focused['diagnostics'],focused['extensions'],focused['residuals'])==(65,61,['extension-no-message'],[])
 assert all(r['accepted'] for r in focused['rows'])
 assert all(all(v['difference'] is None for v in r['comparisons'].values()) for r in focused['rows'] if r['valid'])
 for name,count in [('shared',47),('component',48),('independent',47)]:
  x=report(name);assert x['summary']=={'exact':count,'explicit-nonsuccess':56-count} and x['acceptance']['accepted']
 assert report('independent')['input_mode']=='independent-Raw.checked-cfgTcc-source-preprocessing-and-parsing'
 physical=report('physical');assert physical['summary']=={'exact':58,'explicit_differences':0}
 assert len(physical['harness_checks'])==4 and all(physical['harness_checks'].values())
 erasure=obj('build/assert-messages-onoff-1/erasure.json');assert len(erasure['rows'])==15 and all(r['exact'] for r in erasure['rows'])
 assert obj('build/assert-messages-fast-1.json')['totals']=={'n':64,'same':64}
 assert obj('build/assert-messages-inherited-fast-1.json')['totals']=={'n':18,'same':18}
 diagnostics=obj('build/assert-messages-diagnostics-1.json');rows=[r for rs in diagnostics['results'].values() for r in rs]
 assert diagnostics['totals']=={'n':61,'same':0} and len(rows)==61 and all(r['result']=='same-refusal' for r in rows)
 matrix=obj('build/test-c11/assert-message-diagnostics.json')
 for r in matrix:
  accept=r['category'] in ('valid','ordinary-consumer') or (r['category']=='extension' and r['engine']!='gcc')
  assert (r['exit']==0)==accept
  if not accept:assert r['stderr']
  if r['category']=='false' and r['engine']!='gcc':assert 'before' in r['stderr'] and 'after' in r['stderr']
 for rid in [pins['explicit_build'],pins['full_gate']]:
  req=obj(f'build/worker-build.{rid}.request');res=obj(f'build/worker-build.{rid}.result')
  assert req['request_id']==res['request_id']==rid and res['exit']==0 and res['source_hashes_verified'] and len(req['sha256'])==pins['source_pins']
 original=obj('build/unevaluated-focused-5/comparison.json');assert original['residuals']==['assert-wide']
 audit=obj('build/assert-messages-tree-audit.json');assert audit['regular_files']==552 and audit['changed_files']==['tccgen.c']
 assert [n for n,h in audit['before_sha256'].items() if audit['after_sha256'][n]!=h]==['tccgen.c']
 before=data['build/assert-messages-before-tree/tccgen.c'].decode();after=data['build/assert-messages-patch-check/tccgen.c'].decode()
 lo=after.index('/* C11 6.7.10 accepts every string-literal encoding.');hi=after.index('static void do_Static_assert(void)\n{',lo)
 assert (after[:lo]+after[hi:]).replace('msg = parse_assert_str()->data;', 'msg = parse_mult_str("string constant")->data;')==before
 # Independent opening authority and legacy generic syntax are combined gate controls.
 authority=[obj(p) for p in data if '/injected-binding-gate.' in p and p.endswith('/model-origins/comparison.json')]
 assert len(authority)==1 and authority[0]['exact']==18 and len(authority[0]['harness_checks'])==6 and all(authority[0]['harness_checks'].values())
 message_gate=[obj(p) for p in data if '/assert-messages-gate.' in p and p.endswith('/syntax/comparison.json')]
 assert len(message_gate)==1 and message_gate[0]['exact_valid']==65 and all(r['accepted'] for r in message_gate[0]['rows'])
 native=[obj(p) for p in data if '/assert-messages-gate.' in p and p.endswith('/native/comparison.json')]
 assert len(native)==1 and native[0]['erasure'] and native[0]['native_arena']['exit']==native[0]['execution']['exit']==0
 ghosts=[obj(p) for p in data if '/assert-messages-gate.' in p and p.endswith('/native/on.ghost.json')]
 assert len(ghosts)==1 and ghosts[0]['status']=='ok' and ghosts[0]['token_errors']==[]
 # Recompare all bytes of each native export against independent model output.
 inventories=[(p,obj(p)) for p in data if '/assert-messages-native/' in p and p.endswith('/inventory.json')]
 assert len(inventories)==5
 for path,inventory in inventories:
  assert inventory['successes']==1
  prefix=path.removesuffix('inventory.json')
  for row in inventory['rows']:
   folder=prefix+row['directory']+'/'
   cut=obj(folder+'cut.json')
   for name,info in cut['files'].items():
    raw=data.get(folder+name)
    if raw is None:raw=gzip.decompress(data[folder+name+'.gz'])
    assert len(raw)==info['bytes'] and digest(raw)==info['sha256']
   arena=gzip.decompress(data[folder+'arena.bin.gz']);model=gzip.decompress(data[folder+'model.arena.gz'])
   payload=model.split(b'\n',5)[5]
   assert len(arena)==4194304 and arena==payload.ljust(4194304,b'\0')
   assert row['comparison']['status']=='equal' and row['comparison']['compared_bytes']==4194304
 pairs=[p for p in data if '/assert-messages-fast-pairs/' in p and p.endswith('/model.arena')]
 assert len(pairs)==82
 for p in pairs:assert data[p]==data[p.removesuffix('model.arena')+'reference.arena']
 if a.source_root:
  root=a.source_root.resolve();req=obj(f"build/worker-build.{pins['full_gate']}.request")
  assert all(sha(root/p)==h for p,h in req['sha256'].items()),'source changed'
  assert all(sha(root/p)==h for p,h in pins['prior_evidence'].items()),'historical evidence changed'
  artifacts=obj('build/assert-messages-artifacts.json')
  assert all(sha(root/p)==h for p,h in artifacts['pins'].items()),'binary/image/toolchain changed'
 print(f"verified {len(seen)} members, 65 exact syntax controls in three scopes, 64 new/18 inherited fast pairs, 61 diagnostic refusals, 5 complete native arenas/executions, unchanged frozen sources")
if __name__=='__main__':main()
