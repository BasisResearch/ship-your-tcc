typedef int Up __attribute__((aligned(16)));
typedef int Down __attribute__((aligned(1)));
typedef Up Again;
int main(void){return (({int obj __attribute__((aligned(32))); __alignof__(obj)!=32 || __alignof__(&obj)!=sizeof(void*) || __alignof__((int)obj)!=4 || _Alignof(__typeof__(obj))!=4;}));}
