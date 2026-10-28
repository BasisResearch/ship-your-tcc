struct S {int a[2]; int z;};
static struct S s={.a[0]=1,2,3};
int main(void){return !(s.a[0]==1 && s.a[1]==2 && s.z==3);}
