union U { long b; int a; };
int main(void) { union U u = { .a = 5 }; return u.a; }   /* gcc 5; 5 before, right only by representation overlap */
