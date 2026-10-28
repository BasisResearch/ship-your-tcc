#include <stdio.h>
#include <stddef.h>
struct S { unsigned __attribute__((aligned(8))) a:3, b:5, c:2; unsigned char tail; };
int main(void) { struct S s; unsigned char *p=(unsigned char *)&s; unsigned i; for(i=0;i<sizeof s;++i) p[i]=0; s.a=5; s.b=19; s.c=2;s.tail=71; printf("leading-specifier-auto %lu %lu %lu", (unsigned long)sizeof s, (unsigned long)_Alignof(struct S), (unsigned long)offsetof(struct S,tail)); for(i=0;i<sizeof s;++i) printf(" %02x",(unsigned)p[i]); printf("\n"); return s.a!=(5) || s.b!=(19) || s.c!=(2) || s.tail!=71; }
