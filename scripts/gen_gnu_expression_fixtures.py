#!/usr/bin/env python3
"""GNU compiler-action syntax controls; emitted sources are never edited."""
import argparse, json
from pathlib import Path

HEADER = '''#define GNU_PICK(c,a,b) __builtin_choose_expr(c,a,b)
#define GNU_FALLBACK(a,b) ((a) ? : (b))
typedef const int HeaderNumber;
'''

def fixtures():
    valid = {
        'conditional-adjacent':'int f(int x){return x ?: 3;}',
        'conditional-separated':'int f(int x){return x ? /* retained boundary */ : 3;}',
        'conditional-nested-right':'int f(int x,int y){return x ?: y ?: 7;}',
        'conditional-nested-left':'int f(int x,int y){return (x ?: y) ?: 7;}',
        'conditional-mixed':'int f(int x,int y){return x ? y ?: 3 : x ?: 5;}',
        'conditional-effects':'int f(void){int n=1;return (n++ ?: ++n)+n;}',
        'conditional-call':'int g(void);int f(void){return g() ?: 9;}',
        'conditional-pointer':'int *f(int *x,int *y){return x ?: y;}',
        'conditional-constant':'int a[(0 ?: 3)];int x=1 ?: 2;',
        'typeof-expression':'int x; typeof(x) y;',
        'typeof-type':'typeof(const volatile unsigned long *) p;',
        'typeof-nested':'int x;typeof(typeof(x)) y;',
        'typeof-aliases':'int x;__typeof(x) a;__typeof__(x) b;typeof(x) c;',
        'typeof-qualified':'const volatile int x=3; typeof(x) y=4; typeof(x) *p=&y;',
        'typeof-typedef':'typedef int T; typeof(T) x; typeof(T *) p;',
        'typeof-shadow':'typedef int T;int f(void){int T=3;typeof(T) y=4;return y;}',
        'typeof-shadow-nested':'typedef int T;int f(T x){{int T=3;typeof(T) a=T;}typeof(T) b=x;return b;}',
        'typeof-effects':'int f(void){int n=0;typeof(n++) x=2;return n+x;}',
        'typeof-call':'int g(void);typeof(g()) x;',
        'typeof-comma':'int x,y;typeof((x,y)) z;',
        'typeof-array':'typedef int A[3];typeof(A) a;typeof(a) b;typeof(&a) p;',
        'typeof-function':'int f(int);typeof(f) g;typeof(&f) p;',
        'typeof-abstract':'typeof(int (*)(const int *,long)) f;',
        'typeof-member':'struct S{int n;};typeof(((struct S *)0)->n) x;',
        'typeof-parameter':'int f(typeof(const int *) p,typeof(int [3]) a);',
        'typeof-cast-sizeof':'int f(int x){return sizeof(typeof(x))+((typeof(x))3);}',
        'typeof-type-context':'typedef typeof(int) N;struct S{typeof(N) n;};',
        'typeof-vla':'int f(int n){typeof(int[n++]) a;return sizeof(a)+n;}',
        'typeof-existing-vla':'int f(int n){int a[n++];typeof(a) b;return sizeof(b)+n;}',
        'choose-true':'int x=__builtin_choose_expr(1,2,3);',
        'choose-false':'int x=__builtin_choose_expr(0,2,3);',
        'choose-condition':'int x=__builtin_choose_expr(sizeof(long)==8,0X2UL,010);',
        'choose-nested-selected':'int x=__builtin_choose_expr(1,__builtin_choose_expr(0,2,3),4);',
        'choose-nested-unselected':'int x=__builtin_choose_expr(1,2,__builtin_choose_expr(0,3,4));',
        'choose-effects':'int f(void){int n=0;int x=__builtin_choose_expr(0,++n,7);return n+x;}',
        'choose-lvalue':'int f(void){int a=0,b=0;__builtin_choose_expr(1,a,b)=3;return a+b;}',
        'choose-types':'int x=__builtin_choose_expr(1,3,(void *)0);',
        'choose-string':'char *p=__builtin_choose_expr(0,"a" "b","c\\x41" "1");',
        'choose-function':'int g(void);int f(void){return __builtin_choose_expr(0,g(),3);}',
        'choose-unselected-divzero':'int x=__builtin_choose_expr(1,7,1/0);',
        'combined-typeof-choose':'typeof(__builtin_choose_expr(1,2,(long)3)) x;',
        'combined-typeof-conditional':'int x;typeof(x ?: 3) y;',
        'combined-choose-conditional':'int f(int x){return __builtin_choose_expr(1,x ?: 2,3 ?: 4);}',
        'generic-unselected':'int f(int x){return _Generic(x,int:1,default:__builtin_choose_expr(1,x ?: 2,(typeof(x))3));}',
        'generic-selected':'int f(int x){return _Generic(x,int:__builtin_choose_expr(1,x ?: 2,3),default:4);}',
        'assert-unevaluated':'_Static_assert(sizeof(typeof(__builtin_choose_expr(1,1,2)))==4,"gnu");',
        'macro-nested':'#define PICK(c,a,b) __builtin_choose_expr(c,a,b)\n#define FALLBACK(a,b) ((a) ?: (b))\n#define TYPE(x) __typeof__(x)\nint f(int n){TYPE(PICK(1,n,0)) x=FALLBACK(PICK(0,1,n),7);return x;}\n',
        'macro-origin-spelling':'#define CAT(a,b) a##b\n#define TYPE(x) CAT(__type,of__)(x)\n#line 71 "gnu-origin.c"\nTYPE(0X1UL ?: 010UL) x=__builtin_choose_expr(1,0x2UL,03UL);',
        'include':'#include "gnu-syntax.h"\ntypeof(HeaderNumber) n;int f(int x){return GNU_PICK(1,GNU_FALLBACK(x,7),3);}',
    }
    invalid = {
        'conditional-empty-else':'int f(int x){return x ?: ;}',
        'conditional-void-mismatch':'struct S{int n;};int f(int x){struct S s;return x ?: s;}',
        'typeof-empty':'typeof() x;',
        'typeof-malformed-type':'typeof(int *) ) x;',
        'typeof-undeclared':'typeof(missing) x;',
        'typeof-sideeffect-const':'const int n=1;typeof(++n) x;',
        'choose-nonconstant':'int f(int n){return __builtin_choose_expr(n,1,2);}',
        'choose-missing-operand':'int x=__builtin_choose_expr(1,2,);',
        'choose-unselected-malformed':'int x=__builtin_choose_expr(1,2,3+);',
        'choose-unselected-undeclared':'int x=__builtin_choose_expr(1,2,missing);',
        'choose-unselected-constraint':'int x=__builtin_choose_expr(1,2,*(int)3);',
        'choose-condition-comma':'int x=__builtin_choose_expr((1,0),2,3);',
    }
    compiler_constraints={'typeof-sideeffect-const','choose-condition-comma'}
    return {n:(s+'\n','valid') for n,s in valid.items()} | {
        n:(s+'\n','compiler-constraint-gap' if n in compiler_constraints else 'diagnostic') for n,s in invalid.items()}

def batch():
    return '''typedef const int Number;
int calls,effects;
int call(void){calls++;return 3;}
int main(void){
 int n=2;
 int a=n++ ?: ++effects;
 int b=0 ?: call();
 int c=__builtin_choose_expr(0,++effects,7);
 typeof(n++) d=4;
 typeof(Number) q=5;
 typeof(int (*)(void)) fp=call;
 int v=__builtin_choose_expr(1,(call() ?: 8),++effects);
 int w=_Generic(n,int:__builtin_choose_expr(1,n ?: 9,++effects),default:call());
 int arr[n++];typeof(arr) arr2;
 int m=2;typeof(int[m++]) va;
 return a!=2 || b!=3 || c!=7 || d!=4 || q!=5 || v!=3 || w!=3 ||
   n!=4 || calls!=2 || effects || sizeof(arr2)!=12 || m!=3 || sizeof(va)!=8 || fp!=call;
}
'''

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--out',type=Path,required=True);a=ap.parse_args()
    a.out.mkdir(parents=True,exist_ok=False)
    rows=[]
    for n,(s,c) in fixtures().items():
        (a.out/(n+'.c')).write_text(s);rows.append(dict(name=n,category=c))
    (a.out/'batch.c').write_text(batch());(a.out/'gnu-syntax.h').write_text(HEADER)
    (a.out/'manifest.json').write_text(json.dumps(rows,indent=2)+'\n')
    print(len(rows),'GNU syntax fixtures plus defined batch')
if __name__=='__main__':main()
