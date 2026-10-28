struct S {char a[4]; int z;};
int main(void){struct S v={.a[1]='x','y',0,3}; return !(v.a[0]==0 && v.a[1]=='x' && v.a[2]=='y' && v.a[3]==0 && v.z==3);}
