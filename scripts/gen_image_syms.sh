#!/bin/bash
# The image addresses tcc's -run link uses (F3, Tcc/Model/Run.lean): the
# porting layer patches/0002 adds with tcc_add_symbol (c/src/main.c's
# tcc_baremetal_syms: errno, _read, ..., mkdir, tcc_baremetal_sync_icache),
# tccrun.c's rt_exit ("__rt_exit") and the arena (__tccrun_start/end).
# They depend only on the runtime, which is the same for every program
# (c/tests/run.sh's runtime_sha).
#
#   scripts/gen_image_syms.sh [c/build/base.elf] > build/image.syms
#
# Output: one "name hexaddress" per line.
set -euo pipefail
here=$(cd "$(dirname "$0")/.." && pwd)
elf=${1:-$here/c/build/base.elf}
NM=$(ls -d "$HOME"/toolchains/xpack-riscv-none-elf-gcc-15.2.0-*/bin | head -1)/riscv-none-elf-nm
names="errno _read _write _open _close _lseek _fstat _stat _isatty _link _unlink _sbrk _exit _kill _getpid _gettimeofday _times mkdir tcc_baremetal_sync_icache rt_exit __tccrun_start __tccrun_end"
"$NM" "$elf" | awk -v want="$names" '
  BEGIN { n = split(want, w, " "); for (i = 1; i <= n; i++) need[w[i]] = 1 }
  NF == 3 && ($3 in need) && !($3 in seen) { seen[$3] = 1; print $3, $1 }
  END { for (k in need) if (!(k in seen)) { print "missing symbol " k > "/dev/stderr"; bad = 1 }
        exit bad }'
