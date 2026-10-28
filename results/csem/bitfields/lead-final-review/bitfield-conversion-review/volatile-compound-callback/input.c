struct S{volatile unsigned a:3;};struct S s={1};int n;struct S *f(void){++n;return &s;}int main(void){f()->a+=2;return n!=1 || s.a!=3;}
