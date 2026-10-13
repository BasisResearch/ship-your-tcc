#!/bin/bash
# Generate N small, deterministic, UB-filtered Csmith programs in the common
# subset of tcc, CompCert and Certora's Lean Clight port.
#
#   scripts/gen_csmith.sh N OUTDIR
#
# Seeds BASE_SEED, BASE_SEED+1, ... are tried in order (in parallel batches);
# the first N seeds that pass every filter are kept, so the output depends
# only on (Csmith revision, flags, BASE_SEED, N, filters), not on timing
# (filters use generous timeouts). Each kept program is one self-contained
# file OUTDIR/csmith_<seed>.c (no #include; see scripts/csmith_flatten.py).
#
# Filters per candidate (scripts/csmith_one.sh):
#   size      MIN_LINES (120) <= flattened lines <= MAX_LINES (500)
#   (a)       host gcc -O0 and gcc -O2 -fsanitize=undefined,address (both
#             -funsigned-char, as RISC-V) exit 0 within a timeout, no
#             sanitizer report, same stdout, which is "checksum = %X"
#   (b)       riscv-none-elf-gcc -march=rv64i -mabi=lp64 compiles it
#   (c)       scripts/run_ccomp_interp.sh (if present, lane C) exits 0 with
#             the same stdout: ccomp -interp found no undefined behaviour
#   (d)       xgcc_sail (scripts/run_xgcc_sail.sh) finishes within
#             SAIL_BUDGET steps (default 3M; 0 disables)
#
# Env: BASE_SEED (1), JOBS (8; each Sail run is ~120 MB), MAX_LINES, MIN_LINES,
#      SAIL_BUDGET, CSMITH_FLAGS (default below), KEEP_WORK=1.
#
# Writes OUTDIR/manifest.tsv (kept programs), OUTDIR/rejected.tsv (every
# rejected seed with its reason) and OUTDIR/GENERATOR (revision and flags).
set -u
N=${1:?usage: gen_csmith.sh N OUTDIR}; OUT=${2:?usage: gen_csmith.sh N OUTDIR}
root=$(cd "$(dirname "$0")/.." && pwd)
BASE_SEED=${BASE_SEED:-1}
JOBS=${JOBS:-8}
# Csmith configuration (rationale in results/corpus.md):
#  --ccomp                 CompCert-compatible output (no bitfields, explicit casts)
#  --no-argc               int main(void): no argv, no strcmp
#  --no-volatiles/-pointers  volatile is not covered by the Lean port
#  --no-packed-struct      #pragma pack is outside the common subset
#  --no-bitfields          layout is implementation-defined; ccomp emulates them
#  --no-builtins           (default) no __builtin_*
#  --no-inline-function    no `inline` noise
#  --no-float              (default) integer-only: soft-float is tested separately
#  safe math ON            wrappers avoid signed overflow, bad shifts, div by 0
#  size knobs              few functions, shallow blocks, small arrays/structs,
#                          so tcc-in-Sail compile time stays small
CSMITH_FLAGS=${CSMITH_FLAGS:---ccomp --no-argc --no-volatiles --no-volatile-pointers \
--no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise \
--max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 \
--max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 \
--max-union-fields 3 --max-pointer-depth 2}
export CSMITH_FLAGS MAX_LINES MIN_LINES SAIL_BUDGET
work=$root/build/csmith-work
mkdir -p "$OUT" "$work"
[ -x "$root/build/csmith/install/bin/csmith" ] || { echo "build Csmith into build/csmith first" >&2; exit 1; }

res=$work/results.tsv; : > "$res"
next=$BASE_SEED
batch=$(( N < JOBS ? JOBS : N ))
while :; do
  acc=$(awk -F'\t' '$2=="accept"' "$res" | wc -l)
  [ "$acc" -ge "$N" ] && break
  seq "$next" $(( next + batch - 1 )) | xargs -P "$JOBS" -I{} "$root/scripts/csmith_one.sh" {} "$work" >> "$res"
  next=$(( next + batch ))
  echo "tried $(( next - BASE_SEED )) seeds, accepted $(awk -F'\t' '$2=="accept"' "$res" | wc -l)" >&2
done

sort -n -k1,1 "$res" > "$res.sorted"
# keep the first N accepted seeds; report rejections up to the last kept seed
last=$(awk -F'\t' '$2=="accept"' "$res.sorted" | head -n "$N" | tail -1 | cut -f1)
rm -f "$OUT"/csmith_*.c
printf 'name\tseed\tlines\tbytes\tstdout\tsail_steps_xgcc\n' > "$OUT/manifest.tsv"
printf 'seed\treason\tlines\n' > "$OUT/rejected.tsv"
awk -F'\t' -v last="$last" '$1<=last' "$res.sorted" | while IFS=$'\t' read -r seed st reason lines bytes out steps; do
  if [ "$st" = accept ]; then
    name=$(printf 'csmith_%06d' "$seed")
    cp "$work/$seed/prog.c" "$OUT/$name.c"
    printf '%s\t%s\t%s\t%s\t%s\t%s\n' "$name" "$seed" "$lines" "$bytes" "$out" "$steps" >> "$OUT/manifest.tsv"
  else
    printf '%s\t%s\t%s\n' "$seed" "$reason" "$lines" >> "$OUT/rejected.tsv"
  fi
done
{
  echo "csmith_rev $(cat "$root/scripts/CSMITH_REV")"
  echo "csmith_flags $CSMITH_FLAGS"
  echo "base_seed $BASE_SEED"
  echo "n $N"
  echo "max_lines ${MAX_LINES:-500}"
  echo "min_lines ${MIN_LINES:-120}"
  echo "sail_budget ${SAIL_BUDGET:-3000000}"
  echo "ccomp_stage $([ -x "$root/scripts/run_ccomp_interp.sh" ] && echo on || echo skipped)"
} > "$OUT/GENERATOR"
[ -n "${KEEP_WORK:-}" ] || rm -rf "$work"
echo "kept $N programs in $OUT (seeds $BASE_SEED..$last); rejections: $(tail -n +2 "$OUT/rejected.tsv" | cut -f2 | sort | uniq -c | tr '\n' ';')" >&2
