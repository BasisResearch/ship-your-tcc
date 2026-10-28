typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
typedef __attribute__((aligned(2))) Up Weak; typedef __attribute__((__aligned__(32))) Down Strong;
int main(void){return (_Alignof(Weak)!=2 || _Alignof(Strong)!=32);}
