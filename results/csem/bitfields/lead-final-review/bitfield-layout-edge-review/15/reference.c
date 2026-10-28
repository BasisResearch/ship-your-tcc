#include <stdio.h>
struct __attribute__((packed)) S {unsigned f0:32; unsigned :0; unsigned short f1:15; unsigned f2:31; unsigned short f3:16; unsigned :0; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=2211089588ULL; s.f1=30279ULL; s.f2=1376543471ULL; s.f3=51771ULL; s.z=90;printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}
