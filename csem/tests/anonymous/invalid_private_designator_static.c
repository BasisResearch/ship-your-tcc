#include <stddef.h>
struct S {int reference_boundary_field_for_internal_container; struct {int a;};};
int main(void){static struct S s={.________________________________________________cerb_anon_2={1}}; return s.a;}
