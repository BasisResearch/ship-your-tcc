int a=1;int n;int *get(void){n++;return &a;}int main(void){*get() +=3;return n!=1 || a!=4;}
