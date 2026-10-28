#include <stdio.h>
struct S {unsigned f0:32; unsigned char :0; unsigned f1:31; unsigned short f2:1; unsigned short f3:1; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=1116618630ULL; s.f1=117532794ULL; s.f2=1ULL; s.f3=1ULL; s.z=90;printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}
