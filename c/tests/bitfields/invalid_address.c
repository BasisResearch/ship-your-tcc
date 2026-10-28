struct S { unsigned a:3; }; int f(void) { struct S s; return &s.a != 0; }
int main(void) { return 0; }
