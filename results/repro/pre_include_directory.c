/* An existing directory is not a readable header. tcc currently treats EISDIR as EOF. */
#include "/lib"
int main(void) { return 0; }
