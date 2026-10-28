struct S{int a:3;};int main(void){struct S s={0};double d=-2.75;int x=(s.a=d);return x!= -2 || s.a!= -2;}
