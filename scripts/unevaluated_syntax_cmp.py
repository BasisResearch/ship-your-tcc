#!/usr/bin/env python3
"""Keep complete syntax, recursive origins and three distinct comparison scopes."""
import argparse, copy, gzip, json, os, subprocess
from pathlib import Path
from ghost_cabs_cmp import ROOT, sha, source_pins, build_receipt, first_diff
from injected_origins_cmp import compare, nodes
VALID = {
 'nested-selected': 'int f(int x){return _Generic(x,int:_Generic(x,int:1,default:2),default:3);}',
 'nested-unselected': 'int f(int x){return _Generic(x,int:1,default:_Generic(x,int:2,default:3));}',
 'default-first': 'int f(double x){return _Generic(x,default:7,int:9);}',
 'no-default': 'int f(int x){return _Generic(x,int:7,double:9);}',
 'typedef-control': 'typedef int Number; int f(Number x){return _Generic(x,Number:1,default:2);}',
 'pointer-control': 'typedef int *Pointer; int f(Pointer x){return _Generic(x,Pointer:1,default:2);}',
 'control-effects': 'int f(void){int n=0; return _Generic(n++,int:n,default:n+1);}',
 'unselected-effects': 'int f(void){int n=0; return _Generic(n,int:7,default:++n)+n;}',
 'control-call': 'int call(void); int f(void){return _Generic(call(),int:7,default:9);}',
 'unselected-call': 'int call(void); int f(void){return _Generic(0,int:7,default:call());}',
 'string-associations': 'char *f(int x){return _Generic(x,int:"a" "b",default:"c" "d");}',
 'assert-adjacent': '_Static_assert(1,"raw\\x41" "1" "\\101"); int x;',
 'assert-nested-blocks': 'int f(void){_Static_assert(1,"outer"); { _Static_assert(sizeof(int)==4,"inner"); } return 0;}',
 'assert-struct': 'struct S {_Static_assert(sizeof(int)==4,"member"); int n;};',
 'assert-union': 'union U {_Static_assert(1,"union"); int n;};',
 'assert-nested-struct': 'struct S {struct T {_Static_assert(1,"nested"); int n;} t; _Static_assert(1,"after");};',
 'assert-generic': '_Static_assert(_Generic(0,int:1,default:0),"generic"); int x;',
 'assert-string-generic': '_Static_assert(sizeof(_Generic(0,int:"ab",default:"long"))==3,"string"); int x;',
 'generic-declarations': 'int a[_Generic(0,int:2,default:3)]; int f(void){int n=_Generic(0,int:1,default:2); return n;}',
 'macro-origins': '#define PICK(x) _Generic((x),int:(x)+1,default:(x)+2)\n#define ASSERT(e) _Static_assert((e),"macro" " msg")\n#line 70 "unevaluated.c"\nASSERT(PICK(0)==1); int f(int n){return PICK(n++);}',
 'include-origins': '#include "syntax.h"\n_Static_assert(SELECT(0)==1,"header"); int f(int n){return SELECT(n);}',
}
RESIDUALS = {
 'assert-wide': '_Static_assert(1,L"wide"); int x;',
}
INVALID = {
 'false-file': '_Static_assert(0,"false file");',
 'false-block': 'int f(void){_Static_assert(0,"false block"); return 0;}',
 'false-struct': 'struct S {_Static_assert(0,"false member"); int n;};',
 'duplicate-default': 'int f(void){return _Generic(0,int:1,default:2,default:3);}',
 'duplicate-match': 'int f(void){return _Generic(0,int:1,int:2);}',
 'no-match': 'int f(void){return _Generic(0,double:1);}',
 'missing-colon': 'int f(void){return _Generic(0,int 1,default:2);}',
 'empty-unselected': 'int f(void){return _Generic(0,int:1,default:);}',
 'malformed-unselected': 'int f(void){return _Generic(0,int:1,default:2+);}',
 'trailing-comma': 'int f(void){return _Generic(0,int:1,);}',
 'assert-no-message': '_Static_assert(1);',
 'assert-nonstring': '_Static_assert(1,2);',
}
HEADER = '#define SELECT(x) _Generic((x),int:1,default:2)\nstruct Header {_Static_assert(1,"included"); int n;};\n'
BATCH = '''typedef int Number;
int called, effects;
int call(void){called++; return 2;}
_Static_assert(sizeof(int)==4,"int" " size");
struct S {_Static_assert(1,"member"); int n;};
int main(void){
 Number n=3;
 _Static_assert(_Generic(0,int:1,default:0),"generic assertion");
 { _Static_assert(sizeof(_Generic(0,int:"ab",default:"long"))==3,"nested"); }
 int a=_Generic(n++,int:7,default:++effects);
 int b=_Generic(call(),default:9,int:_Generic(n,int:5,default:6));
 int c=_Generic((double)0,default:11,int:++effects);
 char *s=_Generic(n,int:"a" "b",default:"c" "d");
 return (a!=7)||(b!=5)||(c!=11)||(n!=3)||called||effects||s[1]!=98;
}
'''

def main():
 ap=argparse.ArgumentParser(description=__doc__);a=ap.add_mutually_exclusive_group(required=True);a.add_argument('--build-id');a.add_argument('--gate-build',action='store_true');ap.add_argument('--out',type=Path,required=True);args=ap.parse_args()
 out=args.out.resolve();out.mkdir(parents=True,exist_ok=False)
 receipt=build_receipt(args.build_id) if args.build_id else {'scope':'preceding gate executable build; outer receipt required'}
 pins=source_pins();driver=sha(Path(__file__));exe=ROOT/'.lake/build/bin/tcc_ghost';binary=sha(exe);rows=[];inputs={}
 for name,source in (VALID|INVALID|RESIDUALS).items():
  d=out/name;d.mkdir();p=d/'prog.c';p.write_text(source+'\n');files=d/'files.json';files.write_text(json.dumps([['/syntax.h',HEADER]] if name=='include-origins' else [])+'\n');inputs[str(p)]=sha(p);inputs[str(files)]=sha(files)
  captures={}
  for mode in ['model','spec','raw-model','raw-spec','independent-raw']:
   cmd=[str(exe),mode,str(p),'--files',str(files)]
   q=subprocess.run(cmd,capture_output=True,timeout=120,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'})
   (d/(mode+'.json.gz')).write_bytes(gzip.compress(q.stdout,mtime=0));(d/(mode+'.stderr')).write_bytes(q.stderr);(d/(mode+'.command.json')).write_text(json.dumps(cmd)+'\n')
   assert q.returncode==0,(name,mode,q.returncode,q.stderr)
   captures[mode]=json.loads(q.stdout)
  scopes={}
  for label,m,s in [('shared-token',captures['model'],captures['spec']),('model-token-component',captures['raw-model'],captures['raw-spec']),('independent-raw',captures['raw-model'],captures['independent-raw'])]:
   diff=first_diff(m['cabs'],s['cabs']) if m['status']==s['status']=='ok' else 'incomplete Cabs construction'
   if label=='independent-raw' and diff is None:diff=compare(m,s)
   scopes[label]=dict(model=m['status'],spec=s['status'],difference=diff)
  # Raw invalid controls are observations even when the grammar accepts them.
  # The compiler must reject, or syntax construction must explicitly diagnose.
  valid=name in VALID
  accepted=all(x['difference'] is None for x in scopes.values()) if valid else captures['raw-model']['status']!='ok'
  if name in RESIDUALS:accepted=captures['raw-model']['status']=='model-rejected' and captures['independent-raw']['status']=='ok'
  rows.append(dict(case=name,valid=valid,residual=name in RESIDUALS,accepted=accepted,comparisons=scopes))
  print(name,'exact' if valid and accepted else ('known-valid-residual' if name in RESIDUALS and accepted else ('diagnostic' if not valid and accepted else 'FAILED')),flush=True)
 (out/'batch.c').write_text(BATCH);inputs[str(out/'batch.c')]=sha(out/'batch.c')
 assert source_pins()==pins and sha(exe)==binary and sha(Path(__file__))==driver
 assert all(sha(Path(p))==h for p,h in inputs.items())
 report=dict(build=receipt,driver_sha256=driver,binary_sha256=binary,source_hashes=pins,inputs=inputs,rows=rows,
             exact_valid=sum(r['valid'] and r['accepted'] for r in rows),diagnostics=sum(not r['valid'] and not r['residual'] and r['accepted'] for r in rows),residuals=[r['case'] for r in rows if r['residual']])
 (out/'comparison.json').write_text(json.dumps(report,indent=2)+'\n')
 return int(not all(r['accepted'] for r in rows))
if __name__=='__main__':raise SystemExit(main())
