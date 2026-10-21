/* F3 model test: a compile error inside an included header context */
#include <stdio.h>
#define BAD(x) x +
struct s { int a; };
int main(void)
{
    struct s v;
    v.b = BAD(1) 2;
    return 0;
}
