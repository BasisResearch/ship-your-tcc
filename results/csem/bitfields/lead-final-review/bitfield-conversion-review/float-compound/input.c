struct S{unsigned a:3;};int main(void){struct S s={1};double d=2.75;int x=(s.a+=d);return x!=3 || s.a!=3;}
