#include <stdio.h>
struct __attribute__((packed)) S {unsigned char f0:7; unsigned short f1:15; unsigned short f2:15; unsigned long long f3:63; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=44ULL; s.f1=6223ULL; s.f2=7951ULL; s.f3=2440560912204074439ULL; s.z=90;printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}
