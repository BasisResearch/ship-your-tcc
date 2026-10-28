union U {struct {int a[2]; int b;} s; int n;}; struct S {union U u; int z;};
int main(void){struct S v={.u.s.a[0]=9,8,.u.n=7,.u.s.a[0]=1,2,3,4}; return !(v.u.s.a[0]==1 && v.u.s.a[1]==2 && v.u.s.b==3 && v.z==4);}
