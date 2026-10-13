#!/bin/bash
# Usage: scripts/check_patch_neutral.sh [files.c...]
#
# Checks that patches/*.patch change nothing unless TCC_RV64I_SOFT is
# defined: builds riscv64-tcc WITHOUT the macro from the pristine
# vendor/tinycc and from the patched tree (build/regress/{pristine,patched})
# and requires byte-identical objects for every file (default: the corpus).
set -euo pipefail
here=$(cd "$(dirname "$0")/.." && pwd)
cd "$here"
XPACK=${XPACK:-$(ls -d "$HOME"/toolchains/xpack-riscv-none-elf-gcc-15.2.0-*/bin | head -1)}
INC="-nostdinc -I$here/build/tcc-src/include -I$XPACK/../riscv-none-elf/include"
scripts/tcc_src.sh build/tcc-src
for v in pristine patched; do
  s=$here/vendor/tinycc; [ $v = patched ] && s=$here/build/tcc-src
  mkdir -p build/regress/$v
  ( cd build/regress/$v && { [ -f config.mak ] || "$s/configure" --config-backtrace=no --config-bcheck=no >/dev/null; } \
    && make -s -j8 riscv64-tcc )
done
mkdir -p build/regress/o
files=("$@"); [ ${#files[@]} -eq 0 ] && files=(c/tests/tcc/t*.c)
nd=0
for f in "${files[@]}"; do
  n=$(basename "$f" .c)
  build/regress/pristine/riscv64-tcc $INC -c "$f" -o build/regress/o/$n.pristine.o
  build/regress/patched/riscv64-tcc $INC -c "$f" -o build/regress/o/$n.patched.o
  cmp -s build/regress/o/$n.pristine.o build/regress/o/$n.patched.o || { echo "DIFF $n"; nd=$((nd+1)); }
done
echo "neutral check: ${#files[@]} files, $nd differ"
[ $nd = 0 ]
