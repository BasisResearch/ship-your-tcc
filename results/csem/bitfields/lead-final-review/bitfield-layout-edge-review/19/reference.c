#include <stdio.h>
struct S {unsigned short f0:16; unsigned f1:1; unsigned short :3; unsigned char f2:8; unsigned long long f3:63; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=135ULL; s.f1=0ULL; s.f2=163ULL; s.f3=3258191643268489843ULL; s.z=90;printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}
