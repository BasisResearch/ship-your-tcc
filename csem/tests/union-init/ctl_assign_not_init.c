union U { int i; unsigned char c[4]; };
int main(void) { union U u; u.c[1] = 7; u.c[0] = 0; return u.c[1]*10 + u.c[0]; }
