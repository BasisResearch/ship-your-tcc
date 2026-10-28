struct S { unsigned a:3; };
int main(void) { struct S s={1}; return sizeof(s.a++); }
