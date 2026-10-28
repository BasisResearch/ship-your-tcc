#include <stddef.h>
struct S { signed a:3, b:5 __attribute__((aligned(8))), c:2; unsigned char tail; };
int main(void) { static struct S s; unsigned char *p=(unsigned char *)&s; unsigned i; for(i=0;i<sizeof s;++i) p[i]=0; s.a=-2; s.b=-3; s.c=-1;s.tail=71; return s.a!=(-2) || s.b!=(-3) || s.c!=(-1) || s.tail!=71; }
