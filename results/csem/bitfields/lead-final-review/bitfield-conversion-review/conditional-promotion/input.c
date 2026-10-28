struct S{unsigned a:3;};int main(void){struct S s={1};return (1?s.a:s.a)-2 != -1;}
