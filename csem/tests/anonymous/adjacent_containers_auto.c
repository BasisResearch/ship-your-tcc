struct S {struct {int a,b;}; struct {int c,d;};int z;};
int main(void) { struct S s={.b=2,3,4,5}; return s.a!=0||s.b!=2||s.c!=3||s.d!=4||s.z!=5; }
