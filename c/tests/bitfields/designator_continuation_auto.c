struct S { unsigned a:3; unsigned :4; unsigned b:5; int c; };
int main(void) { struct S s={.a=5,17,39}; return s.a!=5 || s.b!=17 || s.c!=39; }
