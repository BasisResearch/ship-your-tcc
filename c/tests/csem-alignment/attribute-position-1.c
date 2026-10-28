typedef int __attribute__((aligned(16))) Up;
int main(void){return _Alignof(Up)!=16 || sizeof(Up)!=4;}
