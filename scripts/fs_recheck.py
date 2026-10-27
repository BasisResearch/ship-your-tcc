#!/usr/bin/env python3
"""Recheck the current CSem image configuration against retained Sail observations.

This never reruns or changes Sail. It verifies source, embedded-file and base
image identity with the original pinned batch, then records new CSem evidence.
"""
import hashlib
import json
import os
from pathlib import Path
import subprocess
import fs_diff as fs


def main():
    root = Path(__file__).resolve().parents[1]
    ref = root / 'results/csem/fs-probes'
    pins = json.loads((ref/'pins.json').read_text())
    main_root = Path(subprocess.check_output(['git', 'rev-parse', '--path-format=absolute', '--git-common-dir'], cwd=root, text=True).strip()).parent
    fslist = main_root/'c/build/fs.list'
    required = [main_root/'c/build/base.elf', fslist]
    for line in fslist.read_text().splitlines():
        if line and not line.startswith('#'):
            host = Path(line.split('=', 1)[1])
            required.append(host if host.is_absolute() else main_root/'c'/host)
    for p in required:
        assert fs.sha(p) == pins[str(p)], f'changed reference image/file: {p}'
    dest = root/'results/csem/fs-rv64-recheck'
    dest.mkdir(exist_ok=True)
    subprocess.run([str(root/'scripts/csem_image_libc_prep.sh')], cwd=root, check=True, stdout=subprocess.DEVNULL)
    current = required + [root/'.lake/build/bin/csem_run', root/'scripts/csem_run_image.sh',
                          Path(__file__), root/'scripts/fs_diff.py', ref/'pins.json']
    current += [p for p in (root/'csem/runtime').rglob('*') if p.is_file()]
    current += [root/'build/csem-image-libc/libc.core']
    current += list((root/'c/tests/fs').glob('*.c')) + [p for p in ref.iterdir() if p.is_file()]
    frozen = {str(p):fs.sha(p) for p in sorted(set(current))}
    rows = []
    for src in sorted((root/'c/tests/fs').glob('*.c')):
        assert fs.sha(src) == pins[str(src)], f'changed source: {src}'
        old = json.loads((ref/(src.stem+'.json')).read_text())
        assert old['source_sha256'] == fs.sha(src)
        cmd = [str(root/'scripts/csem_run_image.sh'), '--first', str(src)]
        rc, out, err = fs.run(cmd, dict(os.environ, CSEM_IMAGE_FS_LIST=str(fslist)), 180)
        lines = [l for l in out.decode().splitlines() if not l.startswith('EXECUTION ')]
        m = fs.DEFINED.fullmatch(lines[0]) if len(lines) == 1 else None
        observed = ref/(src.stem+'.sail.out')
        meta = ref/(src.stem+'.sail.meta')
        status = 'FAIL'
        if old['status'] == 'AGREE_ONE_BRANCH' and 'program_steps=' in meta.read_text() and rc == 0 and m:
            if fs.unescape(m[2]) == observed.read_bytes() and fs.unescape(m[3]) == b'' and int(m[1]) % 256 == old['sail_exit']:
                status = 'AGREE_ONE_BRANCH'
        elif src.stem == 'mkdir' and old['status'] == 'MISSING_LIBRARY' and rc != 0 and b'CerbFS refusal' in err:
            status = 'MISSING_LIBRARY'
        (dest/(src.stem+'.lean.out')).write_bytes(out)
        (dest/(src.stem+'.lean.err')).write_bytes(err)
        rows.append(dict(probe=src.stem, status=status, command=cmd, lean_exit=rc,
                         sail_exit=old['sail_exit'], source_sha256=fs.sha(src),
                         sail_stdout_sha256=fs.sha(observed), sail_meta_sha256=fs.sha(meta)))
        print(src.stem, status, flush=True)
    assert all(fs.sha(Path(p)) == h for p,h in frozen.items()), 'input drift during recheck'
    (dest/'summary.json').write_text(json.dumps(dict(rows=rows, sha256=frozen),indent=2)+'\n')
    return 0 if len(rows)==9 and sum(r['status']=='AGREE_ONE_BRANCH' for r in rows)==8 and sum(r['status']=='MISSING_LIBRARY' for r in rows)==1 else 1


if __name__ == '__main__':
    raise SystemExit(main())
