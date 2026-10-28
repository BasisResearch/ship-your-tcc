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
from contextlib import contextmanager
import fcntl
import hashlib
import json
from pathlib import Path
import os
import re
import subprocess
import sys
import tempfile
import time
from source_fixtures import fixture_files, fixture_entries, load_invocation

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


@contextmanager
def invocation_lock():
    """Protect this worktree's shared reference cache and source-named scratch.

    Jobs within one invocation remain parallel. Separate invocations wait,
    including native captures, so no process observes another's partial files.
    """
    os.makedirs(WORK, exist_ok=True)
    with open(os.path.join(WORK, '.invocation.lock'), 'a') as lock:
        try:
            fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError:
            print('model_diff: waiting for this worktree\'s active comparison',
                  file=sys.stderr, flush=True)
            fcntl.flock(lock, fcntl.LOCK_EX)
        yield

CORPORA = {
    'tests': 'c/tests/*.c', 'run': 'c/tests/run/*.c', 'tcc': 'c/tests/tcc/*.c', 'p3': 'c/tests/p3/*.c',
    'compcert': 'corpus/compcert/*.c', 'tcctests': 'corpus/tcc/*.c', 'csmith': 'corpus/csmith/*.c',
    'tests2': 'build/tcc-src/tests/tests2/*.c',
    'gen-cgen': 'build/model_corpus/cgen/*.c', 'gen-csmith': 'build/model_corpus/csmith/*.c',
    'model': 'c/tests/model/*.c', 'c11': 'c/tests/c11/*.c',
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
    # objcopy seeks when writing binary output. /dev/stdout can contain only
    # partial .text even when the process fails; always check a regular file.
    with tempfile.TemporaryDirectory(prefix='model-runtime-') as work:
        binary = os.path.join(work, 'runtime.bin')
        subprocess.run([os.path.join(TC, 'riscv-none-elf-objcopy'), '-O', 'binary',
                        '-R', '.embed', elf, binary], capture_output=True, check=True)
        runtime = open(binary, 'rb').read()
    if not runtime:
        raise RuntimeError('empty reference runtime image')
    h = hashlib.sha256()
    def add(label, data):
        for part in (label.encode(), data):
            h.update(len(part).to_bytes(8, 'big'))
            h.update(part)
    add('cache-schema', b'independent-c11-handoff-v4')
    add('runtime-without-embed', runtime)
    # The runtime excludes .embed, whose headers and archives still determine
    # compilation. Include every ordered image path and its exact file bytes.
    add('fs.list', open(FSLIST, 'rb').read())
    for line in open(FSLIST):
        if not line.strip():
            continue
        image_path, host_path = line.rstrip('\n').split('=', 1)
        add('fs:' + image_path, open(host_path, 'rb').read())
    for path in ('c/src/gen_embed.sh', 'scripts/ref_arena.sh',
                 'scripts/rv64emu.c', 'scripts/model_diff.py', 'scripts/source_fixtures.py'):
        add(path, open(os.path.join(ROOT, path), 'rb').read())
    return h.hexdigest()


def parse_arena(path):
    b = Path(path).read_bytes()
    lines = b.split(b'\n', 5)
    patterns = [rb'TCCARENA 1', rb'base [0-9a-f]+', rb'entry [0-9a-f]+',
                rb'code [0-9a-f]+ [0-9a-f]+', rb'size [0-9a-f]+']
    if len(lines) != 6 or not all(re.fullmatch(p, h) for p, h in zip(patterns, lines[:5])):
        raise RuntimeError('malformed arena header: '+path)
    hdr = {f[0]: f[1:] for f in (line.decode().split() for line in lines[:5])}
    size = int(hdr['size'][0], 16)
    base, entry = int(hdr['base'][0], 16), int(hdr['entry'][0], 16)
    code, length = [int(x, 16) for x in hdr['code']]
    if size != len(lines[5]) or size > 4 * 1024 * 1024:
        raise RuntimeError('invalid arena payload length: '+path)
    if not (base <= entry < base + 4 * 1024 * 1024 and
            base <= code < base + 4 * 1024 * 1024 and length <= base + 4 * 1024 * 1024 - code):
        raise RuntimeError('invalid arena addresses: '+path)
    return hdr, lines[5]


def ref_run(prog, extra, args, libs, rtkey, sail=None):
    """scripts/ref_arena.sh, cached: (status, data) where status is 'ran'
    (data = arena path) or 'exit' (data = (code, console bytes))."""
    key = sha('independent-c11-handoff-v4', Path(prog).read_bytes(), repr(extra), repr(args), repr(libs), rtkey, Path(ROOT, 'c/src/gen_embed.sh').read_bytes(),
              *[Path(h).read_bytes() for _, h in extra])
    if sail:
        key = sha(key, 'native-sail-full-arena-v1', json.dumps(sail, sort_keys=True))
    base = os.path.join(CACHE, key)
    if os.path.exists(base + '.status'):
        st = Path(base + '.status').read_text().split()
        if st and st[0] == 'ran':
            parse_arena(base + '.arena')
            if sail:
                validate_sail_cache(base, sail)
            return 'ran', base + '.arena', st[1] if len(st) > 1 else ''
        # Old caches can contain emulator limits/traps classified as exits.
        # Re-run those records; neither is a compiler refusal.
        if len(st) >= 2 and st[0] == 'exit' and 0 < int(st[1]) <= 255 and int(st[1]) not in (124, 126):
            return 'exit', (int(st[1]), Path(base + '.console').read_bytes()), st[2] if len(st) > 2 else ''
    env = dict(os.environ)
    if sail:
        env['ELF_OUT'] = base + '.elf'
    env['TCC_ARGS'] = ''.join('"%s",' % a for a in args)
    # a non-empty TCC_LIBS turns off ref_arena.sh's own -lm retry (we retry here,
    # for the reference and the model alike); ' ' embeds no library
    env['TCC_LIBS'] = ''.join('"%s",' % a for a in libs) or ' '
    if extra:
        env['EXTRA_FILES'] = ' '.join('%s=%s' % (p, os.path.abspath(h)) for p, h in extra)
    r = subprocess.run([os.path.join(ROOT, 'scripts', 'ref_arena.sh'), prog, base + '.arena', base + '.console'],
                       capture_output=True, env=env)
    if r.returncode not in (0, 125):
        raise RuntimeError('reference harness failure (%d) for %s: %s' %
                           (r.returncode, prog, r.stderr.decode(errors='replace')[-500:]))
    records = [line for line in r.stderr.splitlines() if line.startswith(b'rv64emu: steps=')]
    if len(records) != 1:
        raise RuntimeError('missing/duplicate reference completion record: '+prog)
    con = Path(base + '.console').read_bytes() if os.path.exists(base + '.console') else b''
    if r.returncode == 0:
        if not re.fullmatch(rb'rv64emu: steps=\d+ compile_steps=\d+ exit=- status=handoff', records[0]):
            raise RuntimeError('invalid handoff completion record: '+prog)
        parse_arena(base + '.arena')
        if sail:
            capture_sail(base, prog, sail)
        Path(base + '.status').write_text('ran\n')
        return 'ran', base + '.arena', ''
    m = re.fullmatch(rb'rv64emu: steps=\d+ compile_steps=- exit=(-?\d+) status=exit', records[0])
    if not m or (int(m.group(1)) & 255) == 0:
        raise RuntimeError('invalid compiler-refusal completion record: '+prog)
    code = int(m.group(1)) & 0xff
    lm = 'lm' if not libs and b'unresolved reference' in con else ''
    Path(base + '.status').write_text('exit %d %s\n' % (code, lm))
    return 'exit', (code, con), lm


def validate_sail_cache(base, sail):
    from compare_sail_arena import compare as compare_cut
    evidence = json.loads(Path(base + '.sail.json').read_text())
    if (evidence['status'] != 'equal' or evidence['compared_bytes'] != 4 * 1024 * 1024 or
            evidence['capture_settings'] != sail or
            evidence['model_arena_sha256'] != hashlib.sha256(Path(base + '.arena').read_bytes()).hexdigest()):
        raise RuntimeError('invalid cached Sail arena evidence: ' + base)
    observed = compare_cut(Path(base + '.sail'), Path(base + '.arena'))
    if any(evidence.get(k) != v for k, v in observed.items()):
        raise RuntimeError('cached Sail capture differs from evidence: ' + base)
    return evidence


def capture_sail(base, prog, sail):
    """A fast handoff selects an image; only Sail supplies the second observation.

    No cached success is published until all 4 MiB and handoff metadata agree.
    A failed/limited capture is a harness error, never a compiler refusal.
    """
    import gzip
    import shutil
    import uuid
    from compare_sail_arena import compare as compare_cut
    available = int(next(line.split()[1] for line in Path('/proc/meminfo').read_text().splitlines()
                         if line.startswith('MemAvailable:'))) * 1024
    if available < 25 * 1024 ** 3:
        raise RuntimeError('less than 25 GiB available before Sail capture; retry later')
    if hashlib.sha256(Path(sail['binary']).read_bytes()).hexdigest() != sail['binary_sha256']:
        raise RuntimeError('Sail capture binary changed during comparison')
    if hashlib.sha256(Path(sail['binary']).with_name('build.json').read_bytes()).hexdigest() != sail['build_manifest_sha256']:
        raise RuntimeError('Sail capture build manifest changed during comparison')
    for name, digest in sail['scripts'].items():
        if hashlib.sha256(Path(ROOT, name).read_bytes()).hexdigest() != digest:
            raise RuntimeError('Sail capture script changed during comparison: ' + name)
    with tempfile.TemporaryDirectory(prefix='sail-cut-', dir=WORK) as tmp:
        cut = Path(tmp) / 'capture'
        unit = 'syt-sail-' + uuid.uuid4().hex + '.scope'
        command = ['systemd-run', '--user', '--scope', '--unit', unit, '-p', 'MemoryMax=12G',
                   sys.executable, os.path.join(ROOT, 'scripts/capture_cut.py'), 'capture',
                   '--elf', base + '.elf', '--source', prog, '--out', str(cut),
                   '--binary', sail['binary'], '--max-steps', str(sail['max_steps'])]
        with open(base + '.sail.log', 'wb') as log:
            try:
                result = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT,
                                        timeout=sail['timeout_seconds'])
            except subprocess.TimeoutExpired:
                # Only this invocation's uniquely named scope is stopped.
                subprocess.run(['systemctl', '--user', 'stop', unit], check=True,
                               stdout=log, stderr=subprocess.STDOUT)
                raise
        if result.returncode:
            raise RuntimeError(f'Sail capture failed ({result.returncode}); see {base}.sail.log')
        evidence = compare_cut(cut, Path(base + '.arena'))
        evidence['capture_settings'] = sail
        # Retain compact complete captures. ELF is retained on failures; for a
        # successful case its exact hash plus image/source inputs are recorded.
        retained = Path(base + '.sail')
        retained.mkdir(exist_ok=True)
        for path in cut.iterdir():
            if path.name in ('arena.bin', 'arena.present.bin'):
                (retained / (path.name + '.gz')).write_bytes(gzip.compress(path.read_bytes(), mtime=0))
            else:
                shutil.copyfile(path, retained / path.name)
        Path(base + '.sail.json').write_text(json.dumps(evidence, indent=2) + '\n')
    os.unlink(base + '.elf')


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


def compare(prog, mode, rtkey, sail=None, invocation=None):
    name = os.path.relpath(prog, ROOT)
    tag = sha(name, mode)
    # gen_embed and ModelMain both prepend -std=c11 to these identical args.
    # Share the exact F2 corpus fixtures, including macro self-includes and
    # parent-directory aliases. Inputs never come from observed compiler output.
    extra = [(virtual, str(host)) for virtual, host in fixture_entries(prog)]
    args, src = ['-w'], prog
    if invocation is not None:
        extra, args, src = invocation['extra'], invocation['args'], invocation['source']
    if mode == 'obj':
        objdir = os.path.join(WORK, 'obj')
        os.makedirs(objdir, exist_ok=True)
        obj = os.path.join(objdir, tag + '.o')
        r = subprocess.run([CROSS, '-std=c11', '-nostdinc', '-I', os.path.join(ROOT, 'c/build/fs/tccinclude'),
                            '-I', os.path.join(ROOT, 'c/build/fs/include'), '-c', prog, '-o', obj],
                           capture_output=True)
        if r.returncode != 0:
            return dict(name=name, result='skip', why='cross tcc -c failed')
        empty = os.path.join(objdir, 'empty.c')
        open(empty, 'w').close()
        extra, args, src = [('/prog.o', obj)], ['-w', '/prog.o'], empty
    rst, rdata, lm = ref_run(src, extra, args, [], rtkey, sail)
    libs = ['-lm'] if lm else []
    if lm:
        rst, rdata, _ = ref_run(src, extra, args, libs, rtkey, sail)
    mst, mdata, dt = model_run(src, extra, args, libs, tag)
    res = dict(name=name, model_s=dt, ref=rst, model=mst)
    if invocation is not None:
        res['invocation'] = dict(argv=invocation['argv'], source=invocation['source'], files=invocation['extra'])
    if sail:
        res['reference'] = 'native Sail + rv64emu' if rst == 'ran' else 'rv64emu refusal only; no Sail coverage'
        if rst == 'ran':
            res['sail'] = validate_sail_cache(rdata.removesuffix('.arena'), sail)
    if mst == 'model-error':
        res.update(result='unsupported-asm' if mdata in ('model-error unsupported: asm statement', 'model-error unsupported: global asm block') else 'model-error', why=mdata)
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
    ap.add_argument('--invocations', action='store_true', help='targets are byte-pinned multi-TU invocation manifests')
    ap.add_argument('-j', type=int, default=8)
    ap.add_argument('--limit', type=int, default=0)
    ap.add_argument('--no-report', action='store_true')
    ap.add_argument('--json-out', help='retain every case, counts and input/image pins')
    ap.add_argument('--require-success', type=int, default=0, help='minimum identical successful arenas')
    ap.add_argument('--sail-capture', type=Path, help='also compare every successful arena with this native Sail capture binary')
    ap.add_argument('--sail-max-steps', type=int, default=100_000_000)
    ap.add_argument('--sail-timeout', type=int, default=3600, help='seconds per native capture')
    ap.add_argument('-v', action='store_true')
    a = ap.parse_args()
    if a.j < 1 or a.j > 8:
        sys.exit('at most 8 concurrent emulator runs on the shared box')
    if a.invocations and a.obj:
        ap.error('--invocations and --obj cannot be combined')
    a.invocation_data = {}
    if a.invocations:
        try:
            a.invocation_data = {os.path.abspath(t): load_invocation(t, ROOT) for t in a.targets}
        except (OSError, ValueError, KeyError, TypeError) as e:
            ap.error(str(e))
    sail = None
    if a.sail_capture:
        if a.sail_max_steps < 0 or a.sail_timeout <= 0:
            ap.error('Sail runtime limits must be nonnegative steps and positive seconds')
        binary = a.sail_capture.resolve()
        manifest = binary.parent / 'build.json'
        sail = dict(binary=str(binary), binary_sha256=hashlib.sha256(binary.read_bytes()).hexdigest(),
                    build_manifest_sha256=hashlib.sha256(manifest.read_bytes()).hexdigest(),
                    max_steps=a.sail_max_steps, timeout_seconds=a.sail_timeout,
                    scripts={p: hashlib.sha256(Path(ROOT, p).read_bytes()).hexdigest()
                             for p in ('scripts/capture_cut.py', 'scripts/gen_capture_cut.py',
                                       'scripts/compare_sail_arena.py', 'scripts/model_diff.py')})
    groups = []
    for t in a.targets:
        if a.invocations:
            groups.append(('invocations', [os.path.abspath(t)]))
        elif t == 'all':
            groups += [(c, corpus_files(c)) for c in CORPORA]
        elif t in CORPORA:
            groups.append((t, corpus_files(t)))
        else:
            groups.append((os.path.relpath(os.path.dirname(os.path.abspath(t)), ROOT), [os.path.abspath(t)]))
    if a.limit < 0 or a.require_success < 0:
        ap.error('limits and required successes must be nonnegative')
    if a.limit:
        groups = [(group, files[:a.limit]) for group, files in groups]
    seen = set()
    for group, files in groups:
        if not files:
            ap.error(f'empty corpus {group}: generate/populate it before comparison')
        for source in files:
            if not os.path.isfile(source):
                ap.error(f'input does not exist: {source}')
            if source in seen:
                ap.error(f'duplicate input: {source}')
            seen.add(source)
    with invocation_lock():
        return run_comparison(a, groups, sail)


def run_comparison(a, groups, sail):
    # Rebuild before fingerprinting: patches can change the runtime and its
    # exported addresses. Never pair a new reference with stale model symbols
    # or cache it under the previous image's key.
    with open(os.path.join(ROOT, 'c', '.build.lock'), 'w') as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        subprocess.run(['make', '-s', '-C', os.path.join(ROOT, 'c'),
                        'BUILD=build', 'FLAGS=0', 'ELF=build/base.elf', 'build/base.elf'],
                       check=True, stdout=subprocess.DEVNULL)
        # Other invocations may already be running model processes. Publish
        # a complete symbol table atomically so readers never see truncation.
        with tempfile.NamedTemporaryFile(dir=os.path.dirname(SYMS), delete=False) as f:
            symbol_tmp = f.name
            try:
                subprocess.run([os.path.join(ROOT, 'scripts', 'gen_image_syms.sh')], stdout=f, check=True)
                f.flush()
                os.replace(symbol_tmp, SYMS)
            finally:
                if os.path.exists(symbol_tmp):
                    os.unlink(symbol_tmp)
    os.makedirs(CACHE, exist_ok=True)
    os.makedirs(WORK, exist_ok=True)
    rtkey = runtime_key()
    inputs = {str(p): hashlib.sha256(p.read_bytes()).hexdigest()
              for _, files in groups for f in files for p in ([] if a.invocations else fixture_files(f))}
    for invocation in a.invocation_data.values():
        inputs.update(invocation['pins'])
    mode = 'obj' if a.obj else 'c'
    results = {}
    t0 = time.time()
    with cf.ThreadPoolExecutor(a.j) as ex:
        futs = {}
        for g, files in groups:
            for f in files:
                futs[ex.submit(compare, f, mode, rtkey, sail, a.invocation_data.get(f))] = (g, f)
        for fu in cf.as_completed(futs):
            g, source = futs[fu]
            try:
                r = fu.result()
            except Exception as e:  # harness failure
                r = dict(name=os.path.relpath(source, ROOT), result='harness-error', why=str(e))
            results.setdefault(g, []).append(r)
            if a.v or r['result'] not in ('same', 'same-refusal', 'skip'):
                print('%-12s %-45s %s' % (r['result'], r['name'], r.get('why', '')[:160]), flush=True)
                if a.v and r.get('detail'):
                    print(r['detail'])
    wall = time.time() - t0
    rows = []
    tot = dict(n=0, same=0)
    for g, rs in results.items():
        c = {k: sum(1 for r in rs if r['result'] == k) for k in ('same', 'same-refusal', 'diff', 'model-error', 'unsupported-asm', 'skip', 'harness-error')}
        n = len(rs) - c['skip']
        tot['n'] += n
        tot['same'] += c['same']
        ms = [r['model_s'] for r in rs if 'model_s' in r]
        rows.append((g, n, c['same'], c['diff'], c['model-error'], c['skip'], c['harness-error'], c['same-refusal'], c['unsupported-asm'],
                     (sum(ms) / len(ms)) if ms else 0))
    print('\n%-14s %6s %6s %6s %6s %6s %6s %7s %6s' %
          ('corpus', 'total', 'same', 'diff', 'merr', 'skip', 'herr', 'refusal', 'asm'))
    for r in rows:
        print('%-14s %6d %6d %6d %6d %6d %6d %7d %6d' % r[:9])
    print('%-14s %6d %6d   (%.0f s wall)' % ('all', tot['n'], tot['same'], wall))
    if not a.no_report:
        write_report(results, rows, tot, mode, wall, sail)
    if any(hashlib.sha256(Path(p).read_bytes()).hexdigest() != h for p, h in inputs.items()):
        raise RuntimeError('source/fixture changed during comparison')
    if a.json_out:
        pins = {p: hashlib.sha256(open(os.path.join(ROOT, p), 'rb').read()).hexdigest()
                for p in ['vendor/TINYCC_REV', 'patches/0001-tcc-rv64i-soft.patch',
                          'patches/0002-tcc-baremetal-run.patch', 'patches/0003-tcc-c11.patch',
                          'patches/0004-tcc-binary128-literals.patch', 'c/build/base.elf',
                          'c/src/gen_embed.sh', 'scripts/model_diff.py',
                          'scripts/source_fixtures.py', '.lake/build/bin/tcc_model']}
        pins.update({os.path.relpath(f, ROOT): h for f, h in inputs.items()})
        with open(a.json_out, 'w') as out:
            json.dump(dict(command=sys.argv, runtime_key=rtkey, pins=pins, results=results,
                           rows=rows, totals=tot, wall_seconds=wall,
                           reference=('native Sail full arenas plus rv64emu; refusals have no Sail coverage' if sail
                                      else 'rv64emu diagnostic; Sail arena equality pending'),
                           sail_settings=sail,
                           compiler_args=(None if a.invocations else ['-std=c11', '-w']),
                           invocations={p: d['argv'] for p, d in a.invocation_data.items()}), out, indent=2)
            out.write('\n')
    if tot['same'] < a.require_success:
        print(f'required {a.require_success} identical successful arenas, got {tot["same"]}', file=sys.stderr)
        return 1
    return int(any(r['result'] not in ('same', 'same-refusal', 'skip')
                   for rs in results.values() for r in rs))


def write_report(results, rows, tot, mode, wall, sail=None):
    os.makedirs(os.path.dirname(REPORT), exist_ok=True)
    rev = subprocess.run(['git', '-C', ROOT, 'rev-parse', '--short', 'HEAD'], capture_output=True, text=True).stdout.strip()
    L = ['# F3: TccModel against tcc, byte for byte', '',
         'Generated by `scripts/model_diff.py%s` at %s (commit %s). Reference: the image\'s tcc -run on'
         ' build/rv64emu (scripts/ref_arena.sh); model: `tcc_model`. "same" = identical arena. Matching compile/link refusals are reported separately and do not count toward the F3 exit criterion.' % (' --obj' if mode == 'obj' else '',
                                                                                 time.strftime('%Y-%m-%d %H:%M'), rev), '']
    if sail:
        L += ['Native Sail capture is also enabled: every successful arena matches all 4 MiB'
              ' and handoff metadata on Sail. Refusals are fast-reference diagnostics only.'
              ' These are native observations, not reachability proofs.', '']
    if mode == 'obj':
        L += ['Mode `--obj`: link-only. The program is compiled by the host cross tcc and loaded as /prog.o'
              ' with an empty /prog.c, so this checks the model\'s object/archive loading, GOT/PLT,'
              ' relocation and arena layout, not its compiler.', '']
    L += ['| corpus | programs | identical | different | model error | skipped | harness error | matching refusal | unsupported asm | model s/prog |',
          '|---|---|---|---|---|---|---|---|---|---|']
    for r in rows:
        L.append('| %s | %d | %d | %d | %d | %d | %d | %d | %d | %.2f |' % r)
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
