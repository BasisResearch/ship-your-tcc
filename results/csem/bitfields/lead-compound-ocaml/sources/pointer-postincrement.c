int main(void){int a[2]={1,2};int *p=a;*p++ +=3;return p!=a+1 || a[0]!=4 || a[1]!=2;}
