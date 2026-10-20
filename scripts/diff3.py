#!/usr/bin/env python3
"""Differential runner: run every corpus program under up to five oracles
and compare their stdout and exit status.

    scripts/diff3.py [--config scripts/oracles.toml] [--jobs N]
                     [--oracles gcc_native,xgcc_sail,...] [--corpus csmith,compcert,tcc]
                     [--progs GLOB] [--no-cache] [--summary-only]

Oracles and their interface are defined in scripts/oracles.toml. For each
program it writes results/diff3/<corpus>/<prog>.json (every oracle's
outcome, stdout, exit code, wall time, meta, and the pairwise comparison),
and results/diff3/summary.md + summary.tsv: per-oracle outcome counts,
per-pair agreement, and one row per disagreement with its classification.

Outcome of one oracle run:
  ok       finished; compared by (stdout, exit code)
  timeout  exit 124 or killed by the wall timeout
  n/a      exit 125 (or a code mapped to n/a): the oracle cannot run it
  stuck    a code mapped to stuck (ccomp/Lean 126): UB or unsupported construct
  absent   the oracle's `requires` file is missing (placeholder)
Two oracles AGREE when both are ok with identical stdout and exit code,
DISAGREE when both are ok and differ, and are INCOMPARABLE otherwise.

Classification of disagreements: results/diff3/classifications.tsv, rows
    corpus <TAB> prog <TAB> pair-or-* <TAB> category <TAB> note
with category exactly one of CATEGORIES. Unlisted disagreements are
"unclassified".
"""
import argparse
import concurrent.futures as cf
import fnmatch
import glob
import hashlib
import json
import os
import signal
import subprocess
import sys
import tempfile
import threading
import time
import tomllib

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, 'results', 'diff3')
CACHE = os.path.join(OUT, '.cache')
ORDER = ['gcc_native', 'xgcc_sail', 'tcc_sail', 'ccomp_interp', 'lean_clight']
CATEGORIES = ['tcc bug', 'Lean-port bug', 'CompCert-frontend issue',
              'unspecified behaviour', 'harness bug', 'unclassified']


def sha(b):
    return hashlib.sha256(b).hexdigest()


def load_config(path):
    cfg = tomllib.load(open(path, 'rb'))
    d = cfg.get('defaults', {})
    oracles = {}
    for name, o in cfg.get('oracle', {}).items():
        o = dict(o)
        o.setdefault('timeout', d.get('timeout', 1800))
        o.setdefault('env', {})
        o.setdefault('heavy', False)
        oracles[name] = o
    return d, oracles


def oracle_version(name, o):
    h = hashlib.sha256()
    h.update(name.encode())
    h.update(o['cmd'].encode())
    h.update(json.dumps(o['env'], sort_keys=True).encode())
    # Harness semantics (including status mappings and timeouts) are cache inputs.
    with open(__file__, 'rb') as harness:
        h.update(harness.read())
    h.update(json.dumps({k: o.get(k) for k in ('codes', 'timeout', 'requires', 'completion_meta')},
                        sort_keys=True).encode())
    if o.get('version_cmd'):
        env = dict(os.environ, **{k: str(v) for k, v in o['env'].items()})
        r = subprocess.run(o['version_cmd'], shell=True, capture_output=True,
                           cwd=ROOT, env=env, timeout=30)
        h.update(r.stdout)
        h.update(r.stderr)
        h.update(str(r.returncode).encode())
    for f in o.get('version_files', []):
        p = os.path.join(ROOT, f)
        h.update(f.encode())
        matches = sorted(x for x in glob.glob(p, recursive=True) if os.path.isfile(x))
        if not matches:
            h.update(b'<missing>')
        for path in matches:
            h.update(os.path.relpath(path, ROOT).encode())
            with open(path, 'rb') as source:
                for chunk in iter(lambda: source.read(1024 * 1024), b''):
                    h.update(chunk)
    return h.hexdigest()[:16]


def run_process(cmd, env, timeout, capture_output=True):
    """Own a process group so a timeout also stops the oracle's descendants."""
    p = subprocess.Popen(['bash', '-c', cmd], cwd=ROOT, env=env,
                         stdin=subprocess.DEVNULL, start_new_session=True,
                         stdout=subprocess.PIPE if capture_output else None,
                         stderr=subprocess.PIPE if capture_output else None)
    try:
        out, err = p.communicate(timeout=timeout)
        return p.returncode, out or b'', err or b'', False
    except subprocess.TimeoutExpired:
        try:
            os.killpg(p.pid, signal.SIGKILL)
        except ProcessLookupError:
            pass
        out, err = p.communicate()
        return p.returncode, out or b'', err or b'', True


def oracle_outcome(rc, o, killed=False):
    if killed or rc == 124:
        return 'timeout'
    # Signals are harness/runtime failures, never a C main return value.
    if rc is None or rc < 0:
        return 'n/a'
    codes = {int(k): v for k, v in o.get('codes', {}).items()}
    return codes.get(rc, 'n/a' if rc == 125 else 'ok')


def missing_completion_meta(o, meta):
    return [k for k in o.get('completion_meta', [])
            if not meta.get(k, '').isdigit() or int(meta[k]) <= 0]


class Gate:
    """Concurrency limit for heavy (emulator) runs plus a free-memory check."""

    def __init__(self, max_heavy, min_free_gb):
        self.sem = threading.Semaphore(max_heavy)
        self.min_free_kb = min_free_gb * 1024 * 1024

    def avail_kb(self):
        for l in open('/proc/meminfo'):
            if l.startswith('MemAvailable:'):
                return int(l.split()[1])
        return 1 << 60

    def __enter__(self):
        self.sem.acquire()
        while self.avail_kb() < self.min_free_kb:
            time.sleep(10)

    def __exit__(self, *a):
        self.sem.release()


def run_oracle(name, o, prog, key, gate, use_cache):
    cpath = os.path.join(CACHE, name, key + '.json')
    if use_cache and os.path.exists(cpath):
        r = json.load(open(cpath))
        r['cached'] = True
        return r
    if o.get('requires') and not os.path.exists(os.path.join(ROOT, o['requires'])):
        return dict(outcome='absent')
    cmd = o['cmd'].replace('{prog}', prog).replace('{root}', ROOT)
    with tempfile.NamedTemporaryFile('r', suffix='.meta', delete=False) as mf:
        meta_path = mf.name
    env = dict(os.environ, ORACLE_META=meta_path, **{k: str(v) for k, v in o['env'].items()})
    ctx = gate if o['heavy'] else _Null()
    with ctx:
        t0 = time.time()
        rc, out, err, killed = run_process(cmd, env, o['timeout'])
        wall = time.time() - t0
    meta = {}
    try:
        with open(meta_path) as meta_file:
            for l in meta_file:
                if '=' in l:
                    k, v = l.rstrip('\n').split('=', 1)
                    meta[k] = v
    finally:
        os.unlink(meta_path)
    outcome = oracle_outcome(rc, o, killed)
    if outcome == 'ok':
        missing = missing_completion_meta(o, meta)
        if missing:
            outcome = 'n/a'
            err += ('\ndiff3: oracle missing completion metadata: ' +
                    ', '.join(missing) + '\n').encode()
    if outcome == 'ok':
        rc = rc % 256
    elif killed:
        rc = None
    elif rc is not None and rc < 0:
        err += f'\ndiff3: oracle terminated by signal {-rc}\n'.encode()
    r = dict(outcome=outcome, exit=rc, stdout=out.decode('utf-8', 'backslashreplace'),
             stdout_sha=sha(out), stderr_tail=err.decode('utf-8', 'backslashreplace')[-2000:],
             wall_s=round(wall, 3), meta=meta, cmd=cmd)
    os.makedirs(os.path.dirname(cpath), exist_ok=True)
    tmp = cpath + '.tmp%d' % os.getpid()
    with open(tmp, 'w') as cache_file:
        json.dump(r, cache_file)
    os.replace(tmp, cpath)
    r['cached'] = False
    return r


class _Null:
    def __enter__(self):
        pass

    def __exit__(self, *a):
        pass


def compare(a, b):
    if a['outcome'] != 'ok' or b['outcome'] != 'ok':
        return 'incomparable'
    if a['stdout_sha'] == b['stdout_sha'] and a['exit'] == b['exit']:
        return 'agree'
    return 'disagree'


def load_classifications():
    p = os.path.join(OUT, 'classifications.tsv')
    cls = {}
    if not os.path.exists(p):
        return cls
    for n, line in enumerate(open(p), 1):
        if not line.strip() or line.startswith('#'):
            continue
        f = line.rstrip('\n').split('\t')
        if len(f) < 4 or f[3] not in CATEGORIES:
            sys.exit(f'{p}:{n}: bad row (category must be one of {CATEGORIES})')
        cls[(f[0], f[1], f[2])] = (f[3], f[4] if len(f) > 4 else '')
    return cls


def classify(cls, corpus, prog, pair):
    return cls.get((corpus, prog, pair)) or cls.get((corpus, prog, '*')) or ('unclassified', '')


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--config', default=os.path.join(ROOT, 'scripts', 'oracles.toml'))
    ap.add_argument('--jobs', type=int)
    ap.add_argument('--oracles')
    ap.add_argument('--corpus', default='csmith,compcert,tcc')
    ap.add_argument('--progs', default='*')
    ap.add_argument('--no-cache', action='store_true')
    ap.add_argument('--summary-only', action='store_true', help='rebuild the summary from the per-program JSON')
    a = ap.parse_args()
    d, oracles = load_config(a.config)
    names = [n for n in ORDER if n in oracles] + sorted(set(oracles) - set(ORDER))
    if a.oracles:
        names = [n for n in names if n in a.oracles.split(',')]
    progs = []
    for c in a.corpus.split(','):
        cd = os.path.join(ROOT, 'corpus', c)
        if not os.path.isdir(cd):
            continue
        for f in sorted(os.listdir(cd)):
            if f.endswith('.c') and fnmatch.fnmatch(f[:-2], a.progs):
                progs.append((c, f[:-2], os.path.join(cd, f)))
    if not a.summary_only:
        versions = {n: oracle_version(n, oracles[n]) for n in names}
        gate = Gate(d.get('max_heavy', 8), d.get('min_free_gb', 25))
        jobs = a.jobs or d.get('jobs', 8)
        tasks = {}
        results = {}
        done = 0
        with cf.ThreadPoolExecutor(jobs) as ex:
            for c, p, path in progs:
                h = sha(open(path, 'rb').read())
                for n in names:
                    key = f'{h}-{versions[n]}'
                    fut = ex.submit(run_oracle, n, oracles[n], path, key, gate, not a.no_cache)
                    tasks[fut] = (c, p, n, h)
            for fut in cf.as_completed(tasks):
                c, p, n, h = tasks[fut]
                try:
                    r = fut.result()
                except Exception as e:  # a harness bug: record it, keep going
                    r = dict(outcome='n/a', exit=None, stdout='', stdout_sha='', stderr_tail=f'diff3: {e!r}')
                r['version'] = versions[n]
                results.setdefault((c, p), {'sha256': h})[n] = r
                done += 1
                if done % 25 == 0 or done == len(tasks):
                    print(f'[{done}/{len(tasks)}] {c}/{p} {n}: {r["outcome"]}', file=sys.stderr)
        for (c, p), rs in results.items():
            os.makedirs(os.path.join(OUT, c), exist_ok=True)
            jp = os.path.join(OUT, c, p + '.json')
            old = json.load(open(jp)) if os.path.exists(jp) else {}
            # Results from a previous source revision cannot be compared with
            # the selected oracles' results for the new revision.
            oo = old.get('oracles', {}) if old.get('sha256') == rs['sha256'] else {}
            oo.update({n: rs[n] for n in names})
            rec = dict(corpus=c, prog=p, source=os.path.relpath(dict((x[1], x[2]) for x in progs if x[0] == c)[p], ROOT),
                       sha256=rs['sha256'], oracles=oo)
            json.dump(rec, open(jp, 'w'), indent=1, sort_keys=True)
    summarise(progs, names)


def summarise(progs, names):
    cls = load_classifications()
    recs = []
    for c, p, _ in progs:
        jp = os.path.join(OUT, c, p + '.json')
        if os.path.exists(jp):
            recs.append(json.load(open(jp)))
    pairs = [(x, y) for i, x in enumerate(names) for y in names[i + 1:]]
    corpora = sorted({r['corpus'] for r in recs}, key=lambda c: ['csmith', 'compcert', 'tcc'].index(c) if c in ('csmith', 'compcert', 'tcc') else 9)
    lines = ['# diff3 summary', '',
             f'Programs: {len(recs)} ({", ".join(f"{c}: {sum(1 for r in recs if r["corpus"] == c)}" for c in corpora)}). '
             'Oracles and interface: scripts/oracles.toml.', '',
             '## Oracle outcomes', '',
             '| corpus | oracle | ok | timeout | n/a | stuck | absent | median wall s | median steps |',
             '|---|---|---|---|---|---|---|---|---|']
    tsv = ['corpus\tprog\tpair\tverdict\tcategory\tnote\tleft\tright']
    for c in corpora:
        rc = [r for r in recs if r['corpus'] == c]
        for n in names:
            os_ = [r['oracles'].get(n, {'outcome': 'absent'}) for r in rc]
            cnt = {k: sum(1 for o in os_ if o['outcome'] == k) for k in ('ok', 'timeout', 'n/a', 'stuck', 'absent')}
            walls = sorted(o['wall_s'] for o in os_ if 'wall_s' in o)
            steps = sorted(int(o['meta']['steps']) for o in os_ if o.get('meta', {}).get('steps', '').isdigit())
            med = lambda v: str(v[len(v) // 2]) if v else '-'
            lines.append(f'| {c} | {n} | {cnt["ok"]} | {cnt["timeout"]} | {cnt["n/a"]} | {cnt["stuck"]} | {cnt["absent"]} | {med(walls)} | {med(steps)} |')
    lines += ['', '## Pairwise agreement', '',
              '| corpus | pair | agree | disagree | incomparable |', '|---|---|---|---|---|']
    dis = []
    for c in corpora:
        rc = [r for r in recs if r['corpus'] == c]
        for x, y in pairs:
            cnt = {'agree': 0, 'disagree': 0, 'incomparable': 0}
            for r in rc:
                ox = r['oracles'].get(x, {'outcome': 'absent'})
                oy = r['oracles'].get(y, {'outcome': 'absent'})
                v = compare(ox, oy)
                cnt[v] += 1
                pair = f'{x}/{y}'
                if v == 'disagree':
                    cat, note = classify(cls, c, r['prog'], pair)
                    dis.append((c, r['prog'], pair, cat, note, ox, oy))
                if ox['outcome'] != 'absent' and oy['outcome'] != 'absent':
                    cat, note = classify(cls, c, r['prog'], pair) if v == 'disagree' else ('', '')
                    tsv.append('\t'.join([c, r['prog'], pair, v, cat, note, desc(ox), desc(oy)]))
            if any(r['oracles'].get(x, {}).get('outcome', 'absent') != 'absent' and
                   r['oracles'].get(y, {}).get('outcome', 'absent') != 'absent' for r in rc):
                lines.append(f'| {c} | {pair_name(x, y)} | {cnt["agree"]} | {cnt["disagree"]} | {cnt["incomparable"]} |')
    lines += ['', '## Disagreements', '']
    if not dis:
        lines.append('None.')
    else:
        lines += ['| corpus | program | pair | classification | note | left | right |', '|---|---|---|---|---|---|---|']
        for c, p, pair, cat, note, ox, oy in dis:
            lines.append(f'| {c} | {p} | {pair} | {cat} | {note} | {desc(ox)} | {desc(oy)} |')
    lines += ['', '## Not ok', '', '| corpus | program | oracle | outcome | detail |', '|---|---|---|---|---|']
    for r in recs:
        for n in names:
            o = r['oracles'].get(n)
            if o and o['outcome'] in ('timeout', 'n/a', 'stuck'):
                detail = (o.get('stderr_tail') or '').strip().splitlines()
                lines.append(f'| {r["corpus"]} | {r["prog"]} | {n} | {o["outcome"]} | {detail[-1][:100] if detail else ""} |')
    os.makedirs(OUT, exist_ok=True)
    open(os.path.join(OUT, 'summary.md'), 'w').write('\n'.join(lines) + '\n')
    open(os.path.join(OUT, 'summary.tsv'), 'w').write('\n'.join(tsv) + '\n')
    print('\n'.join(lines[:60]))


def pair_name(x, y):
    return f'{x} vs {y}'


def desc(o):
    if o['outcome'] != 'ok':
        return o['outcome']
    s = o['stdout'].strip().splitlines()
    first = s[0][:40] if s else ''
    return f'exit {o["exit"]}, {len(o["stdout"])} B, sha {o["stdout_sha"][:8]}, "{first}"'.replace('|', '\\|')


if __name__ == '__main__':
    main()
