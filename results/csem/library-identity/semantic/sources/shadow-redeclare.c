static int printf(int);
static int printf(int x){return x+10;}
static int printf(int);
int main(void){return printf(7)!=17;}
