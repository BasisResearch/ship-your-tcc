#!/usr/bin/env python3
"""Collect the CompCert and tcc test programs into corpus/{compcert,tcc}/.

    scripts/collect_corpus.py manifest   # (re)check every candidate, write
                                         # corpus/{compcert,tcc}/manifest.tsv
    scripts/collect_corpus.py copy       # copy the included programs from the
                                         # sources into corpus/{compcert,tcc}/

Sources (not committed; scripts/collect_corpus.sh fetches them):
  build/compcert-tests   AbsInt/CompCert-small-tests at the commit CompCert
                         v3.17 pins as its test/ submodule (Certora/clean is
                         a 3.17 fork): c/*.c (benchmarks) and regression/*.c
  vendor/tinycc/tests    tests2/*.c with *.expect, and tcctest.c

Every candidate gets one manifest row with status include/exclude and a
reason. A program is included when it is a single self-contained file in
the common subset (see EXCLUDE_PATTERNS and the per-program tables), host
gcc and the RV64I cross gcc both compile it, host gcc -O0 runs it to
completion within the cost limit, and (when there is a reference output)
host gcc's stdout matches it. Benchmarks whose default size is too big for
the Sail emulator (~47k instructions/s) are shrunk by editing their size
parameter (SHRINK); the edit is recorded in the `shrink` column and the
reference output is then not compared.

The `externals` column lists the library functions the program references
(undefined symbols of `gcc -O0 -fno-builtin -c`); `ccomp -interp` only
implements printf (plus malloc/free/memcpy as builtins), so this predicts
which programs the ccomp/Lean oracles can run. The oracles themselves are
the authority: a program they cannot run is reported as n/a, not as a
disagreement.
"""
import concurrent.futures as cf
import os
import re
import shutil
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CCT = os.path.join(ROOT, 'build', 'compcert-tests')
TCCT = os.path.join(ROOT, 'vendor', 'tinycc', 'tests')
TC = os.path.expanduser('~/toolchains/xpack-riscv-none-elf-gcc-15.2.0-1/bin')
XGCC = os.path.join(TC, 'riscv-none-elf-gcc')
CFLAGS = ['-std=gnu11', '-fpermissive', '-w', '-funsigned-char']
# cost limit on host x86-64 gcc -O0 user-mode instructions; the Sail steps of
# the RV64I build are ~1-3x that for integer code and far more for doubles
# (every FP operation is a libgcc soft-float call)
NATIVE_LIMIT = int(os.environ.get('NATIVE_LIMIT', 5_000_000))
JOBS = int(os.environ.get('JOBS', 8))

# (regex on comment-stripped source, reason) -> exclude
EXCLUDE_PATTERNS = [
    (r'\b(__asm__|__asm|asm)\b\s*(volatile|__volatile__|goto)?\s*\(', 'inline asm'),
    (r'\b(setjmp|longjmp|sigsetjmp)\b', 'setjmp/longjmp'),
    (r'\b(signal|sigaction|raise|kill)\s*\(|<signal\.h>', 'signals'),
    (r'\b(fork|wait|pthread_\w+|sem_\w+|mmap|thrd_\w+)\s*\(|<pthread\.h>|"threads\.h"', 'processes/threads/mmap'),
    (r'\b(scanf|getchar|fgets|gets|getc|fgetc|fread|getline)\s*\(|\bstdin\b', 'reads stdin'),
    (r'\b(fopen|freopen|tmpfile|remove|rename|open|unlink)\s*\(', 'file I/O'),
    (r'\blong\s+double\b', 'long double (tcc RV64 = binary128, soft tf helpers; %Lf printing)'),
    (r'\b(_Complex|complex)\b|<complex\.h>', 'complex numbers'),
    (r'\(\s*\{', 'GNU statement expression (CompCert rejects)'),
    (r'goto\s*\*|[=,{(]\s*&&\s*[A-Za-z_]', 'computed goto / label address (CompCert rejects)'),
    (r'\b(typeof|__typeof__|__typeof)\b', 'typeof (CompCert rejects)'),
    (r'\b(_Thread_local|__thread)\b', 'thread-local storage'),
    (r'\b_Atomic\b|<stdatomic\.h>|\b__atomic_\w+|\b__sync_\w+', 'atomics'),
    (r'\balloca\b', 'alloca'),
    (r'\bvolatile\b', 'volatile (not covered by the Lean Clight port)'),
    (r'%[-+ #0-9.]*[hlLqjzt]*[pan]', 'printf %p/%a/%n (unspecified, outside the Lean printf model)'),
    (r'__builtin_(?!va_)\w+', 'GCC builtins'),
    (r'#\s*pragma\s+pack|__attribute__\s*\(\(\s*(__)?packed', 'packed structs'),
    (r'__attribute__\s*\(\(\s*(__)?(constructor|destructor|cleanup)', 'constructor/destructor/cleanup attributes'),
    (r'\b(wprintf|setlocale|mbstowcs|wcstombs)\b|%l[sc]', 'wide-char/locale I/O'),
    (r'#\s*include\s*<(windows\.h|sys/\w+\.h|unistd\.h|uchar\.h)>', 'OS-specific header'),
    # ccomp -interp calls main(0, NULL); gcc_native and the Sail harness pass
    # argc=1, argv={"prog"}: only tests that cannot tell 0 from 1 are kept
    (r'\bargv\s*\[\s*0\s*\]|\bargc\s*(==|!=)\s*[01]\b|\bargc\s*(>\s*0|>=\s*1|<\s*1|<=\s*0)\b'
     r'|printf\s*\([^;]*\bargc\b|\bwhile\s*\(\s*argc|\bfor\s*\([^;]*;[^;]*\bargc\b',
     'depends on the value of argc/argv[0] (ccomp -interp passes argc=0, argv=NULL)'),
]

# Per-program decisions that a pattern cannot make.
MANUAL_EXCLUDE = {
    # CompCert benchmarks
    'bench_knucleotide': 'reads Results/knucleotide-input.txt (file I/O)',
    # CompCert regression tests
    'regr_NaNs': 'processor-dependent NaN payloads (no reference output upstream)',
    'regr_funct1': 'known CompCert failure (FAILURES in regression/Makefile)',
    'regr_sections': 'section/attribute placement test (no runtime behaviour)',
    'regr_annot1': 'CompCert __builtin_annot',
    'regr_extasm': 'extended inline asm',
    'regr_aligned': 'alignment attribute test, compiled-only upstream',
    'regr_alias': 'alias attribute (linker-level), compiled-only upstream',
    'regr_attribs1': 'section/alignment attributes, compiled-only upstream',
    'regr_alignas': '_Alignas layout test, compiled-only upstream',
    # tcc tests2
    't_27_sizeof': 'printf %d given size_t arguments (format/argument type mismatch, undefined; ccomp -interp prints <int argument expected>, found by diff3)',
    't_31_args': 'needs command-line arguments',
    't_46_grep': 'needs command-line arguments and reads a file',
    't_60_errors_and_warnings': 'compile-error snippets (-dt)',
    't_96_nodata_wanted': 'compile-time snippets (-dt)',
    't_104_inline': 'two translation units (104+_inline.c)',
    't_120_alias': 'two translation units (120+_alias.c)',
    't_106_versym': 'pthread / symbol versioning',
    't_108_constructor': 'constructors (NORUN upstream)',
    't_112_backtrace': 'bounds checker / backtrace (-b)',
    't_113_btdll': 'DLLs',
    't_114_bound_signal': 'bounds checker, signals',
    't_115_bound_setjmp': 'bounds checker, setjmp',
    't_116_bound_setjmp2': 'bounds checker, setjmp',
    't_117_builtins': 'tcc builtins, run twice with -b',
    't_121_struct_return': 'bounds checker (-b) upstream',
    't_122_vla_reuse': 'VLA (CompCert rejects), bounds checker',
    't_123_vla_bug': 'VLA (CompCert rejects)',
    't_78_vla_label': 'VLA (CompCert rejects)',
    't_79_vla_continue': 'VLA (CompCert rejects)',
    't_125_atomic_misc': 'atomics, -dt snippets',
    't_124_atomic_counter': 'atomics, pthread',
    't_126_bound_global': 'bounds checker',
    't_128_run_atexit': 'atexit with -run, -dt snippets',
    't_132_bound_test': 'bounds checker',
    't_138_arm64_encoding': 'arm64 asm',
    't_139_arm64_errors': 'arm64 asm',
    't_140_arm64_extasm': 'arm64 asm',
    't_141_riscv_asm': 'riscv asm encodings',
    't_144_tls': 'thread-local storage',
    't_145_winarm64_interlocked': 'Windows arm64',
    't_146_tls_extern': 'thread-local storage, two objects',
    't_148_linker_symbols': 'linker-symbol test, shared objects',
    't_98_al_ax_extend': 'i386 only',
    't_99_fastcall': 'i386 only',
    't_85_asm-outside-function': 'x86 asm',
    't_127_asm_goto': 'x86 asm goto',
    't_34_array_assignment': 'array assignment (not C; SKIP upstream)',
    't_95_bitfields_ms': 'MS bitfield layout (-mms-bitfields)',
    't_76_dollars_in_identifiers': 'needs -fdollars-in-identifiers',
    't_42_function_pointer': 'NORUN upstream (does not test the right thing with -run)',
    't_tcctest': 'needs the tcc build tree (config.h, tcclib.h, include-syntax tests); 4.5k lines of GCC extensions compared against gcc, not a single-file program',
}

# Benchmarks shrunk to fit the Sail emulator: (regex, replacement) on the
# source. The cost after shrinking is in the manifest (native_kinstr).
SHRINK = {
    'bench_fib': [(r'else n = 35;', 'else n = 15;')],
    'bench_integr': [(r'else n = 10000000;', 'else n = 1000;')],
    'bench_qsort': [(r'else n = 100000;', 'else n = 500;')],
    'bench_lists': [(r'else n = 1000;', 'else n = 100;'), (r'else niter = 20000;', 'else niter = 4;')],
    'bench_binarytrees': [(r'argc < 2 \? 12 :', 'argc < 2 ? 4 :')],
    'bench_fannkuch': [(r'\(argc>1\) \? atoi\(argv\[1\]\) : 10', '(argc>1) ? atoi(argv[1]) : 6')],
    'bench_fft': [(r'else n = 18;', 'else n = 6;')],
    'bench_fftsp': [(r'else n = 12;', 'else n = 3;')],
    'bench_nsieve': [(r'argc < 2 \? 9 :', 'argc < 2 ? 1 :')],
    'bench_spectral': [(r'\(argc == 2\) \? atoi\(argv\[1\]\) : 1000\)', '(argc == 2) ? atoi(argv[1]) : 6)')],
    'bench_mandelbrot': [(r'w = h = 1000;', 'w = h = 8;')],
    'bench_nbody': [(r'argc < 2 \? 1000000 :', 'argc < 2 ? 20 :')],
    'bench_almabench': [(r'#define TEST_LOOPS 20', '#define TEST_LOOPS 1'), (r'#define TEST_LENGTH 36525', '#define TEST_LENGTH 2')],
    'bench_aes': [(r'do_bench\(1000000\);', 'do_bench(10);')],
    'bench_sha1': [(r'do_bench\(200000\);', 'do_bench(4);')],
    # n is the matrix size; main prints S[1..19], so the print loop shrinks with it
    'bench_bisect': [(r'n = 500;', 'n = 8;'), (r'for\(i=1; i<20; i\+\+\)', 'for(i=1; i<8; i++)')],
    'bench_perlin': [(r'x \+= 0\.1\)', 'x += 2.5)'), (r'y \+= 0\.1\)', 'y += 2.5)'), (r'z \+= 0\.1\)', 'z += 2.5)')],
    'bench_vmach': [(r'I\(WCONST, 30, 0, 0\)', 'I(WCONST, 12, 0, 0)'), (r'"fib\(30\)', '"fib(12)'),
                    (r'/\* 0 \*/ I\(WCONST, 6, 0, 0\),\n/\* 1 \*/ I\(WCONST, 12, 0, 0\),\n/\* 2 \*/ I\(WCONST, 18, 0, 0\)',
                     '/* 0 */ I(WCONST, 3, 0, 0),\n/* 1 */ I(WCONST, 6, 0, 0),\n/* 2 */ I(WCONST, 9, 0, 0)'),
                    (r'"tak\(18, 12, 6\)', '"tak(9, 6, 3)')],
    'bench_sha3': [(r'#define DATALEN 100000', '#define DATALEN 1000'), (r'#define NITER 25', '#define NITER 1')],
    'bench_fftw': [(r'#define NRUNS \(100 \* 1000\)', '#define NRUNS 1')],
    'bench_siphash24': [(r'for\(i = 0; i < 1000000; i\+\+\) \{', 'for(i = 0; i < 4; i++) {')],
    'bench_chomp': [(r'ncol = 7;', 'ncol = 3;'), (r'nrow = 7;', 'nrow = 3;')],
    'regr_int64': [(r'for \(i = 0; i < 100; i\+\+\) \{', 'for (i = 0; i < 20; i++) {')],
    'regr_casts3': [(r'for \(i = 0; i < 1000000; i\+\+\) \{', 'for (i = 0; i < 1000; i++) {')],
    'bench_nsievebits': [(r'#define NITER 2\b', '#define NITER 1'), (r'n = ac < 2 \? 9 :', 'n = ac < 2 ? 1 :')],
}


def strip_comments(s):
    s = re.sub(r'/\*.*?\*/', ' ', s, flags=re.S)
    s = re.sub(r'//[^\n]*', '', s)
    # string literals: keep the quotes, drop contents (patterns must not
    # match words inside printf formats, except the %ls/%lc check)
    return s


def candidates():
    out = []
    for f in sorted(os.listdir(os.path.join(CCT, 'c'))):
        if f.endswith('.c'):
            out.append(('compcert', 'bench_' + f[:-2], os.path.join(CCT, 'c', f),
                        os.path.join(CCT, 'c', 'Results', f[:-2])))
    rd = os.path.join(CCT, 'regression')
    for f in sorted(os.listdir(rd)):
        if f.endswith('.c'):
            n = f[:-2]
            ref = None
            for suf in ('-riscV-64', '-riscV', '-64', ''):
                p = os.path.join(rd, 'Results', n + suf)
                if os.path.exists(p):
                    ref = p
                    break
            out.append(('compcert', 'regr_' + n, os.path.join(rd, f), ref))
    td = os.path.join(TCCT, 'tests2')
    for f in sorted(os.listdir(td)):
        if f.endswith('.c') and re.match(r'\d+_', f):
            n = f[:-2]
            e = os.path.join(td, n + '.expect')
            out.append(('tcc', 't_' + n, os.path.join(td, f), e if os.path.exists(e) else None))
    out.append(('tcc', 't_tcctest', os.path.join(TCCT, 'tcctest.c'), None))
    return out


def inline_local_includes(text, path, depth=0):
    """Inline `#include "x"` (CompCert's ../endian.h). Returns (text, ok)."""
    ok = True

    def rep(m):
        nonlocal ok
        p = os.path.normpath(os.path.join(os.path.dirname(path), m.group(1)))
        if depth > 3 or not os.path.exists(p):
            ok = False
            return m.group(0)
        t, ok2 = inline_local_includes(open(p, errors='replace').read(), p, depth + 1)
        ok = ok and ok2
        return f'/* begin {m.group(1)} (inlined by collect_corpus.py) */\n{t}\n/* end {m.group(1)} */\n'
    return re.sub(r'^\s*#\s*include\s*"([^"]+)"', rep, text, flags=re.M), ok


def prepare(src, name, path):
    text = open(path, errors='replace').read()
    shrink = '-'
    if name in SHRINK:
        notes = []
        for pat, rep in SHRINK[name]:
            text2, k = re.subn(pat, rep, text)
            if k == 0:
                raise RuntimeError(f'{name}: shrink pattern {pat!r} did not match')
            text = text2
            notes.append(' '.join(rep.split()))
        shrink = '; '.join(notes)
    local = re.findall(r'^\s*#\s*include\s*"([^"]+)"', text, flags=re.M)
    if local:
        if src == 'compcert' and all(l.endswith('endian.h') for l in local):
            text, ok = inline_local_includes(text, path)
            if not ok:
                return text, shrink, 'local include not found'
        else:
            return text, shrink, 'needs local include files (' + ', '.join(local) + ')'
    return text, shrink, None


def norm_ws(s):
    return [re.sub(r'\s+', ' ', l).strip() for l in s.splitlines() if l.strip()]


def run(cmd, **kw):
    return subprocess.run(cmd, capture_output=True, **kw)


def check(cand):
    src, name, path, ref = cand
    row = dict(name=name, source=os.path.relpath(path, ROOT), status='exclude', reason='',
               shrink='-', expect=os.path.relpath(ref, ROOT) if ref else '-',
               externals='-', native_kinstr='-', lines='-')
    if name in MANUAL_EXCLUDE:
        row['reason'] = MANUAL_EXCLUDE[name]
        return row
    try:
        text, shrink, why = prepare(src, name, path)
    except RuntimeError as e:
        row['reason'] = str(e)
        return row
    row['shrink'] = shrink
    row['lines'] = str(text.count('\n'))
    if why:
        row['reason'] = why
        return row
    body = strip_comments(text)
    if not re.search(r'\bmain\s*\(', body):
        row['reason'] = 'no main (compile-only test)'
        return row
    for pat, why in EXCLUDE_PATTERNS:
        if re.search(pat, body if '%' not in pat else text):
            row['reason'] = why
            return row
    with tempfile.TemporaryDirectory() as d:
        c = os.path.join(d, 'prog.c')
        open(c, 'w').write(text)
        r = run(['gcc', *CFLAGS, '-O0', '-o', os.path.join(d, 'a.out'), c, '-lm'])
        if r.returncode:
            row['reason'] = 'host gcc rejects: ' + first_error(r.stderr)
            return row
        r = run(['gcc', *CFLAGS, '-O0', '-fno-builtin', '-fno-stack-protector', '-fno-pic', '-c', '-o', os.path.join(d, 'nb.o'), c])
        syms = []
        if r.returncode == 0:
            nm = run(['nm', '-u', os.path.join(d, 'nb.o')], text=True).stdout
            syms = sorted({l.split()[-1] for l in nm.splitlines() if l.strip()})
            # gcc emits memcpy/memset for struct copies/initialisers itself
            syms = [s for s in syms if s not in ('memcpy', 'memset') or re.search(r'\b' + s + r'\b', body)]
        row['externals'] = ','.join(syms) or 'none'
        r = run([XGCC, *CFLAGS, '-march=rv64i', '-mabi=lp64', '-O0', '-c', '-o', os.path.join(d, 'x.o'), c])
        if r.returncode:
            row['reason'] = 'riscv-none-elf-gcc/newlib rejects: ' + first_error(r.stderr)
            return row
        pf = os.path.join(d, 'perf.txt')
        try:
            r = run(['perf', 'stat', '-x,', '-e', 'instructions:u', '-o', pf, os.path.join(d, 'a.out')],
                    cwd=d, timeout=30, stdin=subprocess.DEVNULL)
        except subprocess.TimeoutExpired:
            row['reason'] = 'host run timed out (30 s)'
            return row
        instr = None
        for l in open(pf):
            if 'instructions' in l:
                try:
                    instr = int(l.split(',')[0])
                except ValueError:
                    pass
        row['native_kinstr'] = str(instr // 1000) if instr is not None else '?'
        if r.returncode != 0:
            row['reason'] = f'host gcc run exits {r.returncode}'
            return row
        if instr is None or instr > NATIVE_LIMIT:
            row['reason'] = f'too slow for Sail: {row["native_kinstr"]}k native instructions > {NATIVE_LIMIT // 1000}k'
            return row
        if ref and shrink == '-':
            exp = open(ref, errors='replace').read()
            # tcc's .expect files capture 2>&1: drop compiler diagnostics
            exp = '\n'.join(l for l in exp.splitlines()
                            if not re.match(r'^\S+\.c:\d+:( \d+:)? (warning|error|note)', l))
            if norm_ws(exp) != norm_ws(r.stdout.decode(errors='replace')):
                row['reason'] = 'host gcc stdout differs from the reference output (implementation-specific)'
                return row
    row['status'] = 'include'
    row['reason'] = 'ok'
    return row


def first_error(stderr):
    for l in stderr.decode(errors='replace').splitlines():
        if 'error' in l:
            return re.sub(r'^.*?prog\.c:\d+:\d+: ', '', l).strip()[:120]
    return 'failed'


COLS = ['name', 'source', 'status', 'reason', 'shrink', 'expect', 'externals', 'native_kinstr', 'lines']


def manifest():
    rows = []
    with cf.ThreadPoolExecutor(JOBS) as ex:
        for c, row in zip(candidates(), ex.map(check, candidates())):
            rows.append((c[0], row))
    for src in ('compcert', 'tcc'):
        d = os.path.join(ROOT, 'corpus', src)
        os.makedirs(d, exist_ok=True)
        with open(os.path.join(d, 'manifest.tsv'), 'w') as f:
            f.write('\t'.join(COLS) + '\n')
            for s, row in rows:
                if s == src:
                    f.write('\t'.join(' '.join(row[k].split()) for k in COLS) + '\n')
        inc = sum(1 for s, r in rows if s == src and r['status'] == 'include')
        print(f'{src}: {inc} included of {sum(1 for s, _ in rows if s == src)}')


def copy():
    for src in ('compcert', 'tcc'):
        d = os.path.join(ROOT, 'corpus', src)
        for f in os.listdir(d):
            if f.endswith('.c'):
                os.unlink(os.path.join(d, f))
        n = 0
        for line in open(os.path.join(d, 'manifest.tsv')).read().splitlines()[1:]:
            row = dict(zip(COLS, line.split('\t')))
            if row['status'] != 'include':
                continue
            text, shrink, why = prepare(src, row['name'], os.path.join(ROOT, row['source']))
            assert why is None, row['name']
            hdr = f'/* corpus/{src}/{row["name"]}.c: copied from {row["source"]} by scripts/collect_corpus.py'
            if shrink != '-':
                hdr += '; shrunk: ' + shrink.replace('*/', '* /').replace('/*', '/ *')
            open(os.path.join(d, row['name'] + '.c'), 'w').write(hdr + ' */\n' + text)
            n += 1
        print(f'{src}: copied {n} programs to corpus/{src}/')


if __name__ == '__main__':
    {'manifest': manifest, 'copy': copy}[sys.argv[1] if len(sys.argv) > 1 else 'copy']()
