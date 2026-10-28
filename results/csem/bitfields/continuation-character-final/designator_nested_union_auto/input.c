struct S {union {struct {int a; int b[3]; int c;} s; int n;} u; int z;};
int main(void){struct S v={.u.s.b[1]=2,3,4,5}; return !(v.u.s.a==0 && v.u.s.b[0]==0 && v.u.s.b[1]==2 && v.u.s.b[2]==3 && v.u.s.c==4 && v.z==5);}
