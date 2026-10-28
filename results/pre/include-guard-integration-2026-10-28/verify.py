#!/usr/bin/env python3
"""Offline validation of retained guard/EOF observations; no theorem claim."""
from pathlib import Path
import argparse,hashlib,json,tarfile,lzma,gzip,re,csv,io
H=lambda b:hashlib.sha256(b).hexdigest()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--sources',action='store_true');a=ap.parse_args()
 here=Path(__file__).resolve().parent;root=here.parents[2];pins=json.loads((here/'pins.json').read_text())
 assert H(Path(__file__).read_bytes())==pins['verifier_sha256']
 for path,h in pins.get('verification_failures',{}).items():assert H((here/path).read_bytes())==h
 assert H((here/'evidence.tar.xz').read_bytes())==pins['archive_sha256']
 with tarfile.open(here/'evidence.tar.xz') as t:
  ms=t.getmembers();assert all(m.isfile() and not m.name.startswith('/') and '..' not in Path(m.name).parts for m in ms)
  data={m.name:t.extractfile(m).read() for m in ms}
 assert len(data)==len(ms) and {n:H(b) for n,b in data.items()}==pins['members']
 J=lambda n:json.loads(data[n]);Z=lambda n:json.loads(lzma.decompress(data[n]))
 q=J('gate/request.json');v=J('gate/result.json')
 assert q['request_id']==v['request_id']=='guard-openings-full-1' and v['exit']==0 and v['source_hashes_verified']
 assert len(q['sha256'])==4163 and data['gate/log.txt'].rstrip().endswith(b'check: OK')
 for p,h in q['sha256'].items():
  assert H(data['sources/'+h])==h
  if a.sources:assert H((root/p).read_bytes())==h,p
 assert len(pins['requests'])==5
 for rid in pins['requests']:
  req=J('builds/worker-build.'+rid+'.request');res=J('builds/worker-build.'+rid+'.result')
  assert req['request_id']==res['request_id']==rid and res['source_hashes_verified']
  assert (res['exit']!=0)==(rid in ['guard-openings-lean-1','guard-openings-lean-2'])
  for h in req['sha256'].values():assert H(data['sources/'+h])==h
 for p,h in pins['references'].items():assert H((root/p).read_bytes())==h
 for name,key in [('native-index.json','native_index_sha256'),('supplemental-inputs.json','supplemental_sha256')]:assert H((here/name).read_bytes())==pins[key]
 supplement=json.loads((here/'supplemental-inputs.json').read_text())
 for h in supplement.values():assert H(data['sources/'+h])==h
 assert len(pins['runtime_fs'])==153
 for entry in pins['runtime_fs']:assert H(data[entry['object']])==entry['sha256']
 assert H(data['runtime/c/build/base.elf'])==J('new-native/arenas.json')['pins']['c/build/base.elf']
 before=J('before32/report.json');after=J('focused-2/report.json');first=J('focused-1/report.json')
 assert len(before['rows'])==32 and before['exact']==14 and len(first['rows'])==32 and first['exact']==31
 assert len(after['rows'])==after['exact']==35
 old={x['name']:x for x in before['rows']}
 def observations(base,start):
  m=Z(base+'raw-model.stdout.xz');s=Z(base+'origin-spec.stdout.xz');assert m['status']==s['status']=='ok'
  physical=[x for x in m['sources'] if not x.get('generated')];header=physical[1]
  assert bytes(physical[0]['opened_as'])==b'/prog.c' and bytes(header['opened_as'])==b'/lib/tcc/include/tccdefs.h' and header['included_at'] is not None
  user=[x for x in physical if x['id']!=header['id']];ids={x['id'] for x in user}
  assert m['section_bytes_equal'] and m['console_equal'] and not m['token_errors']
  assert len(m['model_origins'])==len(m['tokens']) and all(x['source']==header['id'] for x in m['model_origins'][:start]) and all(x['source'] in ids for x in m['model_origins'][start:])
  assert m['tokens'][start:]==s['tokens'] and m['origins'][start:]==s['origins']
  assert bytes(s['sources'][0]['opened_as'])==b'<command line>' and not s['sources'][0]['contents']
  view=lambda x:{k:x[k] for k in ['opened_as','identity','contents','included_at','logical_view']}
  assert [view(x) for x in user]==[view(x) for x in s['sources'][1:]]
  return m,s
 def focus(prefix,report):
  for row in report['rows']:
   name=row['name'];base=prefix+'/'+name+'/'
   assert row['exact'] and row['difference'] is None and row['issue'] is None
   assert H(data[base+'main.c'])==row['source_sha256'] and H(data[base+'header.h'])==row['header_sha256']
   for mode,run in row['runs'].items():assert run['exit']==0 and H(lzma.decompress(data[base+mode+'.stdout.xz']))==run['stdout_sha256']
   m,s=observations(base,row['model_user_token_start'])
   counts=[sum(bytes(x['opened_as'])==b'/header.h' for x in xs) for xs in [m['sources'],s['sources']]]
   host=lzma.decompress(data[base+'host.stdout.xz']).decode().splitlines();counts.append(sum(x.startswith('-> ') and x.endswith('/header.h') for x in host));assert len(set(counts))==1
   if name in old:assert (row['source_sha256'],row['header_sha256'])==(old[name]['source_sha256'],old[name]['header_sha256'])
 focus('focused-2',after)
 paths=J('path-corrected/report.json');assert len(paths['rows'])==6
 for row in paths['rows']:
  base='path-corrected/'+row['case']+'/'
  assert row['exact'] and row['opens']==2 and H(data[base+'main.c'])==row['source_sha256'] and H(data[base+'files.json'])==row['files_sha256']
  headers=J(base+'files.json')
  for name,body in headers:assert data[base+name.lstrip('/')]==body.encode()
  for mode,run in row['runs'].items():assert run['exit']==0 and H(lzma.decompress(data[base+mode+'.stdout.xz']))==run['stdout_sha256']
  m,s=observations(base,row['user_token_start'])
  assert len([x for x in m['sources'] if not x.get('generated')])-2==len(s['sources'])-2==2
  host=lzma.decompress(data[base+'host.stdout.xz']).decode().splitlines()
  assert sum(x.startswith('-> ') and any(x.endswith(name) for name,_ in headers) for x in host)==2
 # Initial host had /dir while both virtual inputs lacked it; retain both rejections.
 assert Z('path-initial/parent-alias/raw-model.stdout.xz')['status']=='model-rejected'
 assert Z('path-initial/parent-alias/origin-spec.stdout.xz')['status']=='preprocessing-error'
 gate_roots=re.findall(r'^include guard artifacts: (.+)$',data['gate/log.txt'].decode(),re.M);assert len(gate_roots)==1
 gp='gates/'+Path(gate_roots[0]).name+'/comparison';focus(gp,J(gp+'/report.json'))
 b=J('corpus-before/receipt.json');c=J('corpus-after/receipt.json');assert b['exit']==c['exit']==0 and b['source_hashes_verified'] and c['source_hashes_verified'] and b['sources']==c['sources'] and len(b['sources'])==846
 for p,h in b['sources'].items():assert supplement[p]==h
 tables=[]
 for prefix,receipt in [('corpus-before',b),('corpus-after',c)]:
  raw=data[prefix+'/comparison.tsv'];assert H(raw)==receipt['comparison_sha256'];tables.append(list(csv.DictReader(io.StringIO(raw.decode()),delimiter='\t')))
 for x,y in zip(*tables):
  for k in ['file','vs_tcc','vs_gcc','spec_tokens','tcc_tokens']:assert x[k]==y[k]
 assert J('corpus-comparison.json')['same_source_hashes']
 # Verify exact source/bundle observations independently of the summary counts.
 multi=J('post/multitu/comparison.json');arenas=J('post/multitu-arenas/report.json');assert len(multi['rows'])==8 and all(multi['guards'].values())
 assert sum(x['unit_count'] for x in multi['rows'])==15 and sum(x['exact'] for x in multi['rows'])==5
 assert multi['source_hashes']==arenas['source_hashes'] and multi['runtime_key']==arenas['runtime_key']
 for row in multi['rows']:
  base='post/multitu/'+row['case']+'/';bundle=J(base+'on.ghost.json');assert row['erasure'] and len(bundle['units'])==row['unit_count']
  for suffix in ['stdout','stderr','console']:assert data[base+'on.'+suffix]==data[base+'off.'+suffix]
  for i,u in enumerate(bundle['units']):
   assert u['ordinal']==i and bytes(u['filename']).decode()==row['expected_units'][i]['filename'] and H(bytes(u['source']['contents']))==row['expected_units'][i]['sha256']
   if row['exact']:
    raw=J(base+f'raw-{i}.stdout');assert raw['status']==u['status']=='ok'
    for k in ['cabs','bindings','excluded','tokens','origins']:assert raw[k]==u[k]
    assert raw['sources']==u['opening_files'] and not u['token_errors']
  if row['category']!='diagnostic':assert data[base+'on.arena']==data[base+'off.arena']==data['post/multitu-arenas/'+row['case']+'/reference.arena']
 assert all(x['same_units'] and x['same_virtual_files'] for x in J('post/pins.json')['original_fixture_comparison'])
 for scope,count in [('shared',51),('component',52),('independent',51)]:
  d=J('post/'+scope+'/comparison.json');assert d['acceptance']['accepted'] and d['summary']=={'exact':count,'explicit-nonsuccess':56-count}
 d=J('post/physical/comparison.json');assert d['summary']=={'exact':58,'explicit_differences':0} and all(d['harness_checks'].values())
 assert len(J('post/onoff/erasure.json')['rows'])==15 and all(x['exact'] for x in J('post/onoff/erasure.json')['rows'])
 def capture(prefix,cmp,model):
  cut=J(prefix+'cut.json')
  for n,info in cut['files'].items():
   raw=data[prefix+n] if prefix+n in data else gzip.decompress(data[prefix+n+'.gz']);assert H(raw)==info['sha256'] and len(raw)==info['bytes']
  get=lambda n:data[prefix+n] if prefix+n in data else gzip.decompress(data[prefix+n+'.gz'])
  arena=get('arena.bin');present=get('arena.present.bin');parts=model.split(b'\n',5)
  assert len(parts)==6 and parts[0]==b'TCCARENA 1' and len(parts[5])==int(parts[4].split()[1],16)
  base=int(parts[1].split()[1],16);entry=int(parts[2].split()[1],16);code,length=[int(x,16) for x in parts[3].split()[1:]]
  assert base<=entry<base+4194304 and base<=code<base+4194304 and 0<=length<=base+4194304-code
  assert cmp['handoff']=={'x10':entry,'x14':code,'x15':length}
  assert len(arena)==len(present)==4194304 and arena==parts[5].ljust(4194304,b'\0')
  assert set(present)<={0,1} and sum(present)==cut['arena_present_bytes'] and all(p or b==0 for b,p in zip(arena,present))
  assert cmp['status']=='equal' and cmp['compared_bytes']==4194304 and H(arena)==cmp['arena_sha256'] and H(model)==cmp['model_arena_sha256'] and cut['elf_sha256']==cmp['elf_sha256']
  if prefix+'source.c' in data:assert H(data[prefix+'source.c'])==cut['source_sha256']==cmp['source_sha256']
  regs={n:int(x,16) for n,x in re.findall(r'⟨Register\.(\w+), 0x([0-9a-fA-F]+)#64⟩',data[prefix+'registers.txt'].decode())};assert regs['PC']==cut['pc'] and all(regs[n]==v for n,v in cmp['handoff'].items())
 index=json.loads((here/'native-index.json').read_text());assert len(index)==6
 for entry in index:
  inv=J('gate-native/'+entry['prefix']+'/inventory.json');assert inv==entry['inventory'] and inv['successes']==1
  for row in inv['rows']:
   p='gate-native/'+entry['prefix']+'/'+row['directory']+'/';capture(p,row['comparison'],gzip.decompress(data[p+'model.arena.gz']))
 inv=J('new-native/capture/inventory.json');assert inv['successes']==3 and not inv['residuals']
 for row in inv['rows']:
  p='new-native/capture/'+row['directory']+'/';capture(p,row['comparison'],gzip.decompress(data[p+'model.arena.gz']))
 original={Path(x['source']).parent.name:x['comparison'] for x in inv['rows']}
 receipt=J('new-native/receipt.json');assert receipt['source_hashes_verified'] and len(receipt['rows'])==3
 for row in receipt['rows']:
  assert original[row['case']]['elf_sha256']==row['elf_sha256'] and original[row['case']]['capture_manifest_sha256']==row['capture_manifest_sha256']
  p='new-native/'+row['case']+'/';elf=lzma.decompress(data[p+'prog.elf.xz']);assert H(elf)==row['elf_sha256'] and row['exit']==0 and row['stdout_equal'] and data[p+'stdout']==b''
  assert data[p+'main.c'] in elf and data[p+'header.h'] in elf
  obs=J(p+'fresh-comparison.json')['observation'];assert obs['elf_sha256']==row['elf_sha256'] and obs['source_sha256']==H(data[p+'main.c']);capture(p+'fresh-capture/',obs,data[p+'reference.arena'])
  for name,h in row['artifacts'].items():assert H(data[p+name])==h
 print('Verified',len(data),'members;4163 gate inputs/5 source epochs;35 focused cases;846 before/after corpus inputs;15 TU records/5 checked bundles;6 gate captures,3 original captures,3 fresh same-ELF captures/executions')
if __name__=='__main__':main()
