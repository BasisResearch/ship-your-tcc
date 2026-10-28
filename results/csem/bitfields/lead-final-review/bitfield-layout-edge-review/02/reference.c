#include <stdio.h>
struct S {unsigned short f0:16; unsigned char f1:7; unsigned short f2:16; unsigned char :0; unsigned short f3:15; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=32002ULL; s.f1=78ULL; s.f2=5926ULL; s.f3=23915ULL; s.z=90;printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}
