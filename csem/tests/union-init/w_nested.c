union In { int i; char c[4]; };
union Out { long l; union In in; };
int main(void) { union Out o = { .in = { .c[1] = 5 } }; return o.in.c[1]*10 + o.in.c[0]; }   /* gcc 50; 0 before */
