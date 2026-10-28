int x=7; struct S {int *a[2]; int z;};
static struct S v={.a[0]=&x,&x,3};
int main(void){return !(*v.a[0]==7 && *v.a[1]==7 && v.z==3);}
