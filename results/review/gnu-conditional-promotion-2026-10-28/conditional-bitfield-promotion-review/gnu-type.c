int main(void){struct S{unsigned int x:3;}s={0}; return _Generic((s.x ?: 0),int:0,default:1);}
