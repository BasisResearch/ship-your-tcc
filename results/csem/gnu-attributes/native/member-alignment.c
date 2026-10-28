#include <stdio.h>
struct __attribute__((aligned(16))) S { char c; int x; };
struct __attribute__((packed,aligned(16))) P { char c; int x; };
struct M { char c __attribute__((aligned(16))); int x; };
int main(void) {struct S s;struct P p;struct M m;printf("%zu %zu %zu %zu %zu %zu\n",__alignof__(s),__alignof__(s.c),__alignof__(p),__alignof__(p.c),__alignof__(m),__alignof__(m.c));}
