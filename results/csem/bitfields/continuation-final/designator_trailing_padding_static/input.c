struct A {int x;unsigned :3;}; struct S {struct A a;int y;};
static struct S v={.a.x=1,2};
int main(void){return !(v.a.x==1 && v.y==2);}
