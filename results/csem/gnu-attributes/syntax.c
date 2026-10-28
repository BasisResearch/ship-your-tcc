struct __uint128__ { char x[16]; } __attribute((__aligned__(16)));
__attribute__((unused)) typedef int T __attribute__((aligned(16)));
struct __attribute__((packed)) S {char c; int a __attribute__((aligned(8)));};
int f(void) __attribute__((__noreturn__,unused));
int main(void){__attribute__((unused)) int x=1; return x;}
