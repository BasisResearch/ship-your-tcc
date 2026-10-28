typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
Up x;
int main(void){return (_Alignof(__typeof__(x+x))!=16 || _Alignof(__typeof__(x?x:x))!=16 || _Alignof(__typeof__(x+1))!=4 || _Alignof(__typeof__(x==x))!=4);}
