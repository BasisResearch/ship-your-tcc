#!/bin/bash
# Usage: scripts/test_tcc_patch.sh [c/tests/tcc/tNN_*.c ...]
#
# Differential test of the TCC_RV64I_SOFT code generator. For each program:
#   1. build/cross/riscv64-tcc -c (tcc's include/ + the xpack newlib headers)
#   2. scripts/check_isa.sh on the tcc object (RV64I only, soft-float ELF flags)
#   3. link with xpack gcc (-march=rv64i -mabi=lp64 -mcmodel=medany
#      -nostartfiles -T harness/link.ld) against harness/crt0.S + htif.c
#      (from ship-your-lua), riscv64-libtcc1.a, newlib -lm -lc and -lgcc,
#      and check_isa the whole linked ELF too
#   4. the same program compiled by xpack gcc -O0 for rv64i/lp64 with the
#      same harness and newlib: the primary oracle (same printf, same
#      libgcc soft-float)
#   5. host gcc -O0 -funsigned-char natively: secondary oracle
#   6. both ELFs run on the Sail (Lean) emulator; stdout (emulator chatter
#      filtered as in ship-your-lua's difftest.sh) and exit code compared.
# Sail step counts: --trace-pcs on the `sd` to tohost in _exit, last T row.
# Env: JOBS (default 8), EMU, XPACK, NATIVE_CC.
set -u
here=$(cd "$(dirname "$0")/.." && pwd)
cd "$here"
JOBS=${JOBS:-8}
XPACK=${XPACK:-$(ls -d "$HOME"/toolchains/xpack-riscv-none-elf-gcc-15.2.0-*/bin | head -1)}
EMU=${EMU:-$HOME/Documents/code/ship-your-lua/riscv-lean/lean_emulator/.lake/build/bin/lean_riscv_emulator}
NATIVE_CC=${NATIVE_CC:-gcc}
GCC=$XPACK/riscv-none-elf-gcc
OBJDUMP=$XPACK/riscv-none-elf-objdump
NEWLIB_INC=$XPACK/../riscv-none-elf/include
TCC=$here/build/cross/riscv64-tcc
LIBTCC1=$here/build/cross/riscv64-libtcc1.a
SRC=$here/build/tcc-src
H=$here/c/tests/tcc/harness
OUT=$here/build/test-tcc
CFL="-march=rv64i -mabi=lp64 -mcmodel=medany"
LDFL="$CFL -nostartfiles -T $H/link.ld -Wl,--no-warn-rwx-segments"
MAXSTEPS=${MAXSTEPS:-100000000}

[ -x "$TCC" ] && [ -f "$LIBTCC1" ] || scripts/build_cross_tcc.sh || exit 1
mkdir -p "$OUT"
$GCC $CFL -c "$H/crt0.S" -o "$OUT/crt0.o" || exit 1
$GCC $CFL -O2 -DTCC_HTIF -c "$H/htif.c" -o "$OUT/htif.o" || exit 1

files=("$@"); [ ${#files[@]} -eq 0 ] && files=(c/tests/tcc/t*.c)
names=()
for f in "${files[@]}"; do
  n=$(basename "$f" .c); names+=("$n")
  rm -f "$OUT/$n".*
  # 1-3: tcc
  if "$TCC" -nostdinc -I"$SRC/include" -I"$NEWLIB_INC" -c "$f" -o "$OUT/$n.tcc.o" 2> "$OUT/$n.tcc.err"; then
    scripts/check_isa.sh "$OUT/$n.tcc.o" > "$OUT/$n.isa" 2>&1; echo $? > "$OUT/$n.isa.rc"
    $GCC $LDFL -o "$OUT/$n.tcc.elf" "$OUT/crt0.o" "$OUT/htif.o" "$OUT/$n.tcc.o" "$LIBTCC1" -lm -lc -lgcc 2>> "$OUT/$n.tcc.err" \
      && { scripts/check_isa.sh -q "$OUT/$n.tcc.elf" > "$OUT/$n.elfisa" 2>&1; echo $? > "$OUT/$n.elfisa.rc"; }
  fi
  # 4: gcc rv64i oracle
  $GCC $CFL -O0 -w -c "$f" -o "$OUT/$n.gcc.o" 2> "$OUT/$n.gcc.err" \
    && $GCC $LDFL -o "$OUT/$n.gcc.elf" "$OUT/crt0.o" "$OUT/htif.o" "$OUT/$n.gcc.o" -lm -lc -lgcc 2>> "$OUT/$n.gcc.err"
  # 5: native
  if $NATIVE_CC -O0 -funsigned-char -w "$f" -o "$OUT/$n.native" -lm 2> "$OUT/$n.native.err"; then
    timeout 60 "$OUT/$n.native" > "$OUT/$n.native.out" 2>/dev/null; echo $? > "$OUT/$n.native.rc"
  fi
done

run1() {
  elf=$1
  [ -f "$elf" ] || exit 0
  base=${elf%.elf}
  # the `sd` that writes tohost in _exit (htif.c)
  pc=$("$OBJDUMP" -d "$elf" | awk '/<_exit>:$/ {f=1; next} f && $3 == "sd" {sub(":", "", $1); print $1; exit}')
  printf '0x%s\n' "$pc" > "$base.pcs"
  timeout 1800 "$EMU" "$elf" --trace-pcs "$base.pcs" --max-steps "$MAXSTEPS" > "$base.raw" 2> "$base.trace"
  echo $? > "$base.rc"
  grep -vE '^(TODO: cancel_reservation|PC = 0x|htif_tohost = 0x|SUCCESS$|FAILURE)' "$base.raw" > "$base.out"
  awk -F'\t' '$1=="T"{s=$2+1} END{print s+0}' "$base.trace" > "$base.steps"
}
export -f run1; export EMU OBJDUMP MAXSTEPS
for n in "${names[@]}"; do echo "$OUT/$n.tcc.elf"; echo "$OUT/$n.gcc.elf"; done | xargs -P "$JOBS" -I{} bash -c 'run1 {}'

pass=0; fail=0; npass=0; nfail=0; isabad=0
printf '%-22s %-5s %-7s %-9s %-9s %-11s %-11s %s\n' test isa elf-isa tcc=gcc tcc=host tcc-steps gcc-steps rc
for n in "${names[@]}"; do
  isa=-; eisa=-; r1=FAIL; r2=FAIL; ts=-; gs=-; trc=-
  [ -f "$OUT/$n.isa.rc" ] && { [ "$(cat "$OUT/$n.isa.rc")" = 0 ] && isa=ok || { isa=BAD; isabad=$((isabad+1)); }; }
  [ -f "$OUT/$n.elfisa.rc" ] && { [ "$(cat "$OUT/$n.elfisa.rc")" = 0 ] && eisa=ok || eisa=BAD; }
  [ -f "$OUT/$n.tcc.steps" ] && ts=$(cat "$OUT/$n.tcc.steps")
  [ -f "$OUT/$n.gcc.steps" ] && gs=$(cat "$OUT/$n.gcc.steps")
  [ -f "$OUT/$n.tcc.rc" ] && trc=$(cat "$OUT/$n.tcc.rc")
  if [ -f "$OUT/$n.tcc.out" ] && [ -f "$OUT/$n.gcc.out" ] && cmp -s "$OUT/$n.tcc.out" "$OUT/$n.gcc.out" \
     && [ "$trc" = "$(cat "$OUT/$n.gcc.rc")" ]; then r1=PASS; pass=$((pass+1)); else fail=$((fail+1)); fi
  if [ -f "$OUT/$n.tcc.out" ] && [ -f "$OUT/$n.native.out" ] && cmp -s "$OUT/$n.tcc.out" "$OUT/$n.native.out" \
     && [ "$trc" = "$(cat "$OUT/$n.native.rc")" ]; then r2=PASS; npass=$((npass+1)); else nfail=$((nfail+1)); fi
  printf '%-22s %-5s %-7s %-9s %-9s %-11s %-11s %s\n' "$n" $isa $eisa $r1 $r2 "$ts" "$gs" "$trc"
done
echo "tcc vs gcc-rv64i (Sail): pass=$pass fail=$fail; tcc vs host gcc: pass=$npass fail=$nfail; isa-bad objects=$isabad"
[ $fail = 0 ] && [ $isabad = 0 ]
