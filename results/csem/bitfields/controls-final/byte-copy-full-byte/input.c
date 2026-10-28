struct S { unsigned a:8; }; int main(void) { struct S s={5}, t={0}; unsigned char *p=(unsigned char*)&s,*q=(unsigned char*)&t; unsigned i; for(i=0;i<sizeof s;i++) q[i]=p[i]; return t.a!=5; }
