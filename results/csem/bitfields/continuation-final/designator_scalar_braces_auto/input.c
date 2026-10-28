struct S {int a[2];int z;};
int main(void){struct S v={.a[0]={1},2,3}; return !(v.a[0]==1 && v.a[1]==2 && v.z==3);}
