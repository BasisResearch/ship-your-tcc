static int mine(int);
static int mine(int x){return x+10;}
static int mine(int);
static int other(int);static int other(int x){return x+20;}static int other(int);int main(void){int (*p)(int)=mine;int (*q)(int)=other;return p==q || p(1)!=11 || q(1)!=21 || p!=mine || q!=other;}
