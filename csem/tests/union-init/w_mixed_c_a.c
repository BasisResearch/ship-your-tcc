union U { int a; char c[4]; };
int main(void) { union U u = { .c[2] = 1, .a = 42 }; return u.a; }   /* gcc 42; 42 before, right by luck */
