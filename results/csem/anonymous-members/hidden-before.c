#include <stddef.h>
struct S { int reference_boundary_field_for_internal_container; struct {int a;}; };
static struct S s={0,{1}};
int main(void){return s.________________________________________________cerb_anon_2.a;}
