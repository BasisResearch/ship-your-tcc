struct S {int a[3]; int z;};
int main(void){struct S v={.a=1,.z=4}; return !(v.a[0]==1 && v.a[1]==0 && v.a[2]==0 && v.z==4);}
