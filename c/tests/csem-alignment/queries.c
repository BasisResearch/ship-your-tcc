typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
int main(void){return (_Alignof(Up)!=16 || __alignof__(Down)!=1 || __alignof(Again)!=16 || sizeof(Again)!=4);}
