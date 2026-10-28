struct S { int x; union { struct { unsigned a:3; unsigned :2; int b:4; }; int z; }; int y; };
int main(void) { static struct S s={.a=6,.b=-3,.y=19}; return s.a!=6 || s.b != -3 || s.x!=0 || s.y!=19; }
