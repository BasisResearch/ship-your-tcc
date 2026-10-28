#!/usr/bin/env python3
"""Array alias decay regressions; independent expectations, retained attributes.

Outputs go under build/. Invalid array constraints never add success coverage.
The first reproducer is byte-identical to the original alignment suite.
"""
import argparse
import json
from pathlib import Path

REPRO = b'typedef int A[2] __attribute__((aligned(32))); A a; int main(void){return _Alignof(__typeof__(a))!=32 || _Alignof(__typeof__(a+0))!=8 || __alignof__(a)!=32;}\n'
BASE = 'typedef int A[2] __attribute__((aligned(32))); typedef A B;\n'

def fixtures():
    rows = {'array-alias-decay': (REPRO, False)}
    checks = {
        'arithmetic': '_Alignof(__typeof__(a+0))!=8 || _Alignof(__typeof__(0+a))!=8 || _Alignof(__typeof__(a-0))!=8 || (a+1)-a!=1',
        'queries': '_Alignof(__typeof__(a))!=32 || __alignof__(a)!=32 || sizeof(a)!=8 || sizeof(a+0)!=8 || sizeof(*a)!=4',
        'address': '_Alignof(__typeof__(&a))!=8 || _Alignof(__typeof__(*(&a)))!=32 || _Alignof(__typeof__(*((&a)+0)))!=32 || sizeof(*(&a))!=8',
        'conditional': '_Alignof(__typeof__(1?a:a))!=8 || _Alignof(__typeof__(0?a:0))!=8 || _Alignof(__typeof__(1?0:a))!=8 || _Alignof(__typeof__(1?a:(int*)0))!=8',
        'conditional-address': '_Alignof(__typeof__(*(1?&a:&a)))!=32 || _Alignof(__typeof__(1?&a:&a))!=8',
        'qualifiers': '!_Generic(a+0,const int*:1,default:0) || _Alignof(__typeof__(a+0))!=8 || _Alignof(__typeof__(a))!=32 || __alignof__(a)!=32',
        'volatile': '!_Generic(a+0,volatile int*:1,default:0) || _Alignof(__typeof__(a+0))!=8 || _Alignof(__typeof__(a))!=32',
        'indirect': '_Alignof(__typeof__(*(&a)))!=32 || _Alignof(__typeof__(*(&a)+0))!=8 || _Alignof(__typeof__(*(a+0)))!=4',
        'comma': '_Alignof(__typeof__((0,a)))!=8 || _Alignof(__typeof__((0,a+0)))!=8',
        'nested': '_Alignof(__typeof__(**q))!=32 || _Alignof(__typeof__(**q+0))!=8 || _Alignof(__typeof__(*q))!=8',
        'initialize': '_Alignof(__typeof__(p))!=8 || p!=a || p[1]!=7 || _Alignof(__typeof__(a))!=32',
    }
    for storage in ['auto','static']:
        for name,test in checks.items():
            qual = 'const ' if name=='qualifiers' else 'volatile ' if name=='volatile' else ''
            decl = ('static ' if storage=='static' else '')+qual+'B a={3,7};'
            extra = ' A *p=&a; A **q=&p;' if name=='nested' else ' int *p=a;' if name=='initialize' else ''
            rows[f'{name}-{storage}'] = ((BASE+'int main(void){'+decl+extra+'return '+test+';}\n').encode(),False)
    rows['parameters'] = ((BASE+'int f(A a){return _Alignof(__typeof__(a))!=8 || _Alignof(__typeof__(a+0))!=8 || a[1]!=7;} int g(B a){return f(a);} int main(void){A a={3,7};return g(a);}\n').encode(),False)
    rows['parameter-qualifiers'] = ((BASE+'int f(const B a){return !_Generic(a,const int*:1,default:0) || _Alignof(__typeof__(a))!=8 || a[1]!=7;} int main(void){const B a={3,7};return f(a);}\n').encode(),False)
    rows['calls-spill'] = ((BASE+'int f(void){return 1;} int *id(int *p){return p;} int get(int *p,int x,int *q){return p[x]+q[0];} int variadic(int x,...){return x;} int main(void){A a={3,7};int *p=a+f(); return p[0]!=7 || get(a,f(),id(a))!=10 || variadic(a[0],a)!=3 || _Alignof(__typeof__(a+f()))!=8 || _Alignof(__typeof__(id(a)))!=8 || _Alignof(__typeof__(a))!=32;}\n').encode(),False)
    rows['element-referent'] = (b'typedef struct {long x[4];} E __attribute__((aligned(32))); E a[2]; int main(void){return _Alignof(__typeof__(a+0))!=8 || _Alignof(__typeof__(*(a+0)))!=32 || sizeof(*(a+0))!=32 || _Alignof(__typeof__(*(&a)))!=32;}\n',False)
    rows['vla'] = (b'int f(int n){typedef int A[n] __attribute__((aligned(32))); A a;return _Alignof(__typeof__(a+0))!=8 || _Alignof(__typeof__(a))!=32 || sizeof(a)!=n*4;} int main(void){return f(3);}\n',False)
    # Preserve pointer-alias metadata when no array conversion takes place.
    rows['pointer-alias'] = (b'typedef int *P __attribute__((aligned(16))); P p; int main(void){return _Alignof(__typeof__(p+0))!=16 || _Alignof(__typeof__(*(&p)))!=16 || _Alignof(__typeof__(&p))!=8;}\n',False)
    for name,decl in {
        'invalid-element': 'typedef int E __attribute__((aligned(32))); E a[2];',
        'invalid-array-alias': BASE+'A a[2];',
        'invalid-pointer-array': BASE+'A (*p)[2];',
        'invalid-parameter-array': BASE+'int f(A a[2]);',
        'invalid-typename': BASE+'int x[sizeof(A[2])];',
    }.items():
        rows[name] = ((decl+' int main(void){return 0;}\n').encode(),True)
    return rows

def generate(out):
    out=Path(out);out.mkdir(parents=True,exist_ok=True)
    cases={};invalid=[]
    for name,(source,bad) in fixtures().items():
        p=out/(name+'.c');p.write_bytes(source)
        if bad:invalid.append(str(p))
        else:cases[str(p)]=''
    (out/'manifest.json').write_text(json.dumps(dict(positive=list(cases),invalid=invalid),indent=2)+'\n')
    return cases,invalid

if __name__=='__main__':
    ap=argparse.ArgumentParser();ap.add_argument('out',type=Path);a=ap.parse_args();p,n=generate(a.out);print(f'{len(p)} positive, {len(n)} invalid')
