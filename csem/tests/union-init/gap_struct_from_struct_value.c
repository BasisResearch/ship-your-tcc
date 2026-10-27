struct T { int t; long l; };
struct S { struct T t; };
int main(void) { struct T x = { 1, 2 }; struct S s = { .t = x, .t.l = 5 }; return (int)s.t.l; }   /* gcc 5 */
