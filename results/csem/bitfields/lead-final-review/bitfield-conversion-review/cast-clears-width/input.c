struct S{unsigned a:3;};int main(void){struct S s={1};unsigned x=(unsigned)s.a;return x-2 !=4294967295u;}
