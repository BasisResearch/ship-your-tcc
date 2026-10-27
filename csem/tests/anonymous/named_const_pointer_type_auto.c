struct S {const struct {int a,b;} named; int z;};
int main(void) { struct S s={.named={1,2},3}; return !_Generic(&s.named.a,const int *:1,default:0); }
