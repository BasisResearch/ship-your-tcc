
int main(void) { int s[2]={1,6}; int i=0; s[i++]+=2; return i!=1 || s[0]!=3 || s[1]!=6; }
