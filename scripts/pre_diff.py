#!/usr/bin/env python3
"""pre_diff.py: CSem.Pre.phases14 against `tcc -E` and `gcc -E`, token for token.

    scripts/pre_diff.py [--jobs N] [--corpus-dir DIR] [--out results/pre/f2-diff.tsv]
                        [--only SUBSTR] [--show NAME] [FILE...]

For each C file (default: every corpus file, see `default_files`):
  spec  .lake/build/bin/pre_spec --cfg E (cfgTccE: the cross tcc's -E
        configuration) with the image's header tree (c/build/fs.list);
  tcc   build/cross/riscv64-tcc -E -nostdinc -I<root>/lib/tcc/include
        -I<root>/include <root>/prog.c;
  gcc   riscv-none-elf-gcc -E -undef -nostdinc -std=c99 (same -I) -include
        <root>/predefs.h, where predefs.h is cfgTccE's predefined text minus
        the three __STDC*__ macros gcc defines itself.
<root> is a per-worker directory whose lib/tcc/include and include are
symlinks to c/build/fs/{tccinclude,include} (the image's tree) and whose
prog.c is the file under test, so every include resolves as in the image.
Files the test includes by a plain `"name"` that sit next to it are copied
beside prog.c (and given to the spec as /name).

Normalisation of the -E outputs (the documented rule, results/pre/f2-diff.md):
  1. line markers (`# N "file" ...`, `#line N "file"`) are dropped;
  2. a `#pragma ...` line becomes a pragma marker around its tokens (the
     spec's `[`/`]`);
  3. the remaining text is decomposed into preprocessing tokens by the
     spec's own phase 3 (`pre_spec --lex`); white space is dropped;
  4. inside string literals, the host path <root> is deleted (so
     `"<root>/include/stdio.h"` reads `"/include/stdio.h"`, the image path);
  5. `__DATE__`/`__TIME__`: the spec is given the host tcc's values.
Two token lists agree if they are equal; they agree "modulo -E spacing" if
they differ only where the -E text printed two tokens with nothing between
them that re-lex as one (the printer's artefact, not the preprocessor's): a
window of consecutive tokens on one side whose spellings concatenate to the
spelling of a token on the other.
Both sides failing (a spec error and a non-zero exit) counts as agreement.
"""
import argparse
import concurrent.futures
import os
import re
import shutil
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PRE_SPEC = os.path.join(ROOT, '.lake', 'build', 'bin', 'pre_spec')
TCC = os.path.join(ROOT, 'build', 'cross', 'riscv64-tcc')
FS = os.path.join(ROOT, 'c', 'build', 'fs')
FSLIST = os.path.join(ROOT, 'c', 'build', 'fs.list')
XGCC = None
for d in sorted(os.listdir(os.path.expanduser('~/toolchains'))):
    p = os.path.expanduser('~/toolchains/%s/bin/riscv-none-elf-gcc' % d)
    if d.startswith('xpack-riscv-none-elf-gcc-15.2.0') and os.path.exists(p):
        XGCC = p


def predefs_e():
    s = open(os.path.join(ROOT, 'Spec', 'Pre', 'Tcc', 'Cfg.lean')).read()
    m = re.search(r'def predefsE : String := (".*")\n', s)
    return eval(m.group(1).replace('\\x', '\\x'))


def make_root(tmp):
    root = tempfile.mkdtemp(dir=tmp)
    os.makedirs(os.path.join(root, 'lib', 'tcc'))
    os.symlink(os.path.join(FS, 'tccinclude'), os.path.join(root, 'lib', 'tcc', 'include'))
    os.symlink(os.path.join(FS, 'include'), os.path.join(root, 'include'))
    pre = '\n'.join(l for l in predefs_e().split('\n')
                    if not re.match(r'#define __STDC(_HOSTED|_VERSION)?__ ', l))
    with open(os.path.join(root, 'predefs.h'), 'w') as f:
        f.write(pre + '\n')
    return root


def run(args, timeout=120, cwd=None):
    try:
        p = subprocess.run(args, capture_output=True, timeout=timeout, cwd=cwd)
        return p.returncode, p.stdout.decode('latin-1'), p.stderr.decode('latin-1')
    except subprocess.TimeoutExpired:
        return 124, '', 'timeout'


def parse_toks(out):
    """pre_spec output -> (tokens, error)"""
    toks, err = [], None
    for l in out.split('\n'):
        if not l:
            continue
        if l.startswith('!\t'):
            err = l[2:]
            continue
        k, _, s = l.partition('\t')
        toks.append((k, s))
    return toks, err


def lex_text(text, tmpdir):
    """-E output -> tokens (normalisation steps 1-3)."""
    lines = []
    for l in text.split('\n'):
        if re.match(r'\s*#\s*(line\s+)?\d+(\s|$)', l):
            continue
        m = re.match(r'\s*#\s*pragma\b(.*)$', l)
        if m:
            lines.append('__PRAGMA_BEGIN__ %s __PRAGMA_END__' % m.group(1))
            continue
        lines.append(l)
    fd, path = tempfile.mkstemp(dir=tmpdir, suffix='.i')
    with os.fdopen(fd, 'w', encoding='latin-1') as f:
        f.write('\n'.join(lines) + '\n')
    rc, out, _ = run([PRE_SPEC, '--lex', path])
    os.unlink(path)
    toks, err = parse_toks(out)
    toks = [('[', '') if t == ('I', '__PRAGMA_BEGIN__') else (']', '') if t == ('I', '__PRAGMA_END__') else t
            for t in toks]
    return toks, err


def norm_paths(toks, root):
    return [(k, s.replace(root, '')) if k == 'S' else (k, s) for k, s in toks]


def compare(a, b):
    """'equal', ('spacing', n) or ('diff', i, j)."""
    if a == b:
        return ('equal',)
    i = j = 0
    glued = 0
    while i < len(a) and j < len(b):
        if a[i] == b[j]:
            i += 1
            j += 1
            continue
        # a window on one side spelling one token on the other
        ok = False
        for (x, y, xi, yj) in ((a, b, i, j), (b, a, j, i)):
            acc = ''
            k = xi
            while k < len(x) and len(acc) < len(y[yj][1]):
                acc += x[k][1]
                k += 1
            if k - xi >= 2 and acc == y[yj][1]:
                if x is a:
                    i, j = k, j + 1
                else:
                    i, j = i + 1, k
                ok = True
                glued += 1
                break
        if not ok:
            return ('diff', i, j)
    if i == len(a) and j == len(b):
        return ('spacing', glued)
    return ('diff', i, j)


def date_time(root):
    p = os.path.join(root, 'dt.c')
    with open(p, 'w') as f:
        f.write('__DATE__ __TIME__\n')
    _, out, _ = run([TCC, '-E', '-P', '-nostdinc', p])
    m = re.search(r'"([^"]*)"\s*"([^"]*)"', out)
    return (m.group(1), m.group(2)) if m else ('??? ?? ????', '??:??:??')


def local_headers(path):
    """The files a test names in `#include "x"` (x without a directory) that
    sit next to it: they are placed beside /prog.c, as /x."""
    d = os.path.dirname(path)
    names = re.findall(rb'#\s*include\s*"([^"/]+)"', open(path, 'rb').read())
    return sorted({n.decode() for n in names if os.path.isfile(os.path.join(d, n.decode()))})


def one(path, root, tmpdir, want_gcc=True):
    shutil.copyfile(path, os.path.join(root, 'prog.c'))
    prog = os.path.join(root, 'prog.c')
    for n in os.listdir(root):
        if n not in ('prog.c', 'predefs.h', 'lib', 'include', 'dt.c', 'fs.list'):
            os.unlink(os.path.join(root, n))
    extra = local_headers(path)
    with open(os.path.join(root, 'fs.list'), 'w') as f:
        f.write(open(FSLIST).read())
        for n in extra:
            shutil.copyfile(os.path.join(os.path.dirname(path), n), os.path.join(root, n))
            f.write('/%s=%s\n' % (n, os.path.join(root, n)))
    inc = ['-I' + os.path.join(root, 'lib', 'tcc', 'include'), '-I' + os.path.join(root, 'include')]
    date, time = date_time(root)
    trc, tout, terr = run([TCC, '-E', '-nostdinc'] + inc + [prog])
    src = open(path, 'rb').read()
    if b'__DATE__' in src or b'__TIME__' in src:
        date, time = date_time(root)
    src_rc, sout, _ = run([PRE_SPEC, '--cfg', 'E', '--fslist', os.path.join(root, 'fs.list'),
                           '--date', date, '--time', time, prog])
    stoks, serr = parse_toks(sout)
    res = {'file': path, 'spec_err': serr, 'tcc_rc': trc, 'tcc_err': terr.strip().split('\n')[0] if trc else ''}
    if trc == 0:
        ttoks, lerr = lex_text(tout, tmpdir)
        ttoks = norm_paths(ttoks, root)
    else:
        ttoks, lerr = [], None
    res['spec_n'] = len(stoks)
    res['tcc_n'] = len(ttoks)
    if serr and trc != 0:
        res['tcc'] = 'both-error'
    elif serr:
        res['tcc'] = 'spec-error'
    elif trc != 0:
        res['tcc'] = 'tcc-error'
    else:
        c = compare(stoks, ttoks)
        res['tcc'] = c[0] if c[0] != 'diff' else 'diff'
        if c[0] == 'spacing':
            res['tcc_glued'] = c[1]
        if c[0] == 'diff':
            i, j = c[1], c[2]
            res['tcc_ctx'] = '%s | %s' % (' '.join(s for _, s in stoks[max(0, i - 6):i + 6]),
                                          ' '.join(s for _, s in ttoks[max(0, j - 6):j + 6]))
    if want_gcc and XGCC:
        grc, gout, gerr = run([XGCC, '-E', '-undef', '-nostdinc', '-std=c99'] + inc +
                              ['-include', os.path.join(root, 'predefs.h'), prog])
        if grc == 0:
            gtoks, _ = lex_text(gout, tmpdir)
            gtoks = norm_paths(gtoks, root)
        if serr and grc != 0:
            res['gcc'] = 'both-error'
        elif serr:
            res['gcc'] = 'spec-error'
        elif grc != 0:
            res['gcc'] = 'gcc-error'
            res['gcc_err'] = gerr.strip().split('\n')[0]
        else:
            c = compare(stoks, gtoks)
            res['gcc'] = c[0]
            if c[0] == 'diff':
                i, j = c[1], c[2]
                res['gcc_ctx'] = '%s | %s' % (' '.join(s for _, s in stoks[max(0, i - 6):i + 6]),
                                              ' '.join(s for _, s in gtoks[max(0, j - 6):j + 6]))
    return res


def display(path, corpus_dir):
    """A file's name in reports: relative to the repository, or corpus/… for
    the corpus copies (which may live in another checkout)."""
    a = os.path.abspath(path)
    c = os.path.abspath(corpus_dir)
    if a.startswith(c + os.sep):
        return os.path.join('corpus', os.path.relpath(a, c))
    return os.path.relpath(a, ROOT)


def default_files(corpus_dir):
    fs = []
    for sub in ('compcert', 'csmith', 'tcc'):
        d = os.path.join(corpus_dir, sub)
        if os.path.isdir(d):
            fs += sorted(os.path.join(d, f) for f in os.listdir(d) if f.endswith('.c'))
    for d, _, names in sorted(os.walk(os.path.join(ROOT, 'c', 'tests'))):
        fs += sorted(os.path.join(d, n) for n in names if n.endswith('.c'))
    t2 = os.path.join(ROOT, 'vendor', 'tinycc', 'tests', 'tests2')
    fs += sorted(os.path.join(t2, f) for f in os.listdir(t2) if f.endswith('.c'))
    pp = os.path.join(ROOT, 'vendor', 'tinycc', 'tests', 'pp')
    fs += sorted(os.path.join(pp, f) for f in os.listdir(pp) if f.endswith('.c'))
    return fs


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--jobs', type=int, default=8)
    ap.add_argument('--corpus-dir', default=os.path.join(ROOT, 'corpus'))
    ap.add_argument('--out', default=None)
    ap.add_argument('--only', default=None)
    ap.add_argument('--no-gcc', action='store_true')
    ap.add_argument('files', nargs='*')
    a = ap.parse_args()
    files = a.files or default_files(a.corpus_dir)
    if a.only:
        files = [f for f in files if a.only in f]
    tmp = tempfile.mkdtemp(prefix='pre_diff_')
    roots = [make_root(tmp) for _ in range(a.jobs)]
    results = []
    try:
        with concurrent.futures.ThreadPoolExecutor(a.jobs) as ex:
            free = list(roots)
            import threading
            lock = threading.Lock()

            def task(f):
                with lock:
                    r = free.pop()
                try:
                    return one(f, r, tmp, not a.no_gcc)
                finally:
                    with lock:
                        free.append(r)
            for res in ex.map(task, files):
                results.append(res)
                line = '%-60s tcc=%-10s gcc=%-10s' % (display(res['file'], a.corpus_dir)[-60:],
                                                      res['tcc'], res.get('gcc', '-'))
                if res.get('spec_err'):
                    line += ' spec: ' + res['spec_err']
                if res.get('tcc_ctx'):
                    line += '\n    tcc ctx: ' + res['tcc_ctx']
                if res.get('tcc_err') and res['tcc'] != 'both-error':
                    line += '\n    tcc err: ' + res['tcc_err']
                print(line, flush=True)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    from collections import Counter
    print('tcc:', dict(Counter(r['tcc'] for r in results)))
    print('gcc:', dict(Counter(r.get('gcc', '-') for r in results)))
    if a.out:
        os.makedirs(os.path.dirname(a.out), exist_ok=True)
        with open(a.out, 'w') as f:
            f.write('file\tvs_tcc\tvs_gcc\tspec_tokens\ttcc_tokens\tspec_error\ttcc_error\tgcc_error\ttcc_context\tgcc_context\n')
            for r in results:
                f.write('\t'.join(str(x).replace('\t', ' ') for x in (
                    display(r['file'], a.corpus_dir), r['tcc'], r.get('gcc', '-'), r['spec_n'], r['tcc_n'],
                    r.get('spec_err') or '', r.get('tcc_err', ''), r.get('gcc_err', ''),
                    r.get('tcc_ctx', ''), r.get('gcc_ctx', ''))) + '\n')


if __name__ == '__main__':
    main()
