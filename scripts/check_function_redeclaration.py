#!/usr/bin/env python3
"""C11 function identity across declarations, calls, and object-byte storage.
All engines consume the same generated source; raw and historical Lean paths
are recorded separately. Invalid constraints never count as defined outcomes.
"""
import argparse
import hashlib
import json
import os
import re
import sys
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

def sha(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def cases():
    rows = {}
    check = 'int (*p)(int)=mine; int (*q)(int)=&mine; return mine(7)!=17 || p(7)!=17 || (*q)(7)!=17 || p!=q || p==0;'
    for linkage in ('static ', ''):
        prefix = 'internal' if linkage else 'external'
        p = linkage + 'int mine(int);\n'
        d = linkage + 'int mine(int x){return x+10;}\n'
        m = 'int main(void){' + check + '}\n'
        for name, source in {
            'pdrm':p+d+p+m, 'dprm':d+p+p+m, 'pprdm':p+p+p+d+m,
            'pmdr':p+m+d+p, 'dpmp':d+p+m+p, 'definition':d+m,
            'prototype-definition':p+d+m,
        }.items():
            rows[prefix+'-'+name] = (source, False)
    pre = 'static int mine(int);\nstatic int mine(int x){return x+10;}\nstatic int mine(int);\n'
    bodies = {
        'neutral-minimized': 'int (*f)(int)=mine;return f(7)!=17;',
        'direct': 'return mine(7)!=17;',
        'null-roundtrip': 'int (*p)(int)=0; if(p!=0)return 1;p=mine;if(p==0)return 2;p=0;return p!=0;',
        'local-static': 'static int (*p)(int)=mine;return p!=mine || p(2)!=12;',
        'array': 'int (*p[2])(int)={mine,mine};return p[0]!=p[1] || p[1](3)!=13;',
        'struct': 'struct pair{int (*p)(int);};struct pair a={mine};struct pair b=a;return a.p!=b.p || b.p(4)!=14;',
        'assign': 'int (*p)(int);p=mine;int (*q)(int)=p;return q!=mine || q(5)!=15;',
        'nested': 'int (*p)(int)=mine;{extern int mine(int);int (*q)(int)=mine;{extern int mine(int);if(q!=mine || p!=mine)return 1;}if(q(6)!=16)return 2;}return p(7)!=17;',
        'shadow-object': 'int (*p)(int)=mine;{int mine=29;if(mine!=29)return 1;}return p!=mine || p(7)!=17;',
        'bytes': 'int (*p)(int)=mine;int (*q)(int)=0;unsigned char *a=(unsigned char *)&p;unsigned char *b=(unsigned char *)&q;for(unsigned long i=0;i<sizeof p;i++)b[i]=a[i];return q!=mine || q(8)!=18;',
    }
    for name, body in bodies.items():
        rows[name] = (pre+'int main(void){'+body+'}\n', False)
    rows['global'] = (pre+'int (*saved)(int)=mine;int main(void){int (*q)(int)=saved;return q!=mine || q(3)!=13;}\n', False)
    rows['different'] = (pre+'static int other(int);static int other(int x){return x+20;}static int other(int);int main(void){int (*p)(int)=mine;int (*q)(int)=other;return p==q || p(1)!=11 || q(1)!=21 || p!=mine || q!=other;}\n',False)
    rows['zero-arg'] = ('static int mine(void);static int mine(void){return 17;}static int mine(void);int main(void){int (*p)(void)=mine;return p==0 || p()!=17 || p!=mine;}\n',False)
    rows['proxy-name'] = (pre.replace('mine','printf')+'int main(void){int (*p)(int)=printf;return p(7)!=17 || p!=printf;}\n',False)
    rows['invalid-parameter'] = ('int mine(int);int mine(double);int main(void){return 0;}\n',True)
    rows['invalid-return'] = ('int mine(int);void mine(int);int main(void){return 0;}\n',True)
    rows['invalid-redefinition'] = ('int mine(int x){return x;}int mine(int x){return x+1;}int main(void){return 0;}\n',True)
    rows['invalid-object'] = ('int mine(int);int mine;int main(void){return 0;}\n',True)
    return rows

def invoke(cmd, stem, layout=None):
    cmd = list(map(str,cmd))
    p = subprocess.run(cmd, cwd=ROOT, env=dict(os.environ,LEAN_ABORT_ON_PANIC='1',**({'SYT_RAW_LAYOUT':str(layout)} if layout else {})),capture_output=True,timeout=120)
    stem.with_suffix('.command.json').write_text(json.dumps(cmd,indent=2)+'\n')
    stem.with_suffix('.stdout').write_bytes(p.stdout)
    stem.with_suffix('.stderr').write_bytes(p.stderr)
    return p

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--out',type=Path,required=True)
    ap.add_argument('--oracle',type=Path,default=os.environ.get('CSEM_ORACLE'))
    ap.add_argument('--runtime',type=Path,default=os.environ.get('CSEM_RUNTIME'))
    ap.add_argument('--baseline',action='store_true')
    a=ap.parse_args()
    assert a.oracle and a.runtime, 'source scripts/csem_env.sh and export paths'
    out=a.out.resolve();out.mkdir(parents=True,exist_ok=False)
    srcdir=out/'sources';srcdir.mkdir()
    pins={str(p):sha(p) for p in (a.oracle,ROOT/'.lake/build/bin/csem_raw',ROOT/'.lake/build/bin/csem_run')}
    rows=[]
    for name,(source,invalid) in cases().items():
        src=srcdir/(name+'.c');src.write_text(source);pins[str(src)]=sha(src)
        row=dict(case=name,invalid_constraint=invalid)
        layout=out/(name+'.pp')
        raw=invoke([ROOT/'.lake/build/bin/csem_raw','--checked','--semantic','--layout',layout,src],out/(name+'-raw'))
        # Independent Menhir/OCaml parses the same positioned input; ignore
        # automatic builtins, which otherwise consume ids and hide seed zero.
        cpp=f'{sys.executable} {ROOT / "scripts/raw_frontend_cat.py"}'
        cmd=[a.oracle,'--runtime='+str(a.runtime),'--nolibc','--cpp='+cpp,'--exec','--batch','--mode=exhaustive',src]
        oc=invoke(cmd,out/(name+'-ocaml'),layout)
        gcc=invoke(['gcc','-std=c11','-pedantic-errors','-fno-builtin','-O0',src,'-o',out/(name+'.exe')],out/(name+'-gcc-compile'))
        export=invoke([a.oracle,'--runtime='+str(a.runtime),'--nolibc','--cpp='+cpp,'--cabs-json',src],out/(name+'-cabs'),layout)
        historical=None
        if export.returncode==0:
            jp=out/(name+'.json');jp.write_bytes(export.stdout)
            historical=invoke([ROOT/'.lake/build/bin/csem_run','--batch','--runtime',a.runtime,jp],out/(name+'-lean'))
        envelope=json.loads(raw.stdout) if raw.returncode==0 else None
        sem=envelope.get('semantics') if envelope else None
        same_cabs=False
        if export.returncode==0 and envelope:
            from raw_frontend_cmp import historical_positions
            expected=json.loads(export.stdout);digest=expected.pop('digest')
            same_cabs=expected==historical_positions(envelope['parse']['cabs']) and digest==hashlib.md5(src.read_bytes()).hexdigest()
        row['same_cabs']=same_cabs
        row.update(ocaml_exit=oc.returncode,gcc_compile_exit=gcc.returncode,raw_exit=raw.returncode,raw_semantics=sem,historical_exit=historical.returncode if historical else None)
        if invalid:
            # Export only parses; both semantic paths must reject constraints.
            row['pass']=same_cabs and oc.returncode!=0 and gcc.returncode!=0 and (raw.returncode!=0 or (isinstance(sem,dict) and sem.get('status')=='elaboration-error') or (isinstance(sem,list) and sem and all(x.get('status')=='elaboration-failure' for x in sem))) and historical is not None and historical.returncode!=0
        else:
            run=invoke([out/(name+'.exe')],out/(name+'-gcc-run')) if gcc.returncode==0 else None
            row['gcc_exit']=run.returncode if run else None
            good_raw=isinstance(sem,list) and bool(sem) and all(x.get('status')=='defined' and x.get('exit')==0 and x.get('stdout')=='' for x in sem)
            good_lean=historical is not None and historical.returncode==0 and 'Defined {value: "Specified(0)", stdout: "", stderr: "", blocked: "false"}' in historical.stdout.decode()
            good_oc=oc.returncode==0 and bool(re.search(rb'Defined.*(?:Specified\(0\)|exit.*0)',oc.stdout))
            row['pass']=same_cabs and good_raw and good_lean and good_oc and run is not None and run.returncode==0
        if name=='invalid-object':
            # Independently inherited by pinned OCaml: both accept an invalid
            # object/function conflict. Keep this outside the success count.
            row['status']='INHERITED_CONSTRAINT_GAP'
            row['expected_gap']=same_cabs and oc.returncode==0 and gcc.returncode!=0 and historical is not None and historical.returncode==0 and isinstance(sem,list) and all(x.get('status')=='defined' for x in sem)
        elif name=='proxy-name' and not row['pass']:
            # A pristine historical oracle may still proxy by spelling. The
            # current shared overlay must pass; only exact old failures are
            # retained separately when explicitly running that engine.
            row['status']='HISTORICAL_PROXY_GAP'
            row['expected_gap']=same_cabs and good_raw and gcc.returncode==0 and run.returncode==0 and oc.returncode!=0 and historical is not None and historical.returncode!=0 and b'printf_proxy' in oc.stdout+oc.stderr and b'printf_proxy' in historical.stdout+historical.stderr
        else:
            row['status']='PASS' if row['pass'] else 'DIFFERENCE'
        rows.append(row)
        print(name,row['status'],flush=True)
    assert all(sha(p)==h for p,h in pins.items()), 'inputs or executables changed during comparison'
    (out/'results.json').write_text(json.dumps(dict(pins=pins,rows=rows),indent=2)+'\n')
    return 0 if a.baseline else int(any(not (r['pass'] or r.get('expected_gap',False)) for r in rows))

if __name__=='__main__':raise SystemExit(main())
