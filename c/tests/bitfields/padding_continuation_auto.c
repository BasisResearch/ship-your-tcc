struct S { struct { int x; unsigned :3; } a; int b; };
int main(void) { struct S s={1,2}; return s.a.x!=1 || s.b!=2; }
