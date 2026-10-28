struct S { unsigned a:31; unsigned b:31 __attribute__((packed)); char z; };
int main(void) { static struct S s={0x7fffffff,0x55555555,7}; return s.a!=0x7fffffff || s.b!=0x55555555 || s.z!=7 || sizeof s!=12 || _Alignof(struct S)!=4; }
