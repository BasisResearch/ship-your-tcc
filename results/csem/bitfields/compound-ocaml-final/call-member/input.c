struct S{int x;};struct S s={1};int n;struct S *get(void){n++;return &s;}int main(void){get()->x +=3;return n!=1 || s.x!=4;}
