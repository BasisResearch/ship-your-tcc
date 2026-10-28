#include <stdio.h>
struct S {unsigned long long f0:63; unsigned :0; unsigned short f1:15; unsigned long long f2:63; unsigned char f3:7; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=7762939953835634776ULL; s.f1=18890ULL; s.f2=9214923046800057049ULL; s.f3=44ULL; s.z=90;printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}
