/* GNU extensions tcc accepts. */
#define E1(fmt, ...) printf(fmt, ## __VA_ARGS__)
E1("a") E1("b", 1) E1("c", 1, 2)
#define E2(fmt, args...) printf(fmt, args)
E2("d", 3, 4)
#define E3(x, ...) f(x __VA_ARGS__)
E3(1) E3(1,) E3(1, 2)
int c1 = __COUNTER__, c2 = __COUNTER__;
#if __has_include(<stdio.h>) && !__has_include("nonexistent.h")
has_include
#endif
#ifdef __has_include
has_include_defined
#endif
#pragma once
#warning this is a warning
# 77 "marker.c"
int l = __LINE__;
