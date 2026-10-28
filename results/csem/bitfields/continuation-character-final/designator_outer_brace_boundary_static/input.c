struct A {int x,y;}; struct S {struct A a;int z;};
static struct S v[2]={{.a.x=1},2,3,4};
int main(void){return !(v[0].a.x==1 && v[0].a.y==0 && v[0].z==0 && v[1].a.x==2 && v[1].a.y==3 && v[1].z==4);}
