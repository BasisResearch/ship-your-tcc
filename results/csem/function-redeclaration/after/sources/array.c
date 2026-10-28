static int mine(int);
static int mine(int x){return x+10;}
static int mine(int);
int main(void){int (*p[2])(int)={mine,mine};return p[0]!=p[1] || p[1](3)!=13;}
