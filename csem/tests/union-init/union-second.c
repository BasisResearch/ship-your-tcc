union U { int a; int b; };
int main(void) { union U u = { .b = 7 }; return u.b; }
