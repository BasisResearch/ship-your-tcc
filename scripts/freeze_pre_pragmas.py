#!/usr/bin/env python3
"""Validate pragma repair evidence without counting observer failures as passes."""
from pathlib import Path
import hashlib
import json
import re
import shutil

ROOT = Path(__file__).resolve().parents[1]
DEST = ROOT / 'results/pre'


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    request = json.loads((ROOT / 'build/worker-build.request').read_text())
    result = json.loads((ROOT / 'build/worker-build.result').read_text())
    assert request['request_id'] == result['request_id']
    assert result['exit'] == 0 and result['source_hashes_verified']
    assert all(sha(ROOT / p) == h for p, h in request['sha256'].items())
    assert result['command'][0] == 'flock' and 'MemoryMax=12G' in result['command']
    log = ROOT / result['log']
    text = log.read_text()
    assert 'Build completed successfully' in text
    for name in ['preprocess', 'PreprocessCorrect_Statement', 'PragmaObservationEffects_Statement',
                 'PreprocessObservationNoninterference_Statement']:
        assert "'TccModel." + name + "' depends on axioms: [propext, Classical.choice, Quot.sound]" in text
    tests = ROOT / 'build/pragma-tests.log'
    static = ROOT / 'build/pragma-static.log'
    assert '\nOK\n' in tests.read_text() and 'skipped' not in tests.read_text()
    assert 'check: static stages OK' in static.read_text()
    summaries = {}
    residuals = []
    for lane in ['focused', 'negative', 'observer-residuals', 'corpus']:
        path = DEST / ('pragma-' + lane + '.json')
        report = json.loads(path.read_text())
        assert all(sha(ROOT / p) == h for p, h in report['sha256'].items())
        assert all(sha(p) == h for p, h in {**report['input_sha256'], **report['header_sha256']}.items())
        summaries[lane] = dict(attempted=len(report['rows']), **report['summary'])
        for row in report['rows']:
            assert row['raw'] != 'harness-error', row
            if lane == 'focused':
                assert row['raw'] == row['spec'] == 'exact' and row['observer_preserves_effects'], row
            elif lane == 'negative':
                assert row['raw'] == 'both-refused' and row['spec'] == 'spec-error', row
                residuals.append(dict(lane=lane, classification='intentional negative fixture; joint diagnostic', **row))
            elif lane == 'observer-residuals':
                assert row['raw'] == 'observer-effect-difference' and not row['observer_preserves_effects'], row
                name = Path(row['file']).stem
                classes = {
                    'pragma-nested-operand': 'observer noninterference failure: nested operand accepted plain, refused observed',
                    'pragma-nested-macro': 'observer noninterference failure: nested macro operand accepted plain, refused observed',
                    'pragma-operator-ignored-counter': 'observer noninterference failure: ignored operator counter differs from plain execution',
                }
                residuals.append(dict(lane=lane, classification=classes[name], **row))
            else:
                assert row['raw'] in ['exact', 'both-refused'] and row['observer_preserves_effects'], row
                if row['raw'] == row['spec'] == 'exact':
                    continue
                # Each corpus residual has an independent ruling. New failures
                # must be investigated and classified explicitly before freezing.
                file = row['file']
                if file.endswith('/pp/03.c'):
                    cls = 'constraint/phase-7 residual: spec retains other backslash; compiler-mode lexer refuses'
                elif file.endswith('/pp/21.c'):
                    cls = 'undefined signed overflow in #if: spec rejects; compiler accepts'
                elif file.endswith('/pp/22.c'):
                    cls = 'macro-invocation boundary residual: spec rejects unterminated invocation; compiler accepts'
                elif Path(file).name in ['114_bound_signal.c', '119_random_stuff.c', '145_winarm64_interlocked.c']:
                    cls = 'fixture/environment refusal: unavailable platform header'
                elif Path(file).name in ['18_include.c', '95_bitfields.c']:
                    cls = 'fixture/environment refusal: unavailable relative/recursive C-file include'
                else:
                    raise AssertionError('unclassified corpus residual: ' + file)
                residuals.append(dict(lane=lane, classification=cls, **row))
    builds = DEST / 'pragma-builds'
    builds.mkdir(exist_ok=True)
    n = request['request_id'].rsplit('-', 1)[-1]
    for src, name in [(ROOT / 'build/worker-build.request', 'request-' + n + '.json'),
                      (ROOT / 'build/worker-build.result', 'result-' + n + '.json'),
                      (log, 'build-' + n + '.log'), (tests, 'tests.log'), (static, 'static.log')]:
        shutil.copyfile(src, builds / name)
    (DEST / 'pragma-residuals.json').write_text(json.dumps(residuals, indent=2) + '\n')
    final = dict(request=request, result=result, summaries=summaries,
                 test_methods=int(re.search(r'Ran ([0-9]+) tests', tests.read_text())[1]),
                 sha256={str(p.relative_to(ROOT)): sha(p) for p in [log, tests, static,
                         ROOT / 'scripts/freeze_pre_pragmas.py', DEST / 'pragma-residuals.json',
                         *[DEST / ('pragma-' + lane + '.json') for lane in summaries]]},
                 limitations=['Finite internal-token comparisons; no refinement theorem.',
                              'Three observer noninterference failures retained outside the success quota.',
                              'Existing correctness and noninterference Statements remain open.'])
    (DEST / 'pragma-build.json').write_text(json.dumps(final, indent=2) + '\n')
    print(json.dumps(summaries, indent=2))


if __name__ == '__main__':
    main()
