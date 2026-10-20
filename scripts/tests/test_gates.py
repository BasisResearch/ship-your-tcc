#!/usr/bin/env python3
"""Exercise ISA and completion gates with fake compilers and emulators."""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile


def main():
    with tempfile.TemporaryDirectory(prefix="tcc-gate-regression-") as scratch:
        root = Path(scratch)
        source = Path(__file__).resolve().parents[2]
        def write(path, data):
            p = root / path
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_text(data)
            p.chmod(0o755)
        for name in ['check.sh', 'test_tcc_patch.sh', 'test_tcc_tests2.sh']:
            write('scripts/' + name, (source / 'scripts' / name).read_text())
        write('scripts/check_discipline.py', 'print("discipline: fixture")\n')
        write('abstractions/gate.py', 'print("abstraction: fixture")\n')
        write('scripts/check_isa.sh', '''#!/bin/bash
        for f in "$@"; do
          case "$f" in
            *.elf) [ "${STUB_ISA:-}" != elf ] || exit 1 ;;
            *.o) [ "${STUB_ISA:-}" != object ] || exit 1 ;;
          esac
        done
        exit 0
        ''')
        compiler = '''#!/bin/bash
        out=""; prev=""
        for arg in "$@"; do
          [ "$prev" != -o ] || out="$arg"
          prev="$arg"
        done
        [ -n "$out" ] || exit 1
        [ "${STUB_BUILD_FAIL:-}" != 1 ] || case "$out" in *.tcc.o|*/01_probe.o) exit 1;; esac
        mkdir -p "$(dirname "$out")"
        case "$out" in
          *.native) printf '#!/bin/bash\\necho ok\\nexit "${STUB_EXIT:-0}"\\n' > "$out"; chmod +x "$out" ;;
          *) touch "$out" ;;
        esac
        '''
        write('tools/riscv-none-elf-gcc', compiler)
        write('tools/native', compiler)
        write('build/cross/riscv64-tcc', compiler)
        write('build/cross/riscv64-libtcc1.a', '')
        write('tools/riscv-none-elf-objdump', '#!/bin/bash\nprintf "00000000 <_exit>:\\n  10:   00000000 sd\\n"\n')
        write('tools/emulator', '''#!/bin/bash
        if [ "${STUB_OUTPUT:-}" = mismatch ]; then echo wrong; else echo ok; fi
        case "${STUB_MODE:-normal}" in
         timeout) exit 124 ;;
         fuel) echo TRACE-FUEL-OUT >&2; exit 0 ;;
         incomplete) exit 0 ;;
         *) printf 'T\\t100\\t10\\n' >&2; exit "${STUB_EXIT:-0}" ;;
        esac
        ''')
        write('build/tcc-src/tests/tests2/01_probe.c', 'int main(void) { return 0; }\n')
        write('build/tcc-src/tests/tests2/01_probe.expect', 'ok\n')
        write('probe.c', 'int main(void) { return 0; }\n')
        env = dict(os.environ, XPACK=str(root/'tools'), EMU=str(root/'tools/emulator'), NATIVE_CC=str(root/'tools/native'), JOBS='1')
        checks = 0
        def run(script, good, extra=None, full=False):
            nonlocal checks
            r = subprocess.run(['bash', 'scripts/'+script] + (['probe.c'] if script == 'test_tcc_patch.sh' else ['--static-only'] if script == 'check.sh' and not full else []), cwd=root, env=dict(env, **(extra or {})), capture_output=True, text=True, timeout=10)
            assert (r.returncode == 0) == good, (script, extra, r.returncode, r.stdout, r.stderr)
            checks += 1
        for script in ['test_tcc_patch.sh', 'test_tcc_tests2.sh']:
            run(script, True)
            run(script, True, {'STUB_EXIT':'6'})
            run(script, False, {'STUB_ISA':'elf'})
            run(script, False, {'STUB_ISA':'object'})
            run(script, False, {'STUB_BUILD_FAIL':'1'})
            for mode in ['timeout', 'fuel', 'incomplete']:
                run(script, False, {'STUB_MODE':mode})
        run('test_tcc_tests2.sh', False, {'STUB_OUTPUT':'mismatch'})
        run('check.sh', True)
        run('check.sh', False, {'STUB_ISA':'elf'})
        run('check.sh', False, {'STUB_ISA':'object'})
        # Independently verify the image is checked even without corpus artifacts.
        shutil.rmtree(root/'build/test-tcc')
        shutil.rmtree(root/'build/test-tests2')
        write('c/build/base.elf', '')
        run('check.sh', False, {'STUB_ISA':'elf'})
        # Exercise stage 6 with fake Lake/systemd commands; no Lean build runs.
        write('lakefile.toml', '')
        write('scripts/axioms.lean', '-- fixture\n')
        write('tools/systemd-run', '#!/bin/bash\nexit 0\n')
        write('tools/lake', '''#!/bin/bash
        case "${STUB_AXIOMS:-standard}" in
          failure) echo 'error: unknown theorem'; exit 1 ;;
          partial_failure) echo "'good' depends on axioms: [propext]"; exit 1 ;;
          missing) echo 'warning: no audit commands' ;;
          empty_output) exit 0 ;;
          no_axioms) echo "'good' does not depend on any axioms" ;;
          empty_list) echo "'good' depends on axioms: []" ;;
          forbidden) echo "'bad' depends on axioms: [propext, sorryAx]" ;;
          mixed) echo "'good' depends on axioms: []"; echo "'bad' depends on axioms: [sorryAx]" ;;
          *) echo "'good' depends on axioms: [propext, Classical.choice, Quot.sound]" ;;
        esac
        ''')
        env['PATH'] = str(root/'tools') + os.pathsep + os.environ['PATH']
        for mode in ['standard', 'no_axioms', 'empty_list']:
            run('check.sh', True, {'STUB_AXIOMS':mode}, full=True)
        for mode in ['failure', 'partial_failure', 'missing', 'empty_output', 'forbidden', 'mixed']:
            run('check.sh', False, {'STUB_AXIOMS':mode}, full=True)
        print(f'{checks} lightweight gate regressions passed')


if __name__ == "__main__":
    main()
