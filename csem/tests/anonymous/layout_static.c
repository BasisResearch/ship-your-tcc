#include <stddef.h>
struct S { union { struct { int a[2], b; }; int n; }; int z; }; struct N {union {struct {int a[2],b;} inner; int n;} u; int z;};
int main(void) { static struct S s={0}; return sizeof s!=sizeof(struct N)||_Alignof(struct S)!=_Alignof(struct N)||offsetof(struct S,z)!=offsetof(struct N,z)||offsetof(struct S,b)!=2*sizeof(int); }
