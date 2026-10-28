"""Verify the portable evidence archive, finite counts and actual native images."""
from pathlib import Path
import hashlib
import json
import lzma
import tarfile

ROOT = Path(__file__).resolve().parent
H = lambda b: hashlib.sha256(b).hexdigest()
manifest = json.loads((ROOT / 'manifest.json').read_text())
archive = ROOT / 'evidence.tar.xz'
assert H(archive.read_bytes()) == manifest['archive_sha256']
kept = {}
seen = set()
with tarfile.open(archive, 'r:xz') as tar:
    for member in tar:
        assert member.isfile(), member.name
        assert member.name not in seen and member.name in manifest['files'], member.name
        seen.add(member.name)
        raw = tar.extractfile(member).read()
        assert len(raw) == manifest['files'][member.name]['bytes'], member.name
        assert H(raw) == manifest['files'][member.name]['sha256'], member.name
        if member.name.endswith(('/report.json', '/receipt.json', '/input.c', '/prog.elf.xz')) or member.name in manifest['keep']:
            kept[member.name] = raw
assert seen == set(manifest['files'])
for name, record in manifest['native'].items():
    image = lzma.decompress(kept[name])
    source = kept[record['source_path']]
    assert image.startswith(b'\x7fELF') and H(image) == record['elf_sha256'], name
    assert H(source) == record['source_sha256'], name
    assert image[record['source_offset']:record['source_offset'] + len(source)] == source, name
    receipt = json.loads(kept[record['receipt_path']])
    assert receipt['exit'] == 0 and receipt['source_sha256'] == H(source), name

def report(path):
    return json.loads(kept[path])

focused = report('build/bitattr-focused-1/report.json')
assert focused['counts'] == {'DEFINED_ZERO': 46, 'GCC_SYNTAX_DIAGNOSTIC': 8}
assert len(focused['rows']) == 54 and all(r['full_text_cabs'] for r in focused['rows'])
layouts = [r for r in focused['rows'] if 'layout_equal' in r]
assert len(layouts) == 42 and all(r['layout_equal'] for r in layouts)
assert report('build/bitattr-oracle-cabs-final/report.json')['counts'] == {
    'EXACT_CABS': 46, 'MENHIR_PLACEMENT_LIMITATION': 8}
bitfields = report('build/bitattr-c11-bitfields-final/report.json')['rows']
assert len(bitfields) == 90 and all(r['ok'] for r in bitfields)
continuation = report('build/bitattr-continuation-final/report.json')['rows']
assert len(continuation) == 58 and all(r['ok'] for r in continuation)
assert report('build/bitattr-alignment-final/report.json')['counts'] == {
    'CONSTRAINT_DIAGNOSTIC': 12, 'DEFINED_ZERO': 54,
    'FUNCTION_TYPE_POLICY_OBSTRUCTION': 2, 'OCAML_PLACEMENT_LIMITATION': 3}
assert report('build/bitattr-native-after/receipt.json')['console_equal']
after = kept['build/bitattr-native-after/stdout']
assert after == kept['build/bitattr-native-after/gcc.stdout']
before_rows = kept['build/bitattr-native-before/stdout'].splitlines()
after_rows = after.splitlines()
assert len(before_rows) == len(after_rows) == 42
assert sum(a != b for a, b in zip(before_rows, after_rows)) == 24
assert kept['build/bitattr-native-before/input.c'] == kept['build/bitattr-native-after/input.c']
for name in ('gnu_member_aligned_auto', 'gnu_member_aligned_static',
             'gnu_member_packed_auto', 'gnu_member_packed_static'):
    assert report('build/bitattr-native-original-' + name + '/receipt.json')['console_equal']
gate = report('build/worker-build.bitattr-full-1.result')
assert gate['request_id'] == 'bitattr-full-1' and gate['exit'] == 0 and gate['source_hashes_verified']
print('verified', len(seen), 'artifacts,', len(manifest['native']),
      'actual native ELFs, complete Cabs and all stated finite check counts')
