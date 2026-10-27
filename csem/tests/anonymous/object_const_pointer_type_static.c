struct S {int a;};
int main(void) { static const struct S s={1}; return !_Generic(&s.a,const int *:1,default:0); }
