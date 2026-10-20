#!/usr/bin/env python3
"""gen_pre_cfg.py: generate tcc's preprocessing configuration for CSem.Pre.

    scripts/gen_pre_cfg.py [--check]

Writes (generated files; never edit them by hand, CLAUDE.md Law 7):
  Spec/Pre/Tcc/Cfg.lean      cfgTcc  (the image's `tcc -run /prog.c`) and
                             cfgTccE (the cross tcc's `tcc -E`, used by the
                             differential scripts/pre_diff.py)
  Spec/Pre/Tcc/ImageFS.lean  imageFS: the image's header tree (c/build/fs.list)

Inputs:
  build/tcc-src      vendor/tinycc + patches/ (scripts/tcc_src.sh)
  c/src/tcc-config/config.h   the image's tcc configuration
  c/build/fs.list    the image's file system (make -C c build/fs.list)
  build/cross/riscv64-tcc     the cross tcc (scripts/build_cross_tcc.sh),
                     for the cross-check below

How the predefined text is obtained. tcc builds it in tccpp.c `tcc_predefs`
(the `<command line>` buffer): `#define __TINYC__ 9xx`, then `putdefs` of
`target_machine_defs` (riscv64-gen.c), `target_os_defs` (tccpp.c) and, under
TCC_BAREMETAL, `bm_type_defs` (patches/0002), then flag-dependent defines,
then `#include <tccdefs.h>` and `__BASE_FILE__`. The three string tables are
extracted from the patched sources and evaluated under each configuration
with the host C preprocessor; the flag-dependent part follows tcc_predefs
line by line (its source text is hashed: if tcc_predefs changes, this
generator stops and must be revisited). Cross-check: the -E configuration's
`#define` lines must equal, in order, the first lines of
`riscv64-tcc -dM -E` on an empty file (which lists macros in definition
order), and `__BASE_FILE__` its last line.
"""
import hashlib
import os
import re
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC = os.path.join(ROOT, 'build', 'tcc-src')
FSLIST = os.path.join(ROOT, 'c', 'build', 'fs.list')
CROSS = os.path.join(ROOT, 'build', 'cross', 'riscv64-tcc')
OUT_CFG = os.path.join(ROOT, 'Spec', 'Pre', 'Tcc', 'Cfg.lean')
OUT_FS = os.path.join(ROOT, 'Spec', 'Pre', 'Tcc', 'ImageFS.lean')

# sha256 of tccpp.c's tcc_predefs function text (patched tree). If tcc or a
# patch changes it, re-derive `flag_defines` below and update the hash.
TCC_PREDEFS_SHA = '9ffae30e2d838ed57b3326780ea1929d20a711d2daa2c2020bafabec38ff6ea8'


def read(p):
    with open(p, encoding='latin-1') as f:
        return f.read()


def extract_decl(text, name):
    """The source lines of `... name = <strings> ;`, including #if lines,
    up to the end of the statement at #if depth 0."""
    lines = text.split('\n')
    start = next(i for i, l in enumerate(lines) if re.search(r'\b%s\s*(\[\])?\s*=' % name, l))
    depth, seen_semi, out = 0, False, []
    for l in lines[start:]:
        out.append(l)
        s = l.strip()
        if s.startswith('#if'):
            depth += 1
        elif s.startswith('#endif'):
            depth -= 1
            if depth == 0 and seen_semi:
                break
        elif not s.startswith('#') and ';' in s:
            seen_semi = True
            if depth == 0:
                break
    return '\n'.join(out)


def eval_strtab(decl, defines):
    """Preprocess a declaration with `defines` and evaluate the string
    table: a list of NUL-terminated entries."""
    with tempfile.NamedTemporaryFile('w', suffix='.c', delete=False) as f:
        f.write(decl.replace('ST_DATA', '') + '\n')
        path = f.name
    try:
        args = ['gcc', '-E', '-P', '-x', 'c'] + ['-D%s' % d for d in defines] + [path]
        pp = subprocess.run(args, check=True, capture_output=True, text=True).stdout
    finally:
        os.unlink(path)
    body = pp[pp.index('=') + 1: pp.rindex(';')]
    raw = ''.join(eval(m) for m in re.findall(r'"(?:[^"\\]|\\.)*"', body))
    assert raw.endswith('\0') or raw == '', raw
    return [e for e in raw.split('\0') if e]


def putdef(p):
    # tccpp.c putdef: "#define %s%s\n" with " 1" unless the entry has a space
    return '#define %s%s\n' % (p, '' if ' ' in p else ' 1')


def tcc_version():
    m = re.search(r'#define TCC_VERSION "([^"]+)"', read(os.path.join(ROOT, 'c', 'src', 'tcc-config', 'config.h')))
    return m.group(1)


def predefs(mode):
    """The `<command line>` text of tcc_predefs. mode: 'run' (the image:
    TCC_BAREMETAL, output type MEMORY) or 'E' (the cross tcc, -E)."""
    gen = read(os.path.join(SRC, 'riscv64-gen.c'))
    pp = read(os.path.join(SRC, 'tccpp.c'))
    fn = pp[pp.index('static void tcc_predefs('):]
    fn = fn[:fn.index('\n}\n') + 3]
    sha = hashlib.sha256(fn.encode('latin-1')).hexdigest()
    if sha != TCC_PREDEFS_SHA:
        sys.exit('gen_pre_cfg.py: tcc_predefs changed (sha %s); re-derive flag_defines' % sha)
    common = ['TCC_TARGET_RISCV64=1', 'TCC_RV64I_SOFT=1']
    defs = common + (['TCC_BAREMETAL=1'] if mode == 'run' else [])
    machine = eval_strtab(extract_decl(gen, 'target_machine_defs'), defs)
    osdefs = eval_strtab(extract_decl(pp, 'target_os_defs'), defs)
    bm = eval_strtab(extract_decl(pp, 'bm_type_defs'), defs) if mode == 'run' else []
    v = tcc_version()
    out = '#define __TINYC__ 9%s\n' % v[4:6]
    for p in machine + osdefs + bm:
        out += putdef(p)
    # The flag-dependent defines of tcc_predefs, in its order, for
    # riscv64 (no ARM float ABI), not asm, no backtrace/bcheck (configured
    # off), char_is_unsigned (CHAR_IS_UNSIGNED, riscv64-gen.c), optimize 0,
    # no pthread, no leading underscore (only TCC_TARGET_MACHO), PTR_SIZE 8,
    # LONG_SIZE 8, nostdlib 0, cversion 199901 (libtcc.c tcc_new).
    out += putdef('__TCC_RUN__' if mode == 'run' else '__TCC_PP__')
    out += putdef('__CHAR_UNSIGNED__')
    out += '#define __SIZEOF_POINTER__ 8\n'
    out += '#define __SIZEOF_LONG__ 8\n'
    out += putdef('__STDC__')
    out += '#define __STDC_HOSTED__ 1\n'
    out += '#define __STDC_VERSION__ 199901L\n'
    out += '#include <tccdefs.h>\n'
    out += '#define __BASE_FILE__ "/prog.c"\n'
    return out, bm


def cross_check(text_e):
    """The -E text's #define lines = the first lines of riscv64-tcc -dM -E."""
    if not os.path.exists(CROSS):
        sys.exit('gen_pre_cfg.py: no %s (scripts/build_cross_tcc.sh)' % CROSS)
    tccinc = os.path.join(SRC, 'include')
    dm = subprocess.run([CROSS, '-nostdinc', '-I' + tccinc, '-dM', '-E', '-'], input='',
                        check=True, capture_output=True, text=True).stdout.split('\n')
    dm = [l for l in dm if l]
    mine = [l for l in text_e.split('\n') if l.startswith('#define')]
    head, base = mine[:-1], mine[-1]
    norm = lambda l: re.sub(r'\s+', ' ', l.strip())
    for i, l in enumerate(head):
        if norm(dm[i]) != norm(l):
            sys.exit('gen_pre_cfg.py: cross-check failed at line %d: tcc -dM %r, generated %r' % (i, dm[i], l))
    if not dm[-1].startswith('#define __BASE_FILE__ '):
        sys.exit('gen_pre_cfg.py: cross-check: last -dM line is %r' % dm[-1])
    return len(head)


def lean_str(s):
    out = []
    for ch in s:
        o = ord(ch)
        if ch == '\\':
            out.append('\\\\')
        elif ch == '"':
            out.append('\\"')
        elif ch == '\n':
            out.append('\\n')
        elif ch == '\t':
            out.append('\\t')
        elif 0x20 <= o < 0x7f:
            out.append(ch)
        elif o < 0x80:
            out.append('\\x%02x' % o)
        else:
            raise SystemExit('gen_pre_cfg.py: non-ASCII byte %#x in a header' % o)
    return '"' + ''.join(out) + '"'


HEADER = '''/-
GENERATED by scripts/gen_pre_cfg.py — do not edit (CLAUDE.md Law 7).
'''


def gen_cfg(run_text, e_text, bm, nchecked):
    return HEADER + '''
tcc's choices for C11 translation phases 1–4 (`CSem.Pre.PreCfg`).

* `cfgTcc`: the image's `tcc -run /prog.c` (TCC_BAREMETAL, c/src/tcc-config/
  config.h): predefined text of tccpp.c `tcc_predefs` under that
  configuration, including patches/0002's {nbm} `<stdint.h>` type macros and
  `__TCC_RUN__`; header search `{{B}}/include:/include` with B = /lib/tcc
  (CONFIG_TCC_SYSINCLUDEPATHS); `__DATE__`/`__TIME__` of the image's clock,
  frozen at 0 (c/src/htif.c `_gettimeofday`; newlib's localtime is UTC).
* `cfgTccE`: the cross tcc's `tcc -E` (scripts/build_cross_tcc.sh), as the
  differential runs it (`-nostdinc -I<root>/lib/tcc/include -I<root>/include`,
  file `<root>/prog.c`): `__TCC_PP__` instead of `__TCC_RUN__`, no
  TCC_BAREMETAL type macros. Its first {nchk} `#define` lines were checked
  equal, in order, to `riscv64-tcc -dM -E` of an empty file.

Implementation-defined choices (tccpp.c): no trigraphs; `$` and bytes
≥ 0x80 are identifier characters (`tccpp_new`, `dollars_in_identifiers`);
CR is white space (`is_space`); `<…>` from macro-replaced tokens is joined
without spaces (`parse_include`); plain `char` unsigned (riscv64-gen.c
CHAR_IS_UNSIGNED); include nesting INCLUDE_STACK_SIZE 32 (level ≤ 33 with
the main file at 1), #if nesting IFDEF_STACK_SIZE 64; non-STDC pragmas are
macro-replaced after their first token (`pragma_parse` reads with `next()`).
Extensions tcc implements: `#include_next`, `#pragma once`,
`push_macro`/`pop_macro`, `__COUNTER__`, `__has_include(_next)`,
`#warning`, GNU line markers, `#!` on the first line, GNU named variadic
parameters, `, ## __VA_ARGS__`, an omitted variable argument, `0b`
constants, a differing redefinition replaces the macro (a warning in tcc).
-/
import Spec.Pre.Types

namespace CSem.Pre.Tcc

open CSem.Pre

/-- The extensions tcc accepts (tccpp.c). -/
def extTcc : Ext where
  includeNext := true
  pragmaOnce := true
  pushPopMacro := true
  counter := true
  hasInclude := true
  warning := true
  linemarker := true
  hashBang := true
  namedVariadic := true
  commaPaste := true
  emptyVariadic := true
  binaryLiterals := true
  redefine := true

/-- tcc's predefined text for `tcc -run` in the image. -/
def predefsRun : String := {run}

/-- tcc's predefined text for the cross `tcc -E`. -/
def predefsE : String := {e}

/-- The image's `tcc -run /prog.c`. -/
def cfgTcc : PreCfg where
  trigraphs := false
  extraIdChar := fun c => c = 36 ∨ c ≥ 128
  extraSpace := fun c => c = 13
  predefs := bs predefsRun
  predefsName := bs "<command line>"
  mainName := bs "/prog.c"
  includePaths := []
  sysIncludePaths := [bs "/lib/tcc/include", bs "/include"]
  hdrJoinSpaces := false
  date := bs "Jan  1 1970"
  time := bs "00:00:00"
  maxIncludeDepth := 33
  maxIfDepth := 64
  charSigned := false
  pragmaExpand := true
  ext := extTcc

/-- The cross tcc's `tcc -E`, with the same file-system layout (the
differential maps its host paths back, scripts/pre_diff.py). `date`/`time`
are set by the driver to the host tcc's values. -/
def cfgTccE : PreCfg :=
  {{ cfgTcc with
    predefs := bs predefsE
    includePaths := [bs "/lib/tcc/include", bs "/include"]
    sysIncludePaths := [] }}

end CSem.Pre.Tcc
'''.format(run=lean_str(run_text), e=lean_str(e_text), nbm=len(bm), nchk=nchecked)


def gen_fs():
    entries = []
    for line in read(FSLIST).split('\n'):
        if not line.strip() or '=' not in line:
            continue
        path, host = line.split('=', 1)
        if not path.endswith('.h'):
            continue
        entries.append((path, read(host)))
    out = [HEADER, '''
The image's header tree as a `FileSys` (c/build/fs.list, the `.h` entries:
tcc's own headers under /lib/tcc/include and newlib's under /include, both
with comments stripped by c/src/strip_headers.py). {n} files.
-/
import Spec.Pre.Types

namespace CSem.Pre.Tcc

open CSem.Pre
'''.format(n=len(entries))]
    for i, (p, c) in enumerate(entries):
        out.append('/-- `%s` -/\ndef hdr%d : String := %s\n' % (p, i, lean_str(c)))
    out.append('/-- The image file system (headers). -/\ndef imageFS : FileSys := ⟨[\n')
    out.append(',\n'.join('  (bs %s, bs hdr%d)' % (lean_str(p), i) for i, (p, _) in enumerate(entries)))
    out.append('\n]⟩\n\nend CSem.Pre.Tcc\n')
    return ''.join(out)


def main():
    check = '--check' in sys.argv
    run_text, bm = predefs('run')
    e_text, _ = predefs('E')
    n = cross_check(e_text)
    files = {OUT_CFG: gen_cfg(run_text, e_text, bm, n), OUT_FS: gen_fs()}
    bad = False
    for p, s in files.items():
        if check:
            if not os.path.exists(p) or read(p) != s:
                print('gen_pre_cfg.py: %s is stale' % os.path.relpath(p, ROOT))
                bad = True
        else:
            os.makedirs(os.path.dirname(p), exist_ok=True)
            with open(p, 'w') as f:
                f.write(s)
            print('wrote %s' % os.path.relpath(p, ROOT))
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
