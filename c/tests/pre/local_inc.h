/* included by local_include.c */
#pragma once
const char *inc_file = __FILE__;
int inc_line = __LINE__;
#ifndef LOCAL_COUNT
#define LOCAL_COUNT 1
#else
#undef LOCAL_COUNT
#define LOCAL_COUNT 2
#endif
