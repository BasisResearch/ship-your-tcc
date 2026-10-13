#include <stdio.h>
#include <stdlib.h>
/* malloc/free are CompCert builtins (EF_malloc/EF_free): both oracles run them.
   libc functions such as memcpy/strlen/memset are plain externals: stuck in both. */
struct node { int v; struct node *next; };
int main(void) {
  struct node *head = NULL;
  for (int i = 0; i < 10; i++) {
    struct node *n = malloc(sizeof *n);
    n->v = i * i; n->next = head; head = n;
  }
  int s = 0;
  while (head) { struct node *n = head; s += n->v; head = n->next; free(n); }
  printf("s=%d\n", s);
  return 0;
}
