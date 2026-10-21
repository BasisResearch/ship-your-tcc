#!/bin/bash
# Usage: scripts/test_tcc_tests2.sh [--compile-only]
#
# Extra regression signal for the TCC_RV64I_SOFT code generator: tcc's own
# tests/tests2 programs, compiled by build/cross/riscv64-tcc -c, linked with
# the same harness as scripts/test_tcc_patch.sh, run on the Sail emulator,
# stdout compared with the .expect file (diff -b, as tests2/Makefile does).
# Skipped: tests that need -dt/-b/-run/argv/pthread/TLS/shared libs/two
# files/constructors, or test x86/arm asm (see SKIP below). Our crt0 passes
# argc=0, and stderr shares the console, so a few may still differ for
# harness reasons; failures are listed for inspection.
set -u
here=$(cd "$(dirname "$0")/.." && pwd)
cd "$here"
JOBS=${JOBS:-8}
XPACK=${XPACK:-$(ls -d "$HOME"/toolchains/xpack-riscv-none-elf-gcc-15.2.0-*/bin | head -1)}
EMU=${EMU:-$HOME/Documents/code/ship-your-lua/riscv-lean/lean_emulator/.lake/build/bin/lean_riscv_emulator}
GCC=$XPACK/riscv-none-elf-gcc
OBJDUMP=$XPACK/riscv-none-elf-objdump
NEWLIB_INC=$XPACK/../riscv-none-elf/include
TCC=$here/build/cross/riscv64-tcc
LIBTCC1=$here/build/cross/riscv64-libtcc1.a
SRC=$here/build/tcc-src
T2=$SRC/tests/tests2
H=$here/c/tests/tcc/harness
OUT=$here/build/test-tests2
CFL="-march=rv64i -mabi=lp64 -mcmodel=medany"
MAXSTEPS=${MAXSTEPS:-200000000}
SKIP=" 31_args 34_array_assignment 46_grep 60_errors_and_warnings 85_asm-outside-function 96_nodata_wanted
 98_al_ax_extend 99_fastcall 104_inline 104+_inline 106_versym 108_constructor 112_backtrace 113_btdll
 114_bound_signal 115_bound_setjmp 116_bound_setjmp2 117_builtins 120_alias 120+_alias 124_atomic_counter
 125_atomic_misc 126_bound_global 127_asm_goto 128_run_atexit 132_bound_test 138_arm64_encoding
 139_arm64_errors 140_arm64_extasm 141_riscv_asm 144_tls 145_winarm64_interlocked 146_tls_extern
 148_linker_symbols 95_bitfields_ms 119_random_stuff 136_atomic_gcc_style 122_vla_reuse"
# 119: needs sys/mman.h (not in newlib); 136: C11/__atomic (needs the A
# extension, not in our libtcc1.a); 122: 100000 loop iterations with 4 soft
# divisions each, >200M Sail steps.
SKIP=" $(echo $SKIP) "

[ -x "$TCC" ] || scripts/build_cross_tcc.sh || exit 1
mkdir -p "$OUT"
$GCC $CFL -c "$H/crt0.S" -o "$OUT/crt0.o" || exit 1
$GCC $CFL -O2 -DTCC_HTIF -c "$H/htif.c" -o "$OUT/htif.o" || exit 1
names=(); skipped=0
for f in "$T2"/*.c; do
  n=$(basename "$f" .c)
  case "$SKIP" in *" $n "*) skipped=$((skipped+1)); continue;; esac
  [ -f "$T2/$n.expect" ] || continue
  names+=("$n")
  rm -f "$OUT/$n".*
  fl=""; [ "$n" = 76_dollars_in_identifiers ] && fl=-fdollars-in-identifiers
  if (cd "$T2" && "$TCC" -std=c11 $fl -nostdinc -I"$SRC/include" -I"$NEWLIB_INC" -I. -c "$n.c" -o "$OUT/$n.o") > "$OUT/$n.cc" 2>&1; then
    scripts/check_isa.sh -q "$OUT/$n.o" > "$OUT/$n.isa" 2>&1; echo $? > "$OUT/$n.isa.rc"
    $GCC $CFL -nostartfiles -T "$H/link.ld" -Wl,--no-warn-rwx-segments -o "$OUT/$n.elf" "$OUT/crt0.o" "$OUT/htif.o" "$OUT/$n.o" "$LIBTCC1" -lm -lc -lgcc >> "$OUT/$n.cc" 2>&1 \
      && { scripts/check_isa.sh -q "$OUT/$n.elf" > "$OUT/$n.elfisa" 2>&1; echo $? > "$OUT/$n.elfisa.rc"; }
  fi
done
# Compiler/preprocessor work can exercise the whole existing compilation,
# link and ISA lane without repeating long floating-point Sail executions.
if [ "${1:-}" = --compile-only ]; then
  pass=0; fail=0; failed=""
  for n in "${names[@]}"; do
    if [ -f "$OUT/$n.elf" ] && [ "$(cat "$OUT/$n.isa.rc")" = 0 ] \
       && [ "$(cat "$OUT/$n.elfisa.rc")" = 0 ]; then
      pass=$((pass+1))
    else
      fail=$((fail+1)); failed="$failed $n"
    fi
  done
  echo "tests2 compile/link/ISA: attempted=${#names[@]} pass=$pass fail=$fail skipped=$skipped"
  echo "failed:$failed"
  [ "$fail" = 0 ]; exit $?
fi

run1() {
  n=$1; elf=$OUT/$n.elf
  [ -f "$elf" ] || exit 0
  pc=$("$OBJDUMP" -d "$elf" | awk '/<_exit>:$/ {f=1; next} f && $3 == "sd" {sub(":", "", $1); print $1; exit}')
  printf '0x%s\n' "$pc" > "$OUT/$n.pcs"
  timeout 1200 "$EMU" "$elf" --trace-pcs "$OUT/$n.pcs" --max-steps "$MAXSTEPS" > "$OUT/$n.raw" 2> "$OUT/$n.trace"; echo $? > "$OUT/$n.rc"
  grep -vE '^(TODO: cancel_reservation|PC = 0x|htif_tohost = 0x|SUCCESS$|FAILURE)' "$OUT/$n.raw" > "$OUT/$n.out"
  awk -F'\t' '$1=="T"{s=$2+1} END{print s+0}' "$OUT/$n.trace" > "$OUT/$n.steps"
}
export -f run1; export OUT EMU OBJDUMP MAXSTEPS
printf '%s\n' "${names[@]}" | xargs -P "$JOBS" -I{} bash -c 'run1 {}'
pass=0; fail=0; ccfail=0; isabad=0; failed=""
for n in "${names[@]}"; do
  if [ ! -f "$OUT/$n.elf" ]; then ccfail=$((ccfail+1)); failed="$failed $n(build)"; continue; fi
  [ "$(cat "$OUT/$n.isa.rc")" = 0 ] && [ "$(cat "$OUT/$n.elfisa.rc")" = 0 ] \
    || { isabad=$((isabad+1)); failed="$failed $n(ISA)"; }
  if ! { [ -f "$OUT/$n.rc" ] && [ "$(cat "$OUT/$n.rc")" != 124 ] \
      && [ -f "$OUT/$n.steps" ] && [ "$(cat "$OUT/$n.steps")" -gt 0 ] \
      && ! grep -q 'TRACE-FUEL-OUT' "$OUT/$n.trace"; }; then
    fail=$((fail+1)); failed="$failed $n(incomplete)"; continue
  fi
  # tcc -run prints its warnings before the program output: prepend ours
  { grep "^$n.c:" "$OUT/$n.cc"; cat "$OUT/$n.out"; } > "$OUT/$n.cmp"
  if diff -b "$T2/$n.expect" "$OUT/$n.cmp" > "$OUT/$n.diff"; then pass=$((pass+1)); else fail=$((fail+1)); failed="$failed $n"; fi
done
echo "tests2: run=${#names[@]} pass=$pass fail=$fail build-fail=$ccfail isa-bad=$isabad skipped=$skipped"
echo "failed:$failed"
[ $fail = 0 ] && [ $ccfail = 0 ] && [ $isabad = 0 ]
