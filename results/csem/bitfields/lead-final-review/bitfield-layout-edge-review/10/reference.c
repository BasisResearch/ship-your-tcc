#include <stdio.h>
struct S {unsigned short f0:15; unsigned f1:32; unsigned char f2:7; unsigned char f3:1; unsigned :0; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=7766ULL; s.f1=1230019821ULL; s.f2=52ULL; s.f3=1ULL; s.z=90;printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}
