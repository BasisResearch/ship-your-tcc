#!/usr/bin/env python3
"""F3 differential: the executable Lean model of tcc (tcc_model) against the
real tcc, byte for byte.

For each program, the reference is the image's own tcc -run on the fast
RV64I interpreter (scripts/ref_arena.sh: the Sail model's image, run by
build/rv64emu, arena dumped at the hand-off to the program); the model is
.lake/build/bin/tcc_model on the same file system, command line and image
symbols. When tcc reaches the program, the two arenas (base, entry, the
program's code range, every byte up to the last non-zero one) must be
equal; when tcc stops first, its console bytes and exit status must be.
Matching refusals are diagnostic agreement only, not F3 arena coverage.
Reference-derived long-double padding is never supplied to the model.

    scripts/model_diff.py [--obj] [-j N] [--limit N] [--no-report] CORPUS|FILE...

CORPUS: tests (c/tests/*.c), run (c/tests/run), tcc (c/tests/tcc), p3
(c/tests/p3), compcert, tcctests, csmith (corpus/*), tests2 (tinycc's
tests/tests2), all. --obj: link-only check, the program is compiled by the
host cross tcc (build/cross/riscv64-tcc -c) and loaded as /prog.o with an
empty /prog.c (tcc -w /prog.o -run /prog.c). As ref_arena.sh does, a run
that stops on an unresolved reference is retried with -lm after /prog.c.
Reference runs are cached in build/model_diff/cache (keyed by the inputs and
the runtime image). The summary goes to results/model/f3-diff.md.
"""
import argparse
import concurrent.futures as cf
import hashlib
import os
import re
import subprocess
import sys
import tempfile
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TC = next(d for d in [os.path.expanduser('~/toolchains/xpack-riscv-none-elf-gcc-15.2.0-1/bin')] if os.path.isdir(d))
OBJDUMP = os.path.join(TC, 'riscv-none-elf-objdump')
MODEL = os.path.join(ROOT, '.lake', 'build', 'bin', 'tcc_model')
CROSS = os.path.join(ROOT, 'build', 'cross', 'riscv64-tcc')
CACHE = os.path.join(ROOT, 'build', 'model_diff', 'cache')
WORK = os.path.join(ROOT, 'build', 'model_diff', 'work')
FSLIST = os.path.join(ROOT, 'c', 'build', 'fs.list')
SYMS = os.path.join(ROOT, 'build', 'image.syms')
REPORT = os.path.join(ROOT, 'results', 'model', 'f3-diff.md')

CORPORA = {
    'tests': 'c/tests/*.c', 'run': 'c/tests/run/*.c', 'tcc': 'c/tests/tcc/*.c', 'p3': 'c/tests/p3/*.c',
    'compcert': 'corpus/compcert/*.c', 'tcctests': 'corpus/tcc/*.c', 'csmith': 'corpus/csmith/*.c',
    'tests2': 'build/tcc-src/tests/tests2/*.c',
    'gen-cgen': 'build/model_corpus/cgen/*.c', 'gen-csmith': 'build/model_corpus/csmith/*.c',
    'model': 'c/tests/model/*.c',
    'gen-mutants': 'build/model_corpus/mutants/*.c', 'gen-dtsplit': 'build/model_corpus/dtsplit/*.c',
}


def corpus_files(name):
    import glob
    return sorted(glob.glob(os.path.join(ROOT, CORPORA[name])))


def sha(*parts):
    h = hashlib.sha256()
    for p in parts:
        h.update(p if isinstance(p, bytes) else p.encode())
        h.update(b'\0')
    return h.hexdigest()[:24]


def runtime_key():
    elf = os.path.join(ROOT, 'c', 'build', 'base.elf')
    out = subprocess.run([os.path.join(TC, 'riscv-none-elf-objcopy'), '-O', 'binary', '-R', '.embed', elf, '/dev/stdout'],
                         capture_output=True).stdout
    return hashlib.sha256(out).hexdigest()[:16]


def parse_arena(path):
    b = open(path, 'rb').read()
    hdr, i = {}, 0
    for _ in range(5):
        j = b.index(b'\n', i)
        f = b[i:j].decode().split()
        hdr[f[0]] = f[1:]
        i = j + 1
    return hdr, b[i:]


def ref_run(prog, extra, args, libs, rtkey):
    """scripts/ref_arena.sh, cached: (status, data) where status is 'ran'
    (data = arena path) or 'exit' (data = (code, console bytes))."""
    key = sha(open(prog, 'rb').read(), repr(extra), repr(args), repr(libs), rtkey,
              *[open(h, 'rb').read() for _, h in extra])
    base = os.path.join(CACHE, key)
    if os.path.exists(base + '.status'):
        st = open(base + '.status').read().split()
        if st[0] == 'ran':
            return 'ran', base + '.arena', st[1] if len(st) > 1 else ''
        # Old caches can contain emulator limits/traps classified as exits.
        # Re-run those records; neither is a compiler refusal.
        if int(st[1]) not in (124, 126):
            return 'exit', (int(st[1]), open(base + '.console', 'rb').read()), st[2] if len(st) > 2 else ''
    env = dict(os.environ)
    env['TCC_ARGS'] = ''.join('"%s",' % a for a in args)
    # a non-empty TCC_LIBS turns off ref_arena.sh's own -lm retry (we retry here,
    # for the reference and the model alike); ' ' embeds no library
    env['TCC_LIBS'] = ''.join('"%s",' % a for a in libs) or ' '
    if extra:
        env['EXTRA_FILES'] = ' '.join('%s=%s' % (p, os.path.abspath(h)) for p, h in extra)
    r = subprocess.run([os.path.join(ROOT, 'scripts', 'ref_arena.sh'), prog, base + '.arena', base + '.console'],
                       capture_output=True, env=env)
    if r.returncode in (124, 126):
        raise RuntimeError('reference emulator limit/trap for %s: %s' %
                           (prog, r.stderr.decode(errors='replace')[-500:]))
    m = re.search(rb'rv64emu: steps=(\d+) compile_steps=(\S+) exit=(-?\d+)', r.stderr)
    con = open(base + '.console', 'rb').read() if os.path.exists(base + '.console') else b''
    lm = 'lm' if (r.returncode != 0 and not libs and b'unresolved reference' in con) else ''
    if r.returncode == 0:
        open(base + '.status', 'w').write('ran %s\n' % lm)
        return 'ran', base + '.arena', lm
    if not m:
        raise RuntimeError('reference harness failed for %s: %s' % (prog, r.stderr.decode(errors='replace')[-500:]))
    code = int(m.group(3)) & 0xff
    open(base + '.status', 'w').write('exit %d %s\n' % (code, lm))
    return 'exit', (code, open(base + '.console', 'rb').read()), lm


def model_run(prog, extra, args, libs, tag):
    os.makedirs(WORK, exist_ok=True)
    arena = os.path.join(WORK, tag + '.arena')
    console = os.path.join(WORK, tag + '.console')
    for f in (arena, console):
        if os.path.exists(f):
            os.unlink(f)
    cmd = [MODEL, '--fs', FSLIST, '--syms', SYMS, '--prog', prog, '--arena-out', arena, '--console-out', console]
    for p, h in extra:
        cmd += ['--extra', '%s=%s' % (p, os.path.abspath(h))]
    if args:
        for a in args:
            cmd += ['--arg', a]
    else:
        cmd += ['--no-args']
    for l in libs:
        cmd += ['--larg', l]
    t0 = time.time()
    r = subprocess.run(cmd, capture_output=True, timeout=600)
    dt = time.time() - t0
    out = r.stdout.decode(errors='replace').strip()
    con = open(console, 'rb').read() if os.path.exists(console) else b''
    if r.returncode != 0 and (out == 'ran' or out.startswith('exit ')):
        raise RuntimeError(f'model status/exit mismatch: {r.returncode}: {out}')
    if out == 'ran':
        return 'ran', arena, dt
    if out.startswith('exit '):
        return 'exit', (int(out.split()[1]), con), dt
    return 'model-error', out or r.stderr.decode(errors='replace')[-300:], dt


def disasm(data, base, off, width=24):
    lo = max(0, (off - width) & ~3)
    hi = min(len(data), off + width + 4)
    with tempfile.NamedTemporaryFile(suffix='.bin', delete=False) as f:
        f.write(data)
        name = f.name
    try:
        r = subprocess.run([OBJDUMP, '-D', '-b', 'binary', '-m', 'riscv:rv64', '-M', 'no-aliases',
                            '--adjust-vma=0x%x' % base, '--start-address=0x%x' % (base + lo),
                            '--stop-address=0x%x' % (base + hi), name], capture_output=True)
        lines = [l for l in r.stdout.decode().splitlines() if re.match(r'\s+[0-9a-f]+:', l)]
        return '\n'.join(lines)
    finally:
        os.unlink(name)


def compare(prog, mode, rtkey):
    name = os.path.relpath(prog, ROOT)
    tag = sha(name, mode)
    extra, args, src = [], ['-w'], prog
    if mode == 'obj':
        objdir = os.path.join(WORK, 'obj')
        os.makedirs(objdir, exist_ok=True)
        obj = os.path.join(objdir, tag + '.o')
        r = subprocess.run([CROSS, '-nostdinc', '-I', os.path.join(ROOT, 'c/build/fs/tccinclude'),
                            '-I', os.path.join(ROOT, 'c/build/fs/include'), '-c', prog, '-o', obj],
                           capture_output=True)
        if r.returncode != 0:
            return dict(name=name, result='skip', why='cross tcc -c failed')
        empty = os.path.join(objdir, 'empty.c')
        open(empty, 'w').close()
        extra, args, src = [('/prog.o', obj)], ['-w', '/prog.o'], empty
    rst, rdata, lm = ref_run(src, extra, args, [], rtkey)
    libs = ['-lm'] if lm else []
    if lm:
        rst, rdata, _ = ref_run(src, extra, args, libs, rtkey)
    mst, mdata, dt = model_run(src, extra, args, [], tag)
    if mst == 'exit' and b'unresolved reference' in mdata[1]:
        mst, mdata, dt2 = model_run(src, extra, args, ['-lm'], tag)
        dt += dt2
    res = dict(name=name, model_s=dt, ref=rst, model=mst)
    if mst == 'model-error':
        res.update(result='model-error', why=mdata)
        return res
    if rst != mst:
        why = 'reference %s, model %s' % (rst, mst)
        if mst == 'exit':
            why += ': ' + mdata[1].decode(errors='replace')[:300]
        if rst == 'exit':
            why += '; reference console: ' + rdata[1].decode(errors='replace')[:300]
        res.update(result='diff', why=why)
        return res
    if rst == 'exit':
        if rdata != mdata:
            res.update(result='diff', why='exit %d vs %d; console %r vs %r' % (rdata[0], mdata[0], rdata[1][:300], mdata[1][:300]))
        else:
            res.update(result='same-refusal', why='both stop: exit %d' % rdata[0])
        return res
    rh, rb = parse_arena(rdata)
    mh, mb = parse_arena(mdata)
    if rh == mh and rb == mb:
        res.update(result='same', why='arena %d bytes' % len(rb))
        return res
    base = int(rh['base'][0], 16)
    code, csize = int(rh['code'][0], 16), int(rh['code'][1], 16)
    why = []
    for k in ('base', 'entry', 'code', 'size'):
        if rh.get(k) != mh.get(k):
            why.append('%s %s vs %s' % (k, ' '.join(rh.get(k, [])), ' '.join(mh.get(k, []))))
    n = min(len(rb), len(mb))
    off = next((i for i in range(n) if rb[i] != mb[i]), n if len(rb) != len(mb) else None)
    detail = ''
    if off is not None:
        where = 'program code' if code - base <= off < code - base + csize else 'arena'
        why.append('first difference at +0x%x (0x%x, %s)' % (off, base + off, where))
        detail = 'reference:\n%s\nmodel:\n%s' % (disasm(rb, base, off), disasm(mb, base, off))
    res.update(result='diff', why='; '.join(why), detail=detail)
    return res


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('targets', nargs='+')
    ap.add_argument('--obj', action='store_true')
    ap.add_argument('-j', type=int, default=8)
    ap.add_argument('--limit', type=int, default=0)
    ap.add_argument('--no-report', action='store_true')
    ap.add_argument('-v', action='store_true')
    a = ap.parse_args()
    if a.j > 8:
        sys.exit('at most 8 concurrent emulator runs on the shared box')
    groups = []
    for t in a.targets:
        if t == 'all':
            groups += [(c, corpus_files(c)) for c in CORPORA]
        elif t in CORPORA:
            groups.append((t, corpus_files(t)))
        else:
            groups.append((os.path.relpath(os.path.dirname(os.path.abspath(t)), ROOT), [os.path.abspath(t)]))
    # Rebuild before fingerprinting: patches can change the runtime and its
    # exported addresses. Never pair a new reference with stale model symbols
    # or cache it under the previous image's key.
    with open(os.path.join(ROOT, 'c', '.build.lock'), 'w') as lock:
        import fcntl
        fcntl.flock(lock, fcntl.LOCK_EX)
        subprocess.run(['make', '-s', '-C', os.path.join(ROOT, 'c'),
                        'BUILD=build', 'FLAGS=0', 'ELF=build/base.elf', 'build/base.elf'],
                       check=True, stdout=subprocess.DEVNULL)
        with open(SYMS, 'w') as f:
            subprocess.run([os.path.join(ROOT, 'scripts', 'gen_image_syms.sh')], stdout=f, check=True)
    for group, files in groups:
        if not files:
            ap.error(f'empty corpus {group}: generate/populate it before comparison')
        for source in files:
            if not os.path.isfile(source):
                ap.error(f'input does not exist: {source}')
    os.makedirs(CACHE, exist_ok=True)
    rtkey = runtime_key()
    mode = 'obj' if a.obj else 'c'
    results = {}
    t0 = time.time()
    with cf.ThreadPoolExecutor(a.j) as ex:
        futs = {}
        for g, files in groups:
            if a.limit:
                files = files[:a.limit]
            for f in files:
                futs[ex.submit(compare, f, mode, rtkey)] = g
        for fu in cf.as_completed(futs):
            g = futs[fu]
            try:
                r = fu.result()
            except Exception as e:  # harness failure
                r = dict(name='?', result='harness-error', why=str(e))
            results.setdefault(g, []).append(r)
            if a.v or r['result'] not in ('same', 'same-refusal', 'skip'):
                print('%-12s %-45s %s' % (r['result'], r['name'], r.get('why', '')[:160]), flush=True)
                if a.v and r.get('detail'):
                    print(r['detail'])
    wall = time.time() - t0
    rows = []
    tot = dict(n=0, same=0)
    for g, rs in results.items():
        c = {k: sum(1 for r in rs if r['result'] == k) for k in ('same', 'same-refusal', 'diff', 'model-error', 'skip', 'harness-error')}
        n = len(rs) - c['skip']
        tot['n'] += n
        tot['same'] += c['same']
        ms = [r['model_s'] for r in rs if 'model_s' in r]
        rows.append((g, n, c['same'], c['diff'], c['model-error'], c['skip'], c['harness-error'], c['same-refusal'],
                     (sum(ms) / len(ms)) if ms else 0))
    print('\n%-14s %6s %6s %6s %6s %6s %6s %7s' %
          ('corpus', 'total', 'same', 'diff', 'merr', 'skip', 'herr', 'refusal'))
    for r in rows:
        print('%-14s %6d %6d %6d %6d %6d %6d %7d' % r[:8])
    print('%-14s %6d %6d   (%.0f s wall)' % ('all', tot['n'], tot['same'], wall))
    if not a.no_report:
        write_report(results, rows, tot, mode, wall)
    return int(any(r['result'] not in ('same', 'same-refusal', 'skip')
                   for rs in results.values() for r in rs))


def write_report(results, rows, tot, mode, wall):
    os.makedirs(os.path.dirname(REPORT), exist_ok=True)
    rev = subprocess.run(['git', '-C', ROOT, 'rev-parse', '--short', 'HEAD'], capture_output=True, text=True).stdout.strip()
    L = ['# F3: TccModel against tcc, byte for byte', '',
         'Generated by `scripts/model_diff.py%s` at %s (commit %s). Reference: the image\'s tcc -run on'
         ' build/rv64emu (scripts/ref_arena.sh); model: `tcc_model`. "same" = identical arena. Matching compile/link refusals are reported separately and do not count toward the F3 exit criterion.' % (' --obj' if mode == 'obj' else '',
                                                                                 time.strftime('%Y-%m-%d %H:%M'), rev), '']
    if mode == 'obj':
        L += ['Mode `--obj`: link-only. The program is compiled by the host cross tcc and loaded as /prog.o'
              ' with an empty /prog.c, so this checks the model\'s object/archive loading, GOT/PLT,'
              ' relocation and arena layout, not its compiler.', '']
    L += ['| corpus | programs | identical | different | model error | skipped | harness error | matching refusal | model s/prog |',
          '|---|---|---|---|---|---|---|---|---|']
    for r in rows:
        L.append('| %s | %d | %d | %d | %d | %d | %d | %d | %.2f |' % r)
    L += ['| **all** | **%d** | **%d** | | | | | | |' % (tot['n'], tot['same']), '',
          'Wall time %.0f s.' % wall, '']
    bad = [r for rs in results.values() for r in rs if r['result'] not in ('same', 'same-refusal', 'skip')]
    if bad:
        L += ['## Divergences (first 50)', '']
        for r in sorted(bad, key=lambda r: r['name'])[:50]:
            L.append('- `%s`: %s — %s' % (r['name'], r['result'], r.get('why', '')))
            if r.get('detail'):
                L += ['', '```', r['detail'], '```', '']
    open(REPORT, 'w').write('\n'.join(L) + '\n')
    print('wrote', os.path.relpath(REPORT, ROOT))


if __name__ == '__main__':
    sys.exit(main())
