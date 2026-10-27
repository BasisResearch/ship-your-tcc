union V { unsigned char c[4]; int i; };
int main(void) { union V v = { .c = {1,2,3,4} }; return v.c[0]*100 + v.c[3]; }   /* .c IS the first member */
