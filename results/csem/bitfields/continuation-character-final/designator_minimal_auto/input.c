struct S {int a[2]; int z;};
int main(void){struct S s={.a[0]=1,2,3}; return !(s.a[0]==1 && s.a[1]==2 && s.z==3);}
