union U { int i; unsigned char c[4]; };
int main(void) { union U u = { .c[3] = 6 }; return u.c[3]*10 + u.c[0]; }   /* automatic storage; gcc 60 */
