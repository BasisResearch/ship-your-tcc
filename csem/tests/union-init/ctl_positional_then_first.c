union U { int a; char c[4]; };
int main(void) { union U u = { 9, .a = 2 }; return u.a; }   /* mixing positional and a FIRST-member designator: correct */
