struct A {int x,y;}; struct S {struct A a[2];int z;};
int main(void){struct S v={.a[0].y=1,{2},3}; return !(v.a[0].x==0 && v.a[0].y==1 && v.a[1].x==2 && v.a[1].y==0 && v.z==3);}
