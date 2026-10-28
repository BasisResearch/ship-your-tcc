#include <stddef.h>
struct S { unsigned a:31, b:31 __attribute__((packed)), c:2; unsigned char tail; };
int main(void) { static struct S s; unsigned char *p=(unsigned char *)&s; unsigned i; for(i=0;i<sizeof s;++i) p[i]=0; s.a=12345; s.b=67890; s.c=2;s.tail=71; return s.a!=(12345) || s.b!=(67890) || s.c!=(2) || s.tail!=71; }
