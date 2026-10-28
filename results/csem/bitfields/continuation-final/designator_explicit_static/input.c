struct S {int a[2][2]; int z;};
static struct S v={.a[0]={1}, {2,3},4};
int main(void){return !(v.a[0][0]==1 && v.a[0][1]==0 && v.a[1][0]==2 && v.a[1][1]==3 && v.z==4);}
