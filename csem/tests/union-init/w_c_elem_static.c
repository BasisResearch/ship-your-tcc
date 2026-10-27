union U { int i; unsigned char c[4]; };
union U u = { .c[1] = 7 };   /* static storage */
int main(void) { return u.c[1]*10 + u.c[0]; }   /* gcc 70 */
