union U {char a[4];int n;}; struct S {union U u;int z;};
static struct S v={.u.a="abc",.u.n=7,.u.a="xy",3};
int main(void){return !(v.u.a[0]=='x' && v.u.a[1]=='y' && v.u.a[2]==0 && v.z==3);}
