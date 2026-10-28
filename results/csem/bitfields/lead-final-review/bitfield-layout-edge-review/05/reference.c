#include <stdio.h>
struct S {unsigned char f0:7; unsigned char :0; unsigned long long f1:64; unsigned long long f2:63; unsigned f3:1; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=56ULL; s.f1=14614314616415014241ULL; s.f2=4156627060903811485ULL; s.f3=0ULL; s.z=90;printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}
