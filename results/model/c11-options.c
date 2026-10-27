/* Pragma option state must survive its temporary token/file stream. */
#define REVERSE _Pragma("comment(option, \"-freverse-funcargs -Wwrite-strings\")")
REVERSE
static int counter;
static int next_value(void) { return ++counter; }
static int subtract(int a, int b) { return a - b; }
int main(void) { return subtract(next_value(), next_value()) != 1; }
