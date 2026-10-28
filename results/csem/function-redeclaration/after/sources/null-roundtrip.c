static int mine(int);
static int mine(int x){return x+10;}
static int mine(int);
int main(void){int (*p)(int)=0; if(p!=0)return 1;p=mine;if(p==0)return 2;p=0;return p!=0;}
