#!/usr/bin/env python3
"""GNU typedef alignment queries, declarator boundaries and constraints.

Generated fixtures belong in build/. Explicit expectations are independent
of compiler/model results; negative diagnostics never count as successes.
"""
from pathlib import Path
import argparse
import json

BASE = '''typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
'''

def generate(out):
    out = Path(out)
    out.mkdir(parents=True, exist_ok=True)
    cases = {
        'queries': (BASE, '_Alignof(Up)!=16 || __alignof__(Down)!=1 || __alignof(Again)!=16 || sizeof(Again)!=4'),
        'chain': (BASE + 'typedef Again Third; typedef Third Fourth;\n', '_Alignof(Fourth)!=16 || sizeof(Fourth)!=4'),
        'weaker': (BASE + 'typedef Again Weak __attribute__((aligned(2))); typedef Weak W;\n', '_Alignof(W)!=2 || sizeof(W)!=4 || _Alignof(Up)!=16'),
        'stronger': (BASE + 'typedef Down Strong __attribute__((aligned(32))); typedef Strong S;\n', '_Alignof(S)!=32 || sizeof(S)!=4 || _Alignof(Down)!=1'),
        'prefix-attribute': (BASE + 'typedef __attribute__((aligned(2))) Up Weak; typedef __attribute__((__aligned__(32))) Down Strong;\n', '_Alignof(Weak)!=2 || _Alignof(Strong)!=32'),
        'qualified': (BASE + 'typedef const Up C; typedef volatile Down V;\n', '_Alignof(C)!=16 || _Alignof(V)!=1 || _Alignof(const Again)!=16 || _Alignof(volatile Down)!=1 || sizeof(C)!=4'),
        'pointers': (BASE + 'typedef Up *P; typedef Down *Q; typedef P R;\n', '_Alignof(Up*)!=sizeof(void*) || _Alignof(P)!=sizeof(void*) || _Alignof(R)!=sizeof(void*) || _Alignof(Q)!=sizeof(void*) || _Alignof(Up**)!=sizeof(void*) || sizeof(P)!=sizeof(void*)'),
        'aligned-pointer': (BASE + 'typedef Up *P __attribute__((aligned(32))); typedef P *PP;\n', '_Alignof(P)!=32 || _Alignof(PP)!=sizeof(void*) || sizeof(P)!=sizeof(void*)'),
        'pointer-qualifiers': (BASE + 'typedef Up *const CP; typedef Down *volatile VP; typedef Up *restrict RP;\n', '_Alignof(CP)!=sizeof(void*) || _Alignof(VP)!=sizeof(void*) || _Alignof(RP)!=sizeof(void*)'),
        'arrays': (BASE + 'typedef Down A[3]; typedef A B[2]; typedef const A C;\n', '_Alignof(A)!=1 || _Alignof(B)!=1 || _Alignof(C)!=1 || sizeof(A)!=12 || sizeof(B)!=24 || _Alignof(Down[3])!=1'),
        'array-pointer': (BASE + 'typedef Down A[3]; typedef A *P; typedef Up *V[2];\n', '_Alignof(P)!=sizeof(void*) || _Alignof(V)!=sizeof(void*) || _Alignof(Down(*)[3])!=sizeof(void*) || sizeof(V)!=2*sizeof(void*)'),
        'aggregate-array': ('typedef struct { long a,b; } Wide __attribute__((aligned(16))); typedef Wide A[2];\n', '_Alignof(A)!=16 || sizeof(A)!=32 || _Alignof(Wide)!=16'),
        'functions': (BASE + 'typedef Up F(void); typedef F *FP; Up fun(void){return 7;}\n', '_Alignof(F)!=_Alignof(int(void)) || _Alignof(FP)!=sizeof(void*) || sizeof(FP)!=sizeof(void*) || _Alignof(__typeof__(fun()))!=16 || fun()!=7'),
        'nested-declarator': (BASE + 'typedef Down (*P)[3]; typedef Up (*F)(Down); Up fun(Down x){return x+1;}\n', '_Alignof(P)!=sizeof(void*) || _Alignof(F)!=sizeof(void*) || sizeof(P)!=sizeof(void*) || fun(4)!=5'),
        'typeof': (BASE + 'typedef __typeof__(Up) TU; typedef __typeof__(Down) TD; typedef __typeof__(Again) TA;\n', '_Alignof(TU)!=16 || _Alignof(TD)!=1 || _Alignof(TA)!=16 || _Alignof(__typeof__(Up*))!=sizeof(void*)'),
        'deref': (BASE + 'typedef Up *P; Up fun(void){return 0;}\n', '_Alignof(__typeof__(*(Up*)0))!=16 || _Alignof(__typeof__(*(P)0))!=16 || _Alignof(__typeof__(fun()))!=16'),
        'layout': (BASE + 'struct U {char c; Up x;}; struct D {char c; Down x;};\n', 'offsetof(struct U,x)!=16 || sizeof(struct U)!=32 || _Alignof(struct U)!=16 || offsetof(struct D,x)!=1 || sizeof(struct D)!=5 || _Alignof(struct D)!=1 || sizeof(Up)!=4 || sizeof(Down)!=4'),
        'pointer-layout': (BASE + 'struct P {char c; Up *p;}; struct Q {char c; Down *p;};\n', 'offsetof(struct P,p)!=sizeof(void*) || offsetof(struct Q,p)!=sizeof(void*) || sizeof(struct P)!=2*sizeof(void*)'),
        'alignas': (BASE + 'struct A {char c; _Alignas(Up) int x;}; struct B {char c; _Alignas(Up*) int *p;};\n', 'offsetof(struct A,x)!=16 || _Alignof(struct A)!=16 || offsetof(struct B,p)!=sizeof(void*)'),
        'unary-types': (BASE + 'typedef short H __attribute__((aligned(8))); Up x; H h;\n', '_Alignof(__typeof__(+x))!=16 || _Alignof(__typeof__(-x))!=16 || _Alignof(__typeof__(~x))!=16 || _Alignof(__typeof__(!x))!=4 || _Alignof(__typeof__(+h))!=4'),
        'common-types': (BASE + 'Up x;\n', '_Alignof(__typeof__(x+x))!=16 || _Alignof(__typeof__(x?x:x))!=16 || _Alignof(__typeof__(x+1))!=4 || _Alignof(__typeof__(x==x))!=4'),
        'address-type': (BASE + 'Up x;\n', '_Alignof(__typeof__(&x))!=sizeof(void*) || _Alignof(__typeof__((Up)1))!=4'),
        'object-vs-type': ('int obj __attribute__((aligned(32))); typedef int Alias __attribute__((aligned(16))); Alias a;\n', '_Alignof(__typeof__(obj))!=4 || _Alignof(__typeof__(a))!=16 || _Alignof(__typeof__(&obj))!=sizeof(void*)'),
        'expression-address': (BASE + 'Up x; typedef int *P __attribute__((aligned(32))); P p;\n', '__alignof__(&x)!=sizeof(void*) || __alignof__(&p)!=sizeof(void*) || __alignof__((int)x)!=4 || _Alignof(__typeof__(&x))!=sizeof(void*)'),
        'expression-local': (BASE, '({int obj __attribute__((aligned(32))); __alignof__(obj)!=32 || __alignof__(&obj)!=sizeof(void*) || __alignof__((int)obj)!=4 || _Alignof(__typeof__(obj))!=4;})'),
        'expression-global': ('int obj __attribute__((aligned(32)));\n', '__alignof__(obj)!=32 || __alignof__(&obj)!=sizeof(void*) || __alignof__(+obj)!=4 || __alignof__((obj,obj))!=4 || _Alignof(__typeof__(obj))!=4'),
        'expression-array': ('int arr[3] __attribute__((aligned(32)));\n', '__alignof__(arr)!=32 || __alignof__(&arr)!=sizeof(void*) || sizeof(arr)!=12 || _Alignof(__typeof__(arr))!=4'),
        'expression-parent': ('struct S {char c; int i;}; struct S obj __attribute__((aligned(32)));\n', '__alignof__(obj)!=32 || __alignof__(obj.c)!=1 || __alignof__(obj.i)!=4 || __alignof__(&obj.c)!=sizeof(void*)'),
        'function-typeof': (BASE + 'typedef Up F(void); typedef __typeof__(F) TF; typedef F *FP;\n', '_Alignof(F)!=_Alignof(int(void)) || _Alignof(TF)!=_Alignof(F) || _Alignof(__typeof__(F))!=_Alignof(F) || _Alignof(FP)!=sizeof(void*)'),
        'c11-control': ('typedef int Number; typedef const Number Const; struct S {char c; _Alignas(16) Number x;};\n', '_Alignof(Number)!=4 || _Alignof(Const)!=4 || sizeof(Number)!=4 || offsetof(struct S,x)!=16 || sizeof(struct S)!=32'),
    }
    positives = {}
    for name, (decl, condition) in cases.items():
        p = out / (name + '.c')
        p.write_text('#include <stddef.h>\n#include <stdio.h>\n' + decl + f'int main(void){{int bad=({condition}); printf("{name}=%d\\n",bad);return bad;}}\n')
        positives[str(p)] = name + '=0\n'
    extra = {
        'scopes': BASE + '''int main(void){int bad=0;
 {typedef Down Up; typedef Up Local; bad|=_Alignof(Up)!=1 || _Alignof(Local)!=1;}
 bad|=_Alignof(Up)!=16;
 {typedef Up Down; bad|=_Alignof(Down)!=16;}
 {int Up=3; bad|=_Alignof(__typeof__(Up))!=4 || Up!=3;}
 bad|=_Alignof(Down)!=1; printf("scopes=%d\\n",bad);return bad;}
''',
        'live-stack': BASE + '''Up global;
int main(void){int bad=__alignof__(global)!=16;
 bad|=_Alignof(int)!=4 || _Alignof(Down)!=1 || _Alignof(Again)!=16;
 bad|=_Alignof(int*)!=sizeof(void*) || sizeof(Up)!=4;
 printf("live-stack=%d\\n",bad);return bad;}
''',
    }
    for name, src in extra.items():
        p=out/(name+'.c');p.write_text('#include <stddef.h>\n#include <stdio.h>\n'+src)
        positives[str(p)]=name+'=0\n'
    negatives = {
        'invalid-three': 'typedef int Bad __attribute__((aligned(3)));',
        'invalid-negative': 'typedef int Bad __attribute__((aligned(-2)));',
        'invalid-alias-three': BASE + 'typedef Up Bad __attribute__((aligned(3)));',
        'invalid-scope': 'int f(void){typedef int Local; return sizeof(Local);} int n = _Alignof(Local);',
        'invalid-function-array': BASE + 'typedef Up F(void); typedef F Bad[2];',
        'invalid-overaligned-array': BASE + 'typedef Up Bad[2];',
        'invalid-void-array': 'typedef void V; typedef V Bad[2];',
    }
    invalid=[]
    for name, src in negatives.items():
        p=out/(name+'.c');p.write_text(src+'\nint main(void){return 0;}\n');invalid.append(str(p))
    (out/'manifest.json').write_text(json.dumps(dict(positives=positives,invalid=invalid),indent=2)+'\n')
    return positives, invalid

if __name__ == '__main__':
    ap=argparse.ArgumentParser();ap.add_argument('out',type=Path);a=ap.parse_args()
    pos,neg=generate(a.out);print(f'{len(pos)} positives, {len(neg)} constraints in {a.out}')
