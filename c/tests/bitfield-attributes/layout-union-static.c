#include <stdio.h>
#include <stddef.h>
union S { unsigned a:3, b:5 __attribute__((aligned(8))), c:2; unsigned char tail; };
int main(void) { static union S s; unsigned char *p=(unsigned char *)&s; unsigned i; for(i=0;i<sizeof s;++i) p[i]=0; s.b=19; printf("union-static %lu %lu %lu", (unsigned long)sizeof s, (unsigned long)_Alignof(union S), (unsigned long)offsetof(union S,tail)); for(i=0;i<sizeof s;++i) printf(" %02x",(unsigned)p[i]); printf("\n"); return s.b!=(19); }
