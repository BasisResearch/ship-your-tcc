struct S {union {struct {int a[2]; int b;}; int n;}; int z;};
static struct S v={.a[0]=1,2,3,4};
int main(void){return !(v.a[0]==1 && v.a[1]==2 && v.b==3 && v.z==4);}
