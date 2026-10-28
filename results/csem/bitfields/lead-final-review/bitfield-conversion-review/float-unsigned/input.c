struct S{unsigned a:3;};int main(void){struct S s={0};double d=3.75;int x=(s.a=d);return x!=3 || s.a!=3;}
