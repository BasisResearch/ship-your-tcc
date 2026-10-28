typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
typedef Again Third; typedef Third Fourth;
int main(void){return (_Alignof(Fourth)!=16 || sizeof(Fourth)!=4);}
