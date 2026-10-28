int main(void){struct S{unsigned int x:3;}s={0}; return ((s.x ? s.x : -1)<0 ? 0:1);}
