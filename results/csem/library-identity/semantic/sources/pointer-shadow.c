static int printf(int x){return x+10;}
int main(void){int (*f)(int)=printf;return f(7)!=17;}
