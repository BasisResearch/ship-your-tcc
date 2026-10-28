struct S { int x; unsigned :3; };
int main(void) { struct S s={19}; return s.x!=19 || sizeof s!=8; }
