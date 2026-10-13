#!/bin/bash
# Clone (if needed), configure and build Certora/clean's ccomp + clightgen.
#   - opam switch `certora-clean` (OCaml 4.14.2, Coq/Rocq 9.1.0, Menhir 20260209)
#     created with:  opam switch create certora-clean ocaml-base-compiler.4.14.2 \
#                      --no-switch --repos=default,rocq-released
#                    opam install --switch=certora-clean coq.9.1.0 menhir.20260209
#   - target rv64-linux, preprocessor = xpack riscv-none-elf-gcc with
#     -march=rv64i -mabi=lp64, i.e. newlib headers exactly as the RV64I tcc sees them.
#   - installed into build/certora/install (bin/ccomp, bin/clightgen, compcert.ini,
#     lib/compcert/include = CompCert's own stdarg.h/stddef.h/float.h/...).
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
root=$(cd "$here/.." && pwd)
CERT="$root/build/certora"
SRC="$CERT/src"
COMMIT=123f7f7767e215aceb4821f0d044336d78103ade
TOOLPREFIX=${TOOLPREFIX:-$HOME/toolchains/xpack-riscv-none-elf-gcc-15.2.0-1/bin/riscv-none-elf-}
mkdir -p "$CERT/logs"
if [ ! -d "$SRC" ]; then
  git clone https://github.com/Certora/clean.git "$SRC"
  git -C "$SRC" checkout -q "$COMMIT"
fi
# local patches to the driver (ccomp -interp printf); idempotent
for p in "$root"/patches/certora/*.patch; do
  [ -e "$p" ] || continue
  if git -C "$SRC" apply --reverse --check "$p" 2>/dev/null; then :; else git -C "$SRC" apply "$p"; fi
done
cd "$SRC"
opam exec --switch=certora-clean -- ./configure -prefix "$CERT/install" \
  -toolprefix "$TOOLPREFIX" -clightgen rv64-linux
# preprocess like riscv-none-elf-gcc -march=rv64i -mabi=lp64 (the RV64I/lp64 newlib
# multilib tcc uses); the assembler/linker keep rv64gc/lp64d because CompCert's
# own RISC-V backend emits M/F/D instructions (only matters for the runtime lib).
sed -i -e 's/-mabi=lp64d/-march=rv64gc -mabi=lp64d/g' Makefile.config
sed -i -e 's/^CPREPRO_OPTIONS=.*/CPREPRO_OPTIONS=-march=rv64i -mabi=lp64 -U__GNUC__ -E/' Makefile.config
start=$(date +%s)
systemd-run --user --scope -q -p MemoryMax=12G \
  opam exec --switch=certora-clean -- make -j8 all
systemd-run --user --scope -q -p MemoryMax=12G \
  opam exec --switch=certora-clean -- make install
echo "CompCert build+install took $(( $(date +%s) - start ))s"
