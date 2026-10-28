/* The independent evaluation-order ND must remain visible under every policy. */
int counter;
int next(void) { counter += 1; return counter; }
int pair(int x, int y) { return 10*x+y; }
int main(void) { return pair(next(), next()); }
