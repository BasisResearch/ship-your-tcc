struct S {char c; int i;}; struct S obj __attribute__((aligned(32)));
int main(void){return (__alignof__(obj)!=32 || __alignof__(obj.c)!=1 || __alignof__(obj.i)!=4 || __alignof__(&obj.c)!=sizeof(void*));}
