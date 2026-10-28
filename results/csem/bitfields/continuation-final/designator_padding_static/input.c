struct S {unsigned :3; unsigned a:4; unsigned :2; unsigned b:4; int z;}; struct T {struct S s; int z;};
static struct T v={.s.a=1,2,3,4};
int main(void){return !(v.s.a==1 && v.s.b==2 && v.s.z==3 && v.z==4);}
