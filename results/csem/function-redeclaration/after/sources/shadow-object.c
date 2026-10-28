static int mine(int);
static int mine(int x){return x+10;}
static int mine(int);
int main(void){int (*p)(int)=mine;{int mine=29;if(mine!=29)return 1;}return p!=mine || p(7)!=17;}
