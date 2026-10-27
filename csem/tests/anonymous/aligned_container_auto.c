#include <stddef.h>
struct S {char pre; union {struct {char a;long b;};long n;};int z;}; struct N {char pre;union {struct {char a;long b;} inner;long n;} u;int z;};
int main(void) { struct S s={.a=1,2,3}; return s.a!=1||s.b!=2||s.z!=3||sizeof s!=sizeof(struct N)||offsetof(struct S,z)!=offsetof(struct N,z); }
