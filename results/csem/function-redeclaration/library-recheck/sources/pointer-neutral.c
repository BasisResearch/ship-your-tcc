static int mine(int x){return x+10;}
int main(void){int (*f)(int)=mine;return f(7)!=17;}
