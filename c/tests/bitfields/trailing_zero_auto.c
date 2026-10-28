struct S { char c; unsigned :0; };
int main(void) { struct S s={3}; return s.c!=3 || sizeof s!=4 || _Alignof(struct S)!=1; }
