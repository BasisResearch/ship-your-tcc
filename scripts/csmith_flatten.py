#!/usr/bin/env python3
"""Flatten a Csmith program into ONE self-contained C file.

    csmith_flatten.py CSMITH_INCLUDE_DIR raw.c out.c [--header "comment"] [--includes]

By default the output includes NO header (Csmith's own STANDALONE mode):
it declares `int printf(const char *, ...);` and typedefs the <stdint.h>
exact-width types for LP64 itself. Every program is then compiled by tcc
inside the Sail emulator without lexing ~3000 lines of newlib's stdio.h,
and ccomp/clightgen see no system header at all. With --includes it uses
<stdio.h> and <stdint.h> instead. In both cases it inlines a minimal copy
of Csmith's runtime in place of `#include "csmith.h"`:

* the CRC-32 checksum code and platform_main_begin/end of csmith.h /
  platform_generic.h (no <math.h>, <float.h>, <limits.h>, <assert.h>);
* only the safe_math.h wrappers the program actually calls, preprocessed
  for LP64 (`#if (INTn_MAX>=INT_MAX)` resolved, FUNC_NAME/STATIC/LOG_*/
  UNDEFINED expanded, INTn_MIN/MAX etc. replaced by literals with the LP64
  suffixes: int64_t and uint64_t are long / unsigned long on both x86_64
  and riscv64 lp64).

The CRC output is "checksum = %X\\n" exactly as Csmith's platform_generic.h
prints it, followed by main's `return 0`.
"""
import re
import subprocess
import sys
import tempfile
import os

LP64_LIMITS = {
    'INT_MAX': '2147483647', 'INT_MIN': '(-2147483647-1)',
    'INT8_MIN': '(-128)', 'INT8_MAX': '127', 'UINT8_MAX': '255',
    'INT16_MIN': '(-32767-1)', 'INT16_MAX': '32767', 'UINT16_MAX': '65535',
    'INT32_MIN': '(-2147483647-1)', 'INT32_MAX': '2147483647',
    'UINT32_MAX': '4294967295U',
    'INT64_MIN': '(-9223372036854775807L-1)', 'INT64_MAX': '9223372036854775807L',
    'UINT64_MAX': '18446744073709551615UL',
    'CHAR_BIT': '8',
}

PRELUDE_INCLUDES = '#include <stdio.h>\n#include <stdint.h>\n'

PRELUDE_STANDALONE = r'''
/* standalone LP64 prelude (no system headers): the same types as
   <stdint.h> on x86_64 glibc and riscv64 newlib */
int printf(const char *, ...);
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
'''

RUNTIME = r'''
/* ---- Csmith runtime (flattened from csmith.h / platform_generic.h) ---- */
static uint32_t crc32_tab[256];
static uint32_t crc32_context = 0xFFFFFFFFUL;

static void crc32_gentab(void) {
  uint32_t crc;
  const uint32_t poly = 0xEDB88320UL;
  int i, j;
  for (i = 0; i < 256; i++) {
    crc = i;
    for (j = 8; j > 0; j--) {
      if (crc & 1) {
        crc = (crc >> 1) ^ poly;
      } else {
        crc >>= 1;
      }
    }
    crc32_tab[i] = crc;
  }
}

static void crc32_byte(uint8_t b) {
  crc32_context = ((crc32_context >> 8) & 0x00FFFFFF) ^
                  crc32_tab[(crc32_context ^ b) & 0xFF];
}

static void crc32_8bytes(uint64_t val) {
  crc32_byte((val >> 0) & 0xff);
  crc32_byte((val >> 8) & 0xff);
  crc32_byte((val >> 16) & 0xff);
  crc32_byte((val >> 24) & 0xff);
  crc32_byte((val >> 32) & 0xff);
  crc32_byte((val >> 40) & 0xff);
  crc32_byte((val >> 48) & 0xff);
  crc32_byte((val >> 56) & 0xff);
}

static void transparent_crc(uint64_t val, char *vname, int flag) {
  crc32_8bytes(val);
  if (flag) {
    printf("...checksum after hashing %s : %lX\n", vname,
           crc32_context ^ 0xFFFFFFFFUL);
  }
}
'''

RUNTIME_BYTES = r'''
static void transparent_crc_bytes(char *ptr, int nbytes, char *vname,
                                  int flag) {
  int i;
  for (i = 0; i < nbytes; i++) {
    crc32_byte(ptr[i]);
  }
  if (flag) {
    printf("...checksum after hashing %s : %lX\n", vname,
           crc32_context ^ 0xFFFFFFFFUL);
  }
}
'''

RUNTIME_MAIN = r'''
static void platform_main_begin(void) { }

static void platform_main_end(uint32_t crc, int flag) {
  (void)flag;
  printf("checksum = %X\n", crc);
}
/* ---- end of Csmith runtime ---- */
'''


def safe_math_functions(incdir):
    """name -> preprocessed definition text, for every safe_math.h wrapper."""
    defs = ''.join(f'#define {k} {v}\n' for k, v in LP64_LIMITS.items())
    wrapper = (defs + '#define STATIC static\n#define FUNC_NAME(x) safe_##x\n'
               '#define LOG_INDEX\n#define LOG_EXEC\n#define UNDEFINED(v) (v)\n'
               f'#include "{os.path.join(incdir, "safe_math.h")}"\n')
    with tempfile.NamedTemporaryFile('w', suffix='.c', delete=False) as f:
        f.write(wrapper)
        path = f.name
    try:
        out = subprocess.run(['gcc', '-E', '-P', '-nostdinc', '-undef', path],
                             check=True, capture_output=True, text=True).stdout
    finally:
        os.unlink(path)
    funcs = {}
    for m in re.finditer(r'static\s+[\w ]+?\s+(safe_\w+)\s*\(', out):
        name = m.group(1)
        i = out.index('{', m.end())
        depth = 0
        for j in range(i, len(out)):
            if out[j] == '{':
                depth += 1
            elif out[j] == '}':
                depth -= 1
                if depth == 0:
                    break
        body = out[m.start():j + 1]
        body = re.sub(r'\n\s*\n+', '\n', body)
        funcs[name] = body
    return funcs


def main():
    incdir, raw, outp = sys.argv[1:4]
    header = ''
    rest = sys.argv[4:]
    includes = '--includes' in rest
    if '--header' in rest:
        header = rest[rest.index('--header') + 1]
    src = open(raw).read()
    if '#include "csmith.h"' not in src:
        sys.exit('no #include "csmith.h" in ' + raw)
    funcs = safe_math_functions(incdir)
    body = src.replace('#include "csmith.h"', '')
    used = sorted(set(re.findall(r'\bsafe_\w+', body)) & set(funcs))
    unknown = set(re.findall(r'\bsafe_\w+', body)) - set(funcs)
    if unknown:
        sys.exit(f'unknown safe_math wrappers: {sorted(unknown)}')
    # drop Csmith's leading comment block but keep the Options line
    opts = re.search(r'^// Options:.*$', src, re.M)
    body = re.sub(r'\A(\s*(/\*.*?\*/|//[^\n]*)\s*)*', '', body, flags=re.S)
    body = re.sub(r'\n{3,}', '\n\n', body)
    parts = []
    if header:
        parts.append('/* ' + header + ' */\n')
    if opts:  # drop "-o <path>": the work directory is not part of the program
        parts.append(re.sub(r'\s+-o\s+\S+', '', opts.group(0)).rstrip() + '\n')
    parts.append(PRELUDE_INCLUDES if includes else PRELUDE_STANDALONE)
    parts.append(RUNTIME)
    if 'transparent_crc_bytes' in body:
        parts.append(RUNTIME_BYTES)
    parts.append(RUNTIME_MAIN)
    if used:
        parts.append('/* ---- safe_math.h wrappers used by this program ---- */\n')
        for n in used:
            parts.append(funcs[n] + '\n')
    parts.append('/* ---- generated program ---- */\n')
    parts.append(body.strip() + '\n')
    open(outp, 'w').write(''.join(parts))


if __name__ == '__main__':
    main()
