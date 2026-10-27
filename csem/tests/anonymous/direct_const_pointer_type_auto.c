struct S {const int a; int z;};
int main(void) { struct S s={1,3}; return !_Generic(&s.a,const int *:1,default:0); }
