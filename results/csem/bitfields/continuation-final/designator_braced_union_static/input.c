union U {int n; int a[2];}; struct S {union U u; int z;};
static struct S v={.u.a={1,2},3};
int main(void){return !(v.u.a[0]==1 && v.u.a[1]==2 && v.z==3);}
