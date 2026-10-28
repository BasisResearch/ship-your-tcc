struct A {int a[2];int z;}; struct B {struct A a[2];int z;}; struct C {struct B a[2];int z;};
static struct C v[2]={[0].a[0].a[0].a[0]=1,2,3,4,5,6,7,8,9,10};
int main(void){return !(v[0].a[0].a[0].a[0]==1 && v[0].a[0].a[0].a[1]==2 && v[0].a[0].a[0].z==3 && v[0].a[0].a[1].a[0]==4 && v[0].a[0].a[1].z==6 && v[0].a[0].z==7 && v[0].a[1].a[0].a[0]==8 && v[0].a[1].a[0].z==10);}
