#!/bin/bash
# One Csmith candidate: generate, flatten, filter. Called by gen_csmith.sh.
#   csmith_one.sh SEED WORKDIR
# Prints one TSV line on stdout:
#   SEED  accept|reject  REASON  LINES  BYTES  NATIVE_STDOUT  SAIL_STEPS
# and on accept leaves WORKDIR/SEED/prog.c.
set -u
seed=$1; work=$2
root=$(cd "$(dirname "$0")/.." && pwd)
CSMITH=${CSMITH:-$root/build/csmith/install/bin/csmith}
INC=${CSMITH_INC:-$root/build/csmith/install/include}
TC=$(ls -d "$HOME"/toolchains/xpack-riscv-none-elf-gcc-15.2.0-*/bin | head -1)
MAX_LINES=${MAX_LINES:-500}
MIN_LINES=${MIN_LINES:-120}  # below this Csmith output is trivial (often no globals)
SAIL_BUDGET=${SAIL_BUDGET:-3000000}   # 0 disables the Sail cost stage
d=$work/$seed
rm -rf "$d"; mkdir -p "$d"
lines=0; bytes=0; out0=; steps=
emit() { printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\n' "$seed" "$1" "$2" "$lines" "$bytes" "$out0" "$steps"; exit 0; }
reject() { emit reject "$1"; }

# Flags: see results/corpus.md ("Csmith configuration") for the rationale.
# shellcheck disable=SC2086
timeout 20 "$CSMITH" --seed "$seed" $CSMITH_FLAGS -o "$d/raw.c" > /dev/null 2>&1 || reject csmith-failed
python3 -I "$root/scripts/csmith_flatten.py" "$INC" "$d/raw.c" "$d/prog.c" \
    --header "Csmith $(cat "$root/scripts/CSMITH_REV" 2>/dev/null | cut -c1-12) seed $seed, flattened by scripts/csmith_flatten.py" \
    2> "$d/flatten.log" || reject flatten-failed
lines=$(wc -l < "$d/prog.c"); bytes=$(wc -c < "$d/prog.c")
[ "$lines" -le "$MAX_LINES" ] || reject too-large
[ "$lines" -ge "$MIN_LINES" ] || reject too-small

# (a) host gcc -O0 vs gcc -O2 + UBSan/ASan, both under a timeout
CF="-std=gnu11 -funsigned-char -w"
gcc $CF -O0 -o "$d/o0" "$d/prog.c" 2> "$d/o0.log" || reject host-compile-failed
gcc $CF -O2 -fsanitize=undefined,address -fno-sanitize-recover=all -o "$d/san" "$d/prog.c" 2> "$d/san.log" \
    || reject host-san-compile-failed
timeout 5 "$d/o0" > "$d/o0.out" 2>/dev/null; rc=$?
[ $rc = 124 ] && reject host-timeout
[ $rc = 0 ] || reject "host-exit-$rc"
out0=$(tr '\n' ' ' < "$d/o0.out" | sed 's/ $//')
ASAN_OPTIONS=detect_leaks=0 timeout 20 "$d/san" > "$d/san.out" 2> "$d/san.err"; rc=$?
[ $rc = 124 ] && reject san-timeout
[ $rc = 0 ] || reject sanitizer
[ -s "$d/san.err" ] && reject sanitizer-stderr
cmp -s "$d/o0.out" "$d/san.out" || reject O0-vs-O2san-mismatch
grep -q '^checksum = [0-9A-F]*$' "$d/o0.out" || reject no-checksum

# (b) the RV64I cross compiler accepts it
"$TC/riscv-none-elf-gcc" -std=gnu11 -w -march=rv64i -mabi=lp64 -O0 -c -o "$d/x.o" "$d/prog.c" \
    2> "$d/xgcc.log" || reject xgcc-compile-failed

# (c) ccomp -interp agrees and finds no UB (pluggable: skipped if absent)
if [ -x "$root/scripts/run_ccomp_interp.sh" ]; then
  timeout 120 "$root/scripts/run_ccomp_interp.sh" "$d/prog.c" > "$d/cc.out" 2> "$d/cc.err"; rc=$?
  [ $rc = 0 ] || reject "ccomp-interp-exit-$rc"
  cmp -s "$d/o0.out" "$d/cc.out" || reject ccomp-interp-mismatch
fi

# (d) execution cost on Sail (xgcc_sail), budget SAIL_BUDGET steps
if [ "$SAIL_BUDGET" != 0 ]; then
  : > "$d/sail.meta"
  MAX_STEPS=$SAIL_BUDGET ORACLE_META="$d/sail.meta" timeout 900 \
      "$root/scripts/run_xgcc_sail.sh" "$d/prog.c" > "$d/sail.out" 2> "$d/sail.err"; rc=$?
  steps=$(awk -F= '$1=="steps"{print $2}' "$d/sail.meta" | tail -1)
  [ $rc = 124 ] && reject sail-over-budget
  [ $rc = 0 ] || reject "sail-exit-$rc"
  # a mismatch here is a finding for diff3, not a reason to drop the program
fi
rm -f "$d/o0" "$d/san" "$d/x.o"
emit accept ok
