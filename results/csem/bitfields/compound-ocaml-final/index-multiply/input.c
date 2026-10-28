int main(void){int a[2]={2,3};int i=0;a[i++] *=4;return i!=1 || a[0]!=8 || a[1]!=3;}
