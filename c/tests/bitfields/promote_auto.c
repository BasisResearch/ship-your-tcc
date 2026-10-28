struct S { unsigned a:31; unsigned b:32; };
int main(void) { struct S s={1,1}; return +(s.a)-2 >= 0 || +(s.b)-2 != 4294967295u; }
