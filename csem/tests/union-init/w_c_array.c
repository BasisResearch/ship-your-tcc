union U { int i; unsigned char c[4]; };
int main(void) { union U u = { .c = {1,2,3,4} }; return u.c[0]*100 + u.c[3]; }   /* gcc 104; both engines 0 before */
