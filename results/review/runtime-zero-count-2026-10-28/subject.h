#include <stddef.h>
#define strncpy subject_strncpy
#define strncmp subject_strncmp
char *strncpy (char * restrict s1, const char * restrict s2, size_t n)
{
  char *res = s1;
  while ((*s1++ = *s2++) && --n > 0)
    ;
  while (n && --n > 0)
    *s1++ = '\0';
  return (res);
}

int strncmp(const char *s1, const char *s2, size_t n)
{
  while ((*s1 && *s1 == *s2) && --n > 0)
    s1++, s2++;
  return (*s1 - *s2);
}
#undef strncpy
#undef strncmp
