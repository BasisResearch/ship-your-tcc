union U { int i; char c[4]; };
int main(void) { return (union U){ .c[2] = 8 }.c[2]*10; }   /* compound literal; gcc 80; 0 before */
