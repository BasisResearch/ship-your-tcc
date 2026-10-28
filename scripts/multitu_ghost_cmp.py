#!/usr/bin/env python3
"""Ordered model handoff bundles vs independent Raw.checked on physical inputs.

The independent executable receives actual filenames/bytes and the configured
filesystem only. No exported token, syntax, origin or authority is an input.
"""
import argparse,copy,hashlib,json,os,subprocess
from pathlib import Path
from ghost_cabs_cmp import ROOT,sha,source_pins,build_receipt,first_diff
from injected_origins_cmp import compare,identifiers
from source_fixtures import load_invocation
import model_diff as md

def invoke(command,stem):
 p=subprocess.run(command,cwd=ROOT,capture_output=True,timeout=240,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'})
 Path(str(stem)+'.stdout').write_bytes(p.stdout);Path(str(stem)+'.stderr').write_bytes(p.stderr)
 return dict(command=list(map(str,command)),exit=p.returncode,stdout=p.stdout.decode(),stderr=p.stderr.decode())

def check_bundle(bundle,expected,raw):
 units=bundle.get('units',[])
 if len(units)!=len(expected):return 'ordered unit count'
 for i,(unit,(name,path),spec) in enumerate(zip(units,expected,raw)):
  if unit['ordinal']!=i or bytes(unit['filename']).decode()!=name:return f'unit {i}: order/name'
  cfg=unit['configuration']
  if cfg!={'cversion':201112,'include_paths':[],'system_paths':[list(b'/lib/tcc/include'),list(b'/include')],'command_defines':[],'command_includes':[]}:return f'unit {i}: unexpected raw configuration'
  source=unit.get('source')
  if source is None or source['contents']!=list(path.read_bytes()) or source['opened_as']!=list(name.encode()):return f'unit {i}: input bytes'
  if unit['compiler_result']!=0:return f'unit {i}: compiler nonsuccess'
  issue=compare(dict(unit,section_bytes_equal=True,console_equal=True),spec)
  if issue:return f'unit {i}: '+issue
  physical=[s for s in unit['sources'] if not s['generated']]
  if any(s['logical_view'] is None for s in physical):return f'unit {i}: missing physical log'
 return None

def omission_checks(bundle,expected,raw):
 checks={}
 b=copy.deepcopy(bundle);b['units']=b['units'][-1:];checks['last_unit_only_rejected']=bool(check_bundle(b,expected,raw))
 b=copy.deepcopy(bundle);b['units'].reverse();checks['reordered_bundle_rejected']=bool(check_bundle(b,expected,raw))
 for label,change in [
  ('earlier_cabs_dropped',lambda u:u.update(cabs={})),
  ('earlier_openings_dropped',lambda u:u.update(opening_files=[])),
  ('earlier_authority_dropped',lambda u:u.update(compiler_event=None)),
  ('earlier_bytes_changed',lambda u:u['source']['contents'].append(32))]:
  b=copy.deepcopy(bundle);change(b['units'][0]);checks[label]=bool(check_bundle(b,expected,raw))
 assert all(checks.values()),checks
 return checks

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--build-id',required=True);ap.add_argument('--out',type=Path,required=True);a=ap.parse_args()
 out=a.out.resolve();out.mkdir(parents=True,exist_ok=False)
 receipt=build_receipt(a.build_id);pins=source_pins();binary={str(p):sha(p) for p in [ROOT/'.lake/build/bin/tcc_model',ROOT/'.lake/build/bin/tcc_ghost']};runtime=md.runtime_key();symbols=sha(ROOT/'build/image.syms')
 invocation=load_invocation(ROOT/'c/invocations/tinycc-104-inline.json',ROOT)
 cases=[('inline104',[(x,Path(dict(invocation['extra'])[x])) for x in invocation['args'][1:]]+[('/prog.c',Path(invocation['source']))],invocation['extra'],'success')]
 fixtures=out/'inputs';fixtures.mkdir()
 common='''#ifndef COMMON_H
#define COMMON_H
#ifndef WIDTH
#error missing local WIDTH
#endif
typedef WIDTH T;
static T header_local;
#endif
'''
 first='''#define WIDTH int
#define LEAK 99
#include "same.h"
#include "same.h"
struct Collision { T member; };
_Static_assert(__COUNTER__==0,"counter first");
static T collide=3;
static int local(void){return collide;}
int companion(void){return local();}
'''
 last='''#ifdef LEAK
#error macro leaked across translation units
#endif
#define WIDTH long
#include "two/same.h"
#include "two/same.h"
struct Collision { T member; };
_Static_assert(__COUNTER__==0,"counter reset");
struct Reset {char c; int x;};
_Static_assert(sizeof(struct Reset)==8,"pack reset");
static T collide=4;
static int local(void){return collide;}
int companion(void);
int main(void){return companion()+local()-7;}
'''
 for name,src,category in [
  ('resets',last,'success'),
  ('definition-snapshot',last,'success'),
  ('pack-layout-residual',last,'raw-layout-gap'),
  ('include-cache-residual',last,'guarded-success'),
  ('user-same-header','#define __uint128__ user_uint128\n#include <tccdefs.h>\n'+last,'user-asm-construction'),
  ('later-error','int main(void){return ; +;}\n','diagnostic'),
  ('earlier-error',last,'diagnostic')]:
  prefix=first
  if name=='definition-snapshot':prefix+='int later();\nstatic inline int unused_early(void){return later(1); }\nint later(void){return 0;}\n'
  if name=='pack-layout-residual':prefix+='#pragma pack(1)\n'
  if name=='user-same-header':prefix='#define __uint128__ first_uint128\n#include <tccdefs.h>\n'+prefix
  if name=='earlier-error':prefix='int invalid=;\n'
  directory=fixtures/name;directory.mkdir()
  p=directory/'main.c';p.write_text(src)
  q=directory/'unit.c';q.write_text(prefix)
  header=common if category=='guarded-success' else common.replace('#ifndef COMMON_H\n#define COMMON_H\n','').removesuffix('#endif\n')
  h=directory/'one.h';h.write_text(header)
  j=directory/'two.h';j.write_text(header+'/* distinct physical header */\n')
  cases.append((name,[('/one/unit.c',q),('/prog.c',p)],[('/one/unit.c',str(q)),('/one/same.h',str(h)),('/two/same.h',str(j))],category))
 input_pins=dict(invocation['pins']);rows=[];guards={}
 for name,expected,extras,category in cases:
  dest=out/name;dest.mkdir();fs=dest/'files.json'
  fs.write_text(json.dumps([(virtual,Path(host).read_text()) for virtual,host in extras])+'\n')
  for _,host in extras:input_pins[str(Path(host))]=sha(Path(host))
  for _,path in expected:input_pins[str(path)]=sha(path)
  input_pins[str(fs)]=sha(fs)
  runs=[]
  for mode in ['off','on']:
   base=dest/mode
   command=[str(ROOT/'.lake/build/bin/tcc_model'),'--prog',str(expected[-1][1]),'--arg','-w']
   for filename,_ in expected[:-1]:command+=['--arg',filename]
   for virtual,host in extras:command+=['--extra',virtual+'='+str(host)]
   command+=['--arena-out',str(base)+'.arena','--console-out',str(base)+'.console']
   if mode=='on':command+=['--ghost-out',str(base)+'.ghost.json']
   result=invoke(command,base)
   result['arena']=sha(Path(str(base)+'.arena')) if Path(str(base)+'.arena').exists() else None
   result['console']=sha(Path(str(base)+'.console'))
   if result['arena'] is not None:
    hdr,payload=md.parse_arena(str(base)+'.arena')
    result['full_arena_sha256']=hashlib.sha256(payload.ljust(4*1024*1024,b'\0')).hexdigest()
    result['arena_header']=hdr
   runs.append(result)
  equal=all(runs[0][k]==runs[1][k] for k in ['exit','stdout','stderr','arena','console'])
  assert equal,(name,'on/off status, console or arena difference')
  bundle=json.loads((dest/'on.ghost.json').read_text())
  raw=[];raw_runs=[]
  for i,(filename,path) in enumerate(expected):
   command=[str(ROOT/'.lake/build/bin/tcc_ghost'),'independent-raw',str(path),'--main-name',filename,'--files',str(fs)]
   result=invoke(command,dest/f'raw-{i}');assert result['exit']==0,result
   raw.append(json.loads(result['stdout']))
   raw_runs.append(dict(command=result['command'],exit=result['exit'],stdout_sha256=sha(dest/f'raw-{i}.stdout'),stderr_sha256=sha(dest/f'raw-{i}.stderr')))
  issue=check_bundle(bundle,expected,raw) if category not in ['diagnostic','raw-layout-gap','raw-opening-gap'] else None
  syntax_component=False
  if category=='raw-layout-gap':
   assert raw[0]=={'status':'raw-checked-error','error':'unimplemented-pack-layout'}
   result=invoke([str(ROOT/'.lake/build/bin/tcc_ghost'),'independent-parse',str(expected[0][1]),'--main-name',expected[0][0],'--files',str(fs)],dest/'raw-0-parse-component')
   assert result['exit']==0
   parsed=[json.loads(result['stdout']),raw[1]]
   # Raw.run's phase-four export retains pragma events; the model token
   # arena contains language tokens. This is a separately labeled syntax
   # component, never full Raw.checked or phase-four token agreement.
   for unit,spec in zip(bundle['units'],parsed):
    assert unit['status']==spec['status']=='ok'
    for key in ['cabs','bindings','excluded','origins']:assert unit[key]==spec[key],(name,key)
    assert unit['opening_files']==spec['sources']
   assert compare(dict(bundle['units'][1],section_bytes_equal=True,console_equal=True),parsed[1]) is None
   syntax_component=True
   issue='independent Raw.checked: unimplemented-pack-layout (unit 0)'
   assert runs[0]['stdout']=='ran\n' and runs[0]['arena'] is not None
  if category=='raw-opening-gap':
   for i,(unit,spec) in enumerate(zip(bundle['units'],raw)):
    assert unit['status']==spec['status']=='ok'
    for key in ['cabs','bindings','excluded','tokens','origins']:assert unit[key]==spec[key],(name,i,key)
    assert len(unit['opening_files'])+1==len(spec['sources'])
   issue='independent Raw.checked include-guard opening schedule differs (both units)'
   syntax_component=True
   assert runs[0]['stdout']=='ran\n' and runs[0]['arena'] is not None
  if category=='diagnostic':
   wanted=1 if name=='earlier-error' else 2
   assert len(bundle['units'])==wanted and bundle['units'][-1]['compiler_result']==-1,(name,'missing diagnostic unit')
   assert runs[0]['stdout']=='exit 1\n' and runs[0]['arena'] is None
   if wanted==2:assert check_bundle(dict(units=bundle['units'][:1]),expected[:1],raw[:1]) is None
  elif category not in ['raw-layout-gap','raw-opening-gap']:
   assert issue is None,(name,issue)
   assert runs[0]['stdout']=='ran\n' and runs[0]['arena'] is not None
   event_ids=[i for u in bundle['units'] for i in [u['compiler_event']['root_id'],u['compiler_event']['header_id']]]
   assert len(event_ids)==len(set(event_ids)), 'cross-unit compiler opening IDs collapsed'
   if name=='definition-snapshot':
    assert identifiers(bundle['units'][0]['cabs'],'unused_early')
   if name=='resets':
    guards=omission_checks(bundle,expected,raw)
    # Both TU-local names survive; no global ghost map picks the last one.
    for unit in bundle['units']:assert identifiers(unit['cabs'],'collide')
    # Identical basename does not collapse two physical header identities.
    for i,unit in enumerate(bundle['units']):
     header=('/one/same.h' if i==0 else '/two/same.h').encode()
     assert sum(bytes(s['opened_as'])==header for s in unit['sources'])==2 # actual repeated unguarded openings
   if name=='user-same-header':
    for unit in bundle['units']:
     assert len(unit['bindings'])==21 and len(unit['excluded'])==21
     assert sum(bytes(s['opened_as'])==b'/lib/tcc/include/tccdefs.h' for s in unit['sources'])==2
  rows.append(dict(case=name,category=category,expected_units=[dict(filename=n,source=str(p),sha256=sha(p)) for n,p in expected],raw_runs=raw_runs,unit_count=len(bundle['units']),exact=issue is None and category!='diagnostic',syntax_component_exact=syntax_component,issue=issue,erasure=equal,runs=runs))
  label='checked-exact' if issue is None and category!='diagnostic' else ('matching-diagnostic' if category=='diagnostic' else 'explicit-residual')
  print(name,category,label,len(bundle['units']),'units',flush=True)
 assert source_pins()==pins and all(sha(Path(p))==h for p,h in binary.items())
 assert all(sha(Path(p))==h for p,h in input_pins.items()) and md.runtime_key()==runtime and sha(ROOT/'build/image.syms')==symbols
 report=dict(build=receipt,source_hashes=pins,binary_hashes=binary,runtime_key=runtime,image_symbols_sha256=symbols,input_pins=input_pins,guards=guards,rows=rows)
 (out/'comparison.json').write_text(json.dumps(report,indent=2)+'\n')
if __name__=='__main__':main()
