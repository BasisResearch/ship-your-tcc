struct S { unsigned a:3; };
int main(void) { struct S s={1}; int x=+(s.a++)-2; int y=+(s.a=1)-2; int z=+(++s.a)-3; return x != -1 || y != -1 || z != -1; }
