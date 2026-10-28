static int mine(int);
static int mine(int x){return x+10;}
static int mine(int);
int main(void){int (*p)(int)=mine;{extern int mine(int);int (*q)(int)=mine;{extern int mine(int);if(q!=mine || p!=mine)return 1;}if(q(6)!=16)return 2;}return p(7)!=17;}
