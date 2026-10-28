#include <stdio.h>
struct __attribute__((packed)) S {unsigned short f0:15; unsigned char f1:7; unsigned char :0; unsigned long long f2:63; unsigned long long f3:64; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=20448ULL; s.f1=27ULL; s.f2=228360817133454798ULL; s.f3=345769902696440057ULL; s.z=90;printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}
