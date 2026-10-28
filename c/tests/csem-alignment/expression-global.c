int obj __attribute__((aligned(32)));
int main(void){return (__alignof__(obj)!=32 || __alignof__(&obj)!=sizeof(void*) || __alignof__(+obj)!=4 || __alignof__((obj,obj))!=4 || _Alignof(__typeof__(obj))!=4);}
