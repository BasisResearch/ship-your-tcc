struct S { unsigned a:3; unsigned b:5; };
int main(void) { struct S s={1,2}; return s.a++ + s.a++; }
