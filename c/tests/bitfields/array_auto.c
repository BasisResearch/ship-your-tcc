struct S { unsigned a:3; unsigned :1; int b:4; };
int main(void) { struct S s[2]={{1,-2},{7,-8}}; return s[0].a!=1 || s[1].a!=7 || s[0].b != -2 || s[1].b != -8; }
