#include <stdio.h>
struct __attribute__((packed)) S {unsigned char f0:1; unsigned f1:31; unsigned long long f2:1; unsigned char :0; unsigned long long f3:1; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=0ULL; s.f1=1254292541ULL; s.f2=0ULL; s.f3=1ULL; s.z=90;printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}
