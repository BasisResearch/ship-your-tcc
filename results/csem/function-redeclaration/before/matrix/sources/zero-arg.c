static int mine(void);static int mine(void){return 17;}static int mine(void);int main(void){int (*p)(void)=mine;return p==0 || p()!=17 || p!=mine;}
