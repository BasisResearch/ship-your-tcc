struct S {volatile struct {int a,b;}; int z;};
int main(void) { static struct S s={.a=1,2,3}; return !_Generic(&s.a,volatile int *:1,default:0); }
