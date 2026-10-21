/* 6.10.3.4: rescanning and hide sets. */
#define foo foo
foo
#define bar a bar b
bar
#define f(x) g(x
#define g(x) x)
f(1) 2)
#define AA BB
#define BB AA
AA BB
#define lparen (
#define id(x) x
id lparen 1)
#define apply(m, a) m a
apply(id, (2))
#define h() H
#define k h
k()
k ()
#define recur(x) x recur
recur(1)(2)
#define obj(x) obj
obj(1)(2)
#define NIL(x) x
#define G_0(arg) NIL(G_1)(arg)
#define G_1(arg) NIL(arg)
G_0(42)
