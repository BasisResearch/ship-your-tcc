struct S {char a[2][4]; int z;};
static struct S v={.a[0]="ab","cd",3};
int main(void){return !(v.a[0][0]=='a' && v.a[0][2]==0 && v.a[1][0]=='c' && v.a[1][2]==0 && v.z==3);}
