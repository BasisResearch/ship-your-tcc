int x,n; int *f(void){++n;return &x;} int main(void){*f()+=1;return n;}
