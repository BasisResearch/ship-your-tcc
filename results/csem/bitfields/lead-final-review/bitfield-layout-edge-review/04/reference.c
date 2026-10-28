#include <stdio.h>
struct S {unsigned short f0:15; unsigned :0; unsigned short f1:1; unsigned :0; unsigned long long f2:63; unsigned :0; unsigned f3:32; unsigned short :3; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=4145ULL; s.f1=1ULL; s.f2=8339439151582676895ULL; s.f3=2633616974ULL; s.z=90;printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}
