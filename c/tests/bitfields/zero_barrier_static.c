struct S { unsigned a:1; unsigned :0; unsigned b:1; };
int main(void) { static struct S s={1,1}; return s.a!=1 || s.b!=1 || sizeof s!=8; }
