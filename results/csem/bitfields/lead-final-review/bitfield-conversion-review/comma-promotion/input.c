struct S{unsigned a:3;};int main(void){struct S s={1};return +(0,s.a)-2 != -1;}
