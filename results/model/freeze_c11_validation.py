#!/usr/bin/env python3
"""Check final-run provenance and freeze C11 validation pins (run from root)."""
import hashlib
import importlib.util
import json
from pathlib import Path


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    build = json.loads(Path('results/model/c11-build-6.json').read_text())
    request, result = build['request'], build['result']
    assert request['request'] == result['request'] == 'c11-model-6'
    assert result['exit'] == 0 and result['source_hashes_verified']
    assert all(digest(p) == h for p, h in request['sha256'].items())
    spec = importlib.util.spec_from_file_location('model_diff', 'scripts/model_diff.py')
    harness = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(harness)
    runtime = harness.runtime_key()
    reports = ['c11-focused-final.json', 'c11-all.json', 'c11-binary128-stress.json']
    summaries, residuals = {}, {}
    paths = set(request['sha256'])
    paths.update(json.loads(Path('results/model/c11-pins.json').read_text())['sha256'])
    for name in reports:
        path = Path('results/model') / name
        report = json.loads(path.read_text())
        assert report['runtime_key'] == runtime, (name, 'stale runtime')
        assert all(digest(p) == h for p, h in report['pins'].items()), (name, 'stale input or executable')
        assert report['compiler_args'] == ['-std=c11', '-w']
        cases = [case for group in report['results'].values() for case in group]
        assert not any(c['result'] in ('diff', 'model-error', 'skip') for c in cases), name
        summaries[name] = {category: sum(c['result'] == category for c in cases)
                           for category in sorted({c['result'] for c in cases})}
        residuals[name] = [c for c in cases if c['result'] != 'same']
        if name == 'c11-all.json':
            generated = sum(c['result'] == 'same' for group in ('gen-cgen', 'gen-csmith')
                            for c in report['results'][group])
            assert generated >= 1000, generated
            summaries['successful_generated_cgen_csmith'] = generated
        paths.update(report['pins'])
        paths.add(str(path))
    paths.update(['results/model/freeze_c11_validation.py', 'results/model/c11-build-6.json',
                  'results/model/c11-build-6.log', 'build/rv64emu', 'build/image.syms',
                  'results/repro/tcc_pragma_macro_operand.c'])
    embedded = []
    for line in Path('c/build/fs.list').read_text().splitlines():
        if line:
            image, host = line.split('=', 1)
            embedded.append(dict(image=image, host=host, sha256=digest(host)))
    pins = dict(source_revision=request['source_revision'], successful_binary_build=request['request'],
                sha256={p: digest(p) for p in sorted(paths)},
                fs_list_sha256=digest('c/build/fs.list'), embedded_files=embedded,
                runtime_key=runtime, summaries=summaries,
                reference='rv64emu diagnostic; full Sail arena equality, ghost Cabs and fuel proofs open')
    Path('results/model/c11-pins.json').write_text(json.dumps(pins, indent=2) + '\n')
    Path('results/model/c11-residuals.json').write_text(json.dumps(residuals, indent=2) + '\n')
    print(json.dumps(summaries, indent=2))


if __name__ == '__main__':
    main()
