/* Host-libc formatting of ONE printf conversion, for the Lean Clight runner.
 *
 * The Lean side parses the format string, extracts each conversion spec
 * (flags, width, precision, length modifier normalised by the caller) and the
 * Clight argument value, and calls one of these to render it with the host's
 * snprintf (glibc).  ccomp -interp renders floats the same way (OCaml's
 * caml_format_float is also a host snprintf), so the two oracles agree on
 * float formatting by construction.  Specs and strings are passed as
 * NUL-terminated ByteArrays (C bytes need not be valid UTF-8). */
#include <lean/lean.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static lean_obj_res mk_bytes(const char *buf, int n) {
  if (n < 0) n = 0;
  lean_object *arr = lean_alloc_sarray(1, n, n);
  memcpy(lean_sarray_cptr(arr), buf, n);
  return arr;
}

#define FMT_BODY(...)                                                     \
  const char *spec = (const char *)lean_sarray_cptr(specArr);             \
  int n = snprintf(NULL, 0, spec, __VA_ARGS__);                           \
  if (n < 0) return mk_bytes("", 0);                                      \
  char *buf = malloc((size_t)n + 1);                                      \
  snprintf(buf, (size_t)n + 1, spec, __VA_ARGS__);                        \
  lean_obj_res r = mk_bytes(buf, n);                                      \
  free(buf);                                                              \
  return r;

/* spec must end in "lld"/"lli" (signed) */
LEAN_EXPORT lean_obj_res clightrun_fmt_sll(b_lean_obj_arg specArr, uint64_t v) {
  FMT_BODY((long long)(int64_t)v)
}
/* spec must end in "llu"/"llx"/"llX"/"llo" (unsigned) */
LEAN_EXPORT lean_obj_res clightrun_fmt_ull(b_lean_obj_arg specArr, uint64_t v) {
  FMT_BODY((unsigned long long)v)
}
/* spec must end in "c" */
LEAN_EXPORT lean_obj_res clightrun_fmt_chr(b_lean_obj_arg specArr, uint32_t v) {
  FMT_BODY((int)v)
}
/* spec must end in one of f F e E g G a A */
LEAN_EXPORT lean_obj_res clightrun_fmt_dbl(b_lean_obj_arg specArr, double v) {
  FMT_BODY(v)
}
/* spec must end in "s"; sArr is NUL-terminated */
LEAN_EXPORT lean_obj_res clightrun_fmt_str(b_lean_obj_arg specArr, b_lean_obj_arg sArr) {
  FMT_BODY((const char *)lean_sarray_cptr(sArr))
}
