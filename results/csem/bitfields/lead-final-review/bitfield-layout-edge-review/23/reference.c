#include <stdio.h>
struct S {unsigned long long f0:1; unsigned short f1:1; unsigned short :3; unsigned f2:1; unsigned f3:32; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=1ULL; s.f1=1ULL; s.f2=0ULL; s.f3=1483426551ULL; s.z=90;printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}
