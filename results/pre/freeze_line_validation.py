#!/usr/bin/env python3
"""Verify and retain final line-repair build, arena and Sail evidence."""
import hashlib
import importlib.util
import json
from pathlib import Path
import subprocess


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    request = json.loads(Path('build/worker-build.request').read_text())
    result = json.loads(Path('build/worker-build.result').read_text())
    assert request['request_id'] == result['request_id']
    assert result['exit'] == 0 and result['source_hashes_verified']
    assert all(digest(p) == h for p, h in request['sha256'].items())
    arenas = json.loads(Path('results/pre/line-arenas.json').read_text())
    cases = [case for group in arenas['results'].values() for case in group]
    assert len(cases) == 47 and all(c['result'] == 'same' for c in cases)
    assert len({c['name'] for c in cases}) == 47
    assert arenas['compiler_args'] == ['-std=c11', '-w']
    assert all(digest(p) == h for p, h in arenas['pins'].items())
    spec = importlib.util.spec_from_file_location('model_diff', 'scripts/model_diff.py')
    harness = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(harness)
    assert harness.runtime_key() == arenas['runtime_key']
    sail_request = json.loads(Path('build/worker-sail.request').read_text())
    sail_result = json.loads(Path('build/worker-sail.result').read_text())
    assert sail_result['request_id'] == sail_request['request_id']
    assert sail_result['exit'] == 0
    assert all(digest(p) == h for p, h in sail_request['sha256'].items())
    assert digest(sail_request['command'][2]) == sail_request['emulator_sha256']
    trace = Path(sail_request['stderr']).read_text()
    raw = Path(sail_request['stdout']).read_bytes()
    assert 'TRACE-FUEL-OUT' not in trace and raw == b''
    assert sail_result['source_hashes_verified']
    assert all(digest(p) == h for p, h in sail_result['sha256'].items())
    pcs = [int(p, 16) for p in Path('build/line-sail.pcs').read_text().splitlines()]
    steps = {}
    for row in trace.splitlines():
        fields = row.split('\t')
        if len(fields) >= 3 and fields[0] == 'T':
            pc = int(fields[2], 16)
            if pc in pcs:
                steps.setdefault(pc, int(fields[1]))
    assert all(pc in steps for pc in pcs)
    assert steps[pcs[0]] == sail_request['fast_reference']['compile_steps']
    assert steps[pcs[0]] < steps[pcs[1]] < steps[pcs[2]]
    console = subprocess.check_output(['python3', '-I', 'scripts/sail_console.py', sail_request['stdout']])
    assert console == b''
    assert 'C11: 86 positive cases, 6 negative cases; all passed' in Path('build/test-c11.log').read_text()
    assert 'check: static stages OK' in Path('build/static-gate.log').read_text()
    assert 'pass=37 fail=0' in Path('build/line-cross-sail.log').read_text()
    assert 'pass=1 fail=0' in Path('build/line-varargs-sail.log').read_text()
    paths = set(request['sha256']) | set(arenas['pins'])
    paths.update(['results/pre/freeze_line_validation.py', 'results/pre/line-arenas.json',
                  'results/pre/line-spec.tsv', 'build/test-c11.log', 'build/test-c11/commands.json',
                  'build/static-gate.log', 'build/line-model-diff.log', 'build/line-pre-diff.log',
                  'build/line-cross-sail.log', 'build/line-varargs-sail.log', 'build/line-sail-ref.log',
                  'build/line-fast.trace', 'build/rv64emu', 'build/image.syms', result['log'],
                  sail_request['stdout'], sail_request['stderr']])
    paths.update(sail_request['sha256'])
    paths.update(str(p) for p in Path('build/test-c11/line').glob('*.h'))
    embedded = []
    for row in Path('c/build/fs.list').read_text().splitlines():
        if row:
            image, host = row.split('=', 1)
            embedded.append(dict(image=image, host=host, sha256=digest(host)))
    evidence = dict(generated_by='results/pre/freeze_line_validation.py',
                    request=request, result=result,
                    sail_request=sail_request, sail_result=sail_result,
                    sail_steps=dict(zip(['compile', 'enter', 'exit_entry'], [steps[pc] for pc in pcs])),
                    runtime_key=arenas['runtime_key'], successful_arenas=len(cases),
                    sha256={p: digest(p) for p in sorted(paths)},
                    fs_list_sha256=digest('c/build/fs.list'), embedded_files=embedded,
                    limitation='Full Sail compiler/program exit; complete arena equality uses independent rv64emu reference. Seven strict CSem macro-location differences retained.')
    Path('results/pre/line-build.json').write_text(json.dumps(evidence, indent=2) + '\n')
    print(json.dumps(evidence['sail_steps']))


if __name__ == '__main__':
    main()
