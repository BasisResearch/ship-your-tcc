#include <stddef.h>
struct S { unsigned a:3, b:5 __attribute__((packed, aligned(8))), c:2; unsigned char tail; };
int main(void) { static struct S s; unsigned char *p=(unsigned char *)&s; unsigned i; for(i=0;i<sizeof s;++i) p[i]=0; s.a=5; s.b=19; s.c=2;s.tail=71; return s.a!=(5) || s.b!=(19) || s.c!=(2) || s.tail!=71; }
