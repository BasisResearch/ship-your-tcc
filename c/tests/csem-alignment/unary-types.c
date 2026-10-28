typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
typedef short H __attribute__((aligned(8))); Up x; H h;
int main(void){return (_Alignof(__typeof__(+x))!=16 || _Alignof(__typeof__(-x))!=16 || _Alignof(__typeof__(~x))!=16 || _Alignof(__typeof__(!x))!=4 || _Alignof(__typeof__(+h))!=4);}
