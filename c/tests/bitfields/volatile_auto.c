struct S { unsigned a:3; unsigned b:5; };
int main(void) { volatile struct S s={7,19}; unsigned a=s.a++; s.b=2; return a!=7 || s.a!=0 || s.b!=2; }
