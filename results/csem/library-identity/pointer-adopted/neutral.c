static int mine(int);
static int mine(int x){return x+10;}
static int mine(int);
int main(void){int (*f)(int)=mine;return f(7)!=17;}
