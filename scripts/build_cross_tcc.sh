#!/bin/bash
# Usage: scripts/build_cross_tcc.sh
#
# Builds, under build/ (gitignored):
#   build/tcc-src/            vendor/tinycc + patches/*.patch (scripts/tcc_src.sh)
#   build/cross/riscv64-tcc   x86_64-hosted cross tcc targeting riscv64 with
#                             -DTCC_RV64I_SOFT (RV64I only, lp64 soft-float)
#   build/cross/riscv64-libtcc1.a
#                             tcc's runtime for rv64i/lp64, compiled BY that
#                             cross tcc (see below)
#
# libtcc1.a contents under TCC_RV64I_SOFT (vs upstream RISCV64_O):
#   builtin.o    __builtin_{ffs,clz,ctz,clrsb,popcount,parity}{,l,ll}: tcc
#                lowers these builtins to calls.
#   alloca.o     alloca (tcc lowers alloca() to a call); RV64I asm.
#   armflush.o   __clear_cache via tcc's __riscv64_clear_cache builtin
#                (fence + fence.i); tcc-only, gcc cannot compile it.
#   dsohandle.o  __dso_handle (hidden), referenced by atexit-style code.
#   dropped:     lib-arm64.o (soft long-double __addtf3 & co: libgcc's
#                rv64i/lp64 libgcc.a provides them, and we want libgcc's
#                routines so the sister projects' proofs apply);
#                atomic.o/stdatomic.o (lr/sc/amo = A extension, and raw RVC
#                words) -- C11 _Atomic / __atomic_* are NOT supported;
#                alloca-bt.o/bcheck (bounds checking is configured off).
# It is compiled with the patched cross tcc, not with gcc, because these
# sources are written for tcc (__TINYC__ paths, the __riscv64_clear_cache
# builtin, tcc-asm .S files) and because it is what upstream does; the
# result is checked RV64I-only by scripts/check_isa.sh.
#
# Link order for programs (tcc -run or tcc -c + ld):
#   <objs> riscv64-libtcc1.a libc.a libgcc.a   (libgcc last: libc and
#   libtcc1 both call its __muldi3/__*df3 helpers)
set -euo pipefail
here=$(cd "$(dirname "$0")/.." && pwd)
JOBS=${JOBS:-8}
src=$here/build/tcc-src
out=$here/build/cross
XPACK=${XPACK:-$(ls -d "$HOME"/toolchains/xpack-riscv-none-elf-gcc-15.2.0-*/bin | head -1)}

"$here/scripts/tcc_src.sh" "$src"
mkdir -p "$out"
cd "$out"
if [ ! -f config.mak ] || [ "$src/.tcc_src_stamp" -nt config.mak ]; then
  "$src/configure" --config-backtrace=no --config-bcheck=no >/dev/null
  rm -f riscv64-tcc riscv64-*.o
fi
make -s -j"$JOBS" riscv64-tcc EXTRA-DEFS=-DTCC_RV64I_SOFT

# libtcc1.a
rm -rf lib1 && mkdir lib1
TCC="$out/riscv64-tcc -std=c11 -B$src -I$src/include -nostdinc"
objs=()
for f in builtin.c alloca.S armflush.c dsohandle.c; do
  o=lib1/${f%.*}.o
  $TCC -c "$src/lib/$f" -o "$o"
  objs+=("$o")
done
rm -f riscv64-libtcc1.a
"$out/riscv64-tcc" -ar rcs riscv64-libtcc1.a "${objs[@]}"
"$here/scripts/check_isa.sh" riscv64-libtcc1.a >/dev/null
echo "built $out/riscv64-tcc and $out/riscv64-libtcc1.a"
