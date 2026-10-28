struct S { unsigned a:3; };
int main(void) { struct S s[2]={{1},{6}}; int i=0; s[i++].a+=2; return i!=1 || s[0].a!=3 || s[1].a!=6; }
