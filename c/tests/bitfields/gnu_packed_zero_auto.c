struct __attribute__((packed)) S { unsigned a:3; unsigned :0; unsigned b:5; char z; };
int main(void) { struct S s={5,27,7}; return s.a!=5 || s.b!=27 || s.z!=7 || sizeof s!=6 || _Alignof(struct S)!=1; }
