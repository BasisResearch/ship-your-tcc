#!/bin/bash
# gcc_native oracle (sanity check): host gcc -O0 -funsigned-char (RISC-V
# char is unsigned; x86-64 and riscv64 are both LP64), run natively with
# argv = {"prog"} like the Sail harness's crt0.
#
#   scripts/run_gcc_native.sh prog.c
#
# Oracle contract (scripts/oracles.toml): stdout = the program's stdout
# (stderr is not compared), exit code = its exit status; 124 = timeout
# (NATIVE_TIMEOUT seconds, default 60); 125 = could not compile.
set -u
src=$(readlink -f "$1")
work=$(mktemp -d "${TMPDIR:-/tmp}/gcc_native.XXXXXX")
trap 'rm -rf "$work"' EXIT
if ! gcc -O0 -std=gnu11 -fpermissive -w -funsigned-char -o "$work/prog" "$src" -lm > "$work/cc.log" 2>&1; then
  echo "gcc_native: compile failed" >&2; head -20 "$work/cc.log" >&2; exit 125
fi
cd "$work" && timeout "${NATIVE_TIMEOUT:-60}" bash -c 'exec -a prog ./prog' < /dev/null 2> "$work/stderr"
rc=$?
[ -n "${ORACLE_META:-}" ] && echo "stderr_bytes=$(wc -c < "$work/stderr")" >> "$ORACLE_META"
exit $rc
