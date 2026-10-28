#include <stdio.h>
struct __attribute__((packed)) S {unsigned char f0:8; unsigned char :0; unsigned short f1:15; unsigned char f2:1; unsigned short :3; unsigned f3:31; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=106ULL; s.f1=25917ULL; s.f2=0ULL; s.f3=270047896ULL; s.z=90;printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}
