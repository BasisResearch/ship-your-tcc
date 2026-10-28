struct S { int a:3; };
int main(void) { struct S s={-1}; return s.a>=0; }
