struct A {int x;unsigned :3;}; struct S {struct A a;int y;};
int main(void){struct S v={.a.x=1,2}; return !(v.a.x==1 && v.y==2);}
