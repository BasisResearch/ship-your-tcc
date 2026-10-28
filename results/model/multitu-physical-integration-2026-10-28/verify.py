#!/usr/bin/env python3
"""Verify combined evidence; ordered-TU checks reuse multitu-ghost/verify.py."""
from pathlib import Path
import argparse,hashlib,json,tarfile,gzip,lzma,re
H=lambda b:hashlib.sha256(b).hexdigest()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--sources',action='store_true');a=ap.parse_args()
 here=Path(__file__).resolve().parent;root=here.parents[2]
 pins=json.loads((here/'pins.json').read_text());archive=here/'captures.tar.xz'
 assert H(archive.read_bytes())==pins['archive_sha256']
 assert H((here/'native-index.json').read_bytes())==pins['native_index_sha256']
 with tarfile.open(archive) as t:
  entries=t.getmembers();assert all(x.isfile() and not x.name.startswith('/') and '..' not in Path(x.name).parts for x in entries)
  files={x.name:t.extractfile(x).read() for x in entries}
 assert len(files)==len(entries) and {p:H(b) for p,b in files.items()}==pins['members']
 read=lambda p:json.loads(files[p])
 req=read('gate/request.json');res=read('gate/result.json')
 assert req['request_id']==res['request_id']=='multitu-physical-integrated-1' and res['exit']==0 and res['source_hashes_verified']
 assert files['gate/log.txt'].rstrip().endswith(b'check: OK')
 assert len(req['sha256'])==pins['inputs']==4162
 assert set(pins.get('post_gate_tools',{}))=={'codexwork.sh'}
 for p,h in req['sha256'].items():
  assert H(files['sources/'+h])==h,p
  if a.sources:
   current=H((root/p).read_bytes())
   if p in pins.get('post_gate_tools',{}):
    change=pins['post_gate_tools'][p]
    assert h==change['gate_sha256'] and current==change['adopted_sha256'],p
    assert H((here/change['retained_path']).read_bytes())==current,p
   else:assert current==h,p
 for p,h in pins['references'].items():assert H((root/p).read_bytes())==h,p
 report=read('post/multitu/comparison.json');arena_report=read('post/multitu-arenas/report.json')
 if a.sources:assert all(H((root/p).read_bytes())==h for p,h in report['source_hashes'].items())
 assert report['source_hashes']==arena_report['source_hashes']
 for p,h in report['source_hashes'].items():
  assert H(files['sources/'+h])==h
  if p in req['sha256']:assert req['sha256'][p]==h
 assert report['runtime_key']==arena_report['runtime_key'] and report['image_symbols_sha256']==H(files['runtime/image.syms'])
 assert all(report['guards'].values()) and len(report['guards'])>=6
 total=0;checked=0;arena_pairs=0
 for row in report['rows']:
  case=row['case'];prefix='post/multitu/'+case+'/'
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
   fast='post/multitu-arenas/'+case+'/'
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
 independent=read('post/independent/comparison.json')
 assert independent['acceptance']['accepted'] and independent['summary']=={'exact':51,'explicit-nonsuccess':5}
 assert all(row['result']==('same-refusal' if row['category']=='diagnostic' else 'same') for row in arena_report['rows'])
 assert (total,checked,arena_pairs)==(15,4,6)
 assert len(report['rows'])==8 and len(arena_report['rows'])==8
 for scope,count in [('shared',51),('component',52),('independent',51)]:
  d=read('post/'+scope+'/comparison.json');assert d['acceptance']['accepted'] and d['summary']=={'exact':count,'explicit-nonsuccess':56-count}
 p=read('post/physical/comparison.json');assert p['summary']=={'exact':58,'explicit_differences':0} and len(p['harness_checks'])==4 and all(p['harness_checks'].values())
 d=read('post/onoff/erasure.json');assert len(d['rows'])==15 and all(x['exact'] for x in d['rows'])
 attrs=read('post/attribute-merge-review/report.json');assert len(attrs['rows'])==50
 defined=[x for x in attrs['rows'] if x['kind']=='defined'];assert len(defined)==42
 assert all(x['comparisons']['combined-model']['difference'] is None for x in defined)
 assert sum(x['comparisons']['adopted-model']['difference'] is None for x in defined)==36
 assert not any(x['regression'] for x in attrs['rows'])
 # Axioms of actual theorems, not just Prop-valued Statement definitions.
 source=files['sources/'+req['sha256']['Tcc/Model/PhysicalProof.lean']].decode()
 names={'Tcc.Model.Ghost.PhysicalProof.'+n for n in re.findall(r'^(?:@\[[^\n]*\]\s+)?theorem\s+(\w+)',source,re.M)}
 names|={'Tcc.Model.Ghost.'+n for n in ['physicalBlockErasure','physicalBlockShape','physicalOriginMap','physicalReadView']};assert len(names)==40
 facts=re.findall(r"'([^']+)' (?:depends on axioms: \[([^\]]*)\]|does not depend on any axioms)",files['gate/log.txt'].decode())
 assert names <= {n for n,ax in facts}
 for n,ax in facts:
  if n in names:assert set(ax.split(', '))-{''}<={'propext','Classical.choice','Quot.sound'},n
 def native_capture(prefix,row):
  cut=read(prefix+'cut.json');cmp=row['comparison']
  for n,info in cut['files'].items():
   b=files[prefix+n] if prefix+n in files else gzip.decompress(files[prefix+n+'.gz']);assert len(b)==info['bytes'] and H(b)==info['sha256']
  arena=gzip.decompress(files[prefix+'arena.bin.gz']);presence=gzip.decompress(files[prefix+'arena.present.bin.gz'])
  model=gzip.decompress(files[prefix+'model.arena.gz']);parts=model.split(b'\n',5);assert len(parts)==6 and len(parts[5])==int(parts[4].split()[1],16)
  assert len(arena)==len(presence)==4194304 and arena==parts[5].ljust(4194304,b'\0')
  assert all(x in (0,1) for x in presence) and sum(presence)==cut['arena_present_bytes'] and not any(b for b,p in zip(arena,presence) if not p)
  assert cmp['status']=='equal' and cmp['compared_bytes']==4194304 and H(model)==cmp['model_arena_sha256'] and H(arena)==cmp['arena_sha256']
  regs={n:int(x,16) for n,x in re.findall(r'⟨Register\.(\w+), 0x([0-9a-fA-F]+)#64⟩',files[prefix+'registers.txt'].decode())}
  assert regs['PC']==cut['pc'] and all(regs[n]==v for n,v in cmp['handoff'].items())
  assert cut['elf_sha256']==cmp['elf_sha256'] and H(files[prefix+'source.c'])==cut['source_sha256']==cmp['source_sha256']
 index=json.loads((here/'native-index.json').read_text());assert len(index)==6
 for entry in index:
  comparison=read(str(Path(entry['report']).parent/'comparison.json'));assert comparison['erasure'] and comparison['execution']['exit']==comparison['native_arena']['exit']==0
  inv=read('native/'+entry['prefix']+'/inventory.json');assert inv==entry['inventory'] and inv['successes']==1
  for row in inv['rows']:native_capture('native/'+entry['prefix']+'/'+row['directory']+'/',row)
 inv=read('post/native104/capture/inventory.json');assert inv['successes']==1 and not inv['residuals'] and len(inv['rows'])==1
 for row in inv['rows']:native_capture('post/native104/capture/'+row['directory']+'/',row)
 n=read('post/native104/receipt.json');assert n['exit']==0 and n['console_equal'] and n['source_hashes_verified']
 for p,h in n['artifacts'].items():assert H(files['post/native104/'+p])==h,p
 elf=lzma.decompress(files['post/native104/prog.elf.xz']);assert H(elf)==n['elf_sha256']==inv['rows'][0]['comparison']['elf_sha256']
 invocation=n['invocation'];assert invocation['argv']==['tcc','-std=c11','-w','/104+_inline.c','-run','/prog.c']
 for p,h in invocation['pins'].items():
  rel=str(Path(p).relative_to(req['cwd']));assert req['sha256'][rel]==h and H(files['sources/'+h])==h
 for p in [invocation['source'],dict(invocation['extra'])['/104+_inline.c']]:
  rel=str(Path(p).relative_to(req['cwd']));assert files['sources/'+req['sha256'][rel]] in elf
 expected=files['sources/'+req['sha256']['vendor/tinycc/tests/tests2/104_inline.expect']]
 assert files['post/native104/stdout']==files['post/native104/expected.stdout']==expected
 assert (n['compile_steps'],n['program_steps'],n['total_steps'])==(11461362,64583,11525970)
 print('Verified',len(files),'members;4162inputs;40physical theorem audits;15TU records/4checked bundles/6fast arenas;42defined attribute cases;7complete native captures and fresh104 execution')
if __name__=='__main__':main()
