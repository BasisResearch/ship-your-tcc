#include <stdio.h>
struct S {unsigned f0:31; unsigned long long f1:1; unsigned long long f2:64; unsigned short f3:15; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=135459134ULL; s.f1=0ULL; s.f2=13564652178404912063ULL; s.f3=20971ULL; s.z=90;printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}
