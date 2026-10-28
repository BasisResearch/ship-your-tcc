struct S{_Bool a:1;};int main(void){struct S s={0};double d=0.5;int x=(s.a=d);return x!=1 || s.a!=1;}
