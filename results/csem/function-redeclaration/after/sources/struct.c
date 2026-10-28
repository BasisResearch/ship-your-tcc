static int mine(int);
static int mine(int x){return x+10;}
static int mine(int);
int main(void){struct pair{int (*p)(int);};struct pair a={mine};struct pair b=a;return a.p!=b.p || b.p(4)!=14;}
