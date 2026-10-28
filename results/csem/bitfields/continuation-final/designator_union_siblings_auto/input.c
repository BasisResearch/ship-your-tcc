union U {int n;int a[2];}; struct S {union U a[2];int z;};
int main(void){struct S v={.a[0].a[0]=1,2,3,4}; return !(v.a[0].a[0]==1 && v.a[0].a[1]==2 && v.a[1].n==3 && v.z==4);}
