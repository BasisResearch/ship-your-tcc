/* The image's realpath stub is identity: /prog.c and /./prog.c are
   distinct #pragma once entries although lookup reaches the same bytes. */
#pragma once
#ifndef INCLUDED_ONCE_PATH
#define INCLUDED_ONCE_PATH 1
#include "./prog.c"
int main(void) { return included_once_path != 7; }
#else
enum { included_once_path = 7 };
#endif
