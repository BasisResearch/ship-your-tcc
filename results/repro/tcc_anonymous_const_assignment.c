/* Invalid C11: assignment through a const anonymous container must diagnose. */
struct S { const struct { int a; }; };
int main(void) { struct S s = { .a = 1 }; s.a = 2; return s.a; }
