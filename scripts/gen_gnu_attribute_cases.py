#!/usr/bin/env python3
"""Independent complete attribute probes. Sources and batch are generated."""
import argparse,json,re
from pathlib import Path
POSITIVE={
 'injected-aligned':'struct __uint128__ {char x[16];} __attribute((__aligned__(16))); struct Outer {char pre;struct __uint128__ v;}; int main(void){return sizeof(struct __uint128__)!=16 || _Alignof(struct __uint128__)!=16 || offsetof(struct Outer,v)!=16 || sizeof(struct Outer)!=32;}',
 'tag-prefix':'struct __attribute__((aligned(16))) S {char x;}; int main(void){return sizeof(struct S)!=16 || _Alignof(struct S)!=16;}',
 'tag-suffix':'struct S {char x;} __attribute__((aligned(16))); int main(void){return sizeof(struct S)!=16 || _Alignof(struct S)!=16;}',
 'packed':'struct __attribute__((packed)) S {char c;int i;}; int main(void){struct S s={1,37};return sizeof s!=5 || _Alignof(struct S)!=1 || offsetof(struct S,i)!=1 || s.i!=37;}',
 'packed-aligned':'struct S {char c;int i;} __attribute__((__packed__,__aligned__(8))); int main(void){struct S s={1,37};return sizeof s!=8 || _Alignof(struct S)!=8 || offsetof(struct S,i)!=1 || s.i!=37;}',
 'member-aligned':'struct S {char c;int i __attribute__((aligned(16)));char z;}; int main(void){struct S s={1,37,3};return sizeof s!=32 || _Alignof(struct S)!=16 || offsetof(struct S,i)!=16 || offsetof(struct S,z)!=20 || s.i!=37;}',
 'member-prefix':'struct S {char c;__attribute__((aligned(8))) int i;}; int main(void){return sizeof(struct S)!=16 || offsetof(struct S,i)!=8;}',
 'member-packed':'struct S {char c;int i __attribute__((packed));}; int main(void){struct S s={1,37};return sizeof s!=5 || _Alignof(struct S)!=1 || offsetof(struct S,i)!=1 || s.i!=37;}',
 'nested':'struct __attribute__((packed)) Inner {char c;int i;}; struct Outer {char c;struct Inner inner;int tail;}; int main(void){struct Outer s={1,{2,37},41};return sizeof s!=12 || offsetof(struct Outer,inner)!=1 || offsetof(struct Outer,tail)!=8 || s.inner.i!=37 || s.tail!=41;}',
 'anonymous':'struct __attribute__((packed)) S {char c;union {int i;long n;};char z;}; int main(void){struct S s={.c=1,.n=37,.z=3};return sizeof s!=10 || offsetof(struct S,z)!=9 || s.n!=37 || s.z!=3;}',
 'union':'union __attribute__((aligned(16))) U {char c;int i;}; struct S {char pre;union U u;}; int main(void){return sizeof(union U)!=16 || _Alignof(union U)!=16 || offsetof(struct S,u)!=16 || sizeof(struct S)!=32;}',
 'packed-union':'union __attribute__((packed)) U {char c;int i;}; struct S {char pre;union U u;}; int main(void){return sizeof(union U)!=4 || _Alignof(union U)!=1 || offsetof(struct S,u)!=1 || sizeof(struct S)!=5;}',
 'typedef-up':'typedef int T __attribute__((aligned(16))); struct S {char c;T x;}; int main(void){return sizeof(T)!=4 || _Alignof(T)!=16 || offsetof(struct S,x)!=16 || sizeof(struct S)!=32;}',
 'typedef-down':'typedef int T __attribute__((__aligned__(1))); struct S {char c;T x;}; int main(void){return sizeof(T)!=4 || _Alignof(T)!=1 || offsetof(struct S,x)!=1 || sizeof(struct S)!=5;}',
 'typedef-chain':'typedef int T __attribute__((aligned(16))); typedef T U; int main(void){typedef U V;return sizeof(V)!=4 || _Alignof(V)!=16;}',
 'pointer-placement':'struct S {char c;int * __attribute__((aligned(16))) p;};int main(void){return offsetof(struct S,p)!=16 || sizeof(struct S)!=32;}',
 'extension-declaration':'__extension__ typedef long long T;int main(void){return sizeof(T)!=8;}',
 'keyword-alias':'int f(void) __attribute__((__const__));int f(void){return 7;}int main(void){return f()!=7;}',
 'local':'int main(void){int x __attribute__((unused,aligned(16)))=37;__attribute__((__unused__)) int y=5;return x+y!=42;}',
 'global':'__attribute__((aligned(16),unused)) int x=37; int main(void){return x!=37;}',
 'function':'int f(int) __attribute__((unused)); int f(int x){return x+1;} void stop(void) __attribute__((noreturn)); int main(void){return f(36)!=37;}',
 'macro':'#define ATTR(x) __attribute__((__packed__,__aligned__(x)))\nstruct ATTR(8) S {char c;int i;}; int main(void){return sizeof(struct S)!=8 || _Alignof(struct S)!=8 || offsetof(struct S,i)!=1;}',
 'nested-local':'int main(void){struct __attribute__((packed)) S {char c;int i;};struct S s={1,37};{typedef int T __attribute__((aligned(8)));struct N {char c;T i;};if(offsetof(struct N,i)!=8)return 1;}return sizeof s!=5 || s.i!=37;}',
}
DIAGNOSTIC={
 'align-zero':'int x __attribute__((aligned(0)));',
 'align-three':'int x __attribute__((aligned(3)));',
 'align-empty':'int x __attribute__((aligned()));',
 'align-many':'int x __attribute__((aligned(8,16)));',
 'pack-argument':'struct __attribute__((packed(2))) S {int x;};',
 'unsupported-cleanup':'int x __attribute__((cleanup(release)));',
 'linker-section':'int x __attribute__((section("my_data")));',
 'asm-label':'int f(void) __asm__("other_f");',
}
RESIDUAL={
 'ice':'struct __attribute__((aligned(1<<4))) S {char x;};int main(void){return _Alignof(struct S)!=16;}',
 'empty':'struct __attribute__((aligned(16))) S {}; int main(void){return _Alignof(struct S)!=16;}',
 'function-aligned':'int f(void) __attribute__((aligned(16))); int f(void){return 0;} int main(void){return f();}',
}
KEYWORDS=set('auto break case char const continue default do double else enum extern float for goto if inline int long register restrict return short signed sizeof static struct switch typedef union unsigned void volatile while _Alignas _Alignof _Atomic _Bool _Generic _Noreturn _Static_assert _Thread_local offsetof __attribute __attribute__ __extension__ aligned __aligned__ packed __packed__ unused __unused__ noreturn define'.split())
def generate(d):
 d.mkdir(parents=True,exist_ok=True)
 for name,body in {**POSITIVE,**DIAGNOSTIC,**RESIDUAL}.items():
  (d/(name+'.c')).write_text('#include <stddef.h>\n'+body+'\n')
 batch=['/* Generated by scripts/gen_gnu_attribute_cases.py. */\n#include <stddef.h>\n']
 for i,(name,body) in enumerate(POSITIVE.items()):
  batch.append('/* '+name+' */\n'+re.sub(r'\b[A-Za-z_]\w*',lambda m:m[0] if m[0] in KEYWORDS else f'ga{i}_{m[0]}',body)+'\n')
 batch.append('int main(void){\n'+''.join(f'if(ga{i}_main())return {i+1};\n' for i in range(len(POSITIVE)))+'return 0;}\n')
 (d/'batch.c').write_text(''.join(batch))
 matrix=batch[:-1]+['int printf(const char *,...); int main(void){unsigned mask=0;\n'+''.join(f'if(ga{i}_main())mask|=1u<<{i};\n' for i in range(len(POSITIVE)))+'printf("%u\\n",mask);return mask!=0;}\n']
 (d/'matrix.c').write_text(''.join(matrix))
 (d/'cases.json').write_text(json.dumps(dict(positive=list(POSITIVE),diagnostic=list(DIAGNOSTIC),residual=list(RESIDUAL)),indent=2)+'\n')
if __name__=='__main__':
 ap=argparse.ArgumentParser();ap.add_argument('directory',type=Path);a=ap.parse_args();generate(a.directory)
