static int mine(int);
static int mine(int x){return x+10;}
static int mine(int);
int main(void){int (*p)(int)=mine;int (*q)(int)=0;unsigned char *a=(unsigned char *)&p;unsigned char *b=(unsigned char *)&q;for(unsigned long i=0;i<sizeof p;i++)b[i]=a[i];return q!=mine || q(8)!=18;}
