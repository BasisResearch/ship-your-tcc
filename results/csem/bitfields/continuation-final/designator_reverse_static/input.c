struct S {int a[2];int z;};
static struct S v={.z=9,.a[0]=1,2,3};
int main(void){return !(v.a[0]==1 && v.a[1]==2 && v.z==3);}
