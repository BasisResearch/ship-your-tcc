struct S { unsigned a:3; }; int main(void) { struct S s={5}, t={0}; unsigned char *p=(unsigned char*)&s,*q=(unsigned char*)&t; unsigned i; t=s; return t.a!=5; }
