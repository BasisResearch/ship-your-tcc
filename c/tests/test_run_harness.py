#!/usr/bin/env python3
"""Exercise Phase 1 acceptance/failure paths with tiny tool/emulator stubs.

No compiler, toolchain, Sail run, or existing build tree is needed.
Run: python3 c/tests/test_run_harness.py
"""
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

STUB = r'''#!/usr/bin/env python3
import os, pathlib, sys
name = pathlib.Path(sys.argv[0]).name
args = sys.argv[1:]
mode = os.environ.get('CASE', 'ok')
def write(p, data):
    p = pathlib.Path(p); p.parent.mkdir(parents=True, exist_ok=True); p.write_bytes(data)
if name == 'make':
    if mode == 'build': sys.exit(1)
    write(args[-1], b'ELF')
elif name == 'hostcc':
    out = args[args.index('-o') + 1]
    rc = 6 if 'six' in out else 3 if 'three' in out else 0
    write(out, ('#!/bin/sh\nprintf "hello\\n"\nexit %d\n' % rc).encode())
    pathlib.Path(out).chmod(0o755)
elif name.endswith('-nm'):
    print('80000100 T tcc_baremetal_run\n80000200 T tcc_baremetal_enter')
elif name.endswith('-objdump'):
    print('80000300: 00 sd a5,0(a4)')
elif name.endswith('-readelf'):
    print('  [ 1] .text PROGBITS 80000000 000100 000004 00 AX 0 0 4')
elif name.endswith('-objcopy'):
    src, out = args[-2:]
    data = b'runtime'
    if mode == 'hash' and 'second' in src: data += b'changed'
    write(out, data)
elif name == 'isa':
    sys.exit(mode == 'isa')
elif name == 'emu':
    rc = 6 if 'six' in args[0] else 3 if 'three' in args[0] else 0
    def row(step, pc, a4=0, a5=0):
        regs = ['0'] * 31; regs[13] = format(a4, 'x'); regs[14] = format(a5, 'x')
        print('\t'.join(['T', str(step), pc, '0'] + regs), file=sys.stderr)
    row(0, '80000000')
    row(10, '80000100', 0x86400000, 4)
    if mode != 'marker': row(9 if mode == 'order' else 20, '80000200')
    row(30, '80000300', 0, 2*rc + 1)
    print('@@TCCBM code 0x86400000 00000004\n@@13000000\nhello')
    sys.exit(1 if mode == 'emufail' else rc)
'''


def main():
    harness = Path(__file__).with_name('run.sh')
    with tempfile.TemporaryDirectory(prefix='phase1-harness-test.') as td:
        root = Path(td)
        for d in ('c/tests/run', 'scripts', 'bin'):
            (root / d).mkdir(parents=True)
        shutil.copy(harness, root / 'c/tests/run.sh')
        shutil.copy(harness.parents[2] / 'scripts/sail_console.py', root / 'scripts/sail_console.py')
        stub = root / 'bin/stub'
        stub.write_text(STUB)
        stub.chmod(0o755)
        for name in ('make', 'hostcc', 'emu', 'isa', 'riscv-none-elf-nm',
                     'riscv-none-elf-objdump', 'riscv-none-elf-objcopy', 'riscv-none-elf-readelf'):
            (root / 'bin' / name).symlink_to(stub)
        # The harness invokes both checkers through their normal interfaces.
        for p in ('scripts/check_isa.sh', 'c/tests/isa_check.sh'):
            (root / p).write_text('#!/bin/sh\nexec "$XB/isa" "$@"\n')
            (root / p).chmod(0o755)
        env = dict(os.environ, PATH=str(root / 'bin') + os.pathsep + os.environ['PATH'],
                   XB=str(root / 'bin'), EMU=str(root / 'bin/emu'), HOSTCC='hostcc', JOBS='1')
        def run(case, names, expected, **extra):
            env.update(CASE=case)
            args = []
            for name in names:
                p = root / 'c/tests/run' / (name + '.c'); p.write_text('/* stub input */\n')
                args.append('tests/run/' + p.name)
            result = subprocess.run(['bash', 'tests/run.sh', *args], cwd=root / 'c',
                                    env=dict(env, **extra), stdout=subprocess.PIPE,
                                    stderr=subprocess.STDOUT, text=True)
            if result.returncode != expected:
                raise AssertionError((case, result.returncode, result.stdout))
            print(case + ': ok')
        run('ok', ['zero', 'six', 'three'], 0)
        tsv = (root / 'results/phase1.tsv').read_text()
        assert 'six\tPASS\t6\t6' in tsv and 'three\tPASS\t3\t3' in tsv
        for case in ('isa', 'marker', 'order', 'emufail'):
            run(case, ['zero'], 1)
            assert (root / 'results/phase1.tsv').read_text().splitlines()[-1].split('\t')[1] == 'FAIL'
        run('hash', ['zero', 'second'], 1)
        # A failed rebuild of a previously successful case must not reuse it.
        run('ok', ['zero'], 0)
        run('build', ['zero'], 1)
        run('ok', ['zero'], 1, NORUN='1')  # no last run remains after failed build
        print('Phase 1 harness regression checks passed')


if __name__ == '__main__':
    main()
