/* 6.10.2p4: macro-replaced #include. */
#define H <stddef.h>
#include H
#define Q "stddef.h"
#include Q
#define NAME stddef
#define CAT(a) <a.h>
#include CAT(NAME)
size_t s;
