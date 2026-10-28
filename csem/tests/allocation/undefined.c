/* Allocation choices do not reclassify independent C undefined behavior. */
int a[1];
int main(void) { return a[2]; }
