/* Assorted. */
#define EMPTY
EMPTY # define NOT_A_DIRECTIVE 1
NOT_A_DIRECTIVE
#
# /* null directive with a comment */
#define OBJ(x) (x)
#define OBJ2 (x)
OBJ(1) OBJ2
#define f(a, b) a + b
f(
  1
  ,
  2
)
f((1, 2), (3))
#define g(x) x
g(g(g(1)))
#undef f extra
#define TWO 1 ## 2 ## 3
TWO
#define PP + ## +
PP
#define ARR - ## >
ARR
int $dollar = 1;
#define str(x) #x
str(a  /* c */  b)
