union U { int a; char c[4]; };
int main(void) { union U u = { 9, .c[1] = 3 }; return u.c[0]*10 + u.c[1]; }   /* gcc 3; 90 before */
