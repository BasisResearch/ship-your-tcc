union S { struct {int a,b;}; int n; };
int main(void) { union S s={.a=1,2}; return s.a!=1||s.b!=2; }
