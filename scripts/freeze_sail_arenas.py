#!/usr/bin/env python3
"""Retain offline-recheckable native Sail comparisons from model_diff.py.

This exports existing observations, never constructs execution/proof evidence.
Every source/tool pin is checked before copying; native captures are compared
again with the independently saved model output. Non-success rows remain in
the original report and the exported inventory, never in the success count.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import shutil

import model_diff as m
from compare_sail_arena import compare

ROOT = Path(m.ROOT)


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def freeze(report, out):
    data = json.loads(report.read_text())
    if '--obj' in data['command']:
        raise ValueError('this exporter requires source-compilation observations')
    settings = data['sail_settings']
    if not settings:
        raise ValueError('report has no native Sail observations')
    pins = {**data['pins'], **settings['scripts']}
    for p, h in pins.items():
        if digest(ROOT / p) != h:
            raise ValueError('stale input: ' + p)
    binary = Path(settings['binary'])
    if digest(binary) != settings['binary_sha256'] or digest(binary.with_name('build.json')) != settings['build_manifest_sha256']:
        raise ValueError('capture tool changed')
    if out.exists():
        raise ValueError('refusing to replace retained evidence: ' + str(out))
    matches = {}
    for p in Path(m.CACHE).glob('*.sail/cut.json'):
        matches.setdefault(digest(p), []).append(p.parent)
    # Validate all rows before publishing any new archive.
    selected, residuals = [], []
    for rows in data['results'].values():
        for row in rows:
            if row['result'] != 'same':
                residuals.append(dict(name=row['name'], result=row['result'], why=row.get('why')))
                continue
            evidence = row['sail']
            if evidence['capture_settings'] != settings:
                raise ValueError('mixed capture settings')
            candidates = matches.get(evidence['capture_manifest_sha256'], [])
            if not candidates:
                raise ValueError('missing capture for ' + row['name'])
            capture = candidates[0]
            model = Path(m.WORK) / (m.sha(row['name'], 'c') + '.arena')
            observed = compare(capture, model)
            if any(evidence.get(k) != v for k, v in observed.items()):
                raise ValueError('retained observation differs: ' + row['name'])
            if digest(ROOT / row['name']) != observed['source_sha256']:
                raise ValueError('capture source differs: ' + row['name'])
            selected.append((row, capture, model, observed))
    if not selected:
        raise ValueError('no successful native observations to retain')
    if len(selected) != data['totals']['same']:
        raise ValueError('report success count differs')
    out.mkdir(parents=True)
    inventory = []
    for row, capture, model, observed in selected:
        name = m.sha(row['name'], 'c')
        target = out / name
        shutil.copytree(capture, target)
        shutil.copyfile(ROOT / row['name'], target / 'source.c')
        with gzip.open(target / 'model.arena.gz', 'wb') as f:
            f.write(model.read_bytes())
        inventory.append(dict(source=row['name'], directory=name, comparison=observed,
                              files={p.name:digest(p) for p in sorted(target.iterdir()) if p.is_file()}))
    shutil.copyfile(binary.with_name('build.json'), out / 'capture-build.json')
    shutil.copyfile(report, out / 'comparison-report.json')
    result = dict(evidence='native observations; not reachability proofs',
                  exporter_sha256=digest(__file__),
                  report_sha256=digest(report), successes=len(selected), residuals=residuals,
                  rows=inventory)
    (out / 'inventory.json').write_text(json.dumps(result, indent=2) + '\n')
    return result


if __name__ == '__main__':
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--report', required=True, type=Path)
    ap.add_argument('--out', required=True, type=Path)
    a = ap.parse_args()
    result = freeze(a.report.resolve(), a.out.resolve())
    print(f"retained {result['successes']} complete native comparisons; {len(result['residuals'])} separate residuals")
