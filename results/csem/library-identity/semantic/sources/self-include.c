#ifndef IN_SELF
#define IN_SELF
#include __FILE__
int main(void){return self_value!=37;}
#else
enum {self_value=37};
#endif
