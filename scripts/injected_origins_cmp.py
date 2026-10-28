#!/usr/bin/env python3
"""Full model Cabs vs independent Raw.checked cfgTcc, with all injected aliases.

Each process gets source/files only. Never pass exported tokens, Cabs, origins
or an authority witness back into model construction. raw-spec remains a
separate parser-construction component check in ghost_cabs_cmp.py --raw.
"""
import argparse,copy,gzip,json,os,subprocess
from pathlib import Path
from ghost_cabs_cmp import ROOT,sha,source_pins,build_receipt,first_diff
from check_injected_bindings import validate_bindings
IMAGE='/lib/tcc/include/tccdefs.h'
def nodes(x):
 if isinstance(x,dict):
  yield x
  for y in x.values():yield from nodes(y)
 elif isinstance(x,list):
  for y in x:yield from nodes(y)
def identifiers(cabs,name):
 return [x for x in nodes(cabs) if x.get('name')==name and 'loc' in x]
def compare(m,s):
 if m['status']!='ok' or s['status']!='ok':return 'incomplete Cabs construction'
 if m.get('token_errors'):return 'incomplete model token provenance'
 if m.get('section_bytes_equal') is not True or m.get('console_equal') is not True:return 'ghost erasure bytes differ'
 for key in ['cabs','bindings','excluded','tokens','origins']:
  d=first_diff(m[key],s[key])
  if d:return key+':'+d
 d=first_diff(m['opening_files'],s['sources'])
 if d:return 'opening graph:'+d
 event=m.get('compiler_event')
 if event is None:return 'missing designated opening event'
 if event['authenticated']!=(s['compiler_opening'] is not None):return 'opening authority differs'
 if event['directive']!=s['compiler_opening'] and event['authenticated']:return 'designated directive differs'
 roots=[f for f in m['sources'] if f['id']==event['root_id']]
 headers=[f for f in m['sources'] if f['id']==event['header_id']]
 if len(roots)!=1 or len(headers)!=1:return 'missing actual opening IDs'
 root,header=roots[0],headers[0]
 if root['contents']!=event['predefined'] or header['contents']!=event['header_contents'] or header['included_at']!=event['directive']:return 'event bytes/edge disconnected from actual openings'
 return None

def fixture_cases(original):
 ordinary='int main(void){return 0;}\n'
 # Each tuple: source, virtual files, independent library policy,
 # expected authenticated bindings, expected user asm exclusions.
 # Re-inclusion renames only the otherwise illegally repeated aggregate tag;
 # the opened header bytes and all21 original asm carriers remain complete.
 cases={'pinned':(ordinary,[],[],21,0),'user-same-header':('#define __uint128__ user_uint128\n#include <tccdefs.h>\n'+ordinary,[],[],21,21),
 'label-spoof':('#line 41 "<command line>"\nextern int __builtin_forged(void) __asm__("memcpy");\n'+ordinary,[],[],21,1),
 'builtin-prefix':('extern int __builtin_user(void) __asm__("strlen");\n'+ordinary,[],[],21,1),
 'direct-label':('extern int mine(void) __asm__("strlen");\n'+ordinary,[],[],21,1),
 'macro-label':('#define ASM(x) __asm__(x)\nextern int mine(void) ASM("strlen");\n'+ordinary,[],[],21,1),
 'builtin-redeclaration':('void *__builtin_memcpy(void *, const void *, unsigned long);\n'+ordinary,[],[],21,0),
 'configured-predefine':('int n=__SIZEOF_LONG__;\n'+ordinary,[],[],21,0),
 'empty-openings':('#include "empty.h"\n#include "empty.h"\n'+ordinary,[('/empty.h','')],[],21,0),
 'nested-identity':('#include "outer.h"\nextern int HEADER_NAME;\n#define USER_NAME user_macro\nextern int USER_NAME;\n#undef HEADER_NAME\n#define HEADER_NAME redefined_macro\nextern int HEADER_NAME;\nextern int user_direct;\n'+ordinary,
 [('/outer.h','#include "library.h"\n'),('/library.h','#line 800 "ordinary-label.h"\nextern int header_direct;\n#define HEADER_NAME header_macro\n')],['/library.h'],21,0),
 'header-label':('#include "user.h"\n'+ordinary,[('/user.h','extern int mine(void) __asm__("strlen");\n')],['/user.h'],21,1),
 'redefinition-identity':('#include "library.h"\nextern int SAME;\n#define SAME same\nextern int SAME;\n'+ordinary,[('/library.h','#define SAME same\n')],['/library.h'],21,0),
 'same-header-library':('#define __uint128__ user_uint128\n#include <tccdefs.h>\n'+ordinary,[],[IMAGE],21,21)}
 for name,header,libs in [
  ('relabel-content','#line 701 "ordinary-header.h"\n'+original,[]),
  ('changed-same-path',original+'\n',[]),
  ('library-membership','#line 701 "ordinary-header.h"\n'+original,[IMAGE]),
  ('forged-signature',original.replace('__BOTH(void*, memcpy,','__BOTH(int, memcpy,'),[]),
  ('forged-target',original.replace('__RENAME(#name);','__RENAME("memcpy");'),[])]:
  cases[name]=(ordinary,[(IMAGE,header)],libs,0,21)
 return cases

def check_roles(name,m):
 c=m['cabs'];issues=[]
 if name=='nested-identity':
  for symbol,library,path in [('header_direct',True,'library.h'),('header_macro',True,'library.h'),('user_macro',False,'prog.c'),('redefined_macro',False,'prog.c'),('user_direct',False,'prog.c')]:
   ids=identifiers(c,symbol)
   if len(ids)!=1:issues.append('missing '+symbol);continue
   identity=ids[0]['loc']['begin']['source_identity']
   if identity!={'resolved':[list(path.encode())],'runtimeLibrary':library,'compilerInjected':False}:issues.append('wrong identity '+symbol)
 if name=='redefinition-identity':
  ids=identifiers(c,'same');policies=[i['loc']['begin']['source_identity']['runtimeLibrary'] for i in ids]
  if policies!=[True,False]:issues.append('same redefinition identity '+str(policies))
 # Every full injected declaration/signature/located asm argument is checked,
 # including one alloca intrinsic. validate_bindings also checks containment.
 if len(m['bindings'])==21 and not validate_bindings(m):issues.append('lost complete binding/signature/spelling')
 carriers=[x for x in nodes(c) if x.get('tag')=='Attribute' and x['id']['name']=='asm_label']
 if any(not x.get('args') or not x.get('ns') for x in carriers):issues.append('lost asm arguments/keyword')
 return issues

def omission_checks(m,s):
 checks={}
 for label,mutate in [
  ('missing-event',lambda x:x.update(compiler_event=None)),
  ('missing-graph',lambda x:x.update(opening_files=[])),
  ('extra-opening',lambda x:x['opening_files'].append(copy.deepcopy(x['opening_files'][-1]))),
  ('changed-event-bytes',lambda x:x['compiler_event']['header_contents'].append(10)),
  ('wrong-opening-id',lambda x:x['compiler_event'].update(header_id=999999)),
  ('forged-authority',lambda x:x['compiler_event'].update(authenticated=False)),
 ]:
  x=copy.deepcopy(m);mutate(x);checks[label]=compare(x,s) is not None
 assert all(checks.values()),checks
 return checks

def main():
 ap=argparse.ArgumentParser(description=__doc__);group=ap.add_mutually_exclusive_group(required=True);group.add_argument('--build-id');group.add_argument('--gate-build',action='store_true');ap.add_argument('--out',type=Path,required=True);a=ap.parse_args()
 receipt=build_receipt(a.build_id) if a.build_id else {'scope':'preceding check.sh executable build; completion requires outer frozen gate receipt'};pins=source_pins();driver=sha(Path(__file__));exe=ROOT/'.lake/build/bin/tcc_ghost';binary=sha(exe)
 out=a.out.resolve();out.mkdir(parents=True,exist_ok=False)
 # The actual stripped image bytes must agree with both the stripper and table;
 # check_injected_openings.py independently checks these same byte pins.
 entries=dict(line.split('=',1) for line in (ROOT/'c/build/fs.list').read_text().splitlines())
 original=Path(entries[IMAGE]).read_text();header_pin=sha(Path(entries[IMAGE]));rows=[];checks={};inputs={}
 for name,(source,files,libs,bindings,excluded) in fixture_cases(original).items():
  d=out/name;d.mkdir();src=d/'prog.c';src.write_text(source);fs=d/'files.json';fs.write_text(json.dumps(files)+'\n');inputs[str(src)]=sha(src);inputs[str(fs)]=sha(fs)
  data={}
  for mode in ['raw-model','independent-raw']:
   cmd=[str(exe),mode,str(src),'--files',str(fs)]
   for lib in libs:cmd+=['--library',lib]
   q=subprocess.run(cmd,capture_output=True,timeout=120,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'})
   (d/(mode+'.command.json')).write_text(json.dumps(cmd)+'\n');(d/(mode+'.json.gz')).write_bytes(gzip.compress(q.stdout,mtime=0));(d/(mode+'.stderr')).write_bytes(q.stderr)
   assert q.returncode==0,(name,mode,q.returncode,q.stderr)
   data[mode]=json.loads(q.stdout)
  m,s=data['raw-model'],data['independent-raw'];difference=compare(m,s)
  issues=[] if difference else check_roles(name,m)
  if m.get('status')=='ok':
   for path,body in files:
    openings=[f for f in m['opening_files'] if bytes(f['opened_as']).decode()==path]
    if not openings or any(bytes(f['contents'])!=body.encode() for f in openings):issues.append('requested file bytes not actually opened: '+path)
   if len(m['bindings'])!=bindings or len(m['excluded'])!=excluded:issues.append('wrong binding/exclusion inventory')
  if name=='pinned' and not difference:checks=omission_checks(m,s)
  row=dict(case=name,exact=difference is None and not issues,first_difference=difference,issues=issues,bindings=len(m.get('bindings',[])),excluded=len(m.get('excluded',[])))
  rows.append(row);print(name,'exact' if row['exact'] else 'DIFFERENT',difference or issues or '',flush=True)
 assert source_pins()==pins and sha(exe)==binary and sha(Path(__file__))==driver and sha(Path(entries[IMAGE]))==header_pin
 assert all(sha(Path(p))==h for p,h in inputs.items())
 report=dict(scope='model parser/actions vs independent Raw.checked cfgTcc; full Cabs, tokens, recursive origins and opening graph',build=receipt,binary_sha256=binary,source_hashes=pins,driver_sha256=driver,header_sha256=header_pin,inputs=inputs,harness_checks=checks,rows=rows,exact=sum(r['exact'] for r in rows))
 (out/'comparison.json').write_text(json.dumps(report,indent=2)+'\n')
 return int(not rows or not all(r['exact'] for r in rows))
if __name__=='__main__':raise SystemExit(main())
