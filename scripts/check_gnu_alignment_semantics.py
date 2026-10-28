#!/usr/bin/env python3
"""Unstripped GNU alignment and C11 array constraints, with separate verdicts.

The historical two function-query errors remain expected policy obstructions.
Menhir placement limitations never count as semantic agreements.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
from raw_frontend_cmp import historical_positions

ROOT = Path(__file__).resolve().parents[1]
FIXTURES = ROOT / 'c/tests/csem-alignment'
FUNCTION_POLICY = {'functions', 'function-typeof'}
PLACEMENT_LIMIT = {'prefix-attribute', 'attribute-position-0', 'attribute-position-1'}
INVALID_ARRAY = {'array-'+n for n in ('function-array', 'function-array-local',
    'function-array-typedef', 'void-array', 'void-array-local',
    'void-array-typedef', 'overaligned-array')}
EXPRS = {
    'unary-plus': ('+x',16), 'unary-minus': ('-x',16), 'unary-not': ('~x',16),
    'logical-not': ('!x',4), 'add-alias': ('x+x',16), 'add-int': ('x+1',4),
    'shift-left': ('x<<1',16), 'shift-right': ('x>>1',16),
    'band-alias': ('x&x',16), 'band-int': ('x&1',4),
    'cond-alias': ('x?x:x',16), 'cond-int': ('x?x:1',4),
    'comma': ('(x,x)',16), 'assign': ('x=1',16), 'compound': ('x+=1',16),
    'preinc': ('++x',16), 'postinc': ('x++',16),
    'cast-alias': ('(Up)x',4), 'cast-int': ('(int)x',4),
    'address': ('&x',8), 'indirect': ('*(&x)',16), 'return': ('f()',16),
    'statement': ('({x;})',16), 'generic': ('_Generic(x,int:x)',16),
}

def sha(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def fixtures():
    rows = {p.stem: (p.read_bytes(), p.stem in INVALID_ARRAY) for p in sorted(FIXTURES.glob('*.c'))}
    for n,(e,align) in EXPRS.items():
        rows['transport-'+n] = (('typedef int Up __attribute__((aligned(16))); Up x; '
            'Up f(void){return 0;}\nint main(void){return _Alignof(__typeof__('+e+'))!='+str(align)+';}\n').encode(),False)
    for n,decl in {
        'pointer-to-function-array':'typedef int F(void); typedef F (*P)[1];',
        'pointer-to-void-array':'typedef void V; typedef V (*P)[1];',
        'parameter-function-array':'typedef int F(void); int f(F a[1]);',
        'typename-void-array':'typedef void V; int x[sizeof(V[1])];',
        'nested-incomplete-array':'typedef int A[][2]; typedef A B[2];',
    }.items():
        rows['constraint-'+n] = ((decl+' int main(void){return 0;}\n').encode(), True)
    rows['array-alias-decay'] = (b'typedef int A[2] __attribute__((aligned(32))); A a; int main(void){return _Alignof(__typeof__(a))!=32 || _Alignof(__typeof__(a+0))!=8 || __alignof__(a)!=32;}\n',False)
    rows['member-return'] = (b'typedef int Up __attribute__((aligned(16))); struct S{Up x;}; struct S f(void){struct S s={0};return s;} int main(void){return _Alignof(__typeof__(f().x))!=16;}\n',False)
    # Unevaluated operands and enclosing object alignment are independent.
    rows['unevaluated'] = (b'int main(void){int x=0; return __alignof__(x++)!=4 || x!=0;}\n',False)
    return rows


def invoke(cmd, stem, layout=None):
    cmd = list(map(str,cmd))
    env = dict(os.environ, LEAN_ABORT_ON_PANIC='1')
    if layout: env['SYT_RAW_LAYOUT'] = str(layout)
    p = subprocess.run(cmd,cwd=ROOT,env=env,capture_output=True,timeout=120)
    stem.with_suffix('.command.json').write_text(json.dumps(cmd,indent=2)+'\n')
    stem.with_suffix('.stdout').write_bytes(p.stdout)
    stem.with_suffix('.stderr').write_bytes(p.stderr)
    return p


def defined_zero(sem):
    return isinstance(sem,list) and bool(sem) and all(x.get('status')=='defined' and x.get('exit')==0 and x.get('stdout')=='' for x in sem)


def diagnostic(sem):
    return (isinstance(sem,dict) and sem.get('status')=='elaboration-error' or
        isinstance(sem,list) and bool(sem) and all(x.get('status')=='elaboration-failure' for x in sem))


def good_ocaml_zero(oc):
    return oc.returncode==0 and bool(re.search(rb'Defined[^\n]*Specified\(0\)',oc.stdout))

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--out',type=Path,required=True)
    ap.add_argument('--oracle',type=Path)
    ap.add_argument('--runtime',type=Path)
    ap.add_argument('--tcc',type=Path)
    a=ap.parse_args()
    if not a.oracle:
        fork=Path(subprocess.check_output(['scripts/csem_fork_oracle.sh','--path'],cwd=ROOT,text=True).strip())
        a.oracle=fork/'_build/default/backend/driver/main.exe';a.runtime=fork/'_build/install/default'
    assert a.runtime
    out=a.out.resolve();out.mkdir(parents=True,exist_ok=False)
    raw=ROOT/'.lake/build/bin/csem_raw';text=ROOT/'.lake/build/bin/csem_cparse'
    pins={str(p):sha(p) for p in [raw,text,a.oracle,ROOT/'.lake/build/bin/csem_run',*FIXTURES.glob('*.c')]}
    if a.tcc: a.tcc=a.tcc.resolve();pins[str(a.tcc)]=sha(a.tcc)
    rows=[]
    for name,(source,invalid) in fixtures().items():
        d=out/name;d.mkdir();src=d/'input.c';src.write_bytes(source);pins[str(src)]=sha(src)
        layout=d/'layout.pp'
        p=invoke([raw,'--gnu','--checked','--semantic','--layout',layout,src],d/'raw')
        obj=json.loads(p.stdout) if p.returncode==0 else None
        sem=obj['semantics'] if obj else None
        row=dict(case=name,invalid=invalid,raw_exit=p.returncode,semantics=sem)
        if obj:
            view=d/'text.pp';data=layout.read_bytes();view.write_bytes(data[:-1] if data.endswith(b'\n') else data)
            t=invoke([text,'--gnu','--json',view,src],d/'text')
            row['full_text_cabs']=t.returncode==0 and json.loads(t.stdout)==historical_positions(obj['parse']['cabs'])
        cpp=f'{sys.executable} {ROOT / "scripts/raw_frontend_cat.py"}'
        base=[a.oracle,'--runtime='+str(a.runtime),'--nolibc','--cpp='+cpp]
        ex=invoke([*base,'--cabs-json',src],d/'ocaml-cabs',layout)
        oc=invoke([*base,'--exec','--batch','--mode=exhaustive',src],d/'ocaml',layout)
        row.update(ocaml_parse_exit=ex.returncode,ocaml_exit=oc.returncode)
        row['ocaml_constraint_diagnostic']=oc.returncode>0 and b'constraint violation' in oc.stderr.lower() and b'Fatal error' not in oc.stderr
        row['ocaml_verdicts']=re.findall(r'Defined[^\n]*',oc.stdout.decode(errors='replace'))
        same=False
        if ex.returncode==0 and obj:
            expected=json.loads(ex.stdout);digest=expected.pop('digest')
            same=expected==historical_positions(obj['parse']['cabs']) and digest==hashlib.md5(source).hexdigest()
        row['full_ocaml_cabs']=same
        gcc=invoke(['gcc','-std=c11','-O0',src,'-o',d/'gcc.exe'],d/'gcc-compile')
        q=invoke([d/'gcc.exe'],d/'gcc-run') if gcc.returncode==0 else None
        row['gcc_compile_exit']=gcc.returncode;row['gcc_exit']=q.returncode if q else None
        native_ok=(gcc.returncode!=0 if invalid else q is not None and q.returncode==0 and q.stdout==b'')
        if a.tcc:
            tc=invoke([a.tcc,'-std=c11','-B'+str(a.tcc.parent),'-I'+str(ROOT/'build/test-c11/src/include'),src,'-o',d/'tcc.exe'],d/'tcc-compile')
            tq=invoke([d/'tcc.exe'],d/'tcc-run') if tc.returncode==0 else None
            row['tcc_compile_exit']=tc.returncode;row['tcc_exit']=tq.returncode if tq else None
            native_ok &= tc.returncode!=0 if invalid else tq is not None and tq.returncode==0 and tq.stdout==b''
        if invalid:
            row['status']='CONSTRAINT_DIAGNOSTIC' if diagnostic(sem) and same and row['ocaml_constraint_diagnostic'] and native_ok else 'FAIL'
        elif name in FUNCTION_POLICY:
            row['status']='FUNCTION_TYPE_POLICY_OBSTRUCTION' if diagnostic(sem) and same and row['ocaml_constraint_diagnostic'] and native_ok else 'FAIL'
        elif name=='array-alias-decay' and a.tcc:
            # Independent extra probe: preserve the compiler's 32-vs-8
            # decay discrepancy separately from repaired CSem semantics.
            row['status']='COMPILER_ARRAY_DECAY_DISCREPANCY' if defined_zero(sem) and same and good_ocaml_zero(oc) and row['gcc_exit']==0 and row['tcc_exit']==1 else 'FAIL'
        elif name in PLACEMENT_LIMIT:
            row['status']='OCAML_PLACEMENT_LIMITATION' if defined_zero(sem) and ex.returncode>0 and native_ok else 'FAIL'
        else:
            good_oc=oc.returncode==0 and 'Specified(0)' in oc.stdout.decode() and bool(row['ocaml_verdicts'])
            row['status']='DEFINED_ZERO' if defined_zero(sem) and same and good_oc and native_ok else 'FAIL'
        if not row.get('full_text_cabs'):row['status']='FAIL_TEXT_CABS'
        rows.append(row);print(name,row['status'],flush=True)
    assert all(sha(p)==h for p,h in pins.items()),'sources or executable changed during checks'
    counts={s:sum(r['status']==s for r in rows) for s in sorted({r['status'] for r in rows})}
    (out/'report.json').write_text(json.dumps(dict(rows=rows,counts=counts,pins=pins),indent=2)+'\n')
    print(counts)
    return int(any(r['status'].startswith('FAIL') for r in rows))
if __name__=='__main__':raise SystemExit(main())
