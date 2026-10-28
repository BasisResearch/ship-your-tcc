struct S {int a[2]; int z;};
int main(void){struct S v[]={[1].a[0]=1,2,3,4,5,6}; return !(sizeof(v)/sizeof(v[0])==3 && v[0].z==0 && v[1].a[0]==1 && v[1].a[1]==2 && v[1].z==3 && v[2].a[0]==4 && v[2].a[1]==5 && v[2].z==6);}
