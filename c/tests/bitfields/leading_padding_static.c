struct S { unsigned :3; unsigned a:5; };
int main(void) { static struct S s={23}; return s.a!=23; }
