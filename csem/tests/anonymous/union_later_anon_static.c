union S { int n; struct {int a,b;}; };
int main(void) { static union S s={.a=1,2}; return s.a!=1||s.b!=2; }
