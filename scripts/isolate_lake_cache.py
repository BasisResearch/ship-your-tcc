#!/usr/bin/env python3
"""Detach inherited hard-linked Lake outputs before building a worktree.

Hold the project's shared build lock while running this tool. Lake rewrites
some trace files in place but replaces object files: sharing their inodes
can therefore make a trace certify a different checkout's stale object.
No source or generated proof file is changed. Reflinks are used if available.
"""
import argparse
import fcntl
import os
from pathlib import Path
import shutil
import tempfile


def isolate(root):
    build = root / '.lake/build'
    count = size = 0
    for path in build.rglob('*'):
        if path.is_symlink() or not path.is_file():
            continue
        st = path.stat()
        if st.st_nlink <= 1:
            continue
        fd, tmp = tempfile.mkstemp(prefix='.isolate-', dir=path.parent)
        try:
            with os.fdopen(fd, 'wb') as dst, path.open('rb') as src:
                try:
                    fcntl.ioctl(dst.fileno(), 0x40049409, src.fileno())  # FICLONE
                except OSError:
                    shutil.copyfileobj(src, dst)
            shutil.copystat(path, tmp)
            os.replace(tmp, path)
        finally:
            if os.path.exists(tmp):
                os.unlink(tmp)
        count += 1
        size += st.st_size
    print(f'{root}: detached {count} artifacts ({size // 1048576} MiB)')
    return count


def invalidate_csem(root):
    count = 0
    for source in ['csem/lemlib', 'csem/lean_frontend/generated',
                   'csem/lean_frontend/test', 'csem/cparser', 'Tcc/CSem']:
        base = root / source
        for path in base.rglob('*.lean'):
            rel = path.relative_to(root if source == 'Tcc/CSem' else base)
            trace = (root / '.lake/build/lib/lean' / rel).with_suffix('.trace')
            if trace.exists():
                trace.unlink()
                count += 1
    print(f'{root}: invalidated {count} CSem traces; sum artifacts untouched')


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--invalidate-csem', action='store_true')
    ap.add_argument('roots', nargs='+', type=Path)
    args = ap.parse_args()
    for root in args.roots:
        root = root.resolve()
        if not (root / 'BRIEF.md').is_file() or not (root / 'Tcc').is_dir():
            ap.error(f'not a ship-your-tcc checkout: {root}')
        detached = isolate(root)
        if args.invalidate_csem or detached:
            invalidate_csem(root)


if __name__ == '__main__':
    main()
