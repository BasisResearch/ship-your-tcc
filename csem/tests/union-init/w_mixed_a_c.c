union U { int a; char c[4]; };
int main(void) { union U u = { .a = 42, .c[2] = 1 }; return u.c[0]*100 + u.c[1]*10 + u.c[2]; }   /* gcc 1; 4200 before */
