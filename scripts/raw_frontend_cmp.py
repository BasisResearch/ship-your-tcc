#!/usr/bin/env python3
"""Independent full-Cabs check of raw Lean frontends on a shared positioned view.

The adapter runs first. A separate process gives its declared virtual layout to
pristine pinned Menhir via Cerberus's --cpp hook, bypassing host preprocessing.
Locations and raw-file digests are compared, with no normalisation of Cabs.
Joint rejections never count as equality. Artifacts and exact inputs are pinned.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]


def sha(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def invoke(cmd, artifact, env=None):
    p = subprocess.run([str(x) for x in cmd], cwd=ROOT, env=env,
                       capture_output=True, timeout=120)
    artifact.with_suffix('.stdout').write_bytes(p.stdout)
    artifact.with_suffix('.stderr').write_bytes(p.stderr)
    artifact.with_suffix('.command.json').write_text(json.dumps([str(x) for x in cmd], indent=2)+'\n')
    return p


def historical_positions(value):
    """Project only metadata absent in the historical oracle; keep all positions."""
    if isinstance(value, dict):
        return {k: historical_positions(v) for k,v in value.items() if k != 'source_identity'}
    if isinstance(value, list):
        return [historical_positions(v) for v in value]
    return value


def compare(source, directory, oracle, runtime, extra=()):
    source = Path(source).resolve()
    directory.mkdir(parents=True, exist_ok=False)
    layout = directory/'positioned.pp'
    raw = ROOT/'.lake/build/bin/csem_raw'
    r = invoke([raw, '--layout', layout, *extra, source], directory/'lean')
    row = dict(source=str(source), source_sha256=sha(source), raw_exit=r.returncode)
    if r.returncode:
        return dict(row, status='RAW_REJECT', error=r.stderr.decode(errors='replace'))
    bundle = json.loads(r.stdout)
    env = dict(os.environ, SYT_RAW_LAYOUT=str(layout), LEAN_ABORT_ON_PANIC='1')
    cpp = f'{shlex.quote(sys.executable)} {shlex.quote(str(ROOT/"scripts/raw_frontend_cat.py"))}'
    o = invoke([oracle, f'--runtime={runtime}', f'--cpp={cpp}', '--cabs-json', source],
               directory/'menhir', env)
    row.update(oracle_exit=o.returncode, layout_sha256=sha(layout),
               library_disagreements=sum(x['actualLibrary'] != x['presumedLibrary']
                                         for x in bundle['libraryOrigins']),
               pragma_events=len(bundle['pragmas']))
    if o.returncode:
        return dict(row, status='ORACLE_REJECT', error=o.stderr.decode(errors='replace'))
    expected = json.loads(o.stdout)
    expected_digest = expected.pop('digest')
    if expected_digest != bundle['digest'] or expected_digest != hashlib.md5(source.read_bytes()).hexdigest():
        return dict(row, status='DIGEST_DIFF')
    if expected != historical_positions(bundle['cabs']):
        return dict(row, status='CABS_DIFF')
    # Prove the unchanged text path sees precisely the same positioned input.
    # Cerberus's cpp reader drops exactly one terminal LF.
    text_view = directory/'text-view.pp'
    data = layout.read_bytes()
    text_view.write_bytes(data[:-1] if data.endswith(b'\n') else data)
    t = invoke([ROOT/'.lake/build/bin/csem_cparse', '--json', text_view, source], directory/'text')
    if t.returncode or json.loads(t.stdout) != expected:
        return dict(row, status='TEXT_CABS_DIFF')
    return dict(row, status='AGREE', interface='historical-position-projection; identity checked separately')


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--oracle', required=True)
    ap.add_argument('--runtime', required=True)
    ap.add_argument('--artifacts', type=Path, required=True)
    ap.add_argument('--fslist', type=Path)
    ap.add_argument('--library', action='append', default=[])
    ap.add_argument('--tcc', action='store_true')
    ap.add_argument('sources', nargs='+', type=Path)
    a = ap.parse_args()
    a.artifacts.mkdir(parents=True, exist_ok=False)
    pins = {'oracle': sha(a.oracle), 'raw': sha(ROOT/'.lake/build/bin/csem_raw'),
            'text': sha(ROOT/'.lake/build/bin/csem_cparse'),
            'csem_rev': (ROOT/'vendor/CSEM_REV').read_text()}
    extra = ['--fslist', str(a.fslist.resolve())] if a.fslist else []
    if a.tcc:
        extra += ['--tcc']
    for path in a.library:
        extra += ['--library', path]
    if a.fslist:
        pins['fslist'] = sha(a.fslist)
        pins['headers'] = {line.split('=', 1)[1]: sha(line.split('=', 1)[1])
                           for line in a.fslist.read_text().splitlines() if line}
    initial_sources = {str(p): sha(p) for p in a.sources}
    rows = []
    for i, source in enumerate(a.sources):
        rows.append(compare(source, a.artifacts/str(i), a.oracle, a.runtime, extra))
        print(rows[-1]['status'], source, flush=True)
    if any(sha(p) != h for p, h in initial_sources.items()):
        raise RuntimeError('raw input changed during comparisons')
    if sha(a.oracle) != pins['oracle'] or sha(ROOT/'.lake/build/bin/csem_raw') != pins['raw']:
        raise RuntimeError('executable changed during comparisons')
    if any(sha(p) != h for p, h in pins.get('headers', {}).items()):
        raise RuntimeError('header changed during comparisons')
    (a.artifacts/'results.json').write_text(json.dumps(dict(pins=pins, rows=rows), indent=2)+'\n')
    return int(any(r['status'] != 'AGREE' for r in rows))


if __name__ == '__main__':
    sys.exit(main())
