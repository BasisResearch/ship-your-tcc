struct S {int a[2][2]; int z;};
int main(void){struct S v={.a[0]={1}, {2,3},4}; return !(v.a[0][0]==1 && v.a[0][1]==0 && v.a[1][0]==2 && v.a[1][1]==3 && v.z==4);}
