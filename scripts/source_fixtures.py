"""Corpus files embedded beside /prog.c, independent of any compiler output."""
from pathlib import Path
import re


def fixture_files(path):
    path = Path(path)
    names = re.findall(rb'#\s*include\s*"([^"/]+)"', path.read_bytes())
    direct = [path.parent/n.decode() for n in names if (path.parent/n.decode()).is_file()]
    return sorted({path, *path.parent.glob('*.h'), *direct})


def fixture_entries(path):
    """Retain basename self-includes and ../<directory>/header fixture aliases.

    The caller installs /prog.c first and chooses its image-file precedence.
    The same explicit input files must be passed to every compared engine.
    """
    path = Path(path)
    return [(virtual, host) for host in fixture_files(path)
            for virtual in ['/'+host.name, '/'+path.parent.name+'/'+host.name]]


def load_invocation(manifest, root):
    """Read an explicit, byte-pinned multi-TU invocation; never concatenate units.

    Paths are relative to the repository root. before_run preserves the exact
    order of companion sources preceding `-run /prog.c`. This deliberately
    does not guess build flags from filenames or change standalone corpus runs.
    """
    import hashlib
    import json
    manifest, root = Path(manifest).resolve(), Path(root).resolve()
    raw_manifest = manifest.read_bytes()
    data = json.loads(raw_manifest)
    if set(data) != {'version', 'source', 'source_sha256', 'before_run', 'provenance'} or data['version'] != 1:
        raise ValueError('unsupported invocation manifest schema')
    pins = {str(manifest): hashlib.sha256(raw_manifest).hexdigest()}
    def pinned(name, digest):
        p = (root / name).resolve()
        if not p.is_relative_to(root) or not p.is_file():
            raise ValueError('invocation source must exist inside repository: ' + name)
        actual = hashlib.sha256(p.read_bytes()).hexdigest()
        if actual != digest:
            raise ValueError('invocation source hash mismatch: ' + name)
        pins[str(p)] = actual
        return p
    source = pinned(data['source'], data['source_sha256'])
    provenance = data['provenance']
    if set(provenance) != {'path', 'sha256'}:
        raise ValueError('invocation provenance requires path and sha256')
    pinned(provenance['path'], provenance['sha256'])
    entries = {}
    def add(virtual, host):
        host = str(host)
        if virtual in entries and entries[virtual] != host:
            raise ValueError('conflicting invocation image path: ' + virtual)
        entries[virtual] = host
    for virtual, host in fixture_entries(source):
        add(virtual, host)
    args = ['-w']
    if not isinstance(data['before_run'], list) or not data['before_run']:
        raise ValueError('invocation requires at least one companion')
    for unit in data['before_run']:
        if set(unit) != {'image_path', 'source', 'sha256'}:
            raise ValueError('companion requires image_path, source and sha256')
        virtual = unit['image_path']
        # gen_embed carries argv as C literals and file pairs as shell words.
        if not isinstance(virtual, str) or not re.fullmatch(r'/[A-Za-z0-9_+./-]+\.c', virtual) or '..' in virtual.split('/') or virtual == '/prog.c':
            raise ValueError('invalid companion image path')
        if virtual in args:
            raise ValueError('duplicate companion argument')
        host = pinned(unit['source'], unit['sha256'])
        for alias, fixture in fixture_entries(host):
            add(alias, fixture)
        add(virtual, host)
        args.append(virtual)
    for host in entries.values():
        pins[host] = hashlib.sha256(Path(host).read_bytes()).hexdigest()
    return {'source': str(source), 'extra': list(entries.items()), 'args': args,
            'pins': pins, 'argv': ['tcc', '-std=c11', *args, '-run', '/prog.c']}
