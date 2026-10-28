static int close(int);
int main(void){return close(7)!=23;}
static int close(int x){return x+16;}
