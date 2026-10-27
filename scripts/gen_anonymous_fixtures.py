#!/usr/bin/env python3
"""Complete strict-C11 anonymous member probes; regenerate, never edit outputs."""
from pathlib import Path
import argparse, json, re
ROOT=Path(__file__).resolve().parent.parent
S='struct S { union { struct { int a,b; }; int n; }; int z; };'
A='struct S { union { struct { int a[2], b; }; int n; }; int z; };'
cases={
'scalar_continuation':(S,'struct S s={.a=1,2,3};','s.a!=1||s.b!=2||s.z!=3'),
'scalar_explicit':(S,'struct S s={.a=1,.b=2,.z=3};','s.a!=1||s.b!=2||s.z!=3'),
'array_continuation':(A,'struct S s={.a[0]=1,2,3,4};','s.a[0]!=1||s.a[1]!=2||s.b!=3||s.z!=4'),
'positional_elided':(S,'struct S s={1,2,3};','s.a!=1||s.b!=2||s.z!=3'),
'positional_braced':(S,'struct S s={{{1,2}},3};','s.a!=1||s.b!=2||s.z!=3'),
'anonymous_struct':('struct S { struct {int a,b;}; int z; };','struct S s={.a=1,2,3};','s.a!=1||s.b!=2||s.z!=3'),
'union_first_anon':('union S { struct {int a,b;}; int n; };','union S s={.a=1,2};','s.a!=1||s.b!=2'),
'union_later_anon':('union S { int n; struct {int a,b;}; };','union S s={.a=1,2};','s.a!=1||s.b!=2'),
'union_switch_to_named':(S,'struct S s={.a=1,.b=2,.n=9,3};','s.n!=9||s.z!=3'),
'union_switch_from_named':(S,'struct S s={.n=9,.b=2,3};','s.a!=0||s.b!=2||s.z!=3'),
'union_switch_back':(S,'struct S s={.a=1,.n=9,.b=2,3};','s.a!=0||s.b!=2||s.z!=3'),
'same_member_repeat':(S,'struct S s={.a=1,.b=2,.a=4,5,6};','s.a!=4||s.b!=5||s.z!=6'),
'nested_anonymous':('struct S { struct { union { struct { int a,b; }; long n; }; int c; }; int z; };','struct S s={.a=1,2,3,4};','s.a!=1||s.b!=2||s.c!=3||s.z!=4'),
'mixed_named':('struct S { struct { union {struct {int a,b;}; int n;}; int c;} v; int z;};','struct S s={.v.a=1,2,3,4};','s.v.a!=1||s.v.b!=2||s.v.c!=3||s.z!=4'),
'named_inside_anonymous':('struct S { union {struct {struct {int a,b;} v; int c;}; long n;}; int z;};','struct S s={.v.a=1,2,3,4};','s.v.a!=1||s.v.b!=2||s.c!=3||s.z!=4'),
'braces_then_continuation':(A,'struct S s={.a={1,2},3,4};','s.a[0]!=1||s.a[1]!=2||s.b!=3||s.z!=4'),
'reset_designator':(S,'struct S s={.z=8,.a=1,2,3};','s.a!=1||s.b!=2||s.z!=3'),
'pointer_member':('struct S {union {struct {int *p; int b;}; long n;}; int z;}; int x=7;','struct S s={.p=&x,2,3};','s.p!=&x||*s.p!=7||s.b!=2||s.z!=3'),
'array_objects':(S,'struct S s[2]={[0].a=1,2,3,4,5,6};','s[0].a!=1||s[0].b!=2||s[0].z!=3||s[1].a!=4||s[1].b!=5||s[1].z!=6'),
'null_implicit':('struct S {union {struct {int a; int *p;}; long n;}; int z;};','struct S s={.a=1};','s.a!=1||s.p!=0||s.z!=0'),
'layout':(A+' struct N {union {struct {int a[2],b;} inner; int n;} u; int z;};','struct S s={0};','sizeof s!=sizeof(struct N)||_Alignof(struct S)!=_Alignof(struct N)||offsetof(struct S,z)!=offsetof(struct N,z)||offsetof(struct S,b)!=2*sizeof(int)'),
'address':(S,'struct S s={.a=1,2,3};','&s.a==&s.b||&s.b==&s.z||s.a!=1'),
'arrow':(S,'struct S s={.a=1,2,3};','(&s)->a!=1||(&s)->b!=2||(&s)->z!=3'),
'name_shadow':('struct S { struct {int a,b;} named; union {struct {int a,b;}; int n;}; int z;};','struct S s={.named={7,8},.a=1,2,3};','s.named.a!=7||s.named.b!=8||s.a!=1||s.b!=2||s.z!=3'),
'synthetic_collision':('struct S { int anonymous; struct {int a,b;}; int z;};','struct S s={7,.a=1,2,3};','s.anonymous!=7||s.a!=1||s.b!=2||s.z!=3'),
'const_container':('struct S { const struct {int a,b;}; int z;};','struct S s={.a=1,2,3};','s.a!=1||s.b!=2||s.z!=3'),
'const_object':(S,'const struct S s={.a=1,2,3};','s.a!=1||s.b!=2||s.z!=3'),
 'rvalue_member':(S+' struct S make(void){struct S t={.a=1,2,3};return t;}', 'int dummy=0;', 'make().b!=2||dummy'),
'compound_literal':(S,'int dummy=0;','((struct S){.a=1,2,3}).b!=2||dummy'),
'const_pointer_type':('struct S {const struct {int a,b;}; int z;};','struct S s={.a=1,2,3};','!_Generic(&s.a,const int *:1,default:0)'),
'volatile_pointer_type':('struct S {volatile struct {int a,b;}; int z;};','struct S s={.a=1,2,3};','!_Generic(&s.a,volatile int *:1,default:0)'),
'aligned_container':('struct S {char pre; union {struct {char a;long b;};long n;};int z;}; struct N {char pre;union {struct {char a;long b;} inner;long n;} u;int z;};','struct S s={.a=1,2,3};','s.a!=1||s.b!=2||s.z!=3||sizeof s!=sizeof(struct N)||offsetof(struct S,z)!=offsetof(struct N,z)'),
'adjacent_containers':('struct S {struct {int a,b;}; struct {int c,d;};int z;};','struct S s={.b=2,3,4,5};','s.a!=0||s.b!=2||s.c!=3||s.d!=4||s.z!=5'),
 'named_const_pointer_type':('struct S {const struct {int a,b;} named; int z;};','struct S s={.named={1,2},3};','!_Generic(&s.named.a,const int *:1,default:0)'),
'direct_const_pointer_type':('struct S {const int a; int z;};','struct S s={1,3};','!_Generic(&s.a,const int *:1,default:0)'),
'object_const_pointer_type':('struct S {int a;};','const struct S s={1};','!_Generic(&s.a,const int *:1,default:0)'),
 'container_name_collision':('struct S {struct {int c__________cerb_anon_0;};int z;};','struct S s={.c__________cerb_anon_0=1,2};','s.c__________cerb_anon_0!=1||s.z!=2'),
 'aggregate_copy':(S+' int check(struct S p){struct S copy=p; return copy.a!=1||copy.b!=2||copy.z!=3;}','struct S s={.a=1,2,3};','check(s)'),
'volatile_container':('struct S { volatile struct {int a,b;}; int z;};','struct S s={.a=1,2,3};','s.a!=1||s.b!=2||s.z!=3'),
}
invalid={
'collision_direct':'struct S { int a; struct {int a;}; }; int main(void){return 0;}\n',
'collision_promoted':'struct S { struct {int a;}; union {int a; long b;}; }; int main(void){return 0;}\n',
'const_assignment':S+' int main(void){const struct S s={0}; s.a=1; return 0;}\n',
'const_container_assignment':'struct S {const struct {int a;};}; int main(void){struct S s={0}; s.a=1; return 0;}\n',
'unknown_member':S+' int main(void){struct S s={.missing=1}; return 0;}\n',
'named_not_promoted':'struct S {struct {int a;} named;}; int main(void){struct S s={.a=1}; return 0;}\n',
}
# Deliberately guess the physical name, including the index and width. The
# long public control field dominates the pinned stddef.h member names. This
# verifies public lookup rather than relying on hard-to-guess private names.
control_name='reference_boundary_field_for_internal_container'
private_name='_'*(len(control_name)+1)+'cerb_anon_2'
for storage in ['auto','static']:
    decl='#include <stddef.h>\nstruct S {int '+control_name+'; struct {int a;};};\n'
    obj=('static ' if storage=='static' else '')+'struct S s={0,{1}}; '
    for form,expr in [('member','s.'+private_name+'.a'),('arrow','(&s)->'+private_name+'.a'),('offsetof','offsetof(struct S,'+private_name+')')]:
        invalid[f'private_{form}_{storage}']=decl+'int main(void){'+obj+'return '+expr+';}\n'
    obj=('static ' if storage=='static' else '')+'struct S s={.'+private_name+'={1}}; '
    invalid[f'private_designator_{storage}']=decl+'int main(void){'+obj+'return s.a;}\n'

def generated():
    out={}
    for name,(decl,obj,test) in cases.items():
        for storage in ['auto','static']:
            obj=('static '+obj) if storage=='static' else obj
            # render independently: do not accumulate storage across variants
            source=('#include <stddef.h>\n' if 'offsetof' in test else '')+decl+'\nint main(void) { '+obj+' return '+test+'; }\n'
            out[f'{name}_{storage}.c']=source
    out.update({f'invalid_{k}.c':v for k,v in invalid.items()})
    return out

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--check',action='store_true');args=ap.parse_args()
    dest=ROOT/'csem/tests/anonymous';dest.mkdir(parents=True,exist_ok=True)
    for name,src in generated().items():
        p=dest/name
        if args.check:
            if not p.exists() or p.read_text()!=src: raise SystemExit(f'drift: {p}')
        else:p.write_text(src)
    # Every fixture is self-contained and contains no string/character literal
    # or preprocessing directive, so identifier-token renaming is sufficient.
    keywords=set('auto break case char const continue default do double else enum extern float for goto if inline int long register restrict return short signed sizeof static struct switch typedef union unsigned void volatile while _Alignas _Alignof _Atomic _Bool _Complex _Generic _Imaginary _Noreturn _Static_assert _Thread_local _Generic offsetof'.split())
    parts=['/* Generated by scripts/gen_anonymous_fixtures.py. */\n#include <stddef.h>\n']
    positive=[(n,src) for n,src in generated().items() if not n.startswith('invalid_')]
    for i,(name,src) in enumerate(positive):
        src=src.replace('#include <stddef.h>\n','')
        assert not any(t in src for t in ['"', "'", '#', '/*', '//'])
        parts.append(f'/* {name} */\n'+re.sub(r'\b[A-Za-z_]\w*',lambda m:m[0] if m[0] in keywords else f'a{i}_{m[0]}',src))
    parts.append('int main(void) {\n')
    parts.extend(f'  if (a{i}_main()) return {i+1};\n' for i in range(len(positive)))
    parts.append('  return 0;\n}\n')
    batch=dest.parent/'anonymous-batch.c';src=''.join(parts)
    if args.check:
        if not batch.exists() or batch.read_text()!=src:raise SystemExit(f'drift: {batch}')
    else:batch.write_text(src)
    print(f'anonymous fixtures: {len(cases)*2} positive, {len(invalid)} invalid')
if __name__=='__main__':main()
