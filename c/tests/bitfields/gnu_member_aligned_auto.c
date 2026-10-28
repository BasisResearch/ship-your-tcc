struct S { char c; unsigned a:3 __attribute__((aligned(8))); char z; };
int main(void) { struct S s={1,5,7}; return s.c!=1 || s.a!=5 || s.z!=7 || sizeof s!=16 || _Alignof(struct S)!=8; }
