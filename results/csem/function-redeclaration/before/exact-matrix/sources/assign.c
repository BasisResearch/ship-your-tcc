static int mine(int);
static int mine(int x){return x+10;}
static int mine(int);
int main(void){int (*p)(int);p=mine;int (*q)(int)=p;return q!=mine || q(5)!=15;}
