struct S { unsigned a:3; }; int f(void) { struct S s; return sizeof(s.a); }
int main(void) { return 0; }
