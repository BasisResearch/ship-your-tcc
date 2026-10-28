#include <stddef.h>
union S { unsigned a:3, b:5 __attribute__((aligned(8))), c:2; unsigned char tail; };
int main(void) { static union S s; unsigned char *p=(unsigned char *)&s; unsigned i; for(i=0;i<sizeof s;++i) p[i]=0; s.b=19; return s.b!=(19); }
