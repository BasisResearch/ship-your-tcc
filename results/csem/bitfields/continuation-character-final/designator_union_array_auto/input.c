union U {struct {int a[2]; int b;} s; int n;};
int main(void){union U v[2]={[0].s.a[1]=1,2,3,4,5}; return !(v[0].s.a[0]==0 && v[0].s.a[1]==1 && v[0].s.b==2 && v[1].s.a[0]==3 && v[1].s.a[1]==4 && v[1].s.b==5);}
