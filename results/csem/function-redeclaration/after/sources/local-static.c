static int mine(int);
static int mine(int x){return x+10;}
static int mine(int);
int main(void){static int (*p)(int)=mine;return p!=mine || p(2)!=12;}
