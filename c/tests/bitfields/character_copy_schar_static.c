struct S { signed a:3; };
int main(void) { static struct S s={-3},t={0}; signed char *p=(signed char *)&s,*q=(signed char *)&t; unsigned i; for(i=0;i<sizeof s;++i) q[i]=p[i]; return t.a != -3; }
