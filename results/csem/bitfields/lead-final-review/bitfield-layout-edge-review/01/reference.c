#include <stdio.h>
struct S {unsigned short f0:1; unsigned char f1:8; unsigned short f2:16; unsigned f3:32; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=1ULL; s.f1=201ULL; s.f2=2740ULL; s.f3=2824576302ULL; s.z=90;printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}
