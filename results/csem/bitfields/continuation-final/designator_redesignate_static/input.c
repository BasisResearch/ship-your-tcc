struct S {int a[3]; int z;};
static struct S v={.a=1,.a[1]=2,3,.z=4};
int main(void){return !(v.a[0]==1 && v.a[1]==2 && v.a[2]==3 && v.z==4);}
