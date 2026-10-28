typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
Up x;
int main(void){return (_Alignof(__typeof__(&x))!=sizeof(void*) || _Alignof(__typeof__((Up)1))!=4);}
