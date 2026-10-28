static int mine(int);
static int mine(int x){return x+10;}
static int mine(int);
int main(void){int (*p)(int)=mine; int (*q)(int)=&mine; return mine(7)!=17 || p(7)!=17 || (*q)(7)!=17 || p!=q || p==0;}
