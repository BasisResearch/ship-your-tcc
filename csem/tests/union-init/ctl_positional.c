union U { int a; char c[4]; };
int main(void) { union U u = { 9 }; return u.a; }
