#!/bin/bash
# Minimise a program while it keeps a disagreement between two oracles and
# stays free of undefined behaviour, with C-Vise.
#
#   scripts/minimise.sh prog.c ORACLE_A ORACLE_B [OUT.c]
#
# e.g. scripts/minimise.sh corpus/csmith/csmith_000042.c tcc_sail gcc_native
#
# Interestingness test (written to the work dir as interesting.sh):
#   1. host gcc accepts it without the warnings that signal UB-prone
#      reductions (C-Reduce's classic list: implicit declarations, missing
#      returns, uninitialised use, pointer/integer mix-ups, ...);
#   2. gcc -O1 -fsanitize=undefined,address -funsigned-char runs it to exit
#      within 10 s with no sanitizer report;
#   3. if scripts/run_ccomp_interp.sh exists: ccomp -interp exits 0, i.e.
#      CompCert's reference interpreter finds no undefined behaviour;
#   4. ORACLE_A and ORACLE_B (scripts/oracle.py --require-ok) both report
#      outcome ok under their configured status mappings, and differ.
# Work dir: build/minimise/<prog>-<A>-<B>/ ; the result is copied to OUT.c
# (default results/repro/<prog>-<A>-<B>.c).
#
# C-Vise: build/cvise-env (conda-forge cvise 2.12, installed with micromamba
# because there is no PyPI package and no clang dev headers on this box):
#   build/micromamba/bin/micromamba create -p build/cvise-env -c conda-forge cvise pebble psutil chardet
# Env: CVISE_JOBS (default 4: each test may start two Sail emulators).
set -eu
root=$(cd "$(dirname "$0")/.." && pwd)
src=$(readlink -f "${1:?usage: minimise.sh prog.c ORACLE_A ORACLE_B [OUT.c]}")
A=${2:?}; B=${3:?}
name=$(basename "$src" .c)
out=${4:-$root/results/repro/$name-$A-$B.c}
CVISE_ENV=$root/build/cvise-env
[ -x "$CVISE_ENV/bin/cvise" ] || { echo "no C-Vise in $CVISE_ENV (see the header of this script)" >&2; exit 1; }
w=$root/build/minimise/$name-$A-$B
rm -rf "$w"; mkdir -p "$w"
cp "$src" "$w/prog.c"

cat > "$w/interesting.sh" <<EOF
#!/bin/bash
# cvise interestingness test: see scripts/minimise.sh. Runs in a scratch copy
# made by cvise, on ./prog.c.
ROOT="$root"; A="$A"; B="$B"
EOF
cat >> "$w/interesting.sh" <<'EOF'
f=prog.c
gcc -std=gnu11 -funsigned-char -O1 -Wall -Wextra -c -o /dev/null "$f" > cc.log 2>&1 || exit 1
grep -qE 'implicit declaration|implicit-int|uninitialized|control reaches end|return type defaults|without a cast|incompatible (pointer|integer)|makes (pointer|integer) from|too (few|many) arguments|comparison between pointer and integer|division by zero|array subscript|is used uninitialized|excess elements|ordered comparison of pointer with integer|no return statement|address of (local|stack)|declared .static. but never defined|type of .* defaults to|format .%|expects a matching|expects argument|too many arguments for format|return type of .main. is not|main. is normally a non-static' cc.log && exit 1
gcc -std=gnu11 -funsigned-char -O1 -fsanitize=undefined,address -fno-sanitize-recover=all -w -o san "$f" > /dev/null 2>&1 || exit 1
ASAN_OPTIONS=detect_leaks=0 timeout 10 ./san > san.out 2> san.err || { rc=$?; [ $rc -ge 124 ] && exit 1; }
[ -s san.err ] && grep -qE 'runtime error|AddressSanitizer|LeakSanitizer' san.err && exit 1
if [ -x "$ROOT/scripts/run_ccomp_interp.sh" ]; then
  timeout 300 "$ROOT/scripts/run_ccomp_interp.sh" "$f" > cc.out 2> cc.err || exit 1
fi
python3 -I "$ROOT/scripts/oracle.py" --require-ok "$A" "$f" > a.out 2> a.err; ra=$?
[ $ra = 124 ] || [ $ra = 125 ] && exit 1
python3 -I "$ROOT/scripts/oracle.py" --require-ok "$B" "$f" > b.out 2> b.err; rb=$?
[ $rb = 124 ] || [ $rb = 125 ] && exit 1
if [ $ra = $rb ] && cmp -s a.out b.out; then exit 1; fi
exit 0
EOF
chmod +x "$w/interesting.sh"

cd "$w"
if ! ./interesting.sh; then
  echo "minimise: the original program is not interesting (no $A/$B disagreement, or UB filters reject it)" >&2
  exit 2
fi
PATH="$CVISE_ENV/bin:$PATH" cvise --n "${CVISE_JOBS:-4}" --timeout 1800 ./interesting.sh prog.c > cvise.log 2>&1
mkdir -p "$(dirname "$out")"
{ echo "/* reduced by scripts/minimise.sh from ${src#$root/}: $A and $B disagree */"; cat prog.c; } > "$out"
echo "reduced $(wc -l < "$src") -> $(wc -l < prog.c) lines: $out"
