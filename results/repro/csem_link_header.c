/* The pinned Cerberus unistd.h comments out link despite its Core proxy. */
#include <unistd.h>
int main(void) { return link("/a", "/b") != -1; }
