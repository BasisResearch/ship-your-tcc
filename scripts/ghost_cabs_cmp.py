#!/usr/bin/env python3
"""Exact independent model/spec Cabs comparison; never erase locations/AST fields.

Each process receives only source bytes. The --raw spec path runs
parsePositioned on model tokens: a parser-construction component check.
--independent-raw instead calls Raw.checked cfgTcc on source bytes/files.
Non-successes remain separate from exact successes, with complete artifacts.
"""
import argparse
from collections import Counter
import hashlib,json,os,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
CASES={
 'scalar-declarations':('declarations','extern unsigned long x; static const int y=3; int z;'),
 'multi-declaration':('declarations','int x=1,y=2,z;'),
 'all-qualifiers':('declarations','const volatile unsigned long *restrict p;'),
 'pointer-qualifiers':('declarators','int *const *volatile p;'),
 'nested-declarator':('declarators','int (*p)[3]; int *a[2];'),
 'function-prototype':('function-types','int f(int x, const char *s, ...);'),
 'abstract-params':('function-types','int f(int, const char *, int (*)(int));'),
 'void-prototype':('function-types','int f(void);'),
 'function-pointer':('function-types','int (*fp)(int x, int y);'),
 'definition':('control-flow','int f(int x){return x+1;}'),
 'typedef-scope':('typedefs','typedef int T; T f(T x){T y=x; {int T=2; y+=T;} return y;}'),
 'typedef-shadow':('typedefs','typedef int T; int f(void){int T=3; return T;}'),
 'folded-expression':('expressions','int x=1+2*3;'),
 'spelling':('expressions','unsigned long x=0XffUL; int y=010+0x10;'),
 'float-spelling':('expressions','double x=0x1.8p+2; float y=1.2500f;'),
 'char-spelling':('expressions',r"int x='\x41'; int y='\101';"),
 'string-boundaries':('expressions',r'char *p="\x41" "1" "\101";'),
 'assignment-comma':('expressions','int f(int x){int y=1; return (x+=2,y*=3,x+y);}'),
 'all-binary':('expressions','int f(int a,int b){return ((a*b+a/b-a%b)<<2) >= (b>>1) && a!=b || (a&b)^(a|b);}'),
 'logical-chain':('expressions','int f(int a,int b,int c){return a&&b&&c||a||b;}'),
 'unary':('expressions','int f(int x){int *p=&x; return -x + ~*p + !x + +x;}'),
 'increments':('expressions','int f(int x){x++; --x; return ++x+x--;}'),
 'conditional':('expressions','int f(int x){return x ? 2+3 : 4*5;}'),
 'gnu-conditional':('gnu','int f(int x){return x ?: 3;}'),
 'cast-sizeof':('expressions','int f(int x){return (unsigned int)x+sizeof(x)+sizeof(long);}'),
 'array-subscript':('expressions','int a[3]; int f(int i){return a[i];}'),
 'function-call':('expressions','int g(int,int); int f(int x){return g(x+1,2*x);}'),
 'nested-array-init':('initializers','int a[2][2]={{1,2},{3,4}};'),
 'array-designators':('initializers','int a[5]={[1]=2,[3]=4};'),
 'incomplete-array':('initializers','int a[]={1,2,3};'),
 'struct-members':('declarations','struct S {const int x; volatile long y;};'),
 'nested-designators':('initializers','struct S {int a[2];}; struct S s={.a={[1]=3}};'),
 'if-else':('control-flow','int f(int x){if(x) return 1; else return 2;}'),
 'while-break':('control-flow','int f(int x){while(x){if(x==3)break; x--; continue;} return x;}'),
 'do-while':('control-flow','int f(int x){do {x--;} while(x>0); return x;}'),
 'for-expr':('control-flow','int f(int x){for(x=0;x<3;x++){} return x;}'),
 'for-decl':('control-flow','int f(void){int x=0; for(int i=0;i<3;i++)x+=i; return x;}'),
 'for-empty':('control-flow','int f(int x){for(;;){break;} return x;}'),
 'labels':('control-flow','int f(int x){if(x)goto out; x=1; out: return x;}'),
 'switch':('control-flow','int f(int x){switch(x){case 1:return 2; default:return 3;}}'),
 'case-range':('gnu','int f(int x){switch(x){case 1 ... 3:return 2; default:return 3;}}'),
 'statement-expression':('gnu','int f(int x){return ({int y=x+1; y*2;});}'),
 'typeof':('gnu','int x; typeof(x) y;'),
 'generic':('expressions','int f(int x){return _Generic(x,int:1,default:2);}'),
 'choose':('gnu','int x=__builtin_choose_expr(1,2,3);'),
 'attribute':('gnu','int x __attribute__((aligned(16)));'),
 'static-assert':('declarations','_Static_assert(sizeof(int)==4,"int"); int x;'),
 'ucn':('identities',r'int \u00e9; int f(void){return é;}'),
 'macro-provenance':('provenance','#define ADD(x) ((x)+1)\nint f(int x){return ADD(x);}\n'),
 'line-provenance':('provenance','#line 200 "virtual.c"\nint x=3;\n'),
 'invalid-initializer':('invalid','int x=;'),
 'invalid-declarator':('invalid','int (*x;'),
 'kr-definition':('gnu','int f(x) int x; {return x;}'),
 'inline-unused':('gnu','static inline int f(void){return 1;} int x;'),
 'local-labels':('gnu','int f(void){__label__ out; goto out; out: return 1;}'),
 'pack':('provenance','#pragma pack(1)\nstruct S {char a; int b;};\n'),
}
# These are explicit diagnostics, never successful coverage or Supported
# exclusions. Every other fixture is a required exact positive control.
DIAGNOSTIC_CONTROLS = {
    'gnu-conditional','typeof','choose','attribute',
    'invalid-initializer','invalid-declarator','kr-definition','inline-unused',
    'local-labels','pack',
}
EXPECTED_EXACT = set(CASES) - DIAGNOSTIC_CONTROLS

def source_pins():
    paths = set(ROOT.glob('Tcc/Model/*.lean')) | set(ROOT.glob('Spec/Pre/*.lean'))
    paths |= set(ROOT.glob('csem/cparser/CSemCParser/*.lean'))
    paths |= {ROOT / p for p in [
        'Tcc/ModelMain.lean','lean/Ghost/GhostMain.lean','Spec/ImageFiles.lean',
        'Spec/Pre/Tcc/Cfg.lean','Spec/Pre/Tcc/ImageFS.lean',
        'scripts/ghost_cabs_cmp.py','scripts/ghost_build_request.py',
        'scripts/ghost_origins_cmp.py','scripts/physical_build_request.py','scripts/physical_onoff_cmp.py',
        'scripts/injected_origins_cmp.py','scripts/binding_origin_build_request.py',
        'scripts/unevaluated_syntax_cmp.py','scripts/unevaluated_build_request.py','scripts/unevaluated_native_check.py','Tcc/CSem/RawFrontEnd.lean',
        'lakefile.toml','lake-manifest.json','lean-toolchain','vendor/TINYCC_REV',
        'vendor/CSEM_REV']}
    paths |= set(ROOT.glob('patches/*.patch'))
    return {str(p.relative_to(ROOT)):sha(p) for p in sorted(paths)}

def build_receipt(build_id):
    reqpath=ROOT/'build'/f'worker-build.{build_id}.request'
    resultpath=ROOT/'build'/f'worker-build.{build_id}.result'
    if not resultpath.exists():resultpath=ROOT/'build/worker-build.result'
    req=json.loads(reqpath.read_text());res=json.loads(resultpath.read_text())
    assert req['request_id']==res.get('request_id',res.get('request'))==build_id
    assert res['exit']==0 and res.get('source_hashes_verified'),'build did not pass'
    assert all(sha(ROOT/p)==h for p,h in req['sha256'].items()),'build inputs changed'
    return dict(request_id=build_id,request_sha256=sha(reqpath),receipt_sha256=sha(resultpath))

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def first_diff(a,b,path='$'):
    if type(a)!=type(b):return path
    if isinstance(a,dict):
        if a.keys()!=b.keys():return path+':keys'
        return next((x for k in a if (x:=first_diff(a[k],b[k],path+'.'+k))),None)
    if isinstance(a,list):
        if len(a)!=len(b):return path+':length'
        return next((x for i,(u,v) in enumerate(zip(a,b)) if (x:=first_diff(u,v,f'{path}[{i}]'))),None)
    return None if a==b else path

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--out',type=Path,default=ROOT/'build/ghost-cabs');ap.add_argument('--case',action='append');mode_group=ap.add_mutually_exclusive_group();mode_group.add_argument('--raw',action='store_true');mode_group.add_argument('--independent-raw',action='store_true');ap.add_argument('--diagnostic',action='store_true');ap.add_argument('--min-exact',type=int);ap.add_argument('--build-id');a=ap.parse_args()
    if a.min_exact is not None and a.min_exact<1:ap.error('--min-exact must be positive; use --diagnostic for omission exploration')
    if not a.diagnostic and not a.build_id:ap.error('strict comparison requires --build-id from a passing frozen build')
    if a.case and set(a.case)-set(CASES):ap.error('unknown fixture')
    receipt=build_receipt(a.build_id) if a.build_id else None
    pins=source_pins()
    dest=a.out.resolve();dest.mkdir(parents=True,exist_ok=True)
    exe=ROOT/'.lake/build/bin/tcc_ghost';binary=sha(exe);rows=[]
    for name,(group,source) in CASES.items():
        if a.case and name not in a.case:continue
        path=dest/(name+'.c');path.write_text(source+'\n');source_hash=sha(path);objects={}
        for mode in ['model','spec']:
            try:
                selected=('raw-'+mode) if a.raw or a.independent_raw else mode
                if a.independent_raw and mode=='spec':selected='independent-raw'
                p=subprocess.run([str(exe),selected,str(path)],capture_output=True,text=True,timeout=90,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'})
                (dest/f'{name}.{mode}.stderr').write_text(p.stderr)
                (dest/f'{name}.{mode}.json').write_text(p.stdout)
                if p.returncode:raise RuntimeError(f'exit {p.returncode}')
                objects[mode]=json.loads(p.stdout)
            except Exception as e:objects[mode]={'status':'harness-error','error':str(e)}
        assert sha(path)==source_hash,'input changed during comparison'
        m,s=objects['model'],objects['spec']
        row=dict(case=name,group=group,source_sha256=sha(path),model=m['status'],spec=s['status'])
        if m['status']==s['status']=='ok':
            row['first_difference']=first_diff(m['cabs'],s['cabs'])
            row['status']='exact' if row['first_difference'] is None else 'different'
        else:row['status']='explicit-nonsuccess'
        if m.get('section_bytes_equal') is False:row['status']='erasure-byte-difference'
        if 'harness-error' in [m['status'],s['status']]:row['status']='harness-error'
        if 'gap' in m:row['gap']=m['gap']
        if 'error' in m:row['model_error']=m['error']
        if 'error' in s:row['spec_error']=s['error']
        rows.append(row);print(name,row['status'],row.get('first_difference') or row.get('gap',{}).get('reason',''),flush=True)
    assert sha(exe)==binary,'binary changed during comparison'
    required=EXPECTED_EXACT & {r['case'] for r in rows}
    minimum=a.min_exact if a.min_exact is not None else max(1,len(required))
    unexpected=[r['case'] for r in rows if r['case'] in required and r['status']!='exact']
    exact=sum(r['status']=='exact' for r in rows)
    assert source_pins()==pins,'source changed during comparison'
    accepted=not unexpected and exact>=minimum
    report=dict(acceptance=dict(mode='diagnostic' if a.diagnostic else 'strict',accepted=accepted,minimum_exact=minimum,required_exact=sorted(required),failed_controls=unexpected),source_hashes=pins,build=receipt,input_mode='independent-Raw.checked-cfgTcc-source-preprocessing-and-parsing' if a.independent_raw else ('parser-construction-on-model-positioned-tokens' if a.raw else 'shared-tokens-without-predefined-text'),binary_sha256=binary,source_revision=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip(),
                driver_sha256=sha(Path(__file__)),summary=dict(Counter(r['status'] for r in rows)),rows=rows)
    (dest/'comparison.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report['summary']))
    return int((not a.diagnostic and not accepted) or any(r['status'] in ['different','harness-error','erasure-byte-difference'] for r in rows))
if __name__=='__main__':raise SystemExit(main())
