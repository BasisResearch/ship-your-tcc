static int printf(int);
static int printf(int x){return x+10;}
static int printf(int);
int main(void){int (*p)(int)=printf;return p(7)!=17 || p!=printf;}
