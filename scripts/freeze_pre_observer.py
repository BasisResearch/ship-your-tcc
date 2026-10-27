#!/usr/bin/env python3
"""Validate the current combined observer integration; preserve dated reports.

Run after the successful frozen build, all comparisons, and final --csem gate.
The evidence directory is distinct from token-* and pragma-* history.
"""
from pathlib import Path
import hashlib
import json
import re
import sys
import subprocess

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'scripts'))
from gen_pre_token_cases import CASES, OBSERVER_REGRESSIONS

DEST = ROOT / 'results/pre/observer-integration-2026-10-27'


def sha(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def read(name):
    return json.loads((DEST / name).read_text())


def pins(hashes):
    for p, h in hashes.items():
        assert sha(ROOT / p) == h, 'changed evidence: ' + p


def main():
    build, receipt = read('build-request.json'), read('build-result.json')
    gate, gated = read('gate-request.json'), read('gate-result.json')
    for req, res in [(build, receipt), (gate, gated)]:
        assert req['request_id'] == res['request_id']
        assert res['exit'] == 0 and res['source_hashes_verified']
        pins(req['sha256'])
    common = subprocess.check_output(['git', 'rev-parse', '--git-common-dir'], cwd=ROOT, text=True).strip()
    lock = (ROOT / common).resolve().parent / 'build/lake.lock'
    assert receipt['command'][:2] == ['flock', str(lock)]
    assert 'MemoryMax=12G' in receipt['command']
    assert gated['command'] == ['scripts/check.sh', '--csem']
    assert 'check: OK' in (DEST / 'gate.log').read_text()
    log = (DEST / 'build.log').read_text()
    assert 'Build completed successfully' in log
    for name in ['preprocess', 'PreprocessCorrect_Statement', 'PragmaObservationEffects_Statement',
                 'PreprocessObservationNoninterference_Statement']:
        assert "'TccModel." + name + "' depends on axioms: [propext, Classical.choice, Quot.sound]" in log
    tests = (DEST / 'tests.log').read_text()
    assert '\nOK\n' in tests and 'skipped' not in tests
    c11 = (DEST / 'c11-tests.log').read_text()
    assert 'C11: 89 positive cases, 6 negative cases; all passed' in c11
    assert 'PASS host/gcc/pp/cross/ISA tcc_pragma_nested_operand' in c11
    assert 'PASS host/gcc/pp/cross/ISA pragma_output' in c11
    summaries = {}
    residuals = []
    for lane in ['focused', 'negative', 'corpus']:
        r = read(lane + '.json')
        pins(r['sha256'])
        pins(r['input_sha256'])
        pins(r['header_sha256'])
        rows = r['rows']
        summaries[lane] = dict(attempted=len(rows), **r['summary'])
        if lane != 'corpus':
            expected = {n for n in CASES if n.startswith('error-') == (lane == 'negative')}
            assert {Path(row['file']).stem for row in rows} == expected
            if lane == 'focused':
                assert OBSERVER_REGRESSIONS <= expected
                assert '--diagnostic' not in r['command']
        for row in rows:
            assert row['observer_preserves_effects'], row
            if lane == 'focused':
                assert row['raw'] == row['spec'] == 'exact', row
            elif lane == 'negative':
                assert row['raw'] == 'both-refused' and row['spec'] == 'spec-error', row
            else:
                assert row['raw'] in ['exact', 'both-refused'], row
                if row['raw'] != 'exact' or row['spec'] != 'exact':
                    residuals.append(row)
    # Retain the historical corpus in full; additional runtime coverage is
    # explicit. No refusal/residual is admitted to the strict success quota.
    def key(p):
        for prefix in ['corpus/', 'c/tests/', 'vendor/tinycc/']:
            if prefix in p:
                return p[p.index(prefix):]
        raise AssertionError(p)
    previous = json.loads((ROOT / 'results/pre/pragma-corpus.json').read_text())
    old_keys = {key(r['file']) for r in previous['rows']}
    corpus = read('corpus.json')
    assert {key(r['file']) for r in corpus['rows']} == old_keys | {'c/tests/c11/pragma_output.c'}
    old_residual = {key(r['file']) for r in previous['rows'] if r['raw'] != 'exact' or r['spec'] != 'exact'}
    assert {key(r['file']) for r in residuals} == old_residual
    arenas = read('arenas.json')
    pins(arenas['pins'])
    assert arenas['totals'] == {'n': 3, 'same': 3}
    assert all(r['result'] == 'same' for group in arenas['results'].values() for r in group)
    sails = read('sail.json')
    assert len(sails) == 2
    for r in sails:
        pins(r['pins'])
        assert r['exit'] == 0 and r['stdout_bytes'] == 0
        assert r['meta']['program_steps'] > 0 and r['meta']['compile_steps'] > 0
        assert not r['meta'].get('fuel_out')
    (DEST / 'residuals.json').write_text(json.dumps(residuals, indent=2) + '\n')
    # The manifest pins every retained artifact, including historical failure
    # logs copied here. It deliberately excludes itself.
    files = sorted(p for p in DEST.rglob('*') if p.is_file() and p.name != 'manifest.json')
    final = dict(summaries=summaries, python_test_methods=int(re.search(r'Ran (\d+) tests', tests)[1]),
                 historical_reports={str(p.relative_to(ROOT)): sha(p) for p in
                                     sorted((ROOT / 'results/pre').glob('pragma-*.json'))},
                 sha256={str(p.relative_to(ROOT)): sha(p) for p in files},
                 limitations=['Finite independent comparisons, not binary/model or spec refinement proofs.',
                              'Three preprocessing correctness/effects/noninterference Statements remain open.',
                              'Eight historical corpus residuals retained separately from strict successes.'])
    (DEST / 'manifest.json').write_text(json.dumps(final, indent=2) + '\n')
    print(json.dumps(final['summaries'], indent=2))


if __name__ == '__main__':
    main()
