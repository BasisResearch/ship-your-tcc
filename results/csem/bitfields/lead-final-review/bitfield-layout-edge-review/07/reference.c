#include <stdio.h>
struct S {unsigned char f0:8; unsigned char f1:7; unsigned long long f2:1; unsigned short f3:1; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=161ULL; s.f1=23ULL; s.f2=1ULL; s.f3=0ULL; s.z=90;printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}
