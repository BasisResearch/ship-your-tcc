struct S { int x; unsigned :3; };
int main(void) { struct S s = {1}; return s.x - 1; }
