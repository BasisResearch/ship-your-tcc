struct S { unsigned :0; unsigned a:4; };
int main(void) { struct S s={9}; return s.a!=9 || sizeof s!=4; }
