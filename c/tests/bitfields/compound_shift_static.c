struct S { unsigned a:3; };
int main(void) { static struct S s={3}; s.a<<=1ull; return s.a!=6; }
