static int mine(int);
static int mine(int x){return x+10;}
static int mine(int);
int (*saved)(int)=mine;int main(void){int (*q)(int)=saved;return q!=mine || q(3)!=13;}
