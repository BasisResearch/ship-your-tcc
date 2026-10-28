union S { unsigned a:3; int b; unsigned c:7; };
int main(void) { static union S s={.c=97}; return s.c!=97; }
