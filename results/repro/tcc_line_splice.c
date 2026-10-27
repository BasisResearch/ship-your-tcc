/* __LINE__ names the token's physical source line, before lookahead. */
#line 300
static int line = __LINE__\
;
int main(void) { return line != 300; }
