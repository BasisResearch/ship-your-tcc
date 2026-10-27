struct S {const struct {int a,b;}; int z;};
int main(void) { struct S s={.a=1,2,3}; return !_Generic(&s.a,const int *:1,default:0); }
