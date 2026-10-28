#include <stdio.h>
struct S {unsigned short f0:1; unsigned char f1:1; unsigned f2:31; unsigned long long f3:64; unsigned short :3; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=1ULL; s.f1=1ULL; s.f2=1260751ULL; s.f3=169411111116798216ULL; s.z=90;printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}
