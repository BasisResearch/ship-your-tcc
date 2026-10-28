static int printf(int);
static int printf(int x){return x+10;}
static int printf(int);
int main(void){int (*f)(int)=printf;return f(7)!=17;}
