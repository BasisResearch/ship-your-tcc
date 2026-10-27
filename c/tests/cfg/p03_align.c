/* cfgTcc probe: lp64 alignments and struct layout. */
#include <stddef.h>
#include <stdio.h>

struct s1 { char c; short s; };
struct s2 { char c; int i; };
struct s3 { char c; long l; };
struct s4 { char c; double d; };
struct s5 { char c; long long ll; char t; };
struct s6 { char c; void *p; };
struct s7 { short s; char c[3]; };
struct s8 { char c; float f; };

int main(void) {
    printf("align short=%d int=%d long=%d llong=%d ptr=%d float=%d double=%d\n",
           (int)_Alignof(short), (int)_Alignof(int), (int)_Alignof(long),
           (int)_Alignof(long long), (int)_Alignof(void *), (int)_Alignof(float),
           (int)_Alignof(double));
    printf("s1=%d/%d s2=%d/%d s3=%d/%d s4=%d/%d\n",
           (int)sizeof(struct s1), (int)offsetof(struct s1, s),
           (int)sizeof(struct s2), (int)offsetof(struct s2, i),
           (int)sizeof(struct s3), (int)offsetof(struct s3, l),
           (int)sizeof(struct s4), (int)offsetof(struct s4, d));
    printf("s5=%d/%d s6=%d/%d s7=%d/%d s8=%d/%d\n",
           (int)sizeof(struct s5), (int)offsetof(struct s5, t),
           (int)sizeof(struct s6), (int)offsetof(struct s6, p),
           (int)sizeof(struct s7), (int)_Alignof(struct s7),
           (int)sizeof(struct s8), (int)offsetof(struct s8, f));
    return 0;
}
