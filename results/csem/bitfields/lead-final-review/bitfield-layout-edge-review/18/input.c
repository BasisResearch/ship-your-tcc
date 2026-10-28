struct __attribute__((packed)) S {unsigned short f0:1; unsigned short f1:1; unsigned long long f2:1; unsigned char f3:1; char z; };
int main(void){struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; s.f0=0ULL; s.f1=0ULL; s.f2=1ULL; s.f3=0ULL; s.z=90;return sizeof s!=2 || _Alignof(struct S)!=1 || (char*)&s.z-(char*)&s!=1 || p[0]!=4 || p[1]!=90;}
