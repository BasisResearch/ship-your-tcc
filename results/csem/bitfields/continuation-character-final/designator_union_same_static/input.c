union U {struct {int a[3]; int b;} s; int n;};
static union U v={.s.a[0]=1,2,.s.a[2]=3,4};
int main(void){return !(v.s.a[0]==1 && v.s.a[1]==2 && v.s.a[2]==3 && v.s.b==4);}
