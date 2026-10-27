#!/usr/bin/env python3
"""Deterministic C11 initializer-continuation cases (all defined return zero).

Each observation reads only active union members. Invalid controls are
separate translation units and must retain compiler diagnostics.
"""
from pathlib import Path
import argparse
import json

CASES = {
'minimal': ('struct S {int a[2]; int z;};', 'struct S s={.a[0]=1,2,3};', 's.a[0]==1 && s.a[1]==2 && s.z==3'),
'nested_union': ('struct S {union {struct {int a; int b[3]; int c;} s; int n;} u; int z;};', 'struct S v={.u.s.b[1]=2,3,4,5};', 'v.u.s.a==0 && v.u.s.b[0]==0 && v.u.s.b[1]==2 && v.u.s.b[2]==3 && v.u.s.c==4 && v.z==5'),
'matrix': ('', 'int v[2][3]={[0][1]=1,2,3,4,5};', 'v[0][0]==0 && v[0][1]==1 && v[0][2]==2 && v[1][0]==3 && v[1][1]==4 && v[1][2]==5'),
'struct_path': ('struct A {int x,y;}; struct S {struct A a[2]; int z;};', 'struct S v={.a[0].y=1,2,3,4};', 'v.a[0].x==0 && v.a[0].y==1 && v.a[1].x==2 && v.a[1].y==3 && v.z==4'),
'explicit': ('struct S {int a[2][2]; int z;};', 'struct S v={.a[0]={1}, {2,3},4};', 'v.a[0][0]==1 && v.a[0][1]==0 && v.a[1][0]==2 && v.a[1][1]==3 && v.z==4'),
'elided': ('struct S {int a[3]; int z;};', 'struct S v={.a=1,2,3,4};', 'v.a[0]==1 && v.a[1]==2 && v.a[2]==3 && v.z==4'),
'redesignate': ('struct S {int a[3]; int z;};', 'struct S v={.a=1,.a[1]=2,3,.z=4};', 'v.a[0]==1 && v.a[1]==2 && v.a[2]==3 && v.z==4'),
'outer_designator': ('struct S {int a[3]; int z;};', 'struct S v={.a=1,.z=4};', 'v.a[0]==1 && v.a[1]==0 && v.a[2]==0 && v.z==4'),
'array_designator': ('', 'int v[2][2]={1,[1][0]=2,3};', 'v[0][0]==1 && v[0][1]==0 && v[1][0]==2 && v[1][1]==3'),
'braced_reset': ('struct S {int a[3]; int z;};', 'struct S v={.a[0]=9,8,.a={1},4};', 'v.a[0]==1 && v.a[1]==0 && v.a[2]==0 && v.z==4'),
'union_switch': ('union U {struct {int a[2]; int b;} s; int n;}; struct S {union U u; int z;};', 'struct S v={.u.s.a[0]=9,8,.u.n=7,.u.s.a[0]=1,2,3,4};', 'v.u.s.a[0]==1 && v.u.s.a[1]==2 && v.u.s.b==3 && v.z==4'),
'union_same': ('union U {struct {int a[3]; int b;} s; int n;};', 'union U v={.s.a[0]=1,2,.s.a[2]=3,4};', 'v.s.a[0]==1 && v.s.a[1]==2 && v.s.a[2]==3 && v.s.b==4'),
'union_array': ('union U {struct {int a[2]; int b;} s; int n;};', 'union U v[2]={[0].s.a[1]=1,2,3,4,5};', 'v[0].s.a[0]==0 && v[0].s.a[1]==1 && v[0].s.b==2 && v[1].s.a[0]==3 && v[1].s.a[1]==4 && v[1].s.b==5'),
'incomplete': ('struct S {int a[2]; int z;};', 'struct S v[]={[1].a[0]=1,2,3,4,5,6};', 'sizeof(v)/sizeof(v[0])==3 && v[0].z==0 && v[1].a[0]==1 && v[1].a[1]==2 && v[1].z==3 && v[2].a[0]==4 && v[2].a[1]==5 && v[2].z==6'),
'incomplete_matrix': ('', 'int v[][2]={[1][0]=1,2,3,4};', 'sizeof(v)/sizeof(v[0])==3 && v[0][1]==0 && v[1][0]==1 && v[1][1]==2 && v[2][0]==3 && v[2][1]==4'),
'string': ('struct S {char a[2][4]; int z;};', 'struct S v={.a[0]="ab","cd",3};', "v.a[0][0]=='a' && v.a[0][2]==0 && v.a[1][0]=='c' && v.a[1][2]==0 && v.z==3"),
'string_scalar': ('struct S {char a[4]; int z;};', "struct S v={.a[1]='x','y',0,3};", "v.a[0]==0 && v.a[1]=='x' && v.a[2]=='y' && v.a[3]==0 && v.z==3"),
'anonymous': ('struct S {union {struct {int a[2]; int b;}; int n;}; int z;};', 'struct S v={.a[0]=1,2,3,4};', 'v.a[0]==1 && v.a[1]==2 && v.b==3 && v.z==4'),
'padding': ('struct S {unsigned :3; unsigned a:4; unsigned :2; unsigned b:4; int z;}; struct T {struct S s; int z;};', 'struct T v={.s.a=1,2,3,4};', 'v.s.a==1 && v.s.b==2 && v.s.z==3 && v.z==4'),
'braced_union': ('union U {int n; int a[2];}; struct S {union U u; int z;};', 'struct S v={.u.a={1,2},3};', 'v.u.a[0]==1 && v.u.a[1]==2 && v.z==3'),
'pointer': ('int x=7; struct S {int *a[2]; int z;};', 'struct S v={.a[0]=&x,&x,3};', '*v.a[0]==7 && *v.a[1]==7 && v.z==3'),
'deep': ('struct A {int a[2];int z;}; struct B {struct A a[2];int z;}; struct C {struct B a[2];int z;};', 'struct C v[2]={[0].a[0].a[0].a[0]=1,2,3,4,5,6,7,8,9,10};', 'v[0].a[0].a[0].a[0]==1 && v[0].a[0].a[0].a[1]==2 && v[0].a[0].a[0].z==3 && v[0].a[0].a[1].a[0]==4 && v[0].a[0].a[1].z==6 && v[0].a[0].z==7 && v[0].a[1].a[0].a[0]==8 && v[0].a[1].a[0].z==10'),
'trailing_padding': ('struct A {int x;unsigned :3;}; struct S {struct A a;int y;};', 'struct S v={.a.x=1,2};', 'v.a.x==1 && v.y==2'),
'scalar_braces': ('struct S {int a[2];int z;};', 'struct S v={.a[0]={1},2,3};', 'v.a[0]==1 && v.a[1]==2 && v.z==3'),
'partial_braces': ('struct A {int x,y;}; struct S {struct A a[2];int z;};', 'struct S v={.a[0].y=1,{2},3};', 'v.a[0].x==0 && v.a[0].y==1 && v.a[1].x==2 && v.a[1].y==0 && v.z==3'),
'reverse': ('struct S {int a[2];int z;};', 'struct S v={.z=9,.a[0]=1,2,3};', 'v.a[0]==1 && v.a[1]==2 && v.z==3'),
'union_siblings': ('union U {int n;int a[2];}; struct S {union U a[2];int z;};', 'struct S v={.a[0].a[0]=1,2,3,4};', 'v.a[0].a[0]==1 && v.a[0].a[1]==2 && v.a[1].n==3 && v.z==4'),
'outer_brace_boundary': ('struct A {int x,y;}; struct S {struct A a;int z;};', 'struct S v[2]={{.a.x=1},2,3,4};', 'v[0].a.x==1 && v[0].a.y==0 && v[0].z==0 && v[1].a.x==2 && v[1].a.y==3 && v[1].z==4'),
'union_reset_string': ('union U {char a[4];int n;}; struct S {union U u;int z;};', 'struct S v={.u.a="abc",.u.n=7,.u.a="xy",3};', "v.u.a[0]=='x' && v.u.a[1]=='y' && v.u.a[2]==0 && v.z==3"),

}
BAD = {
'excess': 'struct S {int a[2];int z;}; struct S v={.a[0]=1,2,3,4};',
'union_excess': 'union U {int a[2];int z;}; union U v={.a[0]=1,2,3};',
'bound': 'int v[2][2]={[0][2]=1};',
'negative': 'int v[2][2]={[0][-1]=1};',
'field': 'struct S {int a[2];}; struct S v={.b[0]=1};',
'scalar_array': 'struct S {int x;}; struct S v={.x[0]=1};',
'scalar_field': 'struct S {int x;}; struct S v={.x.y=1};',
'missing_value': 'struct S {int a[2];}; struct S v={.a[0]=,};',
}

def generate(out):
    out = Path(out); out.mkdir(parents=True, exist_ok=True)
    valid, invalid = {}, []
    for name, (types, decl, check) in CASES.items():
        for storage in ('static', 'auto'):
            f = out / f'designator_{name}_{storage}.c'
            if storage == 'static':
                # `x` belongs outside main in the pointer case.
                text = f'{types}\nstatic {decl}\nint main(void){{return !({check});}}\n'
            else:
                text = f'{types}\nint main(void){{{decl} return !({check});}}\n'
            f.write_text(text); valid[str(f)] = ''
    for name, decl in BAD.items():
        f = out / f'invalid_designator_{name}.c'
        f.write_text(decl+'\nint main(void){return 0;}\n'); invalid.append(str(f))
    (out/'manifest.json').write_text(json.dumps({'valid':valid,'invalid':invalid},indent=2)+'\n')
    return valid, invalid

if __name__ == '__main__':
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('out',nargs='?',default='build/designator-focused')
    a=ap.parse_args();v,b=generate(a.out);print(f'{len(v)} valid; {len(b)} invalid')
