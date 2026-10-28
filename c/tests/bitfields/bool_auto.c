struct S { _Bool a:1; _Bool b:1; };
int main(void) { struct S s={0,0}; s.a=42; return s.a!=1 || s.b!=0; }
