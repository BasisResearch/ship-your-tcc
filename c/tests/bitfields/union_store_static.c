union S { unsigned a:3; unsigned char b; };
int main(void) { static union S s={.b=255}; s.a=2; return s.a!=2 || s.b!=250; }
