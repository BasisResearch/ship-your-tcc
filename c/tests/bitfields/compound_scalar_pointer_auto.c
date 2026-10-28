
int main(void) { int s[2]={1,6}; int *p=s; *p++ *=3; return p!=s+1 || s[0]!=3 || s[1]!=6; }
