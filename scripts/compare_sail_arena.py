#!/usr/bin/env python3
"""Compare a validated native Sail cut with an independent model arena.

This checks captured bytes and metadata, not kernel-certified reachability.
Use capture_cut.py first; its manifest and all exported files are required.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re

from model_diff import parse_arena


def require(condition, message):
    if not condition:
        raise ValueError(message)


def read_capture(capture, name):
    path = capture / name
    return path.read_bytes() if path.exists() else gzip.decompress((capture / (name + '.gz')).read_bytes())


def compare(capture, model, trace=None):
    manifest = json.loads((capture / 'cut.json').read_text())
    for name, info in manifest['files'].items():
        require(Path(name).name == name, 'invalid capture filename')
        data = read_capture(capture, name)
        require(len(data) == info['bytes'] and
                hashlib.sha256(data).hexdigest() == info['sha256'],
                'capture file changed: ' + name)
    h, body = parse_arena(str(model))
    base, end = manifest['arena_base'], manifest['arena_end']
    require((base, end) == (0x86400000, 0x86800000), 'wrong arena extent')
    require(int(h['base'][0], 16) == base, 'model base differs')
    arena = read_capture(capture, 'arena.bin')
    presence = read_capture(capture, 'arena.present.bin')
    require(len(arena) == len(presence) == end - base, 'incomplete capture')
    require(all(p in (0, 1) for p in presence) and
            sum(presence) == manifest['arena_present_bytes'], 'bad presence map')
    require(not any(b for b, p in zip(arena, presence) if not p), 'absent nonzero byte')
    require(arena == body.ljust(end - base, b'\0'), 'Sail/model arena mismatch')
    registers = {}
    for name, value in re.findall(r'⟨Register\.(\w+), 0x([0-9a-fA-F]+)#64⟩',
                                  (capture / 'registers.txt').read_text()):
        require(name not in registers, 'duplicate register')
        registers[name] = int(value, 16)
    require(registers['PC'] == manifest['pc'], 'cut PC differs')
    expected = {'x10': int(h['entry'][0], 16),
                'x14': int(h['code'][0], 16), 'x15': int(h['code'][1], 16)}
    for reg, value in expected.items():
        require(registers[reg] == value, 'handoff metadata differs: ' + reg)
    if trace is not None:
        rows = [line.split('\t') for line in trace.read_text().splitlines()
                if line.startswith('T\t')]
        cut = [row for row in rows if int(row[2], 16) == manifest['pc']]
        require(len(cut) == 1, 'expected exactly one traced handoff')
        require(int(cut[0][1]) == manifest['steps'], 'trace step differs')
        for reg, value in expected.items():
            require(int(cut[0][int(reg[1:]) + 3], 16) == value, 'trace register differs')
    require(manifest['flags'] == 0 and not (capture / 'console.txt').read_bytes(),
            'unexpected compiler flags or console')
    return dict(status='equal', evidence='native Sail observation; not a theorem',
                compared_bytes=len(arena), model_payload_bytes=len(body),
                arena_sha256=hashlib.sha256(arena).hexdigest(),
                model_arena_sha256=hashlib.sha256(model.read_bytes()).hexdigest(),
                trace_sha256=hashlib.sha256(trace.read_bytes()).hexdigest() if trace else None,
                capture_manifest_sha256=hashlib.sha256((capture / 'cut.json').read_bytes()).hexdigest(),
                elf_sha256=manifest['elf_sha256'], source_sha256=manifest['source_sha256'],
                steps=manifest['steps'], pc=manifest['pc'], handoff=expected)


if __name__ == '__main__':
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--capture', type=Path, required=True)
    ap.add_argument('--model', type=Path, required=True)
    ap.add_argument('--trace', type=Path, help='optional independent pre-instruction trace')
    args = ap.parse_args()
    try:
        print(json.dumps(compare(args.capture, args.model, args.trace), indent=2))
    except (ValueError, RuntimeError, KeyError, OSError, IndexError) as error:
        ap.exit(1, f'sail-arena: {error}\n')
