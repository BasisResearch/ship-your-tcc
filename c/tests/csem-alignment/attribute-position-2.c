typedef int Up __attribute__((aligned(16)));
int main(void){return _Alignof(Up)!=16 || sizeof(Up)!=4;}
