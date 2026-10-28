#include <stddef.h>
struct S { char head; struct { unsigned a:3, b:5 __attribute__((aligned(8))), c:2; } inner; unsigned char tail; };
int main(void) { struct S s; unsigned char *p=(unsigned char *)&s; unsigned i; for(i=0;i<sizeof s;++i) p[i]=0; s.inner.a=5; s.inner.b=19; s.inner.c=2;s.tail=71; return s.inner.a!=(5) || s.inner.b!=(19) || s.inner.c!=(2) || s.tail!=71; }
