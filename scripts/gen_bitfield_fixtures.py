#!/usr/bin/env python3
"""Generate the focused C11 bitfield matrix and a single native observation TU."""
from pathlib import Path
import argparse,hashlib,json,tempfile
ROOT=Path(__file__).resolve().parents[1]
OUT=ROOT/'c/tests/bitfields'
CASES={
 'character_copy_padding':('struct S { unsigned a:3; };','struct S s={5},t={0}; unsigned char *p=(unsigned char *)&s,*q=(unsigned char *)&t; unsigned i; for(i=0;i<sizeof s;++i) q[i]=p[i]; return t.a!=5;'),
 'character_copy_full':('struct S { unsigned a:8; };','struct S s={213},t={0}; unsigned char *p=(unsigned char *)&s,*q=(unsigned char *)&t; unsigned i; for(i=0;i<sizeof s;++i) q[i]=p[i]; return t.a!=213;'),
 'character_copy_schar':('struct S { signed a:3; };','struct S s={-3},t={0}; signed char *p=(signed char *)&s,*q=(signed char *)&t; unsigned i; for(i=0;i<sizeof s;++i) q[i]=p[i]; return t.a != -3;'),
 'character_copy_signed':('struct S { signed a:3; };','struct S s={-3},t={0}; char *p=(char *)&s,*q=(char *)&t; unsigned i; for(i=0;i<sizeof s;++i) q[i]=p[i]; return t.a != -3;'),
 'compound_scalar_index':('', 'int s[2]={1,6}; int i=0; s[i++]+=2; return i!=1 || s[0]!=3 || s[1]!=6;'),
 'compound_field_index':('struct S { unsigned a:3; };', 'struct S s[2]={{1},{6}}; int i=0; s[i++].a+=2; return i!=1 || s[0].a!=3 || s[1].a!=6;'),
 'compound_scalar_pointer':('', 'int s[2]={1,6}; int *p=s; *p++ *=3; return p!=s+1 || s[0]!=3 || s[1]!=6;'),
 'compound_shift':('struct S { unsigned a:3; };','struct S s={3}; s.a<<=1ull; return s.a!=6;'),
 'unsequenced_fields':('struct S { unsigned a:3; unsigned b:5; };','struct S s={0,0}; int n=(s.a=1)+(s.b=2); return n!=3 || s.a!=1 || s.b!=2;'),
 'unsequenced_updates':('struct S { unsigned a:3; unsigned b:5; };','struct S s={1,2}; int n=s.a++ + s.b++; return n!=3 || s.a!=2 || s.b!=3;'),
 'trailing_zero':('struct S { char c; unsigned :0; };','struct S s={3}; return s.c!=3 || sizeof s!=4 || _Alignof(struct S)!=1;'),
 'gnu_packed_straddle':('struct __attribute__((packed)) S { unsigned a:31; unsigned b:31; char z; };','struct S s={0x7fffffff,0x55555555,7}; return s.a!=0x7fffffff || s.b!=0x55555555 || s.z!=7 || sizeof s!=9 || _Alignof(struct S)!=1;'),
 'gnu_packed_zero':('struct __attribute__((packed)) S { unsigned a:3; unsigned :0; unsigned b:5; char z; };','struct S s={5,27,7}; return s.a!=5 || s.b!=27 || s.z!=7 || sizeof s!=6 || _Alignof(struct S)!=1;'),
 'gnu_member_packed':('struct S { unsigned a:31; unsigned b:31 __attribute__((packed)); char z; };','struct S s={0x7fffffff,0x55555555,7}; return s.a!=0x7fffffff || s.b!=0x55555555 || s.z!=7 || sizeof s!=12 || _Alignof(struct S)!=4;'),
 'gnu_member_aligned':('struct S { char c; unsigned a:3 __attribute__((aligned(8))); char z; };','struct S s={1,5,7}; return s.c!=1 || s.a!=5 || s.z!=7 || sizeof s!=16 || _Alignof(struct S)!=8;'),
 'promotion_after_updates':('struct S { unsigned a:3; };','struct S s={1}; int x=+(s.a++)-2; int y=+(s.a=1)-2; int z=+(++s.a)-3; return x != -1 || y != -1 || z != -1;'),
 'signed':('struct S { signed int a:3; unsigned b:5; int c:30; };','struct S s={-2,27,-123}; return s.a != -2 || s.b != 27 || s.c != -123;'),
 'plain_sign':('struct S { int a:3; };','struct S s={-1}; return s.a>=0;'),
 'padding':('struct S { unsigned a:3; unsigned :5; unsigned b:4; };','struct S s={3,9}; return s.a!=3 || s.b!=9;'),
 'trailing_padding':('struct S { int x; unsigned :3; };','struct S s={19}; return s.x!=19 || sizeof s!=8;'),
 'zero_barrier':('struct S { unsigned a:1; unsigned :0; unsigned b:1; };','struct S s={1,1}; return s.a!=1 || s.b!=1 || sizeof s!=8;'),
 'leading_padding':('struct S { unsigned :3; unsigned a:5; };','struct S s={23}; return s.a!=23;'),
 'leading_zero':('struct S { unsigned :0; unsigned a:4; };','struct S s={9}; return s.a!=9 || sizeof s!=4;'),
 'narrow_assignment':('struct S { unsigned a:3; signed int b:3; };','struct S s={0}; int a=(s.a=15); int b=(s.b=7); return a!=7 || b != -1 || s.a!=7 || s.b != -1;'),
 'increment':('struct S { unsigned a:3; int b:4; };','struct S s={7,-2}; int a=s.a++; int b=++s.b; return a!=7 || s.a!=0 || b != -1 || s.b != -1;'),
 'compound':('struct S { unsigned a:3; int b:4; };','struct S s={6,-3}; s.a+=3; s.b*=2; return s.a!=1 || s.b != -6;'),
 'promote':('struct S { unsigned a:31; unsigned b:32; };','struct S s={1,1}; return +(s.a)-2 >= 0 || +(s.b)-2 != 4294967295u;'),
 'bool':('struct S { _Bool a:1; _Bool b:1; };','struct S s={0,0}; s.a=42; return s.a!=1 || s.b!=0;'),
 'automatic_uninitialized':('struct S { unsigned a:3; unsigned b:5; };','struct S s; s.a=5; s.b=27; return s.a!=5 || s.b!=27;'),
 'automatic_partial':('struct S { unsigned a:3; unsigned b:5; };','struct S s; s.a=5; return s.a!=5;'),
 'volatile':('struct S { unsigned a:3; unsigned b:5; };','volatile struct S s={7,19}; unsigned a=s.a++; s.b=2; return a!=7 || s.a!=0 || s.b!=2;'),
 'volatile_member':('struct S { volatile unsigned a:3; unsigned b:5; };','struct S s={1,19}; s.a+=2; return s.a!=3 || s.b!=19;'),
 'union':('union S { unsigned :3; unsigned a:5; int b; };','union S s={17}; return s.a!=17 || sizeof s!=4;'),
 'union_designator':('union S { unsigned a:3; int b; unsigned c:7; };','union S s={.c=97}; return s.c!=97;'),
 'anonymous':('struct S { int x; union { struct { unsigned a:3; unsigned :2; int b:4; }; int z; }; int y; };','struct S s={.a=6,.b=-3,.y=19}; return s.a!=6 || s.b != -3 || s.x!=0 || s.y!=19;'),
 'array':('struct S { unsigned a:3; unsigned :1; int b:4; };','struct S s[2]={{1,-2},{7,-8}}; return s[0].a!=1 || s[1].a!=7 || s[0].b != -2 || s[1].b != -8;'),
 'designator_continuation':('struct S { unsigned a:3; unsigned :4; unsigned b:5; int c; };','struct S s={.a=5,17,39}; return s.a!=5 || s.b!=17 || s.c!=39;'),
 'padding_continuation':('struct S { struct { int x; unsigned :3; } a; int b; };','struct S s={1,2}; return s.a.x!=1 || s.b!=2;'),
 'aggregate_copy':('struct S { unsigned a:3; int b:5; };','struct S s={7,-9}; struct S t=s; return t.a!=7 || t.b != -9;'),
 'union_store':('union S { unsigned a:3; unsigned char b; };','union S s={.b=255}; s.a=2; return s.a!=2 || s.b!=250;'),
}
INVALID={
 'negative_width':'struct S { unsigned a:-1; };',
 'excess_width':'struct S { unsigned a:33; };',
 'bool_width':'struct S { _Bool a:2; };',
 'named_zero':'struct S { int a:0; };',
 'floating_type':'struct S { double a:3; };',
 'pointer_type':'struct S { int *a:3; };',
 'nonconstant_width':'int n; struct S { unsigned a:n; };',
 'address':'struct S { unsigned a:3; }; int f(void) { struct S s; return &s.a != 0; }',
 'sizeof':'struct S { unsigned a:3; }; int f(void) { struct S s; return sizeof(s.a); }',
 'aligned':'struct S { _Alignas(8) unsigned a:3; };',
}
def main():
 OUT.mkdir(parents=True,exist_ok=True);manifest=[];batch=['/* Generated by scripts/gen_bitfield_fixtures.py. */','#include <stdio.h>']
 for name,(decl,body) in CASES.items():
  for storage in ['auto','static']:
   # static applies to tested object, preserving automatic temporaries
   b=body if storage=='auto' else body.replace('struct S s','static struct S s',1).replace('union S s','static union S s',1).replace('int s[','static int s[',1)
   src=f'{decl}\nint main(void) {{ {b} }}\n';path=OUT/f'{name}_{storage}.c';path.write_text(src)
   manifest.append(dict(path='c/tests/bitfields/'+path.name,kind='gnu-defined' if name.startswith('gnu_') else 'defined',sha256=hashlib.sha256(src.encode()).hexdigest()))
   batch += [f'static int probe_{name}_{storage}(void) {{ {decl} {b} }}']
 for name,decl in INVALID.items():
  src=decl+'\nint main(void) { return 0; }\n';path=OUT/f'invalid_{name}.c';path.write_text(src)
  manifest.append(dict(path='c/tests/bitfields/'+path.name,kind='constraint',sha256=hashlib.sha256(src.encode()).hexdigest()))
 batch += ['int main(void) { int failed=0;']
 for name in CASES:
  for storage in ['auto','static']:
   batch += [f'printf("{name}_{storage}=%d\\n",probe_{name}_{storage}()); failed += probe_{name}_{storage}();'] # invoke twice for static side effects? object initializer applied once and increments changes second run! fix below
 batch += ['return failed; }']
 batch=[s.replace('failed += probe_', 'failed += probe_') for s in batch]
 # single invocation per case, also records its result
 for i,s in enumerate(batch):
  if s.startswith('printf('):
   label=s.split('"')[1].split('=')[0];batch[i]=f'{{ int r=probe_{label}(); printf("{label}=%d\\n",r); failed+=r; }}'
 (OUT/'batch.c').write_text('\n'.join(batch)+'\n')
 layout = r'''/* Generated by scripts/gen_bitfield_fixtures.py. */
#include <stdio.h>
#include <stddef.h>
struct B1 { int a:3; unsigned b:5; int c:30; char z; };
struct B2 { char a:7; int b:10; char z; };
struct B3 { unsigned a:1; unsigned :0; unsigned b:1; char z; };
struct B4 { char c; int x:4; char z; };
struct B5 { long long a:40; int b:30; char z; };
struct B6 { unsigned :3; char z; };
struct B7 { int x; unsigned :3; char z; };
union U { unsigned a:3; unsigned b:17; char z; };
static void dump(const char *name, unsigned char *p, int n, int a, int off) {
  int i; printf("%s size=%d align=%d tail=%d bytes=", name,n,a,off);
  for(i=0;i<n;i++) printf("%02x",(unsigned)p[i]); printf("\n");
}
#define OBS(T, assign) { T s; unsigned char *p=(unsigned char *)&s; int i; for(i=0;i<(int)sizeof s;i++) p[i]=0; assign; dump(#T,p,sizeof s,_Alignof(T),offsetof(T,z)); }
int main(void) {
 OBS(struct B1,s.a=-1;s.b=31;s.c=-5;s.z=0x5a);
 OBS(struct B2,s.a=0x55;s.b=0x2aa;s.z=0x5a);
 OBS(struct B3,s.a=1;s.b=1;s.z=0x5a);
 OBS(struct B4,s.c=0x33;s.x=-3;s.z=0x5a);
 OBS(struct B5,s.a=-1;s.b=-3;s.z=0x5a);
 OBS(struct B6,s.z=0x5a);
 OBS(struct B7,s.x=0x12345678;s.z=0x5a);
 OBS(union U,s.b=0x15555);
 return 0;
}
'''
 (OUT/'layout.c').write_text(layout)
 (OUT/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
if __name__=='__main__':
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--check',action='store_true');args=ap.parse_args()
 if not args.check:main()
 else:
  original=OUT
  with tempfile.TemporaryDirectory(prefix='bitfield-fixtures.') as tmp:
   OUT=Path(tmp);main()
   expected={p.name:p.read_bytes() for p in OUT.iterdir() if p.is_file()}
   actual={p.name:p.read_bytes() for p in original.iterdir() if p.is_file()}
   assert actual==expected,'bitfield fixture generator drift'
  print('bitfield fixtures match generator')
