union U { int i; char c[4]; };
int main(void) { union U u = { .c = "ab" }; return u.c[0] + u.c[2]*10; }   /* gcc 97; 0 before */
