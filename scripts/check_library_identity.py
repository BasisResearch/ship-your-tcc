#!/usr/bin/env python3
"""Resolved identity semantic regressions. Retain every command and byte stream."""
import argparse,hashlib,json,os,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
BIN=ROOT/'.lake/build/bin/csem_raw'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def invoke(cmd,stem):
 p=subprocess.run([str(x) for x in cmd],cwd=ROOT,env=dict(os.environ,LEAN_ABORT_ON_PANIC='1'),capture_output=True,timeout=120)
 stem.with_suffix('.command.json').write_text(json.dumps([str(x) for x in cmd],indent=2)+'\n')
 stem.with_suffix('.stdout').write_bytes(p.stdout);stem.with_suffix('.stderr').write_bytes(p.stderr)
 return p

def generate(d):
 d.mkdir(parents=True,exist_ok=True)
 headers={
 '/runtime/libc/include/fault.h':'#line 501 "ordinary.h"\nstatic int libfail(int z){return 2/z;}\n#define LIBDIV(z) (2/(z))\n#define LIBARG(z) (z)\n#define CAT(a,b) a##b\n#define STRING(a) #a\n',
 '/user/runtime/libc/include/fault.h':'#line 601 "/user/runtime/libc/include/fault.h"\nstatic int userfail(int z){return 3/z;}\n',
 '/libarg.h':'#define LIBZERO 0\n',
 }
 listing=[]
 for i,(name,body) in enumerate(headers.items()):
  p=d/f'header{i}.h';p.write_text(body);listing.append((name,p))
 for p in sorted((ROOT/'csem/runtime/libc/include').rglob('*.h')):
  listing.append(('/runtime/libc/include/'+str(p.relative_to(ROOT/'csem/runtime/libc/include')),p))
 fs=d/'fs.list';fs.write_text(''.join(f'{a}={b}\n' for a,b in listing))
 cases={
 'basic':('int main(void){return 7;}\n',7),
 'stdio':'#include "/runtime/libc/include/stdio.h"\nint main(void){return printf("id %d\\n",7)!=5;}\n',
 'manual-stdio':'int printf(const char *,...);\nint main(void){return printf("id %d\\n",7)!=5;}\n',
 'stdbool':'#include "/runtime/libc/include/stdbool.h"\nint main(void){bool b=true;return b!=1;}\n',
 'line-observables':'#line 713 "ordinary-alias.c"\nint main(void){return __LINE__!=713 || __FILE__[0]!=\'o\';}\n',
 'plant':'#line 701 "/fake/runtime/libcore/std.core"\nint main(void){int z=0;return 2/z;}\n',
 'user-looking':'#include "/user/runtime/libc/include/fault.h"\nint main(void){return userfail(0);}\n',
 'runtime-relabel':'#include "/runtime/libc/include/fault.h"\nint main(void){return libfail(0);}\n',
 'include-alias':'#include "/runtime/libc/include/../include//fault.h"\nint main(void){return libfail(0);}\n',
 'replacement':'#include "/runtime/libc/include/fault.h"\nint main(void){int z=0;return LIBDIV(z);}\n',
 'argument':'#include "/runtime/libc/include/fault.h"\nint main(void){int z=0;return LIBARG(4/z);}\n',
 'user-replacement':'#include "/libarg.h"\n#define USERDIV(z) (4/(z))\nint main(void){return USERDIV(LIBZERO);}\n',
 'paste':'#include "/runtime/libc/include/fault.h"\nint main(void){int zero=0;return 2/CAT(ze,ro);}\n',
 'stringify':'#include "/runtime/libc/include/fault.h"\nint main(void){return STRING(hello)[0]!=\'h\';}\n',
 'shadow-before':'static int printf(int x){return x+10;}\nint main(void){return printf(7)!=17;}\n',
 'shadow-after':'static int printf(int);\nint main(void){return printf(7)!=17;}\nstatic int printf(int x){return x+10;}\n',
 'shadow-redeclare':'static int printf(int);\nstatic int printf(int x){return x+10;}\nstatic int printf(int);\nint main(void){return printf(7)!=17;}\n',
 'pointer-shadow':'static int printf(int x){return x+10;}\nint main(void){int (*f)(int)=printf;return f(7)!=17;}\n',
 'pointer-neutral':'static int mine(int x){return x+10;}\nint main(void){int (*f)(int)=mine;return f(7)!=17;}\n',
 'redecl-pointer-shadow':'static int printf(int);\nstatic int printf(int x){return x+10;}\nstatic int printf(int);\nint main(void){int (*f)(int)=printf;return f(7)!=17;}\n',
 'redecl-pointer-neutral':'static int mine(int);\nstatic int mine(int x){return x+10;}\nstatic int mine(int);\nint main(void){int (*f)(int)=mine;return f(7)!=17;}\n',
 'shadow-close':'static int close(int);\nint main(void){return close(7)!=23;}\nstatic int close(int x){return x+16;}\n',
 'shadow-block':'static int printf(int x){return x+10;}\nint main(void){extern int printf(int);return printf(7)!=17;}\n',
 'self-include':'#ifndef IN_SELF\n#define IN_SELF\n#include __FILE__\nint main(void){return self_value!=37;}\n#else\nenum {self_value=37};\n#endif\n',
 }
 out=[]
 for name,body in cases.items():
  expected=0
  if isinstance(body,tuple):body,expected=body
  p=d/(name+'.c');p.write_text(body);out.append((name,p,expected))
 stale=d/'stale-main.txt';stale.write_text('#error stale main contents must not be read\n')
 self_source=next(p for n,p,_ in out if n=='self-include')
 listing.append((str(self_source),stale))
 fs.write_text(''.join(f'{a}={b}\n' for a,b in listing))
 return fs,listing,out

def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--artifacts',type=Path,required=True);a=ap.parse_args()
 d=a.artifacts.resolve();d.mkdir(parents=True,exist_ok=False)
 fs,headers,cases=generate(d/'sources');pins={str(p):sha(p) for _,p in headers};pins.update({str(p):sha(p) for _,p,_ in cases});pins[str(BIN)]=sha(BIN)
 common=['--fslist',fs,'--include','/runtime/libc/include']
 for name,_ in headers:
  if name.startswith('/runtime/') or name=='/libarg.h':common+=['--library',name]
 rows=[]
 for name,p,expected in cases:
  q=invoke([BIN,'--checked','--semantic',*common,p],d/name)
  row=dict(case=name,exit=q.returncode)
  if q.returncode:row['status']='HARNESS_ERROR'
  else:
   envelope=json.loads(q.stdout);r=envelope['parse'];sem=envelope['semantics'];row['semantics']=sem
   if name in ['plant','user-looking','runtime-relabel','include-alias','replacement','argument','user-replacement','paste']:
    row['status']='PASS' if isinstance(sem,list) and sem and all(x['status']=='undefined' for x in sem) else 'SEMANTIC_DIFF'
   else:
    row['status']='PASS' if isinstance(sem,list) and sem and all(x['status']=='defined' and x['exit']==expected for x in sem) else 'SEMANTIC_DIFF'
   # A source-labelled position cannot silently fall through to the suffix
   # classifier on this interface. unknown/internal positions have no file.
   def positions(v):
    if isinstance(v,dict):
     if all(k in v for k in ['file','line','col','lexing_file','lexing_line']):yield v
     for c in v.values():yield from positions(c)
    elif isinstance(v,list):
     for c in v:yield from positions(c)
   pos=list(positions(r['cabs']));row['cabs_positions']=len(pos)
   if not pos or any('source_identity' not in x for x in pos):row['status']='MISSING_IDENTITY'
   if name in ['stdio','manual-stdio'] and any(x.get('stdout')!='id 7\n' for x in sem):row['status']='STDOUT_DIFF'
   # Semantic locations must preserve planted/presumed labels, and user
   # declarations in library-looking directories must remain user code.
   if name=='plant' and any(x.get('file')!='/fake/runtime/libcore/std.core' for x in positions(sem)):row['status']='LABEL_DIFF'
   for consumer in r['consumers']:
    source=consumer['input'];library=source['source_identity']['runtimeLibrary']
    expected_location={'tag':'Loc_point','pos':source}
    if library:
     expected_location={'tag':'Loc_point','pos':{'file':'caller.c','line':123,'col':4,'lexing_file':'caller.c','lexing_line':123,'source_identity':{'resolved':[[ord(c) for c in 'caller.c']],'runtimeLibrary':False}}}
    if any(loc!=expected_location for loc in consumer['locations'].values()):
     row['status']='CONSUMER_DIFF';row.setdefault('consumer_differences',[]).append(consumer)
   row['semantic_consumer_checks']=4*len(r['consumers'])
   row['semantic_positions']=list(positions(sem))
   if name in ['plant','user-looking','runtime-relabel','include-alias','replacement','argument','user-replacement','paste'] and not row['semantic_positions']:
    row['status']='MISSING_SEMANTIC_POSITION'
   if name in ['user-looking','runtime-relabel','include-alias']:
    expected_file='/user/runtime/libc/include/fault.h' if name=='user-looking' else str(p)
    if any(pos['file']!=expected_file for pos in row['semantic_positions']):row['status']='SEMANTIC_ATTRIBUTION_DIFF'

   # Mixed provenance: metadata is inspected alongside full semantic UB
   # outcomes, never counted as a substitute for the latter.
   row['origin_membership']=[dict(identity=x['identitySite']['physical']['identity'],label=x['site']['presumed'],library=x['actualLibrary']) for x in r['libraryOrigins']]
  rows.append(row);print(row['status'],name,flush=True)
 assert all(sha(p)==h for p,h in pins.items()),'pinned input/binary drift'
 (d/'results.json').write_text(json.dumps(dict(pins=pins,rows=rows),indent=2)+'\n')
 pointer_rows=[r for r in rows if r['case'].startswith('redecl-pointer-')]
 assert len(pointer_rows)==2 and pointer_rows[0]['semantics']==pointer_rows[1]['semantics'], 'shadowing changed function-pointer identity'
 return int(any(r['status'] != 'PASS' for r in rows))
if __name__=='__main__':raise SystemExit(main())
