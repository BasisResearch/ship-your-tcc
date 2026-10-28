static int printf(int x){return x+10;}
int main(void){extern int printf(int);return printf(7)!=17;}
